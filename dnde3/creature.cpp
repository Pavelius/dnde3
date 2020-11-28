#include "main.h"

DECLDATA(creature, 256)

static int			skill_level[] = {30, 60, 90};
static dicei		skill_raise[] = {{2, 12}, {1, 10}, {1, 8}, {1, 6}, {1, 5}, {1, 4}, {1, 3}, {1, 2}};
static const char*	skill_names[] = {"�������", "����������", "�������", "������"};
static creature*	current_player;
static int			experience_count[] = {0,
0, 1000, 2500, 4500, 7000, 10000, 13500, 17500, 22000, 27000,
};

void creature::clear() {
	memset(this, 0, sizeof(*this));
	guard = Blocked;
	site_id = location_id = Blocked;
}

creature* creature::getactive() {
	return current_player;
}

short unsigned creature::getid() const {
	if(!this)
		return Blocked;
	return this - bsmeta<creature>::elements;
}

void creature::feel(ability_s id, bool raise) {
	auto& ei = bsmeta<abilityi>::elements[id];
	act("%����� �����������%� ���� %1.", raise ? ei.name_how : ei.curse_how);
}

void creature::add(ability_s id, int v, bool interactive) {
	if(!v)
		return;
	dressoff();
	abilities[id] += v;
	dresson();
	if(interactive)
		feel(id, v > 0);
}

void creature::add(state_s id, int v, bool interactive) {
	if(!v)
		return;
	if(v > 0) {
		if(states.is(id))
			return;
		dressoff();
		states.set(id);
		dresson();
		if(interactive)
			act(bsmeta<statei>::elements[id].text_set);
	} else {
		if(!states.is(id))
			return;
		dressoff();
		states.remove(id);
		dresson();
		if(interactive)
			act(bsmeta<statei>::elements[id].text_remove);
	}
}

void creature::add(variant id, int v, bool interactive) {
	switch(id.type) {
	case Ability: add((ability_s)id.value, v, interactive); break;
	case Spell: add((spell_s)id.value, v, interactive); break;
	case State: add((state_s)id.value, v, interactive); break;
	case Skill: add((skill_s)id.value, v, interactive); break;
	case Item:
	{
		item it; it.create(item_s(id.value), 0, 5, 0, 20);
		equip(it);
		if(bsmeta<itemi>::elements[id.value].weapon.ammunition) {
			// Free ammunitons for items
			item it(bsmeta<itemi>::elements[id.value].weapon.ammunition, v);
			equip(it);
		}
	}
	break;
	case Harm:
		switch(id.value) {
		case Piercing:
		case Slashing:
		case Bludgeon:
			if(roll(Acrobatics)) {
				act("%����� ��������%� � �������.");
				return;
			}
			break;
		}
		damage(v, damage_s(id.value), 0, interactive);
		break;
	}
}

void creature::dispell(variant source, bool interactive) {
	auto id = getid();
	auto ps = bsmeta<boosti>::elements;
	for(auto& e : bsmeta<boosti>()) {
		if(e.owner_id == id || e.source == source) {
			if(e.id)
				add(e.id, e.modifier, interactive);
			else if(source.type == Spell) {
				if(interactive) {
					switch(source.value) {
					case Sleep:
						act("%����� �������� �������%���.");
						break;
					default:
						feel(LifePoints, true);
						break;
					}
				}
			}
			continue;
		}
		*ps++ = e;
	}
	bsmeta<boosti>::source.setcount(ps - bsmeta<boosti>::elements);
}

void creature::recall(variant id, variant source, int modifier, unsigned rounds) {
	auto p = bsmeta<boosti>::add();
	p->id = id;
	p->source = source;
	p->modifier = modifier;
	p->time = rounds;
	p->owner_id = getid();
}

creature* creature::getobject(short unsigned v) {
	if(v == Blocked)
		return 0;
	return bsmeta<creature>::elements + v;
}

void creature::dispell(bool interactive) {
	auto id = getid();
	auto ps = bsmeta<boosti>::elements;
	for(auto& e : bsmeta<boosti>()) {
		if(e.owner_id == id) {
			add(e.id, e.modifier, interactive);
			continue;
		}
		*ps++ = e;
	}
	bsmeta<boosti>::source.setcount(ps - bsmeta<boosti>::elements);
}

void creature::unlink() {
	if(current_player == this)
		current_player = 0;
	site::unlink(*this);
	setguard(Blocked);
	setposition(Blocked);
}

void creature::dressen(int m) {
	auto aw = getallowedweight();
	auto mw = getweight();
	auto penalty_attack = 0;
	auto penalty_speed = 0;
	auto penalty_deflect = 0;
	encumbrance = NoEncumbered;
	if(mw > aw) {
		penalty_speed += 25;
		penalty_deflect += 5;
		encumbrance = Encumbered;
	}
	if(mw > aw * 2) {
		penalty_speed += 25;
		penalty_attack += 15;
		penalty_deflect += 10;
		encumbrance = HeavilyEncumbered;
	}
	abilities[Speed] -= penalty_speed * m;
	abilities[Attack] -= penalty_attack * m;
	abilities[Protection] -= penalty_deflect * m;
}

void creature::dressoff() {
	if(!this)
		return;
	dressen(-1);
	dresssa(-1);
	dress(-1);
}

void creature::dresson() {
	if(!this)
		return;
	dress(1);
	dresssa(1);
	dressen(1);
}

void creature::dress(int m) {
	static slot_s enchant_slots[] = {Head, Neck, TorsoBack, Torso, OffHand, RightFinger, LeftFinger, Elbows, Legs};
	// Modify armor abilities
	for(auto i = Head; i <= Ranged; i = (slot_s)(i + 1)) {
		if(!wears[i])
			continue;
		auto ei = wears[i].getarmor();
		abilities[Protection] += m * ei.protection;
		abilities[Deflect] += m * ei.deflect;
		abilities[Armor] += m * ei.armor;
	}
	// Modify weapon abilities
	for(auto i = Head; i <= Ranged; i = (slot_s)(i + 1)) {
		if(!wears[i])
			continue;
		if(i == OffHand && wears[i].getitem().slot != OffHand)
			continue;
		auto ei = wears[i].getattack();
		abilities[Attack] += m * ei.attack;
	}
	// Apply enchant effect
	for(auto i = Head; i <= Ranged; i = (slot_s)(i + 1)) {
		if(!wears[i])
			continue;
		auto id = wears[i].geteffect();
		if(!id)
			continue;
		auto q = wears[i].getbonus();
		switch(id.type) {
		case Ability:
			abilities[id.value] += m * bsmeta<abilityi>::elements[id.value].getbonus(q);
			break;
		case Skill:
			// Apply only to known skills
			if(skills[id.value])
				skills[id.value] += m * q * 10;
			break;
		}
	}
}

void creature::dresssk(int m) {
	for(auto i = FirstSkill; i <= LastSkill; i = (skill_s)(i + 1)) {
		auto& ei = bsmeta<skilli>::elements[i];
		skills[i] += m * (get(ei.abilities[0]) + get(ei.abilities[1]));
	}
}

void creature::dresssa(int m) {
	for(auto i = Attack; i <= ManaRate; i = (ability_s)(i + 1)) {
		auto& ei = bsmeta<abilityi>::elements[i];
		abilities[i] += m * calculate(ei.formula);
	}
	if(getrole() == Character)
		abilities[LifePoints] += abilities[Constitution] * m;
	else
		abilities[LifePoints] += abilities[Constitution] * m / 2;
}

const char* creature::getlevelname(skill_s v) const {
	auto n = getlevel(v);
	return maptbl(skill_names, n);
}

bool creature::ismaster(skill_s v) const {
	return skills[v] >= skill_level[2];
}

int creature::getlevel(skill_s v) const {
	auto r = skills[v];
	auto n = 0;
	for(auto a : skill_level) {
		if(r < a)
			break;
		n++;
	}
	return n;
}

dicei creature::getraise(skill_s v) const {
	auto n = skills[v] / 10;
	return maptbl(skill_raise, n);
}

void creature::raise(skill_s value) {
	skills[value] += getraise(value).roll();
}

void creature::equip(item it, slot_s id) {
	if(id >= Head && id <= LastWear)
		it.set(KnownStats);
	dressoff();
	wears[id] = it;
	dresson();
}

bool creature::add(item v, bool run, bool talk) {
	if(v.is(Coinable)) {
		money += v.getcost()*v.getcount();
		return true;
	}
	// Second place item to backpack
	for(auto i = Backpack; i <= LastBackpack; i = (slot_s)(i + 1)) {
		if(wears[i]) {
			if(wears[i].stack(v))
				return true;
			continue;
		}
		if(run)
			equip(v, i);
		return true;
	}
	if(talk) {
		static const char* text[] = {
			"�� � ���� ��� ��� ��������?",
			"� ���� ��� ���� �����.",
			"������ ������. ���� ���-������ ��������� �� �������.",
		};
		say(maprnd(text));
	}
	return false;
}

bool creature::equip(item v) {
	if(!isallow(v.getkind()))
		return false;
	// First try to dress this item
	for(auto i = Head; i <= LastWear; i = (slot_s)(i + 1)) {
		if(!v.is(i))
			continue;
		if(wears[i]) {
			if(wears[i].stack(v))
				return true;
			continue;
		}
		equip(v, i);
		return true;
	}
	return add(v, true, false);
}

bool creature::canuse(const item& e, bool talk) const {
	if(!isallow(e.getkind())) {
		if(talk) {
			static const char* text[] = {
				"�� ��, ��� �� ��� ������.",
				"� �� ���� ��� ���� ������������.",
				"� ����� ������ �� ����.",
			};
			say(maprnd(text));
		}
		return false;
	}
	return true;
}

bool creature::cantakeoff(slot_s id, bool talk) {
	if(id >= Head && id <= LastWear) {
		auto& e = wears[id];
		if(e.is(Cursed)) {
			if(talk) {
				static const char* text[] = {
					"������� ����! ��� ���!",
					"���, � ��� �� �����.",
					"��� ���������! ��� ��������!",
				};
				say(maprnd(text));
				e.set(KnownPower);
			}
			return false;
		} else if(e.is(Natural)) {
			if(talk) {
				static const char* text[] = {
					"��� ����� ����!",
					"��� � ��� �����? ������?",
					"�� � ����� ���?",
					"��� �������� �� ��� ��������.",
				};
				say(maprnd(text));
			}
			return false;
		}
	}
	return true;
}

bool creature::equip(item& v1, item& v2, bool run) {
	if(!canuse(v2, true))
		return false;
	dressoff();
	auto v = v1;
	v1 = v2;
	v2 = v;
	dresson();
	v1.set(KnownStats);
	return true;
}

void creature::raiseathletics() {
	static ability_s source[] = {Strenght, Dexterity};
	adat<ability_s, 4> raised;
	auto n = get(Athletics);
	if(!skills[Athletics])
		return;
	for(auto s : source) {
		auto v = get(s);
		auto chance = n - (v - 10) * 10;
		if(rollv(chance)) {
			abilities[s]++;
			raised.add(s);
		}
	}
	if(raised && is(Friendly)) {
		sb.add("��������� ������������� �������� ���������� ");
		auto count = 0;
		for(auto s : raised) {
			count++;
			if(count > 1) {
				if(count == raised.getcount())
					sb.add(" � ");
				else
					sb.add(", ");
			}
			sb.add("%-1", getstr(s));
		}
		sb.add(".");
	}
}

void creature::raiseskills(int number) {
	skillu source(this);
	source.select(*this);
	source.shuffle();
	source.setcaps();
	if(isactive()) {
		source.sort();
		while(number > 0) {
			char temp[260]; stringbuilder sb(temp);
			sb.add("����� ����� �������� (�������� ��� %1i)?", number);
			auto s = source.choose(true, temp, 0);
			raise(s);
			auto v = source.getcap(s);
			if(skills[s] > v)
				skills[s] = v;
			number--;
		}
	} else {
		unsigned index = 0;
		unsigned count = source.getcount();
		while(number > 0) {
			if(index >= count)
				index = 0;
			auto s = source[index++];
			raise(s);
			auto v = source.getcap(s);
			if(skills[s] > v)
				skills[s] = v;
			number--;
		}
	}
}

void creature::randomequip() {
	for(auto& ei : bsmeta<equipmenti>()) {
		if((!ei.race || ei.race == getrace()) && is(ei.type)) {
			for(auto v : ei.features)
				add(v, 3, false);
			break;
		}
	}
	equip(item(Ration, 2));
	money += xrand(3, 18)*GP;
}

void creature::raiseability() {
	auto& ei = getclass();
	switch(abilities[Level]) {
	case 0:
		abilities[Attack] += ei.weapon.base;
		break;
	case 1:
		abilities[LifePoints] += ei.hp;
		abilities[ManaPoints] += ei.mp;
		abilities[Attack] += ei.weapon.multiplier;
		break;
	default:
		if(ei.hp)
			abilities[LifePoints] += xrand(ei.hp / 2, ei.hp);
		if(ei.mp)
			abilities[ManaPoints] += xrand(ei.mp / 2, ei.mp);
		abilities[Attack] += ei.weapon.multiplier;
		// Add level features
		for(auto& pi : bsmeta<leveli>()) {
			if(pi.type == kind && pi.level == abilities[Level]) {
				for(auto& e : pi.features)
					add(e.id, e.value, true);
			}
		}
		break;
	}
}

void creature::applyabilities() {
	const auto& ri = bsmeta<racei>::elements[getrace()];
	const auto& ci = getclass();
	// Create base abilities
	abilities[Level] = 0;
	// Generate abilities
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1))
		abilities[i] = ri.abilities[i] + (rand() % 5) - 2;
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1))
		abilities[i] += ci.ability[i];
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1)) {
		if(abilities[i] < 0)
			abilities[i] = 0;
	}
	// Generate skills
	for(auto e : ri.abilityvs)
		abilities[e.id] += e.value;
	for(auto e : ri.skills)
		raise(e);
	for(auto e : ci.skills)
		raise(e);
	states.set(ri.states);
	// Class spells
	for(auto e : ci.spells)
		add(e, 1, false);
	raiseability();
	dresson();
}

void creature::finish() {
	hp = get(LifePoints);
	mp = get(ManaPoints);
}

void creature::create(race_s race, gender_s gender, class_s type) {
	clear();
	this->variant::type = Role;
	this->variant::value = Character;
	this->kind = type;
	setname(race, gender);
	applyabilities();
	dressoff();
	raise(Climbing);
	if(abilities[Intellegence] >= 9)
		raise(Literacy);
	dresson();
	raiselevel();
	randomequip();
	finish();
}

void creature::getfullname(stringbuilder& sb) const {
	sb.add(getname());
	sb.adds("%-3-%-1 %2i ������", getstr(kind), get(Level), getstr(getrace()));
}

attacki creature::getattack(slot_s id, const item& weapon) const {
	attacki result = {0};
	auto skill = weapon.getitem().skill;
	if(id == Melee || weapon)
		result = weapon.getattack();
	if(!result.dice.max)
		return result;
	auto& ci = getclass();
	result.attack += get(Attack);
	result.dice.min += get(Damage);
	result.dice.max += get(Damage);
	if(skill.type == Skill && skills[skill.value]) {
		// Weapon focus modify damage, attack and speed
		auto& ei = bsmeta<skilli>::elements[skill.value];
		auto value = get(skill_s(skill.value));
		if(ei.weapon.attack)
			result.attack += value / ei.weapon.attack;
		if(ei.weapon.damage)
			result.dice.max += value / ei.weapon.damage;
		if(ei.weapon.speed)
			result.speed += value / ei.weapon.speed;
	}
	switch(id) {
	case Melee:
		if(weapon.is(Versatile) && !wears[OffHand]) {
			result.dice.min += 1;
			result.dice.max += 1;
		}
		if(wears[OffHand].is(Light)) {
			result.attack -= 20;
			result.speed -= 8;
			result.speed += wears[OffHand].getitem().weapon.speed;
			auto& ei = bsmeta<skilli>::elements[TwoWeaponFighting];
			auto value = get(TwoWeaponFighting);
			if(ei.weapon.attack)
				result.attack += value / ei.weapon.attack;
			if(ei.weapon.damage)
				result.dice.max += value / ei.weapon.damage;
			if(ei.weapon.speed)
				result.speed += value / ei.weapon.speed;
		}
		break;
	case OffHand:
		if(weapon.is(Light) && wears[Melee]) {
			result.attack -= 30;
			auto& ei = bsmeta<skilli>::elements[TwoWeaponFighting];
			if(ei.weapon.attack)
				result.attack += get(TwoWeaponFighting) / ei.weapon.attack;
		}
		break;
	}
	result.dice.normalize();
	return result;
}

void creature::activate() {
	current_player = this;
}

creature* creature::getactive(int n) {
	creaturea creatures;
	creatures.select(Friendly);
	creatures.matchbs(true);
	creatures.matchact(Sleep, true);
	if(!creatures || n >= creatures.getcount())
		return 0;
	return creatures[n];
}

const variant* creature::calculate(const variant* p, int& result) const {
	switch(p->type) {
	case Ability: result = get((ability_s)p->value); p++; break;
	case Skill: result = get((skill_s)p->value); p++; break;
	case Number: result = p->value; p++; break;
	default: return 0;
	}
	while(p->type == Formula) {
		switch(p->value) {
		case Negative: result = -result; break;
		case Divide2: result = result / 2; break;
		case Divide3: result = result / 3; break;
		case Divide4: result = result / 4; break;
		case Divide10: result = result / 10; break;
		case Multiply2: result = result * 2; break;
		case Multiply3: result = result * 3; break;
		case Multiply4: result = result * 4; break;
		default: return 0;
		}
		p++;
	}
	return p;
}

int	creature::calculate(const variant* formula) const {
	int result = 0;
	while(formula) {
		int a = 0;
		formula = calculate(formula, a);
		result += a;
	}
	return result;
}

slot_s creature::getwearerslot(const item* p) const {
	if(this
		&& p >= wears
		&& p < (wears + sizeof(wears) / sizeof(wears[0])))
		return slot_s(p - wears);
	return Backpack;
}

void creature::select(itema& a, slot_s i1, slot_s i2, bool filled_only) {
	auto ps = a.data;
	for(auto i = i1; i <= i2; i = (slot_s)(i + 1)) {
		if(filled_only && !wears[i])
			continue;
		*ps++ = &wears[i];
	}
	a.count = ps - a.data;
}

void creature::dropdown(item& item) {
	if(!remove(item, false, true))
		return;
	if(item.ispersonal()) {
		static const char* text[] = {
			"� �� ����� ����.",
			"��� ������ ��� ����� ������� �������������.",
			"�� ����. ��� ��� ������. ��� ������ ���, ��� ������."
		};
		say(maprnd(text));
		return;
	}
	loc.drop(getposition(), item);
	remove(item, true, true);
	consume(StandartEnergyCost / 4);
}

void creature::dropdown() {
	itema items; items.selectb(*this);
	auto pi = items.choose("��������� �������", 0, NoSlotName);
	if(pi)
		dropdown(*pi);
}

void creature::pickup() {
	itema items; items.select(getposition());
	auto pi = items.choose("������� �������", 0, NoSlotName);
	if(pi) {
		auto site = getsite();
		if(site && !pi->is(NotForSale)) {
			auto seller = site->getowner();
			if(seller && seller != this) {
				int cost = pi->getcost();
				if(!seller->ask(*this, "������ ������ �� [%1i] �����?", pi->getcost()))
					return;
				if(getmoney() < cost) {
					static const char* text[] = {
						"����������� ����� ����� ���������� �����.",
						"� ���� ���� ������� ���������� �����.",
						"��� ������� ���������� ����� - ��� ������.",
					};
					seller->say(maprnd(text));
					return;
				}
				if(rollv(get(Bargaining), seller->get(Bargaining)) > 0) {
					cost = cost * 70 / 100;
					static const char* ask_discount[] = {
						"�������? � �� ��������� ������� �� ����� ���������!",
						"���� ����� ������ � ������������. �������� ������?",
						"� �� ���� �������� ������ �� ���� ������. ��� ����� ������, ��� ������ �� �����!",
					};
					static const char* accept_discount[] = {
						"��������. ����� ������ [%1i] �����.",
						"��, � ������� ������. ����� �� [%1i] �����.",
					};
					say(maprnd(ask_discount), cost);
					seller->say(maprnd(accept_discount), cost);
				}
				money -= cost;
				pi->set(NotForSale);
			}
		}
		if(add(*pi, true, true)) {
			pi->clear();
			consume(StandartEnergyCost / 4);
		}
	}
}

void creature::inventory() {
	while(true) {
		itema items; items.select(*this);
		auto pi = items.choose("����������", 0, SlotName);
		if(!pi)
			break;
		auto slot = pi->getwearerslot();
		if(*pi) {
			if(!remove(*pi, false, true)) {
				pause();
				continue;
			}
			if(!add(*pi, false, true)) {
				pause();
				continue;
			}
			add(*pi, true, false);
			remove(*pi, true, false);
		} else {
			if(slot >= Head && slot <= LastWear) {
				items.clear();
				items.selectb(*this);
				items.match(slot, false);
				auto p2 = items.choose("������", 0, NoSlotName, true);
				if(p2) {
					if(!equip(*pi, *p2, true))
						pause();
				}
			}
		}
	}
}

bool creature::remove(item& e, bool run, bool talk) {
	if(!e)
		return false;
	auto slot = e.getwearerslot();
	if(e.is(Cursed) && slot >= Head && slot <= LastWear) {
		if(talk) {
			static const char* text[] = {
				"������� ����! ��� ���!",
				"���, � ��� �� �����.",
				"��� ���������! ��� ��������!",
			};
			say(maprnd(text));
			e.set(KnownPower);
		}
		return false;
	} else if(e.is(Natural)) {
		if(talk) {
			static const char* text[] = {
				"��� ����� ����!",
				"��� � ��� �����? ������?",
				"�� � ����� ���?",
				"��� �������� �� ��� ��������.",
			};
			say(maprnd(text));
		}
		return false;
	}
	if(run) {
		dressoff();
		e.clear();
		dresson();
	}
	return true;
}

int	creature::getweight() const {
	auto r = 0;
	for(auto& e : wears)
		r += e.getweight();
	return r;
}

void creature::select(skilla& a) const {
	auto ps = a.data;
	for(auto i = FirstSkill; i <= LastSkill; i = (skill_s)(i + 1)) {
		if(!skills[i])
			continue;
		*ps++ = i;
	}
	a.count = ps - a.data;
}

void creature::useskills() {
	bool cancel = false;
	skillu source(this);
	source.select(*this);
	source.sort();
	source.setcaps();
	auto s = source.choose(true, "����� ����� ������������?", &cancel);
	if(cancel)
		return;
	creaturea creatures(*this);
	use(creatures, s);
}

void creature::usespells() {
	bool cancel = false;
	spella source;
	source.select(*this);
	//source.sort();
	auto s = source.choose("� ��� ��� �� ������ ����������.", "����� ���������� ������������?", &cancel, this);
	if(cancel)
		return;
	if(!use(s, get(s), 0, true))
		return;
	wait();
}

void creature::cantmovehere() const {
	if(isactive())
		act("���� �� ������.");
}

void creature::look(indext index) {
	auto d1 = location::getdirection(getposition(), index);
	if(d1 != Down && d1 != Up)
		direction = d1;
}

void creature::lookobjects() {
	loc.choose(getposition(), true, true, false);
}

void creature::lookaround() {
	if(isactive()) {
		loc.addobject(getposition(), sb);
		loc.additems(getposition(), sb);
	}
}

void creature::move(indext index) {
	if(is(Drunken) && d100() < 30) {
		index = loc.movernd(getposition(), index);
		static const char* talk[] = {"�� ���� ����!", "��!", "������..", "���������...", "� ���...", "����� � ������� ���%�?"};
		say(maprnd(talk));
	}
	if(index == Blocked) {
		cantmovehere();
		return;
	}
	look(index);
	if(loc.gettile(index) == Wall) {
		cantmovehere();
		return;
	}
	auto current_index = getposition();
	if(loc.is(current_index, Webbed)) {
		if(roll(Climbing))
			loc.remove(current_index, Webbed);
		else {
			act("%����� �������%��� � �������.");
			wait();
			return;
		}
	}
	if(loc.gettrap(current_index) == TrapPit
		&& !loc.is(current_index, Hidden)) {
		if(!roll(Climbing)) {
			act("%����� �� ����%�� ������� �� ���.");
			wait();
			return;
		}
	}
	switch(loc.getobject(index)) {
	case Door:
		if(!loc.is(index, Opened)) {
			if(loc.is(index, Sealed))
				say("����� �������.");
			else {
				//appear();
				loc.set(index, Opened);
			}
			wait();
			return;
		}
		break;
	case StairsDown:
		if(isactive()) {
			if(askyn("������ ���������� ����?")) {
				wait();
				game.use(StairsDown);
			}
		}
		return;
	case StairsUp:
		if(isactive()) {
			if(askyn("������ ��������� ������?")) {
				wait();
				game.use(StairsUp);
			}
		}
		return;
	case Altar:
		return;
	case Tree:
		cantmovehere();
		return;
	}
	auto p = find(index);
	if(p) {
		if(isenemy(p)) {
			meleeattack(*p);
			return;
		} else if(!isactive()) {
			// ������� � ������ ��������� �� ��������
			wait();
			return;
		} else if(p->is(Guardian)) {
			static const char* talk[] = {
				"� ������� ��� �����.",
				"����� �� ������.",
				"�� ��������, � �� ������.",
			};
			appear();
			p->say(maprnd(talk));
			wait();
			return;
		} else if(p->is(Owner)) {
			static const char* talk[] = {
				"�� ������ ���� � ���� ���������.",
				"����� ���� �� ����.",
				"� �� ����� �����",
			};
			appear();
			p->say(maprnd(talk));
			wait();
			return;
		} else {
			p->setposition(getposition());
			p->wait();
			if(d100() < 50) {
				if(p->saybusy()) {
					// ��� ��� �������
				} else if(is(Invisible)) {
					static const char* talk[] = {
						"��� �����?",
						"�� �����.",
						"��� ����������?",
					};
					p->say(maprnd(talk));
				} else {
					static const char* talk[] = {
						"��! �� ��������.",
						"�����, �������.",
						"���� �� ��� �������?",
					};
					p->say(maprnd(talk));
				}
			}
		}
	}
	auto site = getsite();
	auto pnewsite = site::find(index);
	if(site && site->getowner() == this) {
		// �������� ��������� �� �������� ���
		if(site != pnewsite) {
			static const char* talk[] = {
				"�������, �������� ��� ����� �������� � �� ����.",
				"��� ������ ������. ������� � �����.",
				"����� ��� ���� ����?",
			};
			say(maprnd(talk));
			wait();
			return;
		}
	}
	movecost(index);
	if(getposition() != index) {
		set(pnewsite);
		setposition(index);
		usestealth();
		usetrap();
		if((*this))
			lookaround();
	}
}

void creature::movecost(indext index) {
	auto v = StandartEnergyCost;
	v -= (StandartEnergyCost / 20) * abilities[Movement];
	if(v < StandartEnergyCost / 20)
		v = StandartEnergyCost / 20;
	consume(v);
}

void creature::usestealth() {
	if(!is(Invisible))
		return;
	if(roll(MoveSilently))
		return;
	creaturea creatures;
	creatures.select(getposition(), 3); // Can be heard on this distance
	creatures.match(*this, Hostile, false, true);
	for(auto p : creatures) {
		if(p->roll(HearNoises)) {
			static const char* text[] = {"��� ��!", "����� ���!", "�� ��� ������!"};
			appear();
			p->say(maprnd(text));
			p->addexp(10);
			break;
		}
	}
}

creature* creature::find(indext i) {
	for(auto& e : bsmeta<creature>()) {
		if(!e)
			continue;
		if(e.getposition() == i)
			return &e;
	}
	return 0;
}

void creature::consume(int v) {
	if(*this)
		restore_energy -= v;
}

void creature::attack(creature& enemy, const attacki& ai, int bonus, int danger) {
	if(is(Invisible) || enemy.is(Sleep) || enemy.is(Unaware)) {
		appear();
		enemy.dispell(Sleep, true);
		// Attack from invisible state
		// increase danger and chance to hit
		bonus += get(Backstabbing);
		danger += get(Backstabbing) * 3;
	}
	bonus += ai.attack;
	bonus -= enemy.get(Protection);
	if(enemy.is(Unaware)) {
		bonus += 20;
		danger += 20;
	}
	if(enemy.is(Fear)) {
		bonus += 20;
		danger += 20;
	}
	if(bonus < 5)
		bonus = 5;
	look(enemy.getposition());
	if(!rollv(bonus)) {
		act("%����� ��������%�.");
		return;
	}
	auto pierce = 0;
	auto deflect = enemy.get(Deflect);
	if(roll(FindWeakness, -deflect)) {
		danger += 30;
		switch(ai.type) {
		case Piercing:
			danger += 10;
			pierce = 100;
			act("%����� �����%� � �������� �����.");
			break;
		case Slashing:
			act("%����� �����%�� ������������ ����.");
			enemy.add(Wounded, 1, false);
			enemy.bloodstain();
			break;
		case Bludgeon:
			act("%����� �����%�� ������������ ����.");
			enemy.add(Dazzled, 1, false);
			break;
		}
	} else
		act("%����� �����%�.");
	// Calculate damage from danger
	auto dv = ai.dice.roll();
	dv = dv * danger / 100;
	enemy.damage(dv, ai.type, pierce);
}

void creature::meleeattack(creature& enemy, int bonus) {
	auto am = getattack(Melee, wears[Melee]);
	attack(enemy, am, bonus, 100);
	wears[Melee].breaktest();
	if(wears[OffHand] && wears[OffHand].is(Light)) {
		attack(enemy, getattack(OffHand), bonus, 100);
		wears[OffHand].breaktest();
	}
	consume(am.getenergy());
}

bool creature::isenemy(const creature* target) const {
	return is(Hostile) != target->is(Hostile);
}

void creature::say(const char* format, ...) const {
	if(!(*this))
		return;
	sayv(sb, format, xva_start(format));
}

void creature::aimove() {
	// Need active location
	auto d = (direction_s)xrand(Left, RightDown);
	auto i = loc.to(getposition(), d);
	if(loc.isfreenw(i))
		move(i);
	else
		wait();
}

bool creature::needrestore(ability_s id) const {
	auto max = restore_points_percent * get(id) / 100;
	switch(id) {
	case LifePoints: return hp <= max;
	case ManaPoints: return mp <= max;
	}
	return false;
}

void creature::add(spell_s id, int v, bool interactive) {
	if(v > 0) {
		spells[id] += v;
		if(interactive) {
			if(spells[id] == 1)
				act("%����� ������%� ���������� [%+1].", getstr(id));
			else if(spells[id] > 1)
				act("%����� �������%� �������� ����������� [%+1] �� [+%2i] �����.", getstr(id), spells[id]);
		}
	} else if(v < 0) {
		v = -v;
		if(v > spells[id])
			v = spells[id];
		spells[id] -= v;
		if(interactive) {
			if(spells[id] == 0)
				act("%����� �����%� ���������� %+1.", getstr(id));
			else
				act("%����� �������%� ������� �������� ����������� [%+1] �� [-%2i] �����.", getstr(id), spells[id]);
		}
	}
}

void creature::add(skill_s id, int v, bool interactive) {
	if(v > 0) {
		skills[id] += v;
		if(interactive)
			act("%����� �������%� ����� [%+1] �� [+%2i%%].", getstr(id), get(id));
	} else if(v < 0) {
	}
}

bool creature::aispells(creaturea& creatures) {
	spella source;
	source.select(*this);
	source.shuffle();
	for(auto s : source) {
		if(use(creatures, s, get(s), 0, false)) {
			wait();
			return true;
		}
	}
	return false;
}

bool creature::aiskills(creaturea& creatures, bool long_action, bool run) {
	skilla source;
	source.select(*this);
	source.removent();
	source.match(LongAction, !long_action);
	source.shuffle();
	if(!run)
		return source.getcount() != 0;
	else {
		for(auto s : source) {
			if(use(creatures, s))
				return true;
			if(!(*this))
				return true;
		}
	}
	return false;
}

void creature::aiturn(creaturea& creatures, creaturea& enemies, creature* enemy) {
	if(enemies) {
		// ���������� �������� ����� ����� ��� ���� �����
		if(is(Fear) && loc.getrange(enemy->getposition(), getposition()) <= getlos() + 1) {
			if(d100() < chance_scarry_cry) {
				static const char* text[] = {"�������!", "��������!", "����!!!", "�� ��������!!!"};
				say(maprnd(text));
			}
			moveaway(enemy->getposition());
			return;
		}
		// �������� �������� ���� ���� ����� �����������
		if(loc.getrange(enemy->getposition(), getposition()) > 1 && canshoot()) {
			rangeattack(*enemy);
			return;
		}
		if(aispells(creatures))
			return;
		moveto(enemy->getposition());
	} else {
		if(is(Fear))
			add(Fear, -1, true);
		// When we try to stand and think
		if(d100() < chance_act) {
			if(is(LowInt)) {
				if(needrestore(LifePoints)) {
					if(aiuse(creatures, 0, Drinkable, LifePoints))
						return;
					if(aiuse(creatures, 0, Edible, LifePoints))
						return;
				}
			}
			if(is(AveInt)) {
				if(needrestore(ManaPoints)) {
					if(aiuse(creatures, 0, Drinkable, ManaPoints))
						return;
					if(aiuse(creatures, 0, Edible, ManaPoints))
						return;
				}
			}
			if(aiskills(creatures, false, true))
				return;
		}
		// If creature guard some square move to guard position
		if(guard != Blocked) {
			moveto(guard);
			return;
		}
		// If creature have a leader don't move far away from him
		auto leader = getleader();
		if(leader) {
			if(loc.getrange(leader->getposition(), getposition()) > 2) {
				moveto(leader->getposition());
				return;
			}
		}
		// Chance to do nothing
		if(d100() < chance_act) {
			wait();
			return;
		}
		// Investigate items
		aimove();
	}
}

void creature::checksick() {
	if(!is(Sick))
		return;
	if(hp >= get(LifePoints) / 3) {
		damage(1, Magic, 100, false);
		if(isactive())
			sb.add("������� �������������.");
	}
}

void creature::checkpoison() {
	if(is(Poisoned)) {
		if(roll(ResistPoison)) {
			if(poison <= 0)
				add(Poisoned, -1, true);
			else
				poison--;
		} else {
			act("%����� �������� �� ���.");
			damage(1, Magic, 100, false);
		}
	} else if(is(Drunken)) {
		if(roll(ResistPoison, 15)) {
			if(poison <= 0)
				add(Drunken, -1, true);
			else
				poison--;
		}
	}
}

void creature::restoration() {
	const auto pc = StandartEnergyCost;
	while(restore_hits > pc) {
		if(!is(Sick)) {
			if(hp < get(LifePoints))
				hp++;
		}
		restore_hits -= pc;
	}
	restore_hits += get(LifeRate);
	while(restore_mana > pc / 4) {
		if(mp < get(ManaPoints))
			mp++;
		restore_mana -= pc / 4;
	}
	restore_mana += get(ManaRate);
	if(is(Wounded)) {
		// Wounded creature loose 1 hit point each turn
		if(roll(Constitution))
			add(Wounded, -1, true);
		else {
			act("%����� �������� ������.");
			damage(1, Slashing, 100, false);
		}
	}
}

void creature::makemove() {
	if(restore_energy < StandartEnergyCost) {
		restore_energy += get(Speed);
		return;
	}
	// Dazzled creature don't make turn
	if(is(Dazzled)) {
		if(roll(Constitution, 8))
			add(Dazzled, -1, true);
		else {
			wait();
			return;
		}
	}
	if(!is(Invisible)) {
		// �������� ��� ��������� ���������� �����������
		// ������ ���� ����������� ��������� � ������ ������ ����
		// � ���� �� �������, �� �� �������, �� ���� ����, �� ���������!!!
		if(is(Invisibility))
			add(Invisible, 1, true);
	}
	// Sleeped creature don't move
	if(is(Sleep))
		return;
	if(is(Unaware))
		add(Unaware, -1, true);
	// Get nearest creatures
	creaturea creatures;
	creatures.select(getposition(), getlos());
	creaturea enemies = creatures;
	enemies.match(*this, Hostile, false);
	creature* enemy = 0;
	if(enemies) {
		// Combat situation - need eliminate enemy
		enemies.sort(getposition());
		enemy = enemies[0];
	}
	if(isactive()) {
		auto start = restore_energy;
		while(start == restore_energy) {
			location::setcamera(getposition());
			playui();
		}
	} else
		aiturn(creatures, enemies, enemy);
}

bool creature::roll(skill_s v, int bonus, int divider) const {
	auto r = get(v);
	switch(divider) {
	case 0: break;
	case 1: r = r * 2 / 3; break;
	default: r /= divider; break;
	}
	r += bonus;
	return rollv(r);
}

bool creature::rollv(int v) {
	if(v <= 0)
		return false;
	if(v < 5)
		v = 5;
	else if(v > 95)
		v = 95;
	return d100() < v;
}

int	creature::rollv(int v1, int v2) {
	auto r1 = d100();
	auto r2 = d100();
	auto d1 = imax(0, v1 - r1);
	auto d2 = imax(0, v2 - r2);
	return d1 - d2;
}

void creature::applyaward() const {
	auto index = getposition();
	if(index == Blocked)
		return;
	auto award = getaward();
	for(auto& e : bsmeta<creature>()) {
		if(!e)
			continue;
		if(!e.isenemy(this))
			continue;
		e.addexp(award);
	}
}

void creature::bloodstain() const {
	loc.set(getposition(), Blooded);
}

void creature::dropitems() {
	auto index = getposition();
	for(auto i = Backpack; i <= LastWear; i = (slot_s)(i + 1)) {
		auto& e = wears[i];
		if(!e)
			continue;
		if(e.is(Natural))
			continue;
		if(e.getmagic() != Artifact) {
			if(d100() >= chance_drop_item)
				continue;
		}
		e.loot();
		loc.drop(index, e);
		e.destroy(Magic, false);
	}
}

void creature::kill() {
	if(d100() < chance_blood_when_dead)
		bloodstain();
	auto corpse = item::findcorpse((role_s)value);
	if(corpse && d100() < 30) {
		item it(corpse);
		it.seteffect((role_s)value);
		it.set(KnownPower);
		loc.drop(getposition(), it);
	}
	hp = mp = 0;
	applyaward();
	dressoff();
	dropitems();
	dispell(false);
	unlink();
}

void creature::damage(int value, damage_s type, int pierce, bool interactive) {
	auto& di = bsmeta<damagei>::elements[type];
	// Innate resist skills
	if(di.resist && value > 0) {
		auto resist_skill = get(di.resist);
		if(resist_skill < 0)
			value += -resist_skill * value / 100; // Negative resist increase damage
		else {
			if(rollv(resist_skill - value)) {
				act(di.resist_text, value);
				return;
			}
		}
	}
	if(value < 0) {
		value = -value;
		auto mhp = get(LifePoints);
		if(hp + value > mhp)
			value = mhp - hp;
		if(value <= 0)
			return;
		if(interactive)
			act("%����� �����������%� [+%1i] �����������.", value);
		hp += value;
	} else {
		if(di.damage_wears != 0 && di.damage_chance != 0) {
			auto count = di.damage_wears;
			if(count < 0)
				count = value / -count;
			if(d100() < di.damage_chance)
				damagewears(value, type, count);
		}
		auto armor = get(Armor);
		switch(type) {
		case WaterAttack: value /= 3; break;
		case Electricity: pierce += 2; break;
		}
		if(value < 0)
			return;
		if(armor > 0 && pierce > 0) {
			if(pierce > armor)
				armor = 0;
			else
				armor -= pierce;
		}
		value -= armor;
		if(value <= 0) {
			if(interactive)
				act("����� ��������� ���� ����.");
		} else if(hp <= value) {
			if(interactive)
				act("%����� �������%� [%1i] ����������� � ����%�.", value);
			kill();
		} else {
			if(interactive)
				act("%����� �������%� [%1i] �����������.", value);
			hp -= value;
		}
	}
}

unsigned creature::getlevelup() const {
	return experience_count[abilities[Level] + 1];
}

void creature::raiselevel() {
	dressoff();
	abilities[Level] += 1;
	auto start_log = sb.get();
	raiseability();
	raiseathletics();
	if(sb.get() > start_log)
		pause();
	raiseskills();
	dresson();
}

void creature::addexp(int v, bool interactive) {
	v = v * (90 + get(Intellegence)) / 100;
	if(v > 0) {
		experience += v;
		if(interactive)
			act("%����� �������%� [+%1i] �����.", v);
		while(experience >= getlevelup()) {
			if(is(Friendly)) {
				activate();
				act("��� ������������, %����� �������%� ����� ������� �����.");
				act("������ �� %-1 [%2i] ������.", getstr(kind), abilities[Level] + 1);
				pause();
			}
			raiselevel();
		}
	} else {
		if(interactive)
			act("%����� �������%� [-%1i] �����.", v);
	}
}

void creature::enslave() {
	creaturea source;
	source.select(getposition(), 6);
	source.match(*this, Hostile, false);
	auto p = source.choose("� ���� ������ ���������?");
	p->activate();
	wait();
}

void creature::moveto(indext index) {
	auto pos = getposition();
	if(index == pos)
		return;
	if(location::getrange(pos, index) > 1) {
		loc.clearblock();
		loc.blockwalls();
		loc.blockcreatures();
		loc.makewave(index);
		index = loc.stepto(pos);
		if(index == Blocked)
			return;
	}
	move(index);
}

void creature::moveaway(indext index) {
	loc.clearblock();
	loc.blockwalls();
	loc.makewave(index);
	index = loc.stepfrom(getposition());
	if(index == Blocked)
		return;
	move(index);
}

void creature::set(ability_s id, int v) {
	dressoff();
	abilities[id] = v;
	dresson();
}

void creature::set(skill_s id, int v) {
	skills[id] = v;
}

int creature::get(skill_s id) const {
	const auto& ei = bsmeta<skilli>::elements[id];
	return skills[id] + get(ei.abilities[0]) + get(ei.abilities[1]);
}

bool creature::isallow(item_s v) const {
	auto& ci = bsmeta<classi>::elements[kind];
	auto& ei = bsmeta<itemi>::elements[v];
	if(ei.skill.type == Skill && skills[ei.skill.value] > 0)
		return true;
	if(ci.restricted.is(v))
		return false;
	return true;
}

creature* creature::getleader() const {
	if(is(Friendly)) {
		auto pa = getactive();
		if(pa != this)
			return pa;
	}
	return 0;
}

bool creature::cansee(indext i) const {
	return loc.cansee(getposition(), i);
}

bool creature::canshoot() const {
	auto interactive = isactive();
	if(!wears[Ranged]) {
		if(interactive) {
			static const char* text[] = {
				"��� ���� ������������� ������.",
				"��� ��������?",
			};
			say(maprnd(text));
		}
		return false;
	}
	auto am = wears[Ranged].getammo();
	if(am) {
		if(!wears[Amunitions]) {
			if(interactive) {
				static const char* text[] = {
					"��� �����������.",
					"����������� %-1.",
				};
				say(maprnd(text), getstr(am));
			}
			return false;
		}
		if(wears[Amunitions].getkind() != am) {
			if(interactive) {
				static const char* text[] = {
					"�� �������� ����������. ���� %-1.",
					"��� �������� ���������� %-1, � � ������� ������������ %-2.",
				};
				say(maprnd(text), getstr(am), wears[Amunitions].getname());
			}
			return false;
		}
	}
	return true;
}

void creature::shoot() {
	if(!canshoot())
		return;
	creaturea enemies;
	enemies.select(getposition(), getlos());
	enemies.match(*this, Hostile, false);
	enemies.sort(getposition());
	if(!enemies) {
		static const char* text[] = {
			"� �� ���� ������ �� ������ �����. ���� �������?",
			"�� � ���� ��������.",
			"�������� ����?",
		};
		say(maprnd(text));
		return;
	}
	if(location::getrange(enemies[0]->getposition(), getposition()) <= 1) {
		static const char* text[] = {
			"���� ������� ������!",
			"���� ������� � ���������� � �� ��������!",
			"���� ������ �� �����!",
		};
		say(maprnd(text));
		return;
	}
	rangeattack(*enemies[0], 0);
}

void creature::rangeattack(creature& enemy, int bonus) {
	auto ai = getattack(Ranged);
	attack(enemy, ai, 0, 100);
	if(wears[Ranged].getammo())
		wears[Amunitions].use();
	else if(wears[Ranged].iscountable())
		wears[Ranged].use();
	wears[Melee].breaktest();
	consume(ai.getenergy());
}

void creature::testweapons() {
	auto ai = getattack(Melee);
	act("%����� ���������%� ���� ������.");
	act("%1 � ������ [%2i%%] ������� [%3i-%4i] �����������.", wears[Melee].getname(), ai.attack, ai.dice.min, ai.dice.max);
	if(wears[OffHand].is(Light)) {
		auto ai = getattack(OffHand);
		act("%1 � ������ [%2i%%] ������� [%3i-%4i] �����������.", wears[OffHand].getname(), ai.attack, ai.dice.min, ai.dice.max);
	}
	act("���������� ����� ����� [%1i] �������.", ai.getenergy());
	if(wears[Ranged]) {
		auto ai = getattack(Ranged);
		act("%1 � ������ [%2i%%] ������� [%3i-%4i] �����������.", wears[Ranged].getname(), ai.attack, ai.dice.min, ai.dice.max);
		act("������������� ����� ����� [%1i] �������.", ai.getenergy());
	}
}

boosti*	creature::find(variant id) const {
	auto owner_id = getid();
	for(auto& e : bsmeta<boosti>()) {
		if(e.owner_id == owner_id && e.id == id)
			return &e;
	}
	return 0;
}

boosti*	creature::finds(variant source) const {
	auto owner_id = getid();
	for(auto& e : bsmeta<boosti>()) {
		if(e.owner_id == owner_id && e.source == source)
			return &e;
	}
	return 0;
}

void creature::add(spell_s id, unsigned minutes) {
	recall({}, id, 0, game.getrounds() + minutes);
}

void creature::add(ability_s id, variant source, int v, bool interactive, unsigned minutes) {
	if(!v)
		return;
	add(id, v, interactive);
	recall(id, source, -v, game.getrounds() + minutes);
}

void creature::paymana(int value, bool interactive) {
	if(value < 0) {
		auto max = get(ManaPoints);
		value = -value;
		if(mp + value > max)
			value = max - mp;
		if(value <= 0)
			return;
		mp += value;
		if(interactive)
			act("%����� �����������%� %1i ����.", value);
	} else {
		if(value > mp)
			value = mp;
		if(value == 0)
			return;
		mp -= value;
		if(interactive)
			act("%����� �������%� %1i ����.", value);
	}
}

bool creature::aiuse(const creaturea& creatures, const char* title, slot_s slot, variant effect) {
	itema source;
	source.selectb(*this);
	source.select(getposition(), true);
	source.match(slot, false);
	if(effect)
		source.matchboost(effect);
	auto pi = source.choose(title, 0, NoSlotName);
	if(pi)
		return use(creatures, *pi);
	return false;
}

void creature::drink() {
	creaturea creatures(*this);
	aiuse(creatures, "���� ������ ������?", Drinkable, {});
}

void creature::eat() {
	creaturea creatures(*this);
	aiuse(creatures, "��� ������ ������?", Edible, {});
}

void creature::usewands() {
	creaturea creatures(*this);
	aiuse(creatures, "������������ ����� �������?", Zapable, {});
}

void creature::backpack() {
	itema source; source.selectb(*this);
	source.choose("������", 0, NoSlotName);
}

int	creature::getboost(variant id) const {
	auto owner_id = getid();
	auto result = 0;
	for(auto& e : bsmeta<boosti>()) {
		if(e.owner_id == owner_id && e.id == id)
			result += e.modifier;
	}
	return result;
}

void creature::minimap() {
	loc.minimap(getposition(), true);
}

bool creature::match(variant v) const {
	switch(v.type) {
	case Alignment: return true;
	case Class: return kind == v.value;
	case Gender: return getgender() == v.value;
	case Race: return getrace() == v.value;
	case Role: return getrole() == v.value;
	case Skill: return skills[v.value] != 0;
	case State: return is(state_s(v.value));
	case Spell: return spells[v.value] != 0;
	}
	return true;
}

bool creature::ismatch(const creature& opponent, skill_s id, int value) const {
	auto r1 = get(id);
	auto r2 = value + opponent.get(Intellegence) * 2 - 20;
	return  r1 >= r2;
}

void creature::fail(skill_s id) {
	const int chance_fail = 30;
	auto& ei = bsmeta<skilli>::elements[id];
	if(ei.is(Strenght) && d100() < chance_fail) {
		act("%����� ��������%� �����.");
		damage(1, Bludgeon, 100, false);
	} else if((ei.is(Intellegence) || ei.is(Wisdow)) && d100() < chance_fail) {
		act("%����� �����������%� ��������� �������������.");
		paymana(1, false);
	} else if(ei.is(Dexterity) && d100() < chance_fail) {
		act("%����� �������%� �������� �����.");
		damage(1, Bludgeon, 100, false);
	} else if(d100() < chance_fail) {
		if(isactive())
			act("�� ����� ���� �������, �� ��� ���� ������.");
		wait(xrand(2, 4));
	}
}

void creature::appear() {
	add(Invisible, -1, true);
}

void creature::readsomething() {
	if(!skills[Literacy]) {
		if(isactive())
			sb.add("�� �� ��������. ��� ������ ��������� ������.");
		return;
	}
	creaturea creatures(*this);
	use(creatures, Literacy);
}

bool creature::charmresist(int bonus) const {
	if(roll(ResistCharm, bonus)) {
		act("%����� � ������ �� �������%�.");
		return true;
	}
	return false;
}

int creature::getlos() const {
	int v = abilities[Visibility];
	int b = loc.getlight();
	if(b < 0 && !is(Darkvision))
		v += b;
	return imax(1, imin(7, v));
}

void creature::damagewears(int count, damage_s type, int item_count) {
	itema items;
	select(items, Head, LastWear, true);
	items.shuffle();
	int maximum = items.getcount();
	if(item_count > maximum)
		item_count = maximum;
	for(auto i = 0; i < maximum; i++)
		items[i]->damage(count, type, true);
}

bool creature::ask(const nameable& opponent, const char* format, ...) const {
	return askv(sb, opponent, format, xva_start(format));
}

bool creature::askyn(const char* format, ...) const {
	if(!isactive())
		return true;
	act(format);
	return askyn();
}

bool creature::saybusy() {
	if(is(Sleep)) {
		static const char* text[] = {"%����� ����� ����.",
			"%����� ������ �����������.",
			"%����� �� ��� ����������%��� �� ������ ���."
		};
		act(maprnd(text));
		return true;
	}
	if(!isbusy())
		return false;
	static const char* text[] = {"� �����",
		"��� ���� ��������� ����.",
		"����� ��� ��� ����� [%1i] ����� � � ����������."
	};
	say(maprnd(text), -(restore_energy / StandartEnergyCost));
	return true;
}

void creature::usetools() {
	creaturea creatures(*this);
	use(creatures, wears[Tool]);
}

void creature::quitandsave() {
	if(askyn("�� ������������� ������ [���������] ���� � �����?")) {
		game.write();
		exit(0);
	}
}

bool creature::knownreceipt(variant id) const {
	auto i = item::getreceipts().indexof(id);
	if(i == -1)
		return false;
	return recipes.is(i);
}

void creature::learnreceipt(variant id) {
	int receipt_count = recipes.getcount();
	int receipt_maximum = get(Alchemy) / 10;
	act("%����� ����������� ������%� ������.");
	if(knownreceipt(id) || receipt_count >= receipt_maximum) {
		if(isactive())
			sb.add("� ��� ���� ������ �������� �� �������, ������� ��� �� ���������.");
		addexp(2000 * get(Alchemy) / 100);
	} else {
		auto i = item::getreceipts().indexof(id);
		if(i == -1)
			return;
		act("� ��� �������� ���������� ������������ ������� [%1]. ������, ���� ����� �������� �� ������� ��������� ����� ������ ����.", id.getnameof());
		recipes.set(i);
	}
	wait(30);
}

variant creature::choosereceipt(const char* interactive) const {
	auto source = item::getreceipts();
	answeri an;
	for(unsigned i = 0; i < source.count; i++) {
		if(!knownreceipt(source[i]))
			continue;
		an.add(i, "������ %1", source[i].getnameof());
	}
	return source[an.choosev(interactive, false, false, interactive)];
}

site* creature::getsite() const {
	return (site_id != Blocked) ? &bsmeta<site>::elements[site_id] : 0;
}

void creature::set(const site* v) {
	if(!v)
		site_id = Blocked;
	else
		site_id = v - bsmeta<site>::elements;
}

item* creature::finditem(item_s v) {
	for(auto& it : wears) {
		if(it.getkind() == v)
			return &it;
	}
	return 0;
}

void creature::zoomon() {
	game.use(StairsDown);
}

bool creature::canleave(direction_s v) const {
	auto i = getposition();
	if(i == Blocked)
		return false;
	auto x = loc.getx(i);
	auto y = loc.gety(i);
	switch(v) {
	case Left: return x == 0;
	case Right: return x == mmx - 1;
	case Down: return y == mmy - 1;
	case Up: return y == 0;
	default: return false;
	}
}

bool creature::leaving(direction_s v) {
	if(!isactive())
		return false;
	if(!canleave(v))
		return false;
	sb.add("�� ������������� ������ �������� ��� �����?");
	if(!askyn())
		return false;
	game.use(StairsUp);
	wait();
	return true;
}

void creature::testpotion() {
	//loc.growplants();
	//game.decoyfood();
	//additem(Wand1, Domination);
	additem(Wand1, SummonAlly);
}

int	creature::getallowedweight() const {
	return get(Strenght) * 500;
}

bool creature::execute(action_s v, bool run) {
	site* pst;
	switch(v) {
	case GuardPosition:
		if(guard != Blocked)
			return false;
		if(run)
			guard = getposition();
		break;
	case StopGuardPosition:
		if(guard == Blocked)
			return false;
		if(run)
			guard = Blocked;
		break;
	case UseLongActionSkill:
		if(true) {
			creaturea creatures;
			creatures.select(getposition(), getlos());
			return aiskills(creatures, true, run);
		}
		break;
	case MakeDiscount:
		pst = getsite();
		if(!pst || pst->getowner() != this)
			return false;
		if(true) {
			itema items;
			items.select(*pst);
			items.selectg(Sale100);
			if(!items)
				return false;
			if(run) {
				static const char* talk[] = {"�����, ����� ������� ����������. %+1 ������ ������� �������.",
					"���� ������ ��������. ������� � ������. %+1 ������ �������.",
					"%+1 �� ��� ���������� �����. ��� ������.",
				};
				auto pi = items.random();
				char temp[260]; stringbuilder  sb(temp);
				pi->getname(sb, true);
				say(maprnd(talk), temp);
				auto sale = pi->getsale();
				pi->set(sale_s(sale - 1));
			}
		}
		break;
	}
	return true;
}

void creature::decoyfood() {
	for(auto& e : wears) {
		if(!e)
			continue;
		e.decoy();
	}
}

void creature::additem(item_s type, variant effect, bool identified) {
	item it(type);
	it.seteffect(effect);
	if(it.ischargeable())
		it.setcharge(xrand(2, 12));
	if(identified)
		it.set(KnownPower);
	add(it, true, false);
}

item creature::craft(item_s type, variant effect, skill_s skill, int bonus) {
	item result(type);
	if(!roll(skill, bonus)) {
		result.setquality(xrand(0, 3));
		result.set(Cursed);
		return result;
	}
	auto quality = 0;
	if(roll(skill, bonus - 15)) {
		quality++;
		if(roll(skill, bonus - 30)) {
			quality++;
			if(roll(skill, bonus - 45))
				quality++;
		}
	}
	result.setquality(quality);
	result.seteffect(effect);
	return result;
}

const summona& creature::getsummon() const {
	static summona monsters = {GoblinWarrior, GoblinRockthrower, GoblinRockthrowerWarriorF, OrcWarrior};
	if(value != Character) {

	}
	return monsters;
}

void creature::setfriendlyto(const creature& player) {
	if(player.is(Hostile))
		add(Hostile, 1, false);
	else
		add(Friendly, 1, false);
}

void creature::wait(duration_s v) {
	consume(bsmeta<durationi>::elements[v].roll());
}

bool creature::is(condition_s v) const {
	switch(v) {
	case Guardian: return guard != Blocked;
	case Owner: return getsite() && getsite()->getowner() == this;
	case MissHits: return gethits() < get(LifePoints);
	case MissHalfHits: return gethits() < get(LifePoints) / 2;
	case MissAlmostAllHits: return gethits() < get(LifePoints) / 5;
	case MissMana: return getmana() < get(ManaPoints);
	case MissHalfMana: return getmana() < get(ManaPoints) / 2;
	case MissAlmostAllMana: return getmana() < get(ManaPoints) / 5;
	default: return false;
	}
}
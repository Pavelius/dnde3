#include "main.h"

DECLDATA(creature, 256);

static int			skill_level[] = {10, 40, 60, 80};
static dicei		skill_raise[] = {{2, 12}, {1, 6}, {1, 4}, {1, 1}};
static const char*	skill_names[] = {"���������", "�����������", "����������", "����������"};
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

void creature::dressoff() {
	if(!this)
		return;
	dresssa(-1);
	dress(-1);
}

void creature::dresson() {
	if(!this)
		return;
	dress(1);
	dresssa(1);
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
		skills[i] += m*(get(ei.abilities[0]) + get(ei.abilities[1]));
	}
}

void creature::dresssa(int m) {
	for(auto i = Attack; i <= ManaRate; i = (ability_s)(i + 1)) {
		auto& ei = bsmeta<abilityi>::elements[i];
		abilities[i] += m*calculate(ei.formula);
	}
}

const char* creature::getlevelname(skill_s v) const {
	auto n = getlevel(v);
	return maptbl(skill_names, n);
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
	auto n = getlevel(v);
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
		money += v.getcost();
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
			auto s = source[index++]; raise(s);
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
	creatures.match(Sleep, true);
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

void creature::lookaround() {
	if(isactive()) {
		loc.addobject(getposition(), sb);
		loc.additems(getposition(), sb);
	}
}

void creature::move(indext index) {
	if(index == Blocked) {
		cantmovehere();
		return;
	}
	look(index);
	if(loc.gettile(index) == Wall) {
		cantmovehere();
		return;
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
			if(ask("������ ���������� ����?")) {
				wait();
				game.use(StairsDown);
			}
		}
		return;
	case StairsUp:
		if(isactive()) {
			if(ask("������ ��������� ������?")) {
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
		} else if(p->isguard()) {
			static const char* talk[] = {
				"� ������� ��� �����.",
				"����� �� ������.",
				"�� ��������, � �� ������.",
			};
			appear();
			p->say(maprnd(talk));
			wait();
			return;
		} else {
			// ����� �������� �������� � ���������
			auto psite = site::find(p->getposition());
			if(psite && psite->getowner() == p) {
				// � ����������� ������� �� ���������� �������
				static const char* talk[] = {
					"�� ������ ���� � ���� ���������.",
					"����� ���� �� ����.",
					"� �� ����� �����",
				};
				appear();
				p->say(maprnd(talk));
				return;
			}
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
	movecost(index);
	if(getposition() != index) {
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

void creature::usetrap() {
	auto i = getposition();
	if(i == Blocked)
		return;
	auto t = loc.gettrap(i);
	if(!t)
		return;
	auto& ei = bsmeta<trapi>::elements[t];
	auto bonus = ei.modifier;
	if(loc.is(i, Hidden))
		bonus -= 20;
	else
		bonus += 15;
	if(roll(Alertness, bonus)) {
		if(loc.is(i, Hidden) && is(Friendly)) {
			act("%����� ���������%� �������.");
			loc.remove(i, Hidden);
			addexp(10);
		}
	} else {
		loc.remove(i, Hidden);
		act(ei.text_use);
		auto c = ei.damage.roll();
		switch(ei.effect.type) {
		case Role:
			while(c-- > 0) {
				auto p = loc.add(i, (role_s)ei.effect.value);
				if(is(Hostile))
					p->add(Hostile, -1, false);
				else
					p->add(Hostile, 1, false);
			}
			break;
		default:
			add(ei.effect, c, true);
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
		danger += 10;
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

bool creature::aiskills(creaturea& creatures) {
	skilla source;
	source.select(*this);
	source.removent();
	source.shuffle();
	for(auto s : source) {
		if(use(creatures, s))
			return true;
		if(!(*this))
			return true;
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
			if(aiskills(creatures))
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
	}
}

void creature::makemove() {
	const auto pc = StandartEnergyCost * 20;
	if(restore_hits > pc) {
		if(!is(Sick)) {
			if(hp < get(LifePoints))
				hp++;
		}
		restore_hits -= pc;
	} else
		restore_hits += get(LifeRate);
	if(restore_mana > pc) {
		if(mp < get(ManaPoints))
			mp++;
		restore_mana -= pc;
	} else
		restore_mana += get(LifeRate);
	if(restore_energy < StandartEnergyCost) {
		restore_energy += get(Speed);
		return;
	}
	if(is(Wounded)) {
		// Wounded creature loose 1 hit point each turn
		if(roll(Constitution, 5))
			add(Wounded, -1, true);
		else {
			act("%����� �������� ������.");
			damage(1, Slashing, 100, false);
			if(!(*this))
				return;
		}
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
	source.select();
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
	if(ei.skill.type==Skill && skills[ei.skill.value] > 0)
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

bool creature::match(variant id) const {
	switch(id.type) {
	case Race:
		if(getrace() != id.value)
			return false;
		break;
	case Skill:
		if(!skills[id.value])
			return false;
		break;
	case State:
		if(!is(state_s(id.value)))
			return false;
		break;
	case Spell:
		if(!spells[id.value])
			return false;
		break;
	}
	return true;
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
	itema source; source.selectb(*this);
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

bool creature::ismatch(variant v) const {
	switch(v.type) {
	case Alignment: return true;
	case Gender: return getgender() == v.value;
	case Race: return getrace() == v.value;
	case Role: return getrole() == v.value;
	case State:
		if(is((state_s)v.value))
			return true;
		break;
	case Spell:
		if(is((spell_s)v.value))
			return true;
		break;
	case Rarity:
		return d100() < bsmeta<rarityi>::elements[v.value].chance;
	}
	return false;
}

bool creature::ismatch(const creature& opponent, skill_s id, int value) const {
	auto r1 = get(id);
	auto r2 = value + opponent.get(Intellegence) * 2 - 20;
	return  r1 >= r2;
}

bool creature::ismatch(const creature& opponent, variant id) const {
	int r1, r2;
	switch(id.type) {
	case Ability:
		return get((ability_s)id.value) > opponent.get((ability_s)id.value);
	case Skill:
		r1 = get((skill_s)id.value);
		r2 = value + opponent.get((ability_s)id.value) * 2 - 20;
		return  r1 >= r2;
	default:
		return false;
	}
}

bool creature::ismatch(const creature& opponent, const varianta& source) const {
	for(unsigned i = 0; i < sizeof(source) / sizeof(source[0]); i++) {
		auto v = source[i];
		if(!v)
			break;
		// Test for skill with values
		if(i + 1 < sizeof(source) / sizeof(source[0])) {
			auto v1 = source[i + 1];
			if(v.type == Skill && v1.type == Number) {
				if(ismatch(opponent, (skill_s)v.value, v1.value)) {
					i++;
					continue;
				}
			}
		}
		if(ismatch(v))
			continue;
		if(ismatch(opponent, v))
			continue;
		return false;
	}
	return true;
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

bool creature::ask(const char* format, ...) {
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
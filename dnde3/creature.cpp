#include "main.h"

DECLDATA(creature, 256);

static creature*	current_player;
static char	int_checks[] = {2,
2, 2, 2, 2, 3, 3, 3, 3, 4,
4, 4, 5, 5, 6, 6, 6, 7, 7, 7,
8, 8, 9, 9, 9
};

void creature::clear() {
	memset(this, 0, sizeof(*this));
	charmer = horror = Blocked;
	guard = Blocked;
	location_id = Blocked;
	site_id = Blocked;
}

creature* creature::getactive() {
	return current_player;
}

short unsigned creature::getid() const {
	if(!this)
		return Blocked;
	return this - bsmeta<creature>::elements;
}

void creature::add(variant id, int v) {
	switch(id.type) {
	case Ability: abilities[id.value] += v; break;
	case Item: equip(item_s(id.value)); break;
	}
}

void creature::delayed(variant id, int modifier, unsigned rounds) {
	auto p = bsmeta<boosti>::add();
	p->id = id;
	p->modifier = modifier;
	p->time = rounds;
	p->owner = getid();
}

void creature::add(variant id, int v, unsigned time) {
	add(id, v);
	delayed(id, -v, time);
}

creature* creature::getobject(short unsigned v) {
	if(v == Blocked)
		return 0;
	return bsmeta<creature>::elements + v;
}

void creature::isolate() {
	auto id = getid();
	for(auto& e : bsmeta<creature>()) {
		if(!e)
			continue;
		if(e.charmer == id)
			e.charmer = 0;
		if(e.horror == id)
			e.horror = 0;
	}
}

void creature::dress(int m) {
	if(!this)
		return;
	abilities[AttackMelee] += m * (wears[Melee].getitem().weapon.attack + wears[Melee].getmagic() * 3);
	abilities[AttackRanged] += m * (wears[Ranged].getitem().weapon.attack + wears[Ranged].getmagic() * 4);
	for(auto i = Head; i <= Legs; i = (slot_s)(i + 1)) {
		if(!wears[i])
			continue;
		auto mi = wears[i].getmagic();
		auto& ei = wears[i].getitem();
		if(i != Melee && i != Ranged) {
			auto a = m * ei.weapon.attack;
			abilities[AttackMelee] += a;
			abilities[AttackRanged] += a;
		}
		abilities[Deflect] += m * (ei.armor.deflect + mi*ei.armor.multiplier);
		abilities[Armor] += m * ei.armor.armor;
	}
	// Apply calculating values
	abilities[AttackMelee] += m * (get(Strenght));
	abilities[AttackRanged] += m * (get(Dexterity));
	abilities[Deflect] += m * (get(Acrobatics) / 2);
	abilities[LifePoints] += m * (get(Constitution));
	abilities[ManaPoints] += m * (get(Intellegence) + get(Concetration) / 4);
}

dice_s creature::getraise(skill_s id) const {
	auto value = skills[id];
	if(value < 20)
		return D5n20;
	else if(value < 50)
		return D3n13;
	else
		return D1n6;
}

void creature::raise(skill_s value) {
	const auto& dice = bsmeta<dicei>::elements[getraise(value)];
	skills[value] += dice.roll();
}

void creature::equip(item it, slot_s id) {
	dress(-1);
	wears[id] = it;
	dress(1);
}

bool creature::add(item v, bool run) {
	// Second place item to backpack
	for(auto i = FirstBackpack; i <= LastBackpack; i = (slot_s)(i + 1)) {
		if(wears[i])
			continue;
		if(run)
			equip(v, i);
		return true;
	}
	return false;
}

bool creature::equip(item v) {
	// First try to dress this item
	for(auto i = Head; i <= Amunitions; i = (slot_s)(i + 1)) {
		if(!v.is(i))
			continue;
		if(wears[i])
			continue;
		equip(v, i);
		return true;
	}
	return add(v, true);
}

void creature::raiseskills(int number) {
	skill_s source[sizeof(skills) / sizeof(skills[0])];
	auto pb = source;
	for(auto i = Bargaining; i <= TwoWeaponFighting; i = (skill_s)(i + 1)) {
		if(skills[i])
			*pb++ = i;
	}
	unsigned count = pb - source;
	if(!count)
		return;
	zshuffle(source, count);
	unsigned index = 0;
	while(number >= 0) {
		if(index >= count)
			index = 0;
		raise(source[index++]);
		number--;
	}
}

static spell_s choose_spells(creature* p) {
	spell_s source[64];
	auto pb = source;
	auto pe = pb + sizeof(source) / sizeof(source[0]);
	for(auto e = FirstSpell; e <= LastSpell; e = (spell_s)(e + 1)) {
		if(p->get(e))
			continue;
		if(pb < pe)
			*pb++ = e;
	}
	auto count = pb - source;
	if(!count)
		FirstSpell;
	return source[rand() % count];
}

static void start_equipment(creature& e) {
	for(auto& ei : bsmeta<equipmenti>()) {
		if((!ei.race || ei.race == e.getrace()) && e.is(ei.type)) {
			for(auto ef : ei.features)
				e.add(ef, 1);
			break;
		}
	}
	e.equip(Ration);
	e.setmoney(e.getmoney() + xrand(3, 18)*GP);
}

void creature::applyabilities() {
	const auto& ri = bsmeta<racei>::elements[getrace()];
	const auto& ci = getclass();
	// Generate abilities
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1))
		abilities[i] = ri.abilities[i] + (rand() % 5) - 3;
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1))
		abilities[i] += ci.ability[i];
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1)) {
		if(abilities[i] < 0)
			abilities[i] = 0;
	}
	// Generate skills
	for(auto e : ri.skillvs)
		skills[e.id] += e.value;
	for(auto e : ri.skills)
		raise(e);
	for(auto e : ci.skills)
		raise(e);
	dresson();
}

void creature::create(race_s race, gender_s gender, class_s type) {
	clear();
	this->type = type;
	role = Character;
	abilities[Level] = 1;
	setname(race, gender);
	applyabilities();
	if(abilities[Intellegence] >= 9)
		raise(Literacy);
	for(auto e : getclass().spells)
		set(e, 1);
	start_equipment(*this);
	// Повысим навыки
	auto skill_checks = maptbl(int_checks, get(Intellegence));
	raiseskills(skill_checks);
	// Восполним хиты
	add(LifePoints, getclass().hp);
	add(ManaPoints, getclass().mp);
	// Generate name
	hp = get(LifePoints);
	mp = get(ManaPoints);
}

void creature::getfullname(stringbuilder& sb) const {
	sb.add(getname());
	sb.adds("%-3-%-1 %2i уровня", getstr(type), get(Level), getstr(getrace()));
}

attacki creature::getattack(slot_s id) const {
	attacki result = {0};
	auto& weapon = wears[id];
	if(weapon) {
		weapon.get(result);
		result.dice = bsmeta<dicei>::elements[result.damage];
		auto focus = weapon.getfocus();
		// RULE: Weapon focus add bonus to attack and damage
		if(focus && getbasic(focus)) {
			auto fs = get(focus);
			result.attack += fs / 5;
			result.dice.max += fs / 40;
		}
		// RULE: Versatile weapon if used two-handed made more damage.
		if(id == Melee && weapon.is(Versatile) && !wears[OffHand]) {
			result.dice.min += 1;
			result.dice.max += 1;
		}
	} else if(id == Melee) {
		result.type = Bludgeon;
		result.dice.min = 0;
		result.dice.max = 4;
	}
	if(!result.dice.max)
		return result;
	result.attack += 40; // Basic chance to hit
	switch(id) {
	case Melee:
	case OffHand:
		result.attack += get(Strenght);
		result.dice.max += (get(Strenght) - 10) / 2;
		break;
	case Ranged:
		result.attack += get(Dexterity);
		break;
	}
	return result;
}

int creature::getbasic(ability_s id) const {
	return abilities[id];
}

void creature::activate() {
	current_player = this;
}

creature* creature::getactive(int n) {
	auto p = bsmeta<creature>::elements + n;
	if(!(*p))
		return 0;
	return p;
}

const variant* creature::calculate(const variant* p, int& result) const {
	switch(p->type) {
	case Ability: result = get((ability_s)p->value); p++; break;
	case Skill: result = get((ability_s)p->value); p++; break;
	default: return 0;
	}
	while(p->type == Formula) {
		switch(p->value) {
		case Negative: result = -result; break;
		case Divide2: result = result / 2; break;
		case Divide3: result = result / 3; break;
		case Divide4: result = result / 4; break;
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

slot_s creature::getslot(const item* p) const {
	if(this
		&& p >= wears
		&& p < (wears + sizeof(wears) / sizeof(wears[0])))
		return slot_s(p - wears);
	return FirstBackpack;
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
	if(!remove(item, false))
		return;
	auto p = location::getactive();
	if(p) {
		p->drop(getposition(), item);
		remove(item, true);
	}
}

void creature::dropdown() {
	itema items; items.selectb(*this);
	auto pi = items.choose(true, "Выбросить предмет", 0, NoSlotName);
	if(pi)
		dropdown(*pi);
}

void creature::pickup() {
	itema items; items.select(getposition());
	auto pi = items.choose(true, "Поднять предмет", 0, NoSlotName);
	if(pi) {
		if(add(*pi, true))
			pi->clear();
	}
}

void creature::inventory() {
	while(true) {
		itema items; items.select(*this);
		auto pi = items.choose(true, "Инвенторий", 0, SlotName);
		if(!pi)
			break;
		auto slot = pi->getslot();
		if(*pi) {
			if(!remove(*pi, false)) {
				//say("Я не могу это снять.");
				continue;
			}
			if(!add(*pi, false)) {
				//say("У меня уже нету места.");
				continue;
			}
			add(*pi, true);
			remove(*pi, true);
		} else {
			if(slot >= Head && slot <= Amunitions) {
				items.clear();
				items.selectb(*this);
				items.match(slot);
				auto p2 = items.choose(true, "Рюкзак", 0, NoSlotName);
				if(p2) {
					dressoff();
					auto pc = *pi;
					*pi = *p2;
					*p2 = pc;
					dresson();
				}
			}
		}
	}
}

bool creature::remove(item& e, bool run) {
	if(!e)
		return false;
	if(run) {
		dress(-1);
		e.clear();
		dress(1);
	}
	return true;
}

int	creature::getweight() const {
	auto r = 0;
	for(auto& e : wears)
		r += e.getweight();
	return r;
}

int	creature::get(skill_s v) const {
	return skills[v]
		+ get(bsmeta<skilli>::elements[v].abilities[0])
		+ get(bsmeta<skilli>::elements[v].abilities[1]);
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
	skillu source(this);
	source.select(*this);
	source.sort();
	source.setcaps();
	source.choose(true, "Выбирайте навык");
}

void creature::cantmovehere() const {
	if(isactive())
		act("Сюда не пройти.");
}

void creature::move(indext index) {
	auto loc = location::getactive();
	if(!loc)
		return;
	if(index == Blocked) {
		cantmovehere();
		return;
	}
	auto d1 = location::getdirection(getposition(), index);
	if(d1 != Down && d1 != Up)
		direction = d1;
	if(loc->gettile(index) == Wall) {
		cantmovehere();
		return;
	}
	switch(loc->getobject(index)) {
	case Door:
		if(!loc->is(index, Opened)) {
			if(loc->is(index, Sealed))
				say("Здесь заперто.");
			else
				loc->set(index, Opened);
			wait(Round);
			return;
		}
		break;
	case Tree:
		cantmovehere();
		return;
	}
	auto p = find(index);
	if(p) {
		if(isenemy(p)) {
			meleeattack(p);
			return;
		} else if(!isactive()) {
			// Монстры и другие персонажи не меняются
			wait(xrand(Round, Minute));
			return;
		} else if(p->isguard()) {
			static const char* talk[] = {
				"Я охраняю это место.",
				"Здесь не пройти.",
				"Не толкайся, я не отойду.",
			};
			p->say(maprnd(talk));
			wait(xrand(1, 4));
			return;
		} else {
			// Игрок меняется позицией с неигроком
			auto psite = site::find(p->getposition());
			if(psite && psite->getowner() == p) {
				// С владельцами области не поменяешся местами
				static const char* talk[] = {
					"Не толкай меня в моем заведении.",
					"Держи руки на виду.",
					"Я за тобой слежу",
				};
				p->say(maprnd(talk));
				return;
			}
			p->setposition(getposition());
			p->wait(xrand(1, 4));
			if(d100() < 50) {
				static const char* talk[] = {
					"Эй! Не толкайся.",
					"Давай, проходи.",
					"Куда ты так спешишь?",
				};
				p->say(maprnd(talk));
			}
		}
	}
	setposition(index);
	wait(Round);
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

void creature::wait(int segments) {
	restore_action += segments;
}

void creature::meleeattack(creature* target, int bonus, int multiplier) {

}

bool creature::isenemy(const creature* target) const {
	return is(Hostile) != target->is(Hostile);
}

void creature::say(const char* format, ...) const {
	sayv(sb, format, xva_start(format));
}

void creature::makemove() {
	if(isactive()) {
		auto start = restore_action;
		while(start == restore_action) {
			location::setcamera(getposition());
			playui();
		}
	} else {

	}
}

bool creature::roll(skill_s skill, int bonus) const {
	auto r = get(skill) + bonus;
	return d100() < r;
}
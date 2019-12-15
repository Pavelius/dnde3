#include "main.h"

DECLDATA(creature, 256);

static char	int_checks[] = {2,
2, 2, 2, 2, 3, 3, 3, 3, 4,
4, 4, 5, 5, 6, 6, 6, 7, 7, 7,
8, 8, 9, 9, 9
};

void creature::clear() {
	memset(this, 0, sizeof(*this));
	charmer = horror = Blocked;
	guard = Blocked;
	location = Blocked;
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
}

dice_s creature::getraise(skill_s id) const {
	auto value = skills[id];
	if(value < 20)
		return D6n11;
	else if(value < 50)
		return D4n9;
	else
		return D2n7;
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

int	creature::getmaxhits() const {
	auto result = abilities[LifePoints];
	if(role == Character)
		result += get(Constitution);
	else
		result += get(Constitution) / 2;
	return result;
}

int	creature::getmaxmana() const {
	auto result = abilities[ManaPoints];
	result += get(Intellegence);
	result += get(Concetration) / 4;
	return result;
}

bool creature::equip(item v) {
	for(auto i = Head; i <= Amunitions; i = (slot_s)(i + 1)) {
		if(!v.is(i))
			continue;
		if(wears[i])
			continue;
		equip(v, i);
		return true;
	}
	for(auto i = Head; i <= Amunitions; i = (slot_s)(i + 1)) {

	}
	return false;
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
		abilities[i] = xrand(ri.ability_minimum[i], ri.ability_maximum[i]);
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1))
		abilities[i] += ci.ability[i];
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1)) {
		if(abilities[i] < 1)
			abilities[i] = 1;
	}
	// Generate skills
	for(auto e : ri.skillvs)
		skills[e.id] += e.value;
	for(auto e : ri.skills)
		raise(e);
	for(auto e : ci.skills)
		raise(e);
}

void creature::create(race_s race, gender_s gender, class_s type) {
	clear();
	this->race = race;
	this->type = type;
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
	abilities[LifePoints] = getclass().hp;
	abilities[ManaPoints] = getclass().mp;
	// Имя
	setname(race, gender);
	//updateweight();
	role = Character;
	hp = getmaxhits();
	mp = getmaxmana();
}
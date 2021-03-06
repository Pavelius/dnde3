#include "main.h"

static dicei skill_raise[] = {{3, 18}, {3, 12}, {2, 10}, {2, 8}, {2, 8}, {2, 6}, {1, 5}, {1, 4}, {1, 3}};

void statable::copy(statable* source) {
	memcpy(this, source, sizeof(*this));
}

void statable::set(variant i, int v) {
	if(v < 0)
		v = 0;
	switch(i.type) {
	case Ability: abilities[i.value] = v; break;
	case Harm:
		if(v > 0)
			resistance.set(i.value);
		else
			resistance.remove(i.value);
		break;
	case Skill: skills[i.value] = v; break;
	case Spell: spells[i.value] = v; break;
	case State:
		if(v > 0)
			states.set(i.value);
		else
			states.remove(i.value);
		break;
	}
}

int statable::get(variant i) const {
	switch(i.type) {
	case Ability: return abilities[i.value];
	case Harm: return resistance.is(i.value) ? 1 : 0;
	case Skill: return skills[i.value];
	case Spell: return spells[i.value];
	case State: return states.is(i.value) ? 1 : 0;
	default: return 0;
	}
}

int statable::getcap(skill_s v) const {
	return abilities[bsdata<skilli>::elements[v].ability];
}

dicei statable::getraise(skill_s v) const {
	auto n = skills[v] / 10;
	return maptbl(skill_raise, n);
}

void statable::raise(skill_s value) {
	skills[value] += getraise(value).roll();
}

void statable::apply(varianta source) {
	auto modifier = NoModifier;
	for(auto v : source) {
		switch(v.type) {
		case Modifier:
			modifier = (modifier_s)v.value;
			break;
		case Harm:
			switch(modifier) {
			case Immune: immunity.set(v.value); break;
			case Vulnerable: vulnerability.set(v.value); break;
			default: resistance.set(v.value); break;
			}
			break;
		case Skill:
			raise((skill_s)v.value);
			break;
		case State:
			states.set(v.value);
			break;
		default:
			switch(modifier) {
			case Hard: add(v, 5); break;
			default: add(v, 1); break;
			}
			break;
		}
	}
}

void statable::raise(role_s role, class_s type) {
	auto& ei = bsdata<classi>::elements[type];
	abilities[Attack] += ei.weapon.multiplier;
	if(abilities[Level] == 1 && role == Character) {
		abilities[LifePoints] += ei.hp;
		abilities[ManaPoints] += ei.mp;
	} else {
		if(ei.hp)
			abilities[LifePoints] += xrand(1, ei.hp);
		if(ei.mp)
			abilities[ManaPoints] += xrand(1, ei.mp);
	}
	for(auto& pi : bsdata<leveli>()) {
		if(pi.type == role && pi.level == abilities[Level])
			apply(pi.features);
	}
}

void statable::create(class_s type, race_s race) {
	const auto& ri = bsdata<racei>::elements[race];
	const auto& ci = bsdata<classi>::elements[type];
	// Create base abilities
	abilities[Level] = 0;
	abilities[Attack] += ci.weapon.base;
	// Generate abilities
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1))
		set(i, ri.abilities[i] + (rand() % 5) - 2);
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1))
		add(i, ci.ability[i]);
	apply(ri.bonuses);
	apply(ci.bonuses);
}
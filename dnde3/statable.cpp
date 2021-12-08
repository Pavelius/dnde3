#include "main.h"

static dicei skill_raise[] = {{3, 18}, {3, 12}, {3, 10}, {3, 8}, {2, 7}, {1, 6}, {1, 5}, {1, 4}, {1, 3}};

void statable::set(variant i, int v) {
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
	}
}

int statable::get(variant i) const {
	switch(i.type) {
	case Ability: return abilities[i.value];
	case Harm: return resistance.is(i.value) ? 1 : 0;
	case Skill: return skills[i.value];
	case Spell: return spells[i.value];
	default: return 0;
	}
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
	abilities[Level] = 0;
	abilities[Attack] += 50;
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1))
		abilities[i] += ri.abilities[i] + (rand() % 5) - 2;
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1))
		abilities[i] += ci.ability[i];
	apply(ri.bonuses);
	apply(ci.bonuses);
}

static void copy(statable & v1, const statable & v2) {
	v1 = v2;
}

void statable::update(const statable& source) {
	copy(*this, source);
}

void statable::update_finish() {
	auto level = get(Level);
	abilities[AttackMelee] += get(Strenght);
	abilities[AttackMelee] += get(Attack);
	abilities[AttackRanged] += get(Dexterity);
	abilities[AttackRanged] += get(Attack);
	abilities[DamageMelee] += getbonus(Strenght);
	abilities[DamageMelee] += get(Damage);
	abilities[DamageRanged] += get(Damage);
	abilities[Protection] += get(Dexterity) + get(Acrobatics) / 4;
	abilities[Speed] += 90 + get(Dexterity);
	abilities[Luck] += getbonus(Charisma);
	abilities[LifePoints] += imax(level, getbonus(Constitution) * level) + get(Athletics) / 5;
	abilities[LifeRate] += get(Healing) / 4;
	abilities[ManaPoints] += imax(level, getbonus(Wisdow) * level) + get(Concetration) / 2;
	abilities[ManaRate] += get(Concetration) / 4;
	abilities[FaithPoints] += 1 + get(Religion) / 10;
	abilities[Visibility] += 5;
}
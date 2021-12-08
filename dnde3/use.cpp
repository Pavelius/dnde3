#include "main.h"

bool item::isboost(variant id) const {
	// Simple case, when item have effect
	if(is(KnownPower)) {
		auto e = geteffect();
		if(e == id)
			return true;
	}
	if(id.type == Ability) {
		switch(id.value) {
		case LifePoints:
		case ManaPoints:
			return is(Edible);
		}
	}
	return false;
}

bool creature::use(const creaturea& creatures, item& it) {
	if(!it)
		return false;
	auto& ei = it.geti();
	variant effect = it.geteffect();
	variant skill = ei.skill;
	const char* current_string;
	switch(it.geti().slot) {
	case Edible:
		act("%����� ����%� %-1.", it.getname());
		if(d100() < it.getdamage() * 10) {
			act("��� ��������� ����������.");
			if(!resist(Poison, 0, false)) {
				if(d100() < 15)
					add(Sick, 1, true);
				else
					add(PoisonSpell, 1, true);
			}
		} else {
			for(auto& e : bsdata<foodi>()) {
				if(!e.match(this, it))
					continue;
				use(e, it, true);
				break;
			}
		}
		it.use();
		break;
	case Drinkable:
		act("%����� �����%� %-1.", it.getname());
		current_string = sb.get();
		if(d100() >= it.getdamage() * 10) {
			switch(effect.type) {
			case Ability:
				drink((ability_s)effect.value, true, it.getmagic(), 120);
				break;
			case Spell:
				use((spell_s)effect.value, *this, it.getbonus(), 0, true);
				break;
			}
		}
		if(current_string == sb.get())
			act("������ �� ���������.");
		it.use();
		break;
	case Zapable:
		if(effect.type == Spell) {
			if(use((spell_s)effect.value, it.getspelllevel(), &it, false))
				it.set(KnownPower);
			else
				act("%����� �������%� %-1 � ������%� ��������� ���. ������ �� ���������.", it.getname());
		}
		break;
	case Tool:
		if(skill.type == Ability) {
			auto ability_value = get((ability_s)skill.value) * 2;
			ability_value += it.getbonus() * 4;
			if(skill.value == Charisma) {
				// ����������� ����������
				if(mp <= 3) {
					info("�� ������� ����.");
					return false;
				}
				static const char* text[] = {"������� %����� ���������� � ����, ������� �������� ����� �� �������.",
					"%����� ��������� � ��������� �������� � ��� �� ������, ������� ����, �������� ����� �� ���� �, ����������, �� ����� ���������� �� ���, ���� �� �� ����, ��� ������ �������� ...",
					"�������� ��������� �������� �������, �������� �������, ��-��-��!!",
					"��� ��� ���! ���, �� ������!",
					"�� ���������� ��������! ��������-�����! �������� ����� ������ � ����� ���.",
					"��� �����, �� ������� ������, �� �� �������� ��� ��� � �����.",
				};
				say(maprnd(text));
				if(effect && (d100() < 60))
					ability_value += 20;
				else if(ei.effects.data && ei.effects.count) {
					if(ei.effects[0])
						effect = ei.effects[rand() % ei.effects.count];
					else
						effect = ei.effects[1 + rand() % (ei.effects.count - 1)];
				}
				if(effect) {
					creaturea creatures(*this);
					creatures.match(*this, Friendly, false, false);
					if(!creatures) {
						info("������ ��� [����������] ���������� �������, ������� ������ ������� ��� ������ �����������.");
						return false;
					}
					if(rollv(ability_value)) {
						auto index = 0;
						for(auto p : creatures) {
							if(effect.type == Ability)
								p->drink((ability_s)effect.value, true, Mundane, 10);
							else
								p->apply(*this, effect, 1, index, true);
							index++;
						}
					} else {
						auto p = creatures.random();
						static const char* text[] = {"������� ��� ���.",
							"����� �� �� ������.",
							"��� �� ���� �� ������?",
							"������ ���������!",
							"������! ������ ���� ���-��!",
						};
						p->say(maprnd(text));
					}
				}
				paymana(xrand(1, 3), false);
			}
		} else if(skill.type == Skill) {
			if(!skills[skill.value]) {
				info("�� �� �������� ������� [%1], ������� �� ������ ������������ ���� ����������.", getstr((skill_s)skill.value));
				return false;
			}
			auto ability_value = get((skill_s)skill.value);
			ability_value += it.getbonus() * 10;
			if(skill.value == Alchemy) {
				if(!recipes) {
					info("�� �� ������� �� ������ ������������� �������.");
					return false;
				}
				auto power = choosereceipt(isactive() ? "�� ������ ������� ������ ������� �����?" : 0);
				item it = craft(AlchemyPotion, power, Alchemy, ability_value);
				add(it, true, true);
				wait(60);
			}
		}
		break;
	}
	wait();
	return true;
}

void creature::use(const foodi& fi, const item it, bool interactive) {
	auto& ei = bsdata<itemi>::elements[it.getkind()];
	if(fi.hp)
		damage(-xrand(fi.hp / 2, fi.hp), Magic, 0, interactive);
	if(fi.mp)
		paymana(-xrand(fi.mp / 2, fi.mp), interactive);
	auto chance = fi.chance_sick + it.getdamage() * 5;
	if(chance > 0) {
		if(d100() < chance)
			add(Sick, 1, interactive);
	} else {
		if(d100() < -chance)
			add(Sick, -1, interactive);
	}
	chance = fi.chance_poison + it.getdamage() * 15;
	if(chance > 0) {
		if(d100() < chance)
			add(Poisoned, 1, true);
	} else {
		if(d100() < -chance)
			add(Poisoned, -1, true);
	}
}

void creature::drink(ability_s id, bool interactive, item_type_s magic, int minutes) {
	switch(id) {
	case LifePoints: case ManaPoints:
		switch(magic) {
		case Artifact: add(id, 5, interactive); break;
		case Cursed: add(id, -1, interactive); break;
		case Blessed: add(id, 1, interactive); break;
		default:
			if(id == LifePoints)
				damage(-xrand(12, 36), Magic, 0, interactive);
			else
				paymana(-xrand(12, 36), interactive);
			break;
		}
		break;
	case Level:
		switch(magic) {
		case Artifact: addexp(20000, interactive); break;
		case Cursed: addexp(-5000, interactive); break;
		case Blessed: addexp(5000, interactive); ; break;
		default: addexp(500, interactive); break;
		}
		break;
	case Attack: case Protection: case Deflect:
		switch(magic) {
		case Artifact: add(id, 5, interactive); break;
		case Cursed: add(id, -1, interactive); break;
		case Blessed: add(id, 1, interactive); break;
		default: add(id, xrand(6, 24), interactive, minutes); break;
		}
		break;
	default:
		switch(magic) {
		case Artifact: add(id, 2, interactive); break;
		case Cursed: add(id, -1, interactive); break;
		case Blessed: add(id, 1, interactive); break;
		default: add(id, xrand(1, 6), interactive, minutes); break;
		}
		break;
	}
}

bool item::apply(creature& player, variant id, int v, int order, bool run) {
	switch(id.type) {
	case ItemType:
		if(getmagic() == id.value)
			return false;
		if(run)
			set((item_type_s)id.value);
		break;
	case ItemIdentify:
		if(is((identify_s)id.value))
			return false;
		if(run)
			set((identify_s)id.value);
		break;
	case Spell:
		return use((spell_s)id.value, player, v, order, run);
	case Skill:
		return use((skill_s)id.value, player, order, run);
	}
	return true;
}

bool creature::apply(creature& player, variant id, int v, int order, bool run) {
	switch(id.type) {
	case Harm:
		if(run)
			damage(v, (damage_s)id.value, 0, true);
		break;
	case State:
		if(v >= 0) {
			if(is((state_s)id.value))
				return false;
		} else {
			if(!is((state_s)id.value))
				return false;
		}
		if(run)
			add(id, v, true);
		break;
	case Spell: return use((spell_s)id.value, player, v, order, run);
	case Skill: return use((skill_s)id.value, player, order, run);
	default: return false;
	}
	return true;
}

void creature::add(const effecti& e) {
	act(e.text);
	if(e.id.type == Ability) {
		auto id = (ability_s)e.id.value;
		switch(id) {
		case LifePoints:
			damage(xrand(e.value / 2, e.value), Magic, 100, true);
			break;
		case ManaPoints:
			paymana(xrand(e.value / 2, e.value), true);
			break;
		case Strenght: case Dexterity: case Constitution:
		case Intellegence: case Wisdow: case Charisma:
			if(e.permanent)
				add(id, e.value, true);
			else
				add(id, e.value, true, xrand(60, 180));
			break;
		default:
			add(id, e.value, true, xrand(120, 240));
			break;
		}
	} else
		add(e.id, e.value, true);
}

void creature::useobject() {
	indexa indecies;
	indecies.select(getposition(), 1);
	indecies.matchobj(false);
	auto i = indecies.choose(isactive() ? "��� ������������ �� ���� ��� �����?" : 0);
	if(i == -1)
		return;
	use(indecies[i], false);
}
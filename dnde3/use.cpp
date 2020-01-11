#include "main.h"

bool item::isboost(variant id) const {
	// Simple case, when item have effect
	if(is(KnownPower)) {
		auto e = geteffect();
		if(e == id)
			return true;
	}
	// Check for food
	for(auto& e : bsmeta<foodi>()) {
		if(e.type != type)
			continue;
		if(id.type == Ability) {
			if(id.value == LifePoints && e.hp > 0)
				return true;
			if(id.value == ManaPoints && e.mp > 0)
				return true;
		}
	}
	return false;
}

bool creature::use(item& it, bool interactive) {
	variant effect;
	bool consume = true;
	switch(it.getitem().slot) {
	case Edible:
		act("%����� ����%� %-1.", it.getname());
		for(auto& e : bsmeta<foodi>()) {
			if(!e.match(this, it))
				continue;
			use(e, it, interactive);
			break;
		}
		break;
	case Drinkable:
		act("%����� �����%� %-1.", it.getname());
		effect = it.geteffect();
		if(effect.type == Ability)
			potion((ability_s)effect.value, it.getkind(), true, it.getmagic(), it.getquality(), 120);
		else {
			if(interactive)
				act("������ �� ���������.");
		}
		break;
	case Zapable:
		consume = false;
		if(true) {
			auto v = it.geteffect();
			if(v.type == Spell) {
				auto level = it.getbonus();
				if(level > 0) {
					if(cast((spell_s)v.value, level, &it))
						it.set(KnownPower);
					else
						act("%����� �������%� %-1 � ������%� ��������� ���. ������ �� ���������.", it.getname());
				} else {
					it.act("%����� ���������%��� � ���� � ��������%���.");
					consume = true;
				}
			}
		}
		break;
	}
	if(consume)
		it.destroy();
	wait();
	return true;
}

bool creature::use(creaturea& source, skill_s id) {
	auto v = get(id);
	if(v <= 0)
		return false;
	auto& ei = bsmeta<skilli>::elements[id];
	creaturea creatures = source; itema items; indexa indecies;
	if(!ei.target.prepare(*this, creatures, items, indecies, id, get(id))) {
		if(isactive()) {
			switch(id) {
			case Literacy:
				sb.add("� ������� ���� ������, ��� ����� ��������.");
				break;
			default:
				sb.add("�� ���� ����� ���������� ����.");
				break;
			}
		}
		return false;
	}
	ei.target.use(*this, source, creatures, items, indecies, id, v);
	// Appear when do some activity
	if(is(Invisible)) {
		if(ei.target.range != You)
			appear();
	}
	wait();
	return true;
}

bool item::use(spell_s id, creature& player, int level, int order, bool run) {
	switch(id) {
	case BlessItem:
		if(!is(Mundane) || is(Readable))
			return false;
		if(run) {
			set(Blessed);
			set(KnownMagic);
			act("%����� ��������%��� ������� ������� ���������.");
		}
		break;
	case DetectMagic:
		if(is(KnownMagic) || !is(Blessed))
			return false;
		if(run) {
			set(KnownMagic);
			act("%����� ��������%��� ����� ������.");
		}
		break;
	case DetectEvil:
		if(is(KnownMagic) || !is(Cursed))
			return false;
		if(run) {
			set(KnownPower);
			act("%����� ��������%��� [-�������] ������.");
		}
		break;
	case Identify:
		if(is(KnownPower))
			return false;
		if(run) {
			act("%����� ��������%��� ������ ���������.");
			set(KnownPower);
			if(player.isactive()) {
				char temp[260]; stringbuilder st(temp); getname(st, true);
				sb.adds("��� [%-1].", temp);
			}
		}
		break;
	case Repair:
		if(!isdamaged() || getdamage() > level)
			return false;
		if(run) {
			damaged--;
			act("%����� ��������� ������� ��������%� ������������ �����.");
		}
		break;
	default:
		return false;
	}
	return true;
}

bool item::use(skill_s id, creature& player, int order, bool run) {
	switch(id) {
	case Literacy:
		if(!is(Readable))
			return false;
		if(run) {
			auto v = geteffect();
			auto player_rang = 0;
			auto level = getbonus();
			auto consume = is(SingleUse);
			auto b = 0;
			if(is(Unknown))
				b += 10;
			if(is(SingleUse))
				b += 35 - level * 5;
			else {
			}
			auto result = player.roll(Literacy, b);
			if(is(Unknown)) {
				player.act("%����� ����������� ������%� %-1.", getname());
				if(result) {
					player.addexp(10);
					set(KnownPower);
					if(player.isactive()) {
						char temp[260]; stringbuilder st(temp); getname(st, true);
						sb.adds("��� ������� ������, ��� ��� [%-1].", temp);
					}
				} else {
					if(player.isactive())
						player.act("������, ��� ��� � �� ������� ������ ������.");
				}
				consume = false;
			} else {
				switch(v.type) {
				case Spell:
					if(is(SingleUse)) {
						if(is(Blessed) || is(Artifact))
							result = true;
						if(!result || !player.cast((spell_s)v.value, level, this))
							player.act("%����� �������%� %-1 � ������ ��������%�.", getname());
					} else {

					}
					break;
				}
			}
			if(consume) {
				act("%����� ���������%��� � ���� � ��������%���.");
				clear();
			}
		}
		break;
	}
	return true;
}

bool creature::use(spell_s id, creature& player, int level, int order, bool run) {
	if(finds(id))
		return false; // Not allow two spells be effected
	auto& ei = bsmeta<spelli>::elements[id];
	switch(id) {
	case ArmorSpell:
		if(run) {
			add(Armor, id, 1, false, 30 * level);
			act("%����� ������%��� ����� �������.");
		}
		break;
	case BlessSpell:
		if(run) {
			add(Attack, id, level * 5, false, 30);
			add(Damage, id, 2, false, 30);
			act("%����� �������%� ��������� ������ ���.");
		}
		break;
	case CharmPerson:
		if(ismatch(Animal) || get(ResistCharm) >= 100)
			return false;
		if(run) {
			if(charmresist())
				return false;
			if(player.is(Hostile))
				add(Hostile, 1, false);
			else
				add(Hostile, -1, false);
			setguard(Blocked);
			say("%1, ���� ���, � ���� ������!", player.getname());
		}
		break;
	case FearSpell:
		if(get(ResistCharm) >= 100)
			return false;
		if(run) {
			if(charmresist())
				return false;
			add(Fear, 1, true);
		}
		break;
	case HealingSpell:
		if(hp >= get(LifePoints))
			return false;
		if(run)
			damage(-level, Magic, 0, true);
		break;
	case Invisibility:
		if(run) {
			add(Invisible, 1, true);
			add(id, level * 30);
		}
		break;
	case LightSpell:
		if(run) {
			add(Visibility, id, 1, true, level * 60);
			act("����� %����� ��������� ��������� ��������� ����� �����.");
		}
		break;
	case Poison:
		if(run) {
			if(roll(ResistPoison)) {
				act("%����� �������%�� ������ ��� ��� �����������.");
				return false;
			}
			add(id, 20 * level);
			act("%����� ������������%� ����������.");
		}
		break;
	case ShieldSpell:
		if(run) {
			add(Protection, id, 20 * level, false, 20);
			act("������ %����� ��������� �������� ����.");
		}
		break;
	case SlowMonster:
		if(run)
			add(Movement, id, level, true, 60);
		break;
	default:
		if(!ei.bonus)
			return false;
		return apply(player, ei.bonus, level, order, run);
	}
	return true;
}

void creature::suffer(spell_s id) {
	switch(id) {
	case Poison:
		if(roll(ResistPoison))
			return;
		damage(1, Magic, 100, false);
		act("%����� �������� �� ���.");
		break;
	}
}

bool creature::use(skill_s id, creature& player, int order, bool run) {
	auto& ei = bsmeta<skilli>::elements[id];
	switch(id) {
	case HideInShadow:
		if(is(Invisible))
			return false;
		if(run) {
			if(!player.roll(id)) {
				if(isactive())
					sb.add("������� �� �������.");
				wait(xrand(1, 4));
				return false;
			}
			add(Invisible, 1, true);
		}
		break;
	default: return false;
	}
	return true;
}

void creature::use(const foodi& fi, const item it, bool interactive) {
	auto& ei = bsmeta<itemi>::elements[it.getkind()];
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
			use(Poison, *this, 1, 0, true);
	} else {
		if(d100() < -chance)
			dispell(Poison, true);
	}
}

void creature::potion(ability_s id, variant source, bool interactive, item_type_s magic, int quality, int minutes) {
	int v;
	switch(id) {
	case LifePoints: case ManaPoints:
		v = xrand(2, 12);
		switch(magic) {
		case Artifact: add(id, (1 + quality) * 3, interactive); break;
		case Cursed: add(id, -(1 + quality), interactive); break;
		case Blessed:
			if(id == LifePoints)
				damage(-4 * (v + quality), Magic, 0, interactive);
			else
				paymana(-4 * (v + quality), interactive);
			break;
		default:
			if(id == LifePoints)
				damage(-(v + quality), Magic, 0, interactive);
			else
				paymana(-(v + quality), interactive);
			break;
		}
		break;
	case Attack: case Protection: case Deflect:
		v = xrand(6, 24);
		switch(magic) {
		case Artifact: add(id, (1 + quality) * 2, interactive); break;
		case Cursed: add(id, source, -v, interactive, 5 * (minutes + minutes*quality)); break;
		case Blessed: add(id, source, v, interactive, 5 * (minutes + minutes*quality)); break;
		default: add(id, source, v, interactive, minutes + minutes*quality); break;
		}
		break;
	default:
		v = 1;
		switch(magic) {
		case Artifact: add(id, v + quality, interactive); break;
		case Cursed: add(id, -(v + quality), interactive); break;
		case Blessed: add(id, source, v + quality, interactive, 5 * minutes); break;
		default: add(id, source, v + quality, interactive, minutes); break;
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
	case Spell: return use((spell_s)id.value, player, v, order, run);
	case Skill: return use((skill_s)id.value, player, order, run);
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

bool creature::cast(spell_s id, int level, item* magic_source) {
	creaturea creatures(*this);
	return cast(creatures, id, level, magic_source);
}

bool creature::cast(creaturea& source, spell_s id, int level, item* magic_source) {
	auto& ei = bsmeta<spelli>::elements[id];
	if(magic_source) {
		if(magic_source->ischargeable() && magic_source->getcharges() <= 0)
			return false;
	} else {
		if(mp < ei.mp)
			return false;
	}
	variant effect = id;
	auto v = ei.dice.roll();
	if(ei.multiplier)
		v += level*ei.multiplier;
	else
		v += level;
	creaturea creatures = source;
	itema items;
	indexa indecies;
	if(!ei.target.prepare(*this, creatures, items, indecies, id, v))
		return false;
	if(magic_source) {
		if(ei.target.type == Creature && ei.target.range != You)
			act("%����� ��������%� %-1 ����� �����.", magic_source->getname());
		else if(magic_source->is(Readable))
			act("%����� ������%� %-1 � ������ ��������%�.", magic_source->getname());
		else
			act("%����� ������%� %-1 � ��������� ��������� ���.", magic_source->getname());
	} else
		act("%����� �������%� ��������� �������.");
	if(ei.throw_text) {
		if(magic_source)
			act("� ������� %1.", ei.throw_text);
		else
			act("� ������� ������ %1.", ei.throw_text);
	}
	ei.target.use(*this, source, creatures, items, indecies, id, v);
	if(magic_source) {
		if(magic_source->ischargeable())
			magic_source->usecharge();
	} else
		paymana(ei.mp, false);
	return true;
}
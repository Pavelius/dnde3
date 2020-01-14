#include "main.h"

static const char* talk_subjects[] = {"������", "��������", "������", "�������", "�������", "�������", "�������", "���� ������"};
static const char* talk_object[] = {"���������", "��������� ������", "��������� ��������", "����", "�������� ������", "���������� ����������"};
static const char* talk_location[] = {"����������", "������", "�������", "�������", "����"};
static const char* talk_games[] = {"������", "�����", "���������", "������"};

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

bool creature::use(item& it) {
	variant effect;
	const char* current_string;
	switch(it.getitem().slot) {
	case Edible:
		act("%����� ����%� %-1.", it.getname());
		if(d100() < it.getdamage() * 10) {
			act("��� ��������� ����������.");
			if(!roll(ResistPoison)) {
				if(d100() < 15)
					add(Sick, 1, true);
				else
					add(Poison, 1, true);
			}
		} else {
			for(auto& e : bsmeta<foodi>()) {
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
		effect = it.geteffect();
		current_string = sb.get();
		if(d100() >= it.getdamage() * 8) {
			if(effect.type == Ability)
				potion((ability_s)effect.value, it.getkind(), true, it.getmagic(), it.getquality(), 120);
		}
		if(current_string == sb.get())
			act("������ �� ���������.");
		it.use();
		break;
	case Zapable:
		if(true) {
			auto v = it.geteffect();
			if(v.type == Spell) {
				auto level = it.getbonus();
				if(level > 0) {
					if(use((spell_s)v.value, level, &it, false))
						it.set(KnownPower);
					else
						act("%����� �������%� %-1 � ������%� ��������� ���. ������ �� ���������.", it.getname());
				} else
					it.destroy(Magic, true);
			}
		}
		break;
	}
	wait();
	return true;
}

bool creature::use(creaturea& source, skill_s id) {
	auto v = get(id);
	if(v <= 0)
		return false;
	auto pu = isusedisable(id);
	if(pu) {
		if(isactive())
			sb.add(pu, getstr(id));
		return false;
	}
	auto& ei = bsmeta<skilli>::elements[id];
	creaturea creatures = source; itema items; indexa indecies;
	if(!ei.target.prepare(*this, creatures, items, indecies, id, get(id), true))
		return false;
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
			auto level = getbonus();
			auto b = (getitem().quality + level) * 3;
			if(is(Unknown))
				b += 10;
			if(is(Blessed))
				b += 10;
			else if(is(Artifact))
				b += 25;
			if(is(SingleUse))
				b += 35;
			else {
				auto effect = geteffect();
				auto player_rang = 0;
				switch(effect.type) {
				case Spell:
					player_rang = player.get((spell_s)effect.value);
					b -= player_rang * 12;
					break;
				}
			}
			auto result = player.roll(Literacy, b);
			if(is(Unknown)) {
				player.act("%����� ����������� ������%� %-1.", getname());
				if(result) {
					player.addexp(10);
					set(KnownPower);
					if(player.isactive()) {
						char temp[260]; stringbuilder st(temp); getname(st, true);
						sb.adds("��� ������� ������, ��� ��� [%+1].", temp);
					}
				} else {
					if(player.isactive())
						player.act("������, ��� ��� � �� ������� ������ ������.");
				}
			} else {
				switch(v.type) {
				case Spell:
					if(is(SingleUse)) {
						if(is(Blessed) || is(Artifact))
							result = true;
						if(!result || !player.use((spell_s)v.value, level, this, false))
							player.act("%����� �������%� %-1 � ������ ��������%�.", getname());
						destroy(Magic, true);
					} else {
						if(!player.ask("������ ������ ��������������� �����. ������������� ������ ����������?"))
							return false;
						player.act("%����� ������%� %-1 � �����%��� �������.", getname());
						player.wait(60);
						if(result)
							player.add(v, 1, true);
						else {
							static effecti read_books[] = {
								{{}, 0, "����� ��� ��� ���� �������� �� �� ������. ����� ����� ����������� ���������� ��� ���?"},
								{{}, 0, "�� ��������� ��������� �������� �������. ������� �� ����� �������? ����� ����."},
								{ManaPoints, -3, "����� ��������� ������������, � ������ ��������� ������� ������������. �� ���� ������� �����������."},
								{ManaPoints, -4, "� ����� ���� ������� ��������� ������� ����� � �� ���������."},
							};
							player.add(maprnd(read_books), getkind());
						}
						use();
					}
					break;
				}
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
	case ChatPerson:
		if(run)
			player.chat(*this);
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
	case RemovePoisonSpell:
		if(!is(Poison))
			return false;
		if(run)
			dispell(Poison, true);
		break;
	case RemoveSickSpell:
		if(!is(Sick))
			return false;
		if(run)
			add(Sick, -1, true);
		break;
	case SickSpell:
		if(is(Sick))
			return false;
		if(run) {
			if(roll(ResistPoison, 10 - level * 5)) {
				act("%����� ������������%� �������.");
				return false;
			}
			add(Sick, 1, true);
		}
		break;
	case ShieldSpell:
		if(run) {
			add(Protection, id, 20 * level, false, 20);
			act("������ %����� ��������� �������� ����.");
		}
		break;
	case Sleep:
		if(get(ResistCharm) >= 100 || is(Sleep))
			return false;
		if(run) {
			if(charmresist(15 * order))
				return false;
			add(Sleep, xrand(2 * level, 8 * level));
			act("%����� �������� ������%�.");
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
	case Gambling:
		if(!skills[Gambling] || loc.getlight() <= -1)
			return false;
		if(run) {
			auto stack = 100 * (1 + player.get(Gambling) / 20);
			if(player.getmoney() < stack) {
				player.say("��� [%2i] �����, ������� � %1.", maprnd(talk_games), stack);
				return false;
			}
			player.say("����� ������� � %1?", maprnd(talk_games));
			if(getmoney() < stack) {
				say("���. � �� ����. � ������ ���.");
				return false;
			}
			say("���� ��������. � ������ ��.");
			auto result = rollv(player.get(Gambling), get(Gambling));
			if(result > 0) {
				player.setmoney(player.getmoney() + stack);
				setmoney(getmoney() - stack);
				player.act("%����� �������%� [%1i] �����.", stack);
				player.addexp(10);
			} else if(result < 0) {
				player.setmoney(player.getmoney() - stack);
				setmoney(getmoney() + stack);
				player.act("%����� � ��������� ��������%� [%1i] �����.", stack);
			} else
				player.act("���� �� �����. ��� �������� ��� �����.");
		}
		break;
	case PickPockets:
		if(getmoney() <= 20 || loc.getlight() <= -1)
			return false;
		if(run) {
			static const char* talk[] = {
				"������ ������� �������. ��� ... ��� �� �� ����������? ... �����. �����, ����� � ������ ��� ��������.",
				"� - ������ ��� ���?",
				"�� �� ������ ��� ������ � %3? �, �������, � ��� �������� ������.",
				"������ ��� ������� ��� %1 � %2? ���? � ����..."
			};
			player.say(maprnd(talk), maprnd(talk_subjects), maprnd(talk_object), maprnd(talk_location));
			if(player.roll(PickPockets)) {
				auto count = xrand(3, 18)*(1 + player.get(PickPockets) / 5);
				if(count > getmoney())
					count = getmoney();
				setmoney(getmoney() - count);
				player.setmoney(player.getmoney() + count);
				player.act("%����� �����%� [%1i] �����.", count);
				player.addexp(100);
			} else {
				static const char* talk[] = {
					"�� �� ������ ���! �� ���� �� �����!",
					"�� ��� ������! ���� �����! ��, �������!",
					"�� ���. ��� ����, �������, ������ ����� ��� ��������!"
				};
				say(maprnd(talk));
				player.damage(xrand(1, 3), Bludgeon, 100, true);
			}
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
		if(v < 2)
			v = 2;
		v += quality;
		switch(magic) {
		case Artifact: add(id, (1 + quality) * 3, interactive); break;
		case Cursed: add(id, -(1 + quality), interactive); break;
		case Blessed:
			if(id == LifePoints)
				damage(-3 * v, Magic, 0, interactive);
			else
				paymana(-3 * v, interactive);
			break;
		default:
			if(id == LifePoints)
				damage(-v, Magic, 0, interactive);
			else
				paymana(-v, interactive);
			break;
		}
		break;
	case Level:
		v = xrand(1, 6);
		if(v < 1)
			v = 1;
		v += quality;
		switch(magic) {
		case Artifact: v *= 5000; break;
		case Cursed: v *= -1000; break;
		case Blessed: v *= 1000; break;
		default: v *= 250; break;
		}
		addexp(v, interactive);
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
		v = 1 + quality;
		switch(magic) {
		case Artifact: add(id, v, interactive); break;
		case Cursed: add(id, -v, interactive); break;
		case Blessed: add(id, source, v, interactive, 5 * minutes); break;
		default: add(id, source, v, interactive, minutes); break;
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

bool creature::use(spell_s id, int level, item* magic_source, bool show_errors) {
	creaturea creatures(*this);
	return use(creatures, id, level, magic_source, show_errors);
}

bool creature::use(creaturea& source, spell_s id, int level, item* magic_source, bool show_errors) {
	if(!(*this))
		return true;
	auto& ei = bsmeta<spelli>::elements[id];
	if(mp < ei.mp) {
		if(isactive())
			act("�� ������� ����.");
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
	if(!ei.target.prepare(*this, creatures, items, indecies, id, v, show_errors))
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
			magic_source->use();
	} else
		paymana(ei.mp, false);
	return true;
}

void creature::closedoor() {
	indexa indecies;
	indecies.select(getposition(), 1);
	indecies.match(Door, false);
	indecies.match(Opened, false);
	auto i = indecies.choose(isactive() ? "��� ������������ �� ���� ��� �����?" : 0);
	if(i == -1)
		return;
	loc.remove(indecies[i], Opened);
}

void creature::add(const effecti& e, item_s source) {
	act(e.text, getstr(source));
	if(e.id.type == Ability) {
		auto id = (ability_s)e.id.value;
		switch(id) {
		case LifePoints:
			damage(xrand(e.value / 2, e.value), Magic, 100, true);
			break;
		case ManaPoints:
			paymana(xrand(e.value / 2, e.value), true);
			break;
		case Strenght: case Dexterity: case Constitution: case Intellegence: case Wisdow: case Charisma:
			if(e.permanent)
				add(id, e.value, true);
			else
				add(id, source, e.value, true, xrand(60, 180));
			break;
		default:
			add(id, source, e.value, true, xrand(120, 240));
			break;
		}
	} else
		add(e.id, e.value, true);
}

const char* creature::isusedisable(skill_s id) const {
	auto& ei = bsmeta<skilli>::elements[id];
	switch(id) {
	case Healing: return "������������ ������������� �������������� 1 ���� [�����] �� ������������ ���������� �������.";
	case Concetration: return "������������ ������������� �������������� 1 ���� [����] �� ������������ ���������� �������.";
	default:
		if(ei.isweapon())
			return "����� �������� ������� ������ �� ���� [���������], ��������� [����], [��������] ���������� ����� � ������������ ������������� ��� ��� ���������.";
		if(!ei.target)
			return "���� ����� ������������ �������������.";
		return 0;
	}
}
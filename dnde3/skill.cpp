#include "main.h"

static const char* talk_subjects[] = {"������", "��������", "������", "�������", "�������", "�������", "�������", "���� ������"};
static const char* talk_object[] = {"���������", "��������� ������", "��������� ��������", "����", "�������� ������", "���������� ����������"};
static const char* talk_location[] = {"����������", "������", "�������", "�������", "����"};
static const char* talk_games[] = {"������", "�����", "���������", "������"};
static item_s common_plants[] = {Tarnelle, Cherry, RoseHip, Physalis};

BSDATA(skilli) = {{"��������", "��������", Charisma, {}, {Creature, {NotYou}, Close, "� ��� ������ �������������?"}},
	{"����", "������", Charisma, {}, {Creature, {NotYou}, Close, "���� ������ ����������?"}},
	{"����������", "����������", Charisma, {}, {Creature, {NotYou}, Close, "� ��� ������ ����� ����������?"}},
	{"����������", "����������", Dexterity},
	{"��������������", "��������������", Wisdow},
	{"��������", "��������", Strenght, {}, {Object, {}, Close, "����� ����� ������?"}},
	{"��������", "��������", Dexterity, {}, {Creature, {NotYou, Friends}, Close, "���� �����?"}},
	{"�������", "�������", Strenght},
	{"������������", "�����������", Wisdow},
	{"����������� �������", "�������", Dexterity, {}, {Object, {}, Close, "����� ������� �����������?"}},
	{"���� � �������� �����", "������ ������", Dexterity},
	{"������� �����", "�����", Wisdow},
	{"��������� � ����", "����������", Dexterity, {}, {Creature}},
	{"������� �����", "������", Dexterity, {}, {Object, {}, Close, "����� ����� �������?"}},
	{"��������� ����", "������ ��������", Dexterity},
	{"�������� �������", "���������", Dexterity, {}, {Creature, {NotYou}, Close, "���� ����������?"}},
	{"�������", "�������", Intellegence, {}, {Item, {LongAction, AlwaysChoose}, You, "����� ����� ������ �����������?"}},
	{"������� ���", "�����������", Charisma, {}, {Item, {AlwaysChoose}, You, "��� ������ �����������?"}},
	{"�����", "������", Dexterity, {}, {Creature, {NotYou, AllTargets}, Near}},
	{"���������� ����", "���������", Intellegence},
	{"�������� ����", "�������� ���", Charisma, {}, {Creature, {Friends, NotYou}, Close, "� ��� ��������?"}},
	{"�������", "�������", Intellegence, {}, {Object, {}, Reach, "����� ������ �������?"}},
	{"��������� ���", "��������", Wisdow},
	{"�������������", "�����������", Wisdow, {}, {Object, {}, Close, "��� ������� ������?"}},
	{"�����������", "������ � ������", Intellegence, {}, {Item, {LongAction, AlwaysChoose}, You, "��� ������ ���������?"}},
	{"���������� ����", "����������� ����", Strenght, {}, {Object, {LongAction}, Close, "��� �������� ����?"}},
	{"�������", "�������", Wisdow, {}, {Creature, {}, You}},
	{"���� ������", "���� ������", Dexterity},
	{"��������� ����", "���������� ����", Strenght, {}, {Item, {LongAction}, You, "��� ������ ���������������?"}},
	{"���������", "���������", Wisdow},
	{"��������", "��������", Strenght},
	{"�������� �����", "�������� �� ����", Dexterity, {3, 30, 30}},
	{"�������� �����", "�������� �� �����", Strenght, {4, 20, 25}},
	{"�������� �������", "�������� �� �������", Strenght, {5, 15, 30}},
	{"�������� ��������� �������", "���������� ������", Strenght, {4, 10, 35}},
	{"�������� ����� ��������", "�������� ������", Dexterity, {5, 30, 15}},
};
assert_enum(skilli, LastSkill)

skill_s	skilli::getid() const {
	return skill_s(this - bsdata<skilli>::elements);
}

bool creature::use(skill_s id, creature& player, int order, bool run) {
	site* pst;
	auto& ei = bsdata<skilli>::elements[id];
	switch(id) {
	case HideInShadow:
		if(is(Invisible))
			return false;
		if(run) {
			if(!player.roll(id)) {
				if(isactive())
					sb.add("������� �� �������.");
				wait(CoupleMinutes);
				return false;
			}
			add(Invisible, 1, true);
		}
		break;
	case Diplomacy:
		pst = getsite();
		if(pst) {
			if(pst->getowner() != this)
				return false;
			if(!execute(MakeDiscount, false))
				return false;
			if(run) {
				if(!player.roll(id)) {
					static const char* talk[] = {"��, �� ����� ����.", "��, ������ �����������.", "�����, �����, �������."};
					say(maprnd(talk));
				} else
					execute(MakeDiscount, true);
			}
		} else {

		}
		break;
	case Dancing:
		if(is(Hostile) || is(Friendly) || mood < 0 || !is(LowInt))
			return false;
		if(run) {
			if(player.roll(id)) {
				static const char* talk[] = {"�������.", "�������!", "�������, ����� ��� ������!", "�����!"};
				say(maprnd(talk));
				mood += xrand(2, 4);
			} else if(d100() < 50) {
				static const char* talk[] = {"����.", "�������, ���!", "��� � ���� ��� ���������?", "����� �������."};
				say(maprnd(talk));
				mood -= xrand(1, 3);
			}
			wait(Minute);
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
	case Religion:
		if(!faith)
			return false;
		if(!pray(false))
			return false;
		if(run) {
			if(roll(Religion)) {
				act("%����� �������%��� ���� � ��������� ����.");
				pray(true);
			} else
				act("%����� �������%��� ����, �� ������� �� ���� ��������.");
			faith--;
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
				auto count = xrand(3, 18) * (1 + player.get(PickPockets) / 5);
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

bool item::use(skill_s id, creature& player, int order, bool run) {
	switch(id) {
	case Alchemy:
		if(!is(Drinkable) || is(KnownPower))
			return false;
		if(run) {
			if(!player.roll(Alchemy)) {
				player.act("%����� ��������%� %1, �� ��� � �� ����%�� ������ ��� ���.", getname());
				player.fail(Alchemy);
				return false;
			}
			set(KnownPower);
			char temp[260]; stringbuilder sb(temp); getname(sb, true);
			player.say("������ �� ����� �� [%1].", temp);
		}
		break;
	case Cooking:
		if(!is(Edible))
			return false;
		if(run) {
			player.act("%����� ����������%� %1.", getname());
			if(!player.roll(id)) {
				player.fail(id);
				return false;
			}
		}
		break;
	case Smithing:
		if(!is(Iron) || !isdamaged())
			return false;
		if(run) {
			if(!player.roll(id)) {
				player.fail(id);
				return false;
			}
			player.act("%����� �������%� %1.", getname());
			repair();
		}
		break;
	case Literacy:
		if(!is(Readable))
			return false;
		if(run) {
			auto v = geteffect();
			auto level = getbonus();
			auto b = (geti().quality + level) * 3;
			if(is(Unknown))
				b += 10;
			else if(is(SingleUse))
				b += 80;
			if(is(Blessed))
				b += 10;
			else if(is(Artifact))
				b += 25;
			if(!is(SingleUse)) {
				switch(v.type) {
				case Spell:
					b -= player.get((spell_s)v.value) * 12;
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
					if(player.isactive()) {
						player.act("������, ��� ��� � �� ������� ������ ������.");
						player.fail(Literacy);
					}
				}
			} else {
				switch(v.type) {
				case Skill:
					break;
				case Ability:
					if(is(SingleUse)) {
						if(bsdata<itemi>::elements[type].skill == Alchemy)
							player.learnreceipt(v);
						destroy(Magic, true);
					}
					break;
				case Spell:
					if(is(SingleUse)) {
						if(is(Blessed) || is(Artifact))
							result = true;
						if(!result || !player.use((spell_s)v.value, level, this, false))
							player.act("%����� �������%� %-1 � ������ ��������%�.", getname());
						destroy(Magic, true);
					} else {
						if(!player.askyn("������ ������ ��������������� �����. ������������� ������ ����������?"))
							return false;
						player.act("%����� ������%� %-1 � �����%��� �������.", getname());
						player.wait(Hour);
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
	default: return false;
	}
	return true;
}

bool location::use(indext index, skill_s id, creature& player, int level, int order, bool run) {
	switch(id) {
	case Athletics:
		if(getobject(index) != Door || !is(index, Sealed) || is(index, Opened) || is(index, Hidden))
			return false;
		if(run) {
			// Random bonus for door to open
			if(!player.roll(id, (getrand(index) % 21) - 10)) {
				player.act("%����� ������%� �����, �� �� �� ���������.");
				player.fail(id);
				return false;
			}
			player.act("%����� ������%�� ����� � �����.");
			set(index, NoTileObject);
			player.addexp(20);
		}
		break;
	case DisarmTraps:
		if(getobject(index) != Trap || is(index, Hidden))
			return false;
		if(run) {
			if(!player.roll(id)) {
				player.act("%����� �������%��� ����������� �������, �� ���-�� ����� �� ���.");
				player.fail(id);
				return false;
			}
			player.act("%����� ����������%� �������.");
			set(index, NoTileObject);
			player.addexp(50);
		}
		break;
	case Herbalism:
		if(getobject(index) != Plants || !getplantgrow(index))
			return false;
		if(run) {
			auto power = getplantgrow(index);
			auto type = maprnd(common_plants);
			auto it = player.craft(type, {}, id, power * 10);
			it.setcount(xrand(1, 3));
			if(d100() < 15 * power)
				set(index, NoTileObject);
			else
				random[index] = 0;
			player.addexp(10);
			player.act("%����� ������%� %1.", it.getname());
			player.add(it, true, true);
		}
		break;
	case Lockpicking:
		if(getobject(index) != Door || !is(index, Sealed) || is(index, Opened) || is(index, Hidden))
			return false;
		if(run) {
			if(!player.roll(id, (getrand(index) % 31) - 15)) {
				player.act("%����� �� ����%�� ������� �����.");
				player.fail(id);
				return false;
			}
			player.act("%����� �������%� �����.");
			remove(index, Sealed);
			player.addexp(100);
		}
		break;
	default: return false;
	}
	return true;
}

bool creature::use(const creaturea& source, skill_s id) {
	auto v = get(id);
	if(v <= 0)
		return false;
	auto pu = isusedisable(id);
	if(pu) {
		if(isactive())
			sb.add(pu, getstr(id));
		return false;
	}
	auto& ei = bsdata<skilli>::elements[id];
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

const char* creature::isusedisable(skill_s id) const {
	auto& ei = bsdata<skilli>::elements[id];
	switch(id) {
	case PickPockets:
		if(hp < get(LifePoints))
			return "�� �� ������ �������� ���� ���� �������� ���� ���������. ������������ �������� � ���������� �����.";
		return 0;
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
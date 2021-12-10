#include "main.h"

static const char* talk_subjects[] = {"������", "��������", "������", "�������", "�������", "�������", "�������", "���� ������"};
static const char* talk_object[] = {"���������", "��������� ������", "��������� ��������", "����", "�������� ������", "���������� ����������"};
static const char* talk_location[] = {"����������", "������", "�������", "�������", "����"};
static const char* talk_games[] = {"������", "�����", "���������", "������"};
static item_s common_plants[] = {Tarnelle, Cherry, RoseHip, Physalis};

BSDATA(skilli) = {{"��������", "��������", {}, {Creature, {NotYou}, Close, "� ��� ������ �������������?"}},
	{"����", "������", {}, {Creature, {NotYou}, Close, "���� ������ ����������?"}},
	{"����������", "����������", {}, {Creature, {NotYou}, Close, "� ��� ������ ����� ����������?"}},
	{"����������", "����������"},
	{"��������������", "��������������"},
	{"��������", "��������", {}, {Object, {}, Close, "����� ����� ������?"}},
	{"��������", "��������", {}, {Creature, {NotYou, Friends}, Close, "���� �����?"}},
	{"�������", "�������"},
	{"������������", "�����������"},
	{"����������� �������", "�������", {}, {Object, {}, Close, "����� ������� �����������?"}},
	{"������� �����", "�����"},
	{"��������� � ����", "����������", {}, {Creature}},
	{"������� �����", "������", {}, {Object, {}, Close, "����� ����� �������?"}},
	{"��������� ����", "������ ��������"},
	{"�������� �������", "���������", {}, {Creature, {NotYou}, Close, "���� ����������?"}},
	{"�������", "�������", {}, {Item, {LongAction, AlwaysChoose}, You, "����� ����� ������ �����������?"}},
	{"������� ���", "�����������", {}, {Item, {AlwaysChoose}, You, "��� ������ �����������?"}},
	{"�����", "������", {}, {Creature, {NotYou, AllTargets}, Near}},
	{"���������� ����", "���������"},
	{"�������� ����", "�������� ���", {}, {Creature, {Friends, NotYou}, Close, "� ��� ��������?"}},
	{"�������", "�������", {}, {Room, {}, Reach, "����� ������ �������?"}},
	{"��������� ���", "��������"},
	{"�������������", "�����������", {}, {Object, {}, Close, "��� ������� ������?"}},
	{"�����������", "������ � ������", {}, {Item, {LongAction, AlwaysChoose}, You, "��� ������ ���������?"}},
	{"���������� ����", "����������� ����", {}, {Object, {LongAction}, Close, "��� �������� ����?"}},
	{"����������� �����������", "������ � �����", {}, {}},
	{"�������", "�������", {}, {Creature, {}, You}},
	{"���� ������", "���� ������"},
	{"��������� ����", "���������� ����", {}, {Item, {LongAction}, You, "��� ������ ���������������?"}},
	{"���������", "���������"},
	{"��������", "��������"},
	{"�������� �������", "�������� �� �������", {5, 15, 30}},
	{"�������� �����", "�������� �� ����", {3, 30, 30}},
	{"�������� �������", "�������� �� ��������", {4, 20, 25}},
	{"�������� �������", "�������� ����� �������", {4, 20, 25}},
	{"�������� �������", "�������� ������", {4, 20, 25}},
	{"�������� �������", "�������� �������", {4, 20, 25}},
	{"�������� �����", "�������� �� �����", {4, 20, 25}},
};
assert_enum(skilli, LastSkill)

skill_s	skilli::getid() const {
	return skill_s(this - bsdata<skilli>::elements);
}

bool creature::use(skill_s id, creature& player, int order, bool run) {
	site* pst;
	auto& ei = bsdata<skilli>::elements[id];
	switch(id) {
	case History:
		pst = getsite();
		if(!pst)
			return false;
		if(!pst->is(DungeonSite))
			return false;
		if(pst->is(KnownSite))
			return false;
		if(run) {
			if(!roll(id)) {
				fail(id);
				return false;
			}
			static const char* speech[] = {
				"��� ����� ������� �����. ��� %1!",
				"� ���� ��� �� ���������. ��� %1.",
				"������� ����� ����� ������� �� ������� �����. ������, ��� ��� %1."
			};
			pst->set(KnownSite);
			say(maprnd(speech), pst->getname());
			addexp(50, true);
		}
		break;
	case HideInShadow:
		if(is(Invisible))
			return false;
		if(run) {
			if(!player.roll(id)) {
				info("������� �� �������.");
				wait(CoupleMinutes);
				return false;
			}
			add(Invisible, 1, true);
		}
		break;
	case Diplomacy:
		pst = getsite();
		if(!pst)
			return false;
		if(pst->getowner() != this)
			return false;
		if(!execute(MakeDiscount, false))
			return false;
		if(run) {
			if(!player.roll(id)) {
				static const char* talk[] = {
					"��, �� ����� ����.",
					"��, ������ �����������.",
					"�����, �����, �������."
				};
				say(maprnd(talk));
			} else
				execute(MakeDiscount, true);
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
				player.fail(id);
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
			auto b = getbonus() * 10;
			if(is(Unknown))
				b += 10;
			else if(is(SingleUse))
				b += 100;
			else {
				switch(v.type) {
				case Spell:
					b -= player.get((spell_s)v.value) * 12;
					if(player.is(RoomOfSpellLearning))
						b += 50;
					break;
				}
			}
			auto result = player.roll(Literacy, b);
			if(player.is(RoomOfDull))
				result = false;
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
							player.add(maprnd(read_books));
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
			if(!player.roll(id)) {
				player.act("%����� ������%� �����, �� �� �� ���������.");
				player.fail(id);
				return false;
			}
			player.act("%����� ������%�� ����� � �����.");
			set(index, NoTileObject);
			player.addexp(20);
		}
		break;
	case History:
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
		info(pu, getstr(id));
		return false;
	}
	auto& ei = bsdata<skilli>::elements[id];
	creaturea creatures = source; itema items; indexa indecies;
	if(!ei.target.prepare(*this, creatures, items, indecies, id, v, true))
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
			return "�� �� ������ �������� ���� ���� �������� ���� ��������� - ���� ��������� � ��� � �����������. ������������ �������� � ���������� �����.";
		return 0;
	case Dancing:
		if(!getsite())
			return "��������� ����� ������ ���, ��� ���� ���������� ������ �����, ��������, � [�������] ��� ������ ������.";
		return 0;
	case MusicalInstrument: return "������������ ������������� �� ����� ���� �� [����������� �����������].";
	case Healing: return "������������ ������������� �������������� ���� [�����] �� ������������ ���������� �������.";
	case Concetration: return "������������ ������������� �������������� ���� [����] �� ������������ ���������� �������.";
	default:
		if(ei.isweapon())
			return "����� �������� ������� ������ �� ���� [���������], ��������� [����], [��������] ���������� ����� � ������������ ������������� ��� ��� ���������.";
		if(!ei.target)
			return "���� ����� ������������ �������������.";
		return 0;
	}
}
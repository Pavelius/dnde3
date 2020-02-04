#include "main.h"

static item_s common_plants[] = {Tarnelle, Cherry, RoseHip, Physalis};

skilli bsmeta<skilli>::elements[] = {{"��������", "��������", {Charisma, Intellegence}, {}, {Creature, {NotYou}, Close, "� ��� ������ �������������?"}},
{"����", "������", {Charisma, Dexterity}, {}, {Creature, {NotYou}, Close, "���� ������ ����������?"}},
{"����������", "����������", {Charisma, Wisdow}, {}, {Creature, {NotYou}, Close, "� ��� ������ ����� ����������?"}},
//
{"����������", "����������", {Dexterity, Dexterity}},
{"��������������", "��������������", {Wisdow, Dexterity}},
{"��������", "��������", {Strenght, Dexterity}, {}, {Object, {}, Close, "����� ����� ������?"}},
{"��������", "��������", {Dexterity, Dexterity}, {}, {Creature, {NotYou, Friends}, Close, "���� �����?"}},
{"�������", "�������", {Dexterity, Strenght}},
{"������������", "�����������", {Wisdow, Wisdow}},
{"����������� �������", "�������", {Dexterity, Intellegence}, {}, {Object, {}, Close, "����� ������� �����������?"}},
{"���� � �������� �����", "������ ������", {Dexterity, Intellegence}},
{"������� �����", "�����", {Wisdow, Intellegence}},
{"��������� � ����", "����������", {Dexterity, Intellegence}, {}, {Creature}},
{"������� �����", "������", {Dexterity, Intellegence}, {}, {Object, {}, Close, "����� ����� �������?"}},
{"��������� ����", "������ ��������", {Dexterity, Dexterity}},
{"�������� �������", "���������", {Dexterity, Charisma}, {}, {Creature, {NotYou}, Close, "���� ����������?"}},
{"�������", "�������", {Intellegence, Intellegence}, {}, {Item, {AlwaysChoose}, You, "����� ����� ������ �����������?"}},
{"������� ���", "�����������", {Charisma, Intellegence}, {}, {Item, {AlwaysChoose}, You, "��� ������ �����������?"}},
{"�����", "������", {Dexterity, Charisma}},
{"���������� ����", "���������", {Intellegence, Intellegence}},
{"�������� ����", "�������� ���", {Charisma, Dexterity}, {}, {Creature, {Friends, NotYou}, Close, "� ��� ��������?"}},
{"�������", "�������", {Intellegence, Intellegence}, {}, {Object, {}, Reach, "����� ������ �������?"}},
{"��������� ���", "��������", {Wisdow, Constitution}},
{"�������������", "�����������", {Wisdow, Intellegence}, {}, {Object, {}, Close, "��� ������� ������?"}},
{"�����������", "������ � ������", {Intellegence, Intellegence}, {}, {Item, {AlwaysChoose}, You, "��� ������ ���������?"}},
{"���������� ����", "����������� ����", {Strenght, Constitution}, {}, {Object, {}, Close, "��� �������� ����?"}},
{"���� ������", "���� ������", {Dexterity, Constitution}},
{"��������� ����", "���������� ����", {Strenght, Intellegence}, {}, {Object, {}, Close, "��� ������ ���������������?"}},
{"���������", "���������", {Wisdow, Constitution}},
{"��������", "��������", {Strenght, Constitution}},
//
{"�������� �����", "�������� �� ����", {Dexterity, Dexterity}, {3, 30, 30}},
{"�������� �����", "�������� �� �����", {Strenght, Dexterity}, {4, 20, 25}},
{"�������� �������", "�������� �� �������", {Strenght, Constitution}, {5, 15, 30}},
{"�������� ��������� �������", "���������� ������", {Strenght, Strenght}, {4, 10, 35}},
{"�������� ����� ��������", "�������� ������", {Strenght, Dexterity}, {5, 30, 15}},
};
assert_enum(skill, LastSkill);

skill_s	skilli::getid() const {
	return skill_s(this - bsmeta<skilli>::elements);
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
			item it(maprnd(common_plants));
			it.setquality(0);
			if(power > 1)
				it.setquality(xrand(0, 3));
			it.setcount(xrand(1, 3));
			if(!player.roll(id))
				it.set(Cursed);
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
	}
	return true;
}
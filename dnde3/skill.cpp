#include "main.h"

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
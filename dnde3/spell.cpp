#include "main.h"

spelli bsmeta<spelli>::elements[] = {{"��������� �����", "��������� �����", 10, {Creature}},
{"�������������", "�������������", 6, {Creature, {NotYou, Friends, AllTargets}, Reach}, {1, 3}},
{"���������� �������", "����������� ��������", 30, {Item, {AlwaysChoose}, You, "����� ������� ������� ���������?"}},
{"��������� �������", "�����", 20, {Creature, {NotYou, Enemies, RandomTargets}, Near, "��� ������ ����� ������?"}},
{"���������� ���", "����������� ���", 10, {Item, {RandomTargets}}},
{"���������� ����������", "����������� �����", 10, {Item, {RandomTargets}}},
{"�����", "������", 10, {Creature, {Enemies, TwoTargets}, Near}},
{"���������", "���������", 4, {Creature, {Friends}, Reach, "���� �������� �������?"}, {1, 6}, 2},
{"���������", "���������", 20, {Item, {AlwaysChoose}, You, "����� ������� ������ ��������?"}},
{"�����������", "�����������", 8, {Creature, {}, You}},
{"����", "�����", 1, {Creature, {Friends}, Near, "��� �������� ����?"}},
{"��������� �����", "���������� ������", 2, {Creature, {Enemies}, Near}, {1, 4}, 1, Magic, "������� ���������� �����, ���������� ����"},
{"�������", "�������", 2, {Item, {}, You, "����� ������� ��������?"}},
{"������ ��", "���", 10, {Creature, {Friends}, Close, "� ���� ������ ��?"}},
{"������ �������", "�������", 20, {Creature, {Friends}, Close, "���� �������� �� �������?"}},
{"��������� ���", "����", 6, {Creature, {Friends}, Close, "�� ���� �������� ���?"}},
{"���������� ������", "�������������", 3, {Creature, {Enemies}, Close}, {1, 8}, 2, Electricity, "�������� ������������ �����"},
{"���", "���", 10, {Creature, {Enemies}, Reach}},
{"��������� �������", "����������", 12, {Creature, {Enemies, RandomTargets}, Near}},
};
assert_enum(spell, SlowMonster);
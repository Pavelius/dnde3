#include "main.h"

spelli bsmeta<spelli>::elements[] = {{"��������� �����", "��������� �����", 10, {Creature}},
{"�������������", "�������������", 6, {Creature, {NotYou, Friends}, AllTargets, Reach}, {1, 3}},
{"������������� ��������", "����������� ��������", 30, {Item, {}, SingleTarget}},
{"��������� �������", "�����", 15, {Creature, {NotYou}, SingleTarget, Reach}},
{"���������� ���", "����������� ���", 10, {Item, {}, NearestTarget}},
{"���������� ����������", "����������� �����", 5, {Item, {}, NearestTarget}},
{"�����", "������", 5, {Creature, {Enemies}, SingleTarget}},
{"���������", "���������", 3, {Creature, {Friends}, SingleTarget, Reach}, {1, 6}, 2},
{"���������", "���������", 20, {Item}},
{"�����������", "�����������", 8, {Item}},
{"����", "�����", 1, {Creature}},
{"��������� �����", "���������� ������", 2, {Creature, {Enemies}, SingleTarget, Near}},
{"�������", "�������", 2, {Item}},
{"������ ��", "���", 5, {Creature, {Friends}, SingleTarget}},
{"������ �������", "�������", 10, {Creature, {Friends}, SingleTarget}},
{"��������� ���", "����", 6, {Creature, {Friends}, SingleTarget}},
{"���������� ������", "�������������", 3, {Creature, {Enemies}, SingleTarget, Close}, {1, 8}, 3, Electricity},
{"���", "���", 4, {Creature, {Enemies}, NearestTarget}},
{"��������� �������", "����������", 10, {Creature, {Enemies}, SingleTarget}},
};
assert_enum(spell, SlowMonster);
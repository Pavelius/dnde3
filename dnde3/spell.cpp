#include "main.h"

spelli bsmeta<spelli>::elements[] = {{"��������� �����", 10, {Creature}, {}},
{"�������������", 6, {Creature, AllTargets, Reach}, {1, 4}},
{"������������� ��������", 30, {Item, SingleTarget}},
{"��������� �������", 15, {Creature, SingleTarget, Reach}},
{"���������� ���", 10, {Item, RandomTarget}},
{"���������� ����������", 5, {Item, RandomTarget}},
{"�����", 5, {Creature, SingleTarget}},
{"���������", 3, {Creature, SingleTarget}, {1, 8}, 100},
{"���������", 20, {Item, SingleTarget}},
{"�����������", 8, {Item, SingleTarget}},
{"����", 1, {Creature, SingleTarget}},
{"��������� �����", 2, {Creature, SingleTarget, Near}, {1, 6}, 100},
{"������", 2, {Item, SingleTarget}},
{"������ ��", 5, {Creature, SingleTarget}},
{"������ �������", 10, {Creature, SingleTarget}},
{"��������� ���", 6, {Creature, SingleTarget}},
{"���������� ������", 3, {Creature, SingleTarget, Close}, {1, 8}, 200},
{"���", 4, {Creature, NearestTarget}, {1, 8}, 200},
{"��������� �������", 10, {Creature, SingleTarget}, {4}, 100},
};
assert_enum(spell, SlowMonster);
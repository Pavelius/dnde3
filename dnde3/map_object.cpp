#include "main.h"

BSDATA(map_objecti) = {
	{""},
	{"Door", "�����"},
	{"Tree", "������", {BlockMovement}, 4, 3},
	{"Altar", "������", {}, 9},
	{"Statue", "������", {}, 21},
	{"Trap", "�������"},
	{"Plants", "����� � ��������"},
	{"Pool", "�������", {}, 39, 15},
	{"StairsUp", "�������� �����", {}, 57},
	{"StairsDown", "�������� ����", {}, 56},
};
assert_enum(map_objecti, StairsDown)
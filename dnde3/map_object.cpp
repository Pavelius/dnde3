#include "main.h"

BSDATA(map_objecti) = {{""},
{"Door", "�����"},
{"Tree", "������", {BlockMovement}, 4, 3},
{"Altar", "������", {BlockMovement}, 9},
{"Statue", "������", {BlockMovement}, 21},
{"Trap", "�������"},
{"Plants", "����� � ��������"},
{"StairsUp", "�������� �����", {BlockMovement}, 57},
{"StairsDown", "�������� ����", {BlockMovement}, 56},
};
assert_enum(map_objecti, StairsDown)
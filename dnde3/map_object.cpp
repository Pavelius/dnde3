#include "main.h"

map_objecti bsmeta<map_objecti>::elements[] = {{""},
{"Door", "�����"},
{"Tree", "������", {BlockMovement}, 4, 3},
{"Altar", "������", {BlockMovement}},
{"Statue", "������", {BlockMovement}},
{"Trap", "�������"},
{"Plants", "����� � ��������"},
{"StairsUp", "�������� �����", {BlockMovement}, 57},
{"StairsDown", "�������� ����", {BlockMovement}, 56},
};
assert_enum(map_object, StairsDown);
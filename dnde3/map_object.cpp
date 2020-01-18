#include "main.h"

map_objecti bsmeta<map_objecti>::elements[] = {{""},
{"Door", "Дверь"},
{"Tree", "Дерево", {BlockMovement}, 4, 3},
{"Altar", "Алтарь", {BlockMovement}},
{"Statue", "Статуя", {BlockMovement}},
{"Trap", "Ловушка"},
{"Plants", "Травы и растения"},
{"StairsUp", "Лестница вверх", {BlockMovement}, 57},
{"StairsDown", "Лестница вниз", {BlockMovement}, 56},
};
assert_enum(map_object, StairsDown);
#include "main.h"

map_objecti bsmeta<map_objecti>::elements[] = {{""},
{"Door", "Дверь"},
{"Tree", "Дерево", 4, 3},
{"Altar", "Алтарь"},
{"Statue", "Статуя"},
{"Trap", "Ловушка"},
{"Plants", "Травы и растения"},
{"StairsUp", "Лестница вверх", 57},
{"StairsDown", "Лестница вниз", 56},
};
assert_enum(map_object, StairsDown);
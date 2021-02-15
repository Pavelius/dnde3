#include "main.h"

BSDATA(map_objecti) = {{""},
{"Door", "Дверь"},
{"Tree", "Дерево", {BlockMovement}, 4, 3},
{"Altar", "Алтарь", {}, 9},
{"Statue", "Статуя", {}, 21},
{"Trap", "Ловушка"},
{"Plants", "Травы и растения"},
{"StairsUp", "Лестница вверх", {}, 57},
{"StairsDown", "Лестница вниз", {}, 56},
};
assert_enum(map_objecti, StairsDown)
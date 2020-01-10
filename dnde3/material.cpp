#include "main.h"

template<> materiali bsmeta<materiali>::elements[] = {{"Стекло", {{Bludgeon, -40}}},
{"Железо", {{Acid, -40}, {WaterAttack, -20}, {Piercing, 10}}},
{"Кожа", {{Acid, -10}}},
{"Органика", {{Acid, -20}, {WaterAttack, 20}}},
{"Бумага", {{Fire, -40}}},
{"Камень"},
{"Дерево", {{Slashing, -20}, {Piercing, -10}}},
};
assert_enum(material, Wood);
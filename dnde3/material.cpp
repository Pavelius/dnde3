#include "main.h"

template<> materiali bsmeta<materiali>::elements[] = {{"������", {{Bludgeon, -40}}},
{"������", {{Acid, -40}, {WaterAttack, -20}, {Piercing, 10}}},
{"����", {{Acid, -10}}},
{"��������", {{Acid, -20}, {WaterAttack, 20}}},
{"������", {{Fire, -40}}},
{"������"},
{"������", {{Slashing, -20}, {Piercing, -10}}},
};
assert_enum(material, Wood);
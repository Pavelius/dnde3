#include "main.h"

template<> rarityi bsmeta<rarityi>::elements[] = {{"Обычный", 12, 70},
{"Необычный", 8, 40},
{"Редкий", 3, 15},
{"Очень редкий", 1, 5},
{"Уникальный", 1},
};
assert_enum(rarity, Unique);
#include "main.h"

template<> rarityi bsmeta<rarityi>::elements[] = {{"Обычный", 12},
{"Необычный", 8},
{"Редкий", 3},
{"Очень редкий", 1},
{"Уникальный"},
};
assert_enum(rarity, Unique);
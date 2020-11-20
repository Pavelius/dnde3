#include "main.h"

BSDATA(rarityi) = {{"Обычный", 12, 70},
{"Необычный", 8, 40},
{"Редкий", 3, 15},
{"Очень редкий", 1, 5},
{"Уникальный", 1},
};
assert_enum(rarityi, Unique)
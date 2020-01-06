#include "main.h"

statei bsmeta<statei>::elements[] = {{"Anger", "Злой", true},
{"Dazzled", "Ошеламлен", true},
{"Drunken", "Пьяный", true},
{"Friendly", "Дружественен", false},
{"Hostile", "Враждебен", true},
{"Hunger", "Голоден", true},
{"Invisible", "Невидим", false},
{"Poisoned", "Отравлен", true},
{"Sick", "Болен", true},
{"Sleeped", "Спит", true},
{"Wounded", "Ранен", true},
};
assert_enum(state, LastState);
#include "main.h"

statei bsmeta<statei>::elements[] = {{"Anger", "Злой", "злости", true},
{"Dazzled", "Ошеламлен", "ошеломления", true},
{"Drunken", "Пьяный", "опьянения", true},
{"Friendly", "Дружественен", "дружбы", false},
{"Hostile", "Враждебен", "вражды", true},
{"Hunger", "Голоден", "голода", true},
{"Invisible", "Невидим", "невидимости", false},
{"Poisoned", "Отравлен", "отравления", true},
{"Sick", "Болен", "болезни", true},
{"Sleeped", "Спит", "сна", true},
{"Wounded", "Ранен", "рассечения", true},
};
assert_enum(state, LastState);
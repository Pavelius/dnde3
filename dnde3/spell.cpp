#include "main.h"

spelli bsmeta<spelli>::elements[] = {{"Волшебная броня", 10, {Creature}, {}},
{"Благословение", 6, {Creature, AllTargets, Reach}, {1, 4}},
{"Благословение предмета", 30, {Item, SingleTarget}},
{"Очаровать персону", 15, {Creature, SingleTarget, Reach}},
{"Обнаружить зло", 10, {Item, RandomTarget}},
{"Обнаружить волшебство", 5, {Item, RandomTarget}},
{"Страх", 5, {Creature, SingleTarget}},
{"Исцеление", 3, {Creature, SingleTarget}, {1, 8}, 100},
{"Опознание", 20, {Item, SingleTarget}},
{"Невидимость", 8, {Item, SingleTarget}},
{"Свет", 1, {Creature, SingleTarget}},
{"Волшебный заряд", 2, {Creature, SingleTarget, Near}, {1, 6}, 100},
{"Ремонт", 2, {Item, SingleTarget}},
{"Убрать яд", 5, {Creature, SingleTarget}},
{"Убрать болезнь", 10, {Creature, SingleTarget}},
{"Волшебный щит", 6, {Creature, SingleTarget}},
{"Шокирующая хватка", 3, {Creature, SingleTarget, Close}, {1, 8}, 200},
{"Сон", 4, {Creature, NearestTarget}, {1, 8}, 200},
{"Замедлить монстра", 10, {Creature, SingleTarget}, {4}, 100},
};
assert_enum(spell, SlowMonster);
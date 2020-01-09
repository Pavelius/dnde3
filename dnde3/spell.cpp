#include "main.h"

spelli bsmeta<spelli>::elements[] = {{"Волшебная броня", 10, {Creature}},
{"Благословение", 6, {Creature, {NotYou, Friends}, AllTargets, Reach}},
{"Благословение предмета", 30, {Item, {},SingleTarget}},
{"Очаровать персону", 15, {Creature, {NotYou}, SingleTarget, Reach}},
{"Обнаружить зло", 10, {Item, {}, NearestTarget}},
{"Обнаружить волшебство", 5, {Item, {}, NearestTarget}},
{"Страх", 5, {Creature, {Enemies}, SingleTarget}},
{"Исцеление", 3, {Creature, {NotYou, Friends}, SingleTarget, Reach}},
{"Опознание", 20, {Item}},
{"Невидимость", 8, {Item}},
{"Свет", 1, {Creature}},
{"Волшебный заряд", 2, {Creature, {Enemies}, SingleTarget, Near}},
{"Ремонт", 2, {Item}},
{"Убрать яд", 5, {Creature, {Friends}, SingleTarget}},
{"Убрать болезнь", 10, {Creature, {Friends}, SingleTarget}},
{"Волшебный щит", 6, {Creature, {Friends}, SingleTarget}},
{"Шокирующая хватка", 3, {Creature, {Enemies}, SingleTarget, Close}},
{"Сон", 4, {Creature, {Enemies}, NearestTarget}},
{"Замедлить монстра", 10, {Creature, {Enemies}, SingleTarget}},
};
assert_enum(spell, SlowMonster);
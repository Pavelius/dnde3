#include "main.h"

spelli bsmeta<spelli>::elements[] = {{"Волшебная броня", "волшебной брони", 10, {Creature}},
{"Благословение", "благословения", 6, {Creature, {NotYou, Friends}, AllTargets, Reach}, {1, 3}},
{"Благословение предмета", "зачарования предмета", 30, {Item, {}, SingleTarget}},
{"Очаровать персону", "шарма", 15, {Creature, {NotYou}, SingleTarget, Reach}},
{"Обнаружить зло", "обнаружения зла", 10, {Item, {}, NearestTarget}},
{"Обнаружить волшебство", "обнаружения добра", 5, {Item, {}, NearestTarget}},
{"Страх", "страха", 5, {Creature, {Enemies}, SingleTarget}},
{"Исцеление", "исцеления", 3, {Creature, {Friends}, SingleTarget, Reach}, {1, 6}, 2},
{"Опознание", "опознания", 20, {Item}},
{"Невидимость", "невидимости", 8, {Item}},
{"Свет", "света", 1, {Creature}},
{"Волшебный заряд", "волшебного заряда", 2, {Creature, {Enemies}, SingleTarget, Near}},
{"Починка", "починки", 2, {Item}},
{"Убрать яд", "яда", 5, {Creature, {Friends}, SingleTarget}},
{"Убрать болезнь", "болезни", 10, {Creature, {Friends}, SingleTarget}},
{"Волшебный щит", "щита", 6, {Creature, {Friends}, SingleTarget}},
{"Шокирующая хватка", "электричества", 3, {Creature, {Enemies}, SingleTarget, Close}, {1, 8}, 3, Electricity},
{"Сон", "сна", 4, {Creature, {Enemies}, NearestTarget}},
{"Замедлить монстра", "замедления", 10, {Creature, {Enemies}, SingleTarget}},
};
assert_enum(spell, SlowMonster);
#include "main.h"

spelli bsmeta<spelli>::elements[] = {{"Волшебная броня", "волшебной брони", 10, {Creature}},
{"Благословение", "благословения", 6, {Creature, {NotYou, Friends}, AllTargets, Reach}, {1, 3}},
{"Зачаровать предмет", "зачарования предмета", 30, {Item, {}, SingleTarget}},
{"Очаровать персону", "шарма", 15, {Creature, {NotYou}, SingleTarget, Reach}},
{"Обнаружить зло", "обнаружения зла", 10, {Item, {}, NearestTarget}},
{"Обнаружить волшебство", "обнаружения магии", 5, {Item, {}, NearestTarget}},
{"Страх", "страха", 5, {Creature, {Enemies}, SingleTarget}},
{"Исцеление", "исцеления", 3, {Creature, {Friends}, SingleTarget, Reach}, {1, 6}, 2},
{"Опознание", "опознания", 20, {Item, {}, SingleTarget}},
{"Невидимость", "невидимости", 8, {Item}},
{"Свет", "света", 1, {Creature}},
{"Волшебный заряд", "волшебного заряда", 2, {Creature, {Enemies}, SingleTarget, Near}, {1, 4}, 1, Magic, "вылетел светящийся шарик, поразивший цель"},
{"Починка", "починки", 2, {Item}},
{"Убрать яд", "яда", 5, {Creature, {Friends}, SingleTarget}},
{"Убрать болезнь", "болезни", 10, {Creature, {Friends}, SingleTarget}},
{"Волшебный щит", "щита", 6, {Creature, {Friends}, SingleTarget}},
{"Шокирующая хватка", "электричества", 3, {Creature, {Enemies}, SingleTarget, Close}, {1, 8}, 2, Electricity, "сорвался элекрический заряд"},
{"Сон", "сна", 4, {Creature, {Enemies}, NearestTarget}},
{"Замедлить монстра", "замедления", 10, {Creature, {Enemies}, SingleTarget}},
};
assert_enum(spell, SlowMonster);
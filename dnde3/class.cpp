#include "main.h"

classi bsmeta<classi>::elements[] = {{"Крестьянин", 4, 0},
{"Клерик", 8, 8, 0, {0, 0, 0, 0, 2, 1}, {Diplomacy, Literacy, History, Healing, Herbalism}, {BlessSpell, HealingSpell}},
{"Воин", 10, 3, 0, {2, 0, 1, -2, 0, -1}, {Survival, Athletics, Riding, Archery, FightSwords, FightAxes}},
{"Маг", 4, 10, 0, {-2, 0, 0, 2, 1, 2}, {Alchemy, Concetration, History, Literacy}, {ArmorSpell, MagicMissile, Sleep}},
{"Паладин", 10, 6, 0, {2, 0, 1, 0, 1, 2}, {Diplomacy, Athletics, Literacy, FightSwords}, {DetectEvil}},
{"Следопыт", 10, 4, 0, {0, 2, 1, 0, 1, -1}, {Acrobatics, Survival, Athletics, Swimming, Riding, Archery, TwoWeaponFighting}},
{"Вор", 6, 6, 0, {0, 2, 0, 0, 0, 1}, {PickPockets, Lockpicking, HideInShadow, Acrobatics, DisarmTraps, Bluff, Backstabbing}},
{"Монстер", 8, 8, 0},
};
assert_enum(class, Monster);
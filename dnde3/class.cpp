#include "main.h"

classi bsmeta<classi>::elements[] = {{"����������", 4, 0},
{"������", 8, 8, 0, {0, 0, 0, 0, 2, 1}, {Diplomacy, Literacy, History, Healing, Herbalism}, {BlessSpell, HealingSpell}},
{"����", 10, 3, 1, {2, 0, 1, -2, 0, -1}, {Survival, Athletics, WeaponFocusBlades, Riding, WeaponFocusAxes}},
{"���", 4, 10, 0, {-2, 0, 0, 2, 1, 2}, {Alchemy, Concetration, History, Literacy}, {ArmorSpell, MagicMissile, Sleep}},
{"�������", 10, 6, 1, {2, 0, 1, 0, 1, 2}, {Diplomacy, Athletics, Literacy, WeaponFocusBlades}, {DetectEvil}},
{"��������", 10, 4, 1, {0, 2, 1, 0, 1, -1}, {Acrobatics, Survival, Athletics, Swimming, Riding, WeaponFocusBows}},
{"���", 6, 6, 1, {0, 2, 0, 0, 0, 1}, {PickPockets, Lockpicking, HideInShadow, Acrobatics, DisarmTraps, Bluff, Backstabbing}},
{"�������", 8, 8, 0},
};
assert_enum(class, Monster);
#include "main.h"

classi bsmeta<classi>::elements[] = {{"Крестьянин", 4, 0},
{"Клерик", 8, 10, 0, {0, 0, 0, 0, 2, 1}, {Diplomacy, History, Healing}, {BlessSpell, HealingSpell}},
{"Воин", 10, 0, 1, {2, 0, 1, -2, 0, -1}, {Survival, WeaponFocusBlades, WeaponFocusAxes}},
{"Маг", 4, 20, 0, {-2, 0, 0, 2, 1, 2}, {Alchemy, Concetration, Literacy}, {ArmorSpell, MagicMissile, Sleep}},
{"Паладин", 10, 5, 1, {2, 0, 1, 0, 1, 2}, {Diplomacy, Literacy, WeaponFocusBlades}, {DetectEvil}},
{"Следопыт", 10, 0, 1, {0, 2, 1, 0, 1, -1}, {Survival, WeaponFocusBows}},
{"Вор", 6, 0, 1, {0, 2, 0, 0, 0, 1}, {PickPockets, Lockpicking, HideInShadow, Acrobatics, DisarmTraps, Bluff, Backstabbing}},
{"Монстер", 8, 0, 0},
};
assert_enum(class, Monster);
#include "main.h"

classi bsmeta<classi>::elements[] = {{"����������", 4, 0, {}, {40, 3}},
{"������", 8, 8, 0, {0, 0, 0, 0, 2, 1}, {50, 4}, {Diplomacy, Literacy, History, Healing, Herbalism}, {BlessSpell, HealingSpell},
{SwordLong, SwordShort, SwordTwoHanded, BowLong, BowShort}},
{"����", 10, 3, 0, {2, 0, 1, -2, 0, -1}, {50, 5}, {Survival, Athletics, Riding, Archery, FightSwords}, {},
{}},
{"���", 4, 10, 0, {-2, 0, 0, 2, 1, 2}, {40, 3}, {Alchemy, Concetration, History, Literacy}, {ArmorSpell, MagicMissile, Sleep},
{AxeBattle, HammerWar, SwordLong, SwordShort, Spear, BowLong, BowShort, LeatherArmor, StuddedLeatherArmor, ScaleMail, ChainMail, SplintMail, PlateMail}},
{"�������", 10, 6, 0, {2, 0, 1, 0, 1, 2}, {50, 5}, {Diplomacy, Athletics, Literacy, FightSwords}, {DetectEvil},
{}},
{"��������", 10, 4, 0, {0, 2, 1, 0, 1, -1}, {50, 5}, {Acrobatics, Survival, Athletics, Swimming, Riding, Archery, TwoWeaponFighting}, {},
{SwordTwoHanded}},
{"���", 6, 6, 0, {0, 2, 0, 0, 0, 1}, {45, 4}, {PickPockets, Lockpicking, HideInShadow, Acrobatics, DisarmTraps, Bluff, Backstabbing}, {},
{AxeBattle, HammerWar, BowLong, PlateMail}},
{"�������", 8, 8, 0, {}, {50, 5},
{}},
};
assert_enum(class, Monster);
#include "main.h"

classi bsmeta<classi>::elements[] = {
	{"Крестьянин", 4, 0, 0, {-1, 1, 0, -1, 1, 0}, {40, 3}},
	{"Клерик", 8, 8, 0, {1, 0, 0, 0, 2, 1}, {50, 4}, {Diplomacy, Literacy, Literacy, History, Healing, Healing, Healing, Herbalism}, {BlessSpell, HealingSpell},
	{SwordLong, SwordShort, SwordTwoHanded, BowLong, BowShort}},
	{"Воин", 10, 3, 0, {4, 1, 2, -2, 0, -1}, {50, 5}, {Survival, Survival, Athletics, Athletics, Riding, FindWeakness, FocusSwords}, {},
	{}},
	{"Маг", 4, 10, 0, {-2, -1, 0, 4, 1, 2}, {40, 3}, {Alchemy, Concetration, Concetration, History, Literacy, Literacy, Literacy}, {ArmorSpell, MagicMissile, Sleep},
	{AxeBattle, HammerWar, SwordLong, SwordShort, Spear, BowLong, BowShort, LeatherArmor, StuddedLeatherArmor, ScaleMail, ChainMail, SplintMail, PlateMail}},
	{"Паладин", 10, 6, 0, {2, -1, 1, -1, 1, 2}, {50, 5}, {Diplomacy, Athletics, Athletics, Literacy, FocusSwords}, {DetectEvil},
	{}},
	{"Следопыт", 10, 4, 0, {1, 3, 1, 0, 1, -2}, {50, 5}, {Survival, Survival, Survival, Acrobatics, Acrobatics, Athletics, Swimming, Riding, FocusBows, TwoWeaponFighting}, {},
	{SwordTwoHanded}},
	{"Вор", 6, 6, 0, {0, 4, 0, 0, -1, 1}, {45, 4}, {Literacy, PickPockets, PickPockets, Lockpicking, Lockpicking, Lockpicking, HideInShadow, HideInShadow, Acrobatics, DisarmTraps, DisarmTraps, Bluff, MoveSilently, MoveSilently}, {},
	{AxeBattle, HammerWar, BowLong, PlateMail}},
	{"Монстер", 8, 4, 0, {}, {40, 5}, {}, {},
	{}},
};
assert_enum(classi, Monster)

BSDATA(equipmenti) = {
	{Dwarf, Fighter, {AxeBattle, ScaleMail, Shield, BreadDwarven}},
	{Elf, Ranger, {SwordLong, SwordShort, LeatherArmor, BowLong}},
	{Elf, Fighter, {SwordLong, StuddedLeatherArmor, BowLong}},
	{Elf, Mage, {SwordLong, Wand2, Potion1, RingRed, RingBlue}},
	{Halfling, Fighter, {SwordShort, LeatherArmor, BreadHalflings}},
	{Halfling, Theif, {SwordShort, LeatherArmor, RingRed}},
	{Human, Cleric, {Mace}},
	{Human, Fighter, {SwordLong, LeatherArmor, Shield}},
	{Human, Paladin, {SwordLong, ScaleMail}},
	{Human, Ranger, {SwordLong, SwordShort, LeatherArmor}},
	{Human, Theif, {SwordShort, LeatherArmor}},
	{Human, Mage, {Staff, Wand2, Scroll1, Potion1, RingBlue}},
};
DECLFULL(equipmenti)

BSDATA(leveli) = {
	{Fighter, 3, {FocusSwords}},
	{Fighter, 6, {FocusSwords}},
	{Fighter, 10, {Strenght}},
	{Fighter, 12, {Strenght}},
	{Fighter, 14, {Strenght}},
	{Fighter, 16, {Strenght}},
	{Fighter, 20, {Strenght, Constitution}},
	{Theif, 2, {Acrobatics}},
	{Theif, 3, {Lockpicking}},
	{Theif, 4, {PickPockets}},
	{Theif, 4, {PickPockets}},
	{Mage, 2, {MagicMissile}},
	{Mage, 3, {ArmorSpell}},
	{Mage, 4, {MagicMissile}},
	{Cleric, 2, {HealingSpell}},
	{Cleric, 4, {HealingSpell}},
	{Cleric, 6, {HealingSpell}},
};
DECLFULL(leveli)
#include "main.h"

template<> leveli bsmeta<leveli>::elements[] = {{Fighter, 3, {{FocusSwords}}},
{Fighter, 6, {{FocusSwords}}},
{Fighter, 10, {{Strenght, 1}}},
{Fighter, 12, {{Strenght, 1}}},
{Fighter, 14, {{Strenght, 1}}},
{Fighter, 16, {{Strenght, 1}}},
{Fighter, 20, {{Strenght, 1}, {Constitution, 1}}},
// Вор вначале повышает навыки, потом прогресс размазывается
{Theif, 2, {{Acrobatics, 10}}},
{Theif, 3, {{Lockpicking, 10}}},
{Theif, 4, {{PickPockets, 10}}},
//
{Mage, 2, {{MagicMissile, 1}}},
{Mage, 3, {{ArmorSpell, 1}}},
{Mage, 4, {{MagicMissile, 1}}},
};
DECLFULL(leveli);
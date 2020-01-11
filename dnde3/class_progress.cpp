#include "main.h"

template<> leveli bsmeta<leveli>::elements[] = {{Fighter, 4, {{FocusSwords}}},
{Fighter, 8, {{FocusSwords}}},
{Fighter, 12, {{Strenght, 1}}},
{Fighter, 16, {{Strenght, 1}}},
{Fighter, 20, {{Strenght, 1}}},
{Fighter, 24, {{Strenght, 1}}},
{Fighter, 28, {{Strenght, 1}, {Constitution, 1}}},
// Вор вначале повышает навыки, потом прогресс размазывается
{Theif, 2, {{Acrobatics, 10}}},
{Theif, 3, {{Lockpicking, 10}}},
{Theif, 4, {{PickPockets, 10}}},
};
DECLFULL(leveli);
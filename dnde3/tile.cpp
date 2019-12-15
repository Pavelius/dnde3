#include "main.h"

tilei bsmeta<tilei>::elements[] = {{"Plain", "Равнина", Female},
{"Water", "Вода", Female},
{"Floor", "Пол", Male},
{"Wall", "Стена", Female},
{"Road", "Дорога", Female},
{"Swamp", "Болото", Male},
{"Hill", "Холм", Male},
{"Sea", "Море", Male},
{"Foothills", "Подгорье", Male},
{"Mountains", "Горы", Male},
{"CloudPeaks", "Туманные пики", Male},
{"Forest", "Лес", Male},
{"City", "Город", Male},
};
assert_enum(tile, City);
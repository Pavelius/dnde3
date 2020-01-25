#include "main.h"

static const dungeoni plains[] = {{AreaPlain, 1, 1}, {}};
static const dungeoni forest[] = {{AreaForest, 1}, {}};

tilei bsmeta<tilei>::elements[] = {{"Plain", "Равнина", Female, plains},
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
{"Forest", "Лес", Male, forest},
{"Lake", "Озеро", NoGender},
{"City", "Город", Male},
};
assert_enum(tile, City);
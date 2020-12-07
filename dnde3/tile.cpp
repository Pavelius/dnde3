#include "main.h"

static const dungeoni plains[] = {{AreaPlain, 1, 1}, {}};
static const dungeoni forest[] = {{AreaForest, 1}, {}};

BSDATA(tilei) = {{"Plain", "Равнина", '.', Female, plains},
{"Water", "Вода", 'W', Female},
{"Floor", "Пол", '.', Male},
{"Wall", "Стена", '|', Female},
{"Road", "Дорога", '-', Female},
{"Swamp", "Болото", '>', Male},
{"Hill", "Холм", 'H', Male},
{"Sea", "Море", '~', Male},
{"Foothills", "Подгорье", 'o', Male},
{"Mountains", "Горы", '^', Male},
{"CloudPeaks", "Туманные пики", '\"', Male},
{"Forest", "Лес", 'T', Male, forest},
{"Lake", "Озеро", 'L', NoGender},
{"City", "Город", 'C', Male},
};
assert_enum(tilei, City)
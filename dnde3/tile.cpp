#include "main.h"

static const dungeoni plains[] = {{AreaPlain, 1, 1}, {}};
static const dungeoni forest[] = {{AreaForest, 1}, {}};

BSDATA(tilei) = {{"Plain", "�������", '.', Female, plains},
{"Water", "����", 'W', Female},
{"Floor", "���", '.', Male},
{"Wall", "�����", '|', Female},
{"Road", "������", '-', Female},
{"Swamp", "������", '>', Male},
{"Hill", "����", 'H', Male},
{"Sea", "����", '~', Male},
{"Foothills", "��������", 'o', Male},
{"Mountains", "����", '^', Male},
{"CloudPeaks", "�������� ����", '\"', Male},
{"Forest", "���", 'T', Male, forest},
{"Lake", "�����", 'L', NoGender},
{"City", "�����", 'C', Male},
};
assert_enum(tilei, City)
#include "main.h"

static const dungeoni plains[] = {{AreaPlain, 1, 1}, {}};
static const dungeoni forest[] = {{AreaForest, 1}, {}};

tilei bsmeta<tilei>::elements[] = {{"Plain", "�������", Female, plains},
{"Water", "����", Female},
{"Floor", "���", Male},
{"Wall", "�����", Female},
{"Road", "������", Female},
{"Swamp", "������", Male},
{"Hill", "����", Male},
{"Sea", "����", Male},
{"Foothills", "��������", Male},
{"Mountains", "����", Male},
{"CloudPeaks", "�������� ����", Male},
{"Forest", "���", Male, forest},
{"Lake", "�����", NoGender},
{"City", "�����", Male},
};
assert_enum(tile, City);
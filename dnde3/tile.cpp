#include "main.h"

tilei bsmeta<tilei>::elements[] = {{"Plain", "�������", Female},
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
{"Forest", "���", Male},
{"Lake", "�����", NoGender},
{"City", "�����", Male},
};
assert_enum(tile, City);
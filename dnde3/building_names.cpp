#include "main.h"

template<> adjectivei bsmeta<adjectivei>::elements[] = {{"�������", "�������", "�������"},
{"���������", "����������", "���������"},
{"�������", "�������", "�������"},
{"������", "������", "������"},
{"�����", "�����", "�����"},
};
DECLFULL(adjectivei);

template<> objectivei bsmeta<objectivei>::elements[] = {{"�������", Male},
{"��������", Male},
{"����", Male},
{"����", Female},
{"������", Male},
{"��������", Male},
};
DECLFULL(objectivei);

const char* adjectivei::get(gender_s v) const {
	switch(v) {
	case Male: return name_male;
	case Female: return name_female;
	default: return name;
	}
}
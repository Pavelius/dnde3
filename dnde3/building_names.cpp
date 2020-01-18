#include "main.h"

template<> adjectivei bsmeta<adjectivei>::elements[] = {{"шальное", "шальной", "шальная"},
{"волшебное", "вольшебный", "волшебная"},
{"смешное", "смешной", "смешная"},
{"черное", "черный", "черная"},
{"белое", "белый", "белая"},
};
DECLFULL(adjectivei);

template<> objectivei bsmeta<objectivei>::elements[] = {{"человек", Male},
{"единорог", Male},
{"волк", Male},
{"рысь", Female},
{"дракон", Male},
{"перстень", Male},
};
DECLFULL(objectivei);

const char* adjectivei::get(gender_s v) const {
	switch(v) {
	case Male: return name_male;
	case Female: return name_female;
	default: return name;
	}
}
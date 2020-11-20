#include "main.h"

enum duration_value_s : unsigned {
	DRound = StandartEnergyCost / 10,
	DMinute = StandartEnergyCost,
	DCoupleMinutes
};

BSDATA(durationi) = {{"Instant", "Мгновенно", {0}},
{"Round", "Несколько секунд", {DRound}},
{"Minute", "Минута", {DMinute}},
{"CoupleMinutes", "Несколько минут", {2 * DMinute, 5 * DMinute}},
{"HalfHour", "Полчаса", {30 * DMinute}},
{"Hour", "Час", {60 * DMinute}},
};
assert_enum(durationi, Hour)

int	durationi::roll() const {
	if(range[1])
		return xrand(range[0], range[1]);
	return range[0];
}
#include "main.h"

dicei bsmeta<dicei>::elements[] = {{},
{1, 3}, {1, 4},
{1, 6}, {2, 7},
{1, 8},
{1, 10},
{1, 12},
{2, 12},
};
assert_enum(dice, D2n12);

int	dicei::roll() const {
	auto d = max - min;
	if(!d)
		return 0;
	return min + rand() % d;
}

void dicei::normalize() {
	if(min < 1)
		min = 1;
	if(max < 1)
		max = 1;
}
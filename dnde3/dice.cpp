#include "main.h"

BSDATA(dicei) = {
	{},
	{1, 3}, {1, 4}, {1, 6}, {1, 8}, {1, 10}, {1, 12}, {2, 12},
};
assert_enum(dicei, D2d6)

int	dicei::roll() const {
	auto d = max - min;
	if(d <= 0)
		return min;
	return min + rand() % d;
}

void dicei::normalize() {
	if(min < 0)
		min = 0;
	if(max < 1)
		max = 1;
}
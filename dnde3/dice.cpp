#include "main.h"

dicei bsmeta<dicei>::elements[] = {{},
{1, 3}, {1, 4}, {1, 6}, {2, 7}, {3, 8}, {4, 9}, {5, 10}, {6, 11},
{2, 12}, {3, 13}, {4, 14}, {5, 15},
{3, 18}, {4, 19}, {5, 20},
};
assert_enum(dice, D5n20);

int	dicei::roll() const {
	auto d = max - min;
	if(!d)
		return 0;
	return min + rand() % d;
}
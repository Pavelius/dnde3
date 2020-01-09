#include "main.h"

int	dicei::roll() const {
	auto d = max - min;
	if(d <= 0)
		return min;
	return min + rand() % d;
}

void dicei::normalize() {
	if(min < 1)
		min = 1;
	if(max < 1)
		max = 1;
}
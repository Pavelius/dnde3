#include "main.h"

BSDATAC(itemground, 1024);

void itemground::clear() {
	item::clear();
	index = Blocked;
}
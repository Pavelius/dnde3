#include "main.h"

void itema::select(creature& player) {
	auto ps = data;
	for(auto& e : player.wears) {
		if(!e)
			continue;
		*ps++ = &e;
	}
	count = ps - data;
}
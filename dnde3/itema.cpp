#include "main.h"

void itema::select(creature& player, bool only_wear) {
	auto ps = data;
	if(only_wear) {
		for(auto i = Head; i <= Amunitions; i = (slot_s)(i+1))
			*ps++ = &player.wears[i];
	} else {
		for(auto& e : player.wears) {
			if(!e)
				continue;
			*ps++ = &e;
		}
	}
	count = ps - data;
}
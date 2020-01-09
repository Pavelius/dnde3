#include "main.h"

void spella::select(const creature& player) {
	auto ps = data;
	for(auto i = FirstSpell; i <= LastSpell; i = (spell_s)(i + 1)) {
		if(player.get(i) > 0)
			*ps++ = i;
	}
	count = ps - data;
}
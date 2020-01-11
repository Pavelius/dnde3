#include "main.h"

void spella::select(const creature& player) {
	auto ps = data;
	for(auto i = FirstSpell; i <= LastSpell; i = (spell_s)(i + 1)) {
		if(player.get(i) > 0)
			*ps++ = i;
	}
	count = ps - data;
}

void spella::match(target_flag_s v, bool remove) {
	auto ps = data;
	for(auto id : *this) {
		auto& ei = bsmeta<spelli>::elements[id];
		if(remove) {
			if(ei.target.is(v))
				continue;
		} else {
			if(!ei.target.is(v))
				continue;
		}
		*ps++ = id;
	}
	count = ps - data;
}
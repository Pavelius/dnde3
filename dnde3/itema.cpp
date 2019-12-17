#include "main.h"

void itema::select(creature& e) {
	e.select(*this, Head, Amunitions, false);
}

void itema::selectb(creature& e) {
	e.select(*this, FirstBackpack, LastBackpack, true);
}

void itema::match(slot_s v) {
	auto ps = data;
	for(auto p : *this) {
		if(!p->is(v))
			continue;
		*ps++ = p;
	}
	count = ps - data;
}
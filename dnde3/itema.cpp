#include "main.h"

void itema::select(creature& e) {
	e.select(*this, Head, Amunitions, false);
}

void itema::selecta(creature& e) {
	e.select(*this, FirstBackpack, Amunitions, true);
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

static const char* addweight(stringbuilder& sb, int v) {
	sb.clear();
	sb.add("%1i.%2i", v / 100, (v / 10) % 10);
	return sb;
}

void itema::footer(stringbuilder& sb) const {
	auto player = creature::getactive();
	if(!player)
		return;
	char temp[24]; stringbuilder s1(temp);
	sb.add("Общий вес ваших преметов [%1] кг.", addweight(s1, player->getweight()));
}
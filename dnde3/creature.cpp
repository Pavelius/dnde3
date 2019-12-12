#include "main.h"

DECLDATA(creature, 256);

int	creature::get(ability_s id) const {
	return abilities[id];
}

short unsigned creature::getid() const {
	if(!this)
		return Blocked;
	return this - bsmeta<creature>::elements;
}

void creature::post(ability_s i, int modifier, unsigned rounds) {
	auto p = bsmeta<boosti>::add();
	p->id = i;
	p->modifier = modifier;
	p->time = rounds;
	p->owner = getid();
}

void creature::add(variant id, int v, unsigned time) {

}

creature* creature::getobject(short unsigned v) {
	if(v == Blocked)
		return 0;
	return bsmeta<creature>::elements + v;
}

void creature::isolate() {
	auto id = getid();
	for(auto& e : bsmeta<creature>()) {
		if(!e)
			continue;
		if(e.charmer == id)
			e.charmer = 0;
		if(e.horror == id)
			e.horror = 0;
	}
}
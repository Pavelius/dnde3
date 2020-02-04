#include "main.h"

DECLDATA(site, 128)

void site::clear() {
	memset(this, 0, sizeof(*this));
	posable::clear();
	owner_id = Blocked;
}

creature* site::getowner() const {
	return (owner_id == Blocked) ? 0 : bsmeta<creature>::elements + owner_id;
}

site* site::find(indext index) {
	if(index == Blocked)
		return 0;
	point pt;
	pt.x = location::getx(index);
	pt.y = location::gety(index);
	for(auto& e : bsmeta<site>()) {
		if(!e)
			continue;
		if(pt.in(e))
			return &e;
	}
	return 0;
}

void site::setowner(const creature* v) {
	owner_id = v->getid();
}

creature* site::priest() {
	return loc.add(getposition(), Human, (gender_s)xrand(Male, Female), Cleric);
}

creature* site::shopkeeper() {
	return loc.add(getposition(), Shopkeeper);
}

void site::unlink(const creature& player) {
	auto id = player.getid();
	for(auto& e : bsmeta<site>()) {
		if(!e)
			continue;
		if(e.owner_id == id)
			e.owner_id = Blocked;
	}
}
#include "main.h"

BSDATAC(site, 128)

void site::clear() {
	memset(this, 0, sizeof(*this));
	posable::clear();
	owner_id = Blocked;
}

creature* site::getowner() const {
	return (owner_id == Blocked) ? 0 : bsdata<creature>::elements + owner_id;
}

bool site::haslook() const {
	switch(type) {
	case Room:
		return bsdata<roomi>::elements[value].getdescription || bsdata<roomi>::elements[value].text;
	default:
		return false;
	}
}

void site::addlook(stringbuilder& sb) const {
	if(type == Room) {
		auto& ei = bsdata<roomi>::elements[value];
		if(ei.getdescription)
			ei.getdescription(this, sb);
		else
			sb.add(ei.text);
	}
}

site* site::find(indext index) {
	if(index == Blocked)
		return 0;
	point pt;
	pt.x = location::getx(index);
	pt.y = location::gety(index);
	for(auto& e : bsdata<site>()) {
		if(!e)
			continue;
		if(pt.in(e))
			return &e;
	}
	return 0;
}

void site::set(room_s v) {
	type = Room;
	value = v;
	flags = bsdata<roomi>::elements[v].flags;
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
	for(auto& e : bsdata<site>()) {
		if(!e)
			continue;
		if(e.owner_id == id)
			e.owner_id = Blocked;
	}
}
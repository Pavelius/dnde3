#include "main.h"

DECLDATA(site, 128)

creature* site::getowner() const {
	return (owner_id == Blocked) ? 0 : bsmeta<creature>::elements + owner_id;
}

void site::getname(stringbuilder& sb) const {
	sb.add("Таверна");
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

indext site::getposition() const {
	return location::center(*this);
}

void site::setowner(const creature* v) {
	owner_id = v->getid();
}

creature* site::priest() {
	switch(diety) {
	case GodGruumsh: return loc.add(getposition(), Dwarf, Male, Cleric);
	default: return loc.add(getposition(), Human, (gender_s)xrand(Male, Female), Cleric);
	}
}

creature* site::shopkeeper() {
	return loc.add(getposition(), Shopkeeper);
}
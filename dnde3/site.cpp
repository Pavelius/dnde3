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

creature* site::add(race_s race, gender_s gender, class_s type) {
	auto loc = location::getactive();
	if(!loc)
		return 0;
	return loc->add(loc->center(*this), race, gender, type);
}

creature* site::add(role_s type) {
	auto loc = location::getactive();
	if(!loc)
		return 0;
	return loc->add(loc->center(*this), type);
}

indext site::getposition() const {
	return location::center(*this);
}

void site::setowner(const creature* v) {
	owner_id = v->getid();
}

creature* site::priest() {
	creature* player = 0;
	diety = (diety_s)xrand(GodBane, GodTyr);
	switch(diety) {
	case GodGruumsh:
		player = add(Dwarf, Male, Cleric);
		break;
	default:
		player = add(Human, (gender_s)xrand(Male, Female), Cleric);
		break;
	}
	setowner(player);
	return player;
}

creature* site::shopkeeper() {
	auto player = add(Shopkeeper);
	setowner(player);
	return player;
}
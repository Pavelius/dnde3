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
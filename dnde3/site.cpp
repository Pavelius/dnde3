#include "main.h"

DECLDATA(site, 128)

static const char* shop_end[] = {"из-за океана",
"высокого качества",
"по небольшим ценам",
"с изюменкой",
"для королей",
};

static const char* shop_weapon[] = {"Кузница %1",
"Мечи и доспехи %2",
"Оружие %2",
"Хорошие мечи %1",
};

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

indext site::getposition() const {
	return location::center(*this);
}

void site::setowner(const creature* v) {
	owner_id = v->getid();
}

creature* site::priest() {
	if(param.type == God) {
		switch(param.value) {
		case GodGruumsh: return loc.add(getposition(), Dwarf, Male, Cleric);
		default: return loc.add(getposition(), Human, (gender_s)xrand(Male, Female), Cleric);
		}
	}
	return 0;
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

void site::randomname() {
	switch(type) {
	case ShopWeaponAndArmor:
		name[0] = rand() % (sizeof(shop_weapon) / sizeof(shop_weapon[0]));
		name[1] = rand() % (sizeof(shop_end) / sizeof(shop_end[0]));
		break;
	default:
		name[0] = bsmeta<adjectivei>::source.random();
		name[1] = bsmeta<objectivei>::source.random();
		break;
	}
}

void site::getname(stringbuilder& sb) const {
	char temp[260]; stringbuilder sbt(temp);
	const char* format = bsmeta<roomi>::elements[type].title;
	const char* p1 = "";
	const char* p2 = "";
	switch(type) {
	case ShopWeaponAndArmor:
		format = shop_weapon[name[0]];
		if(getowner()) {
			sbt.addof(getowner()->getname());
			p1 = temp;
		} else
			p1 = "Великий кузнец";
		p2 = shop_end[name[1]];
		break;
	default:
		p1 = bsmeta<adjectivei>::elements[name[0]].get(bsmeta<objectivei>::elements[name[1]].gender);
		p2 = bsmeta<objectivei>::elements[name[1]].name;
		break;
	}
	sb.add(format, p1, p2);
}
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
	return loc.add(location::center(*this), race, gender, type);
}

creature* site::add(role_s type) {
	return loc.add(location::center(*this), type);
}

creature* site::adventurer() {
	static race_s races[] = {Human, Elf, Halfling, Dwarf};
	static gender_s genders[] = {Male, Female, Male};
	static class_s classes[] = {Cleric, Cleric, Fighter, Fighter, Mage, Mage, Paladin, Ranger, Theif, Theif};
	return add(maprnd(races), maprnd(genders), maprnd(classes));
}

indext site::getposition() const {
	return location::center(*this);
}

void site::setowner(const creature* v) {
	owner_id = v->getid();
}

creature* site::priest() {
	switch(diety) {
	case GodGruumsh: return add(Dwarf, Male, Cleric);
	default: return add(Human, (gender_s)xrand(Male, Female), Cleric);
	}
}

creature* site::shopkeeper() {
	return add(Shopkeeper);
}

void site::create(int x, int y, int w, int h, site_s type) {
	if(w < 1 || h < 1)
		return;
	switch(type) {
	case Temple:
		diety = (diety_s)xrand(GodBane, GodTyr);
		setowner(priest());
		break;
	case Tavern:
		setowner(add(Bartender));
		for(auto i = xrand(1, 3); i > 0; i--)
			adventurer();
		break;
	case ShopWeaponAndArmor:
		setowner(shopkeeper());
		//create_shop(x, y, w, h, 90, 10, true, item::getitems(source, slots_weapons_armor));
		break;
	case ShopPotionAndScrolls:
		setowner(shopkeeper());
		//create_shop(x, y, w, h, 90, 10, true, item_potion_scrolls);
		break;
	case ShopFood:
		setowner(shopkeeper());
		//create_shop(x, y, w, h, 90, 10, true, item_food, 25, Unknown);
		break;
	case TreasureRoom:
		//create_shop(x, y, w, h, 80, 10, false, item_treasure);
		break;
	case Barracs:
		for(auto i = xrand(2, 4); i > 0; i--)
			add(HumanGuard);
		break;
	case CityHall:
		break;
	}
}
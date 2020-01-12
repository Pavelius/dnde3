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

void site::create() {
	if(!(*this))
		return;
	static slot_s weapons[] = {Melee};
	static slot_s armors[] = {Melee};
	static slot_s potions[] = {Drinkable};
	static slot_s scrolls[] = {Readable};
	static slot_s treasures[] = {Coinable};
	static slot_s edible[] = {Edible};
	switch(type) {
	case Temple:
		diety = (diety_s)xrand(GodBane, GodTyr);
		setowner(priest());
		break;
	case Tavern:
		setowner(add(Bartender));
		for(auto i = xrand(1, 3); i > 0; i--)
			loc.adventurer(getposition());
		break;
	case ShopWeaponAndArmor:
		setowner(shopkeeper());
		loc.loot(*this, armors, 30, loc.level, 10, KnownPower, 0);
		loc.loot(*this, weapons, 70, loc.level, 10, KnownPower, 0);
		break;
	case ShopPotionAndScrolls:
		setowner(shopkeeper());
		loc.loot(*this, scrolls, 70, loc.level, 10, KnownPower, 0);
		loc.loot(*this, potions, 40, loc.level, 10, KnownPower, 0);
		break;
	case ShopFood:
		setowner(shopkeeper());
		loc.loot(*this, edible, 90, loc.level, 20);
		break;
	case TreasureRoom:
		loc.loot(*this, treasures, 60, loc.level, 0, Unknown);
		break;
	case Barracs:
		for(auto i = xrand(2, 4); i > 0; i--)
			add(HumanGuard);
		break;
	case CityHall:
		break;
	}
}
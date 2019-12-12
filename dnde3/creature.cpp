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

void creature::dress(int m) {
	if(!this)
		return;
	abilities[AttackMelee] += m * (wears[Melee].getitem().weapon.attack + wears[Melee].getmagic() * 3);
	abilities[AttackRanged] += m * (wears[Ranged].getitem().weapon.attack + wears[Ranged].getmagic() * 4);
	for(auto i = Head; i <= Legs; i = (slot_s)(i + 1)) {
		abilities[Deflect] += m * wears[i].getitem().armor.deflect + wears[i].getmagic()*wears[i].getitem().armor.multiplier;
		abilities[Armor] += m * wears[i].getitem().armor.armor + wears[i].getmagic() / 2;
	}
}

void creature::equip(item it, slot_s id) {
	dress(-1);
	wears[id] = it;
	dress(1);
}

bool creature::equip(item v) {
	for(auto i = Head; i <= Amunitions; i = (slot_s)(i + 1)) {
		if(!v.is(i))
			continue;
		if(wears[i])
			continue;
		equip(v, i);
		return true;
	}
	return false;
}
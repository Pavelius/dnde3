#include "main.h"

struct foodi {
	item_s				type;
	variant				condition;
	chancev				chances;
	//
	void				apply(creature* player, const item it, bool interactive) const;
	bool				match(const creature* player, const item it) const;
};

static foodi elements[] = {{Ration, {Cursed}, {{Sick, 100}}},
{Ration, {}, {{LifePoints, 8}}},
{BreadDwarven, Dwarf, {{LifePoints, 8}, {Poison, 1}}},
{BreadDwarven, {}, {{LifePoints, 4}, {Poison, 10}}},
{BreadEvlen, {}, {{LifePoints, 4}, {Sick, -10}}},
{BreadHalflings, {Halfling}, {{LifePoints, 6}, {Charisma, 20}, {Strenght, 10}}},
{BreadHalflings, {}, {{LifePoints, 4}, {Charisma, 10}}},
{Apple, {}, {{LifePoints, 2}, {Strenght, 25}}},
{Sausage, {}, {{LifePoints, 6}, {Constitution, 20}}},
{Meat, {}, {{LifePoints, 6}, {Sick, 20}}},
{Cake, {}, {{LifePoints, 4}, {Poison, -10}}},
};

bool foodi::match(const creature* player, const item it) const {
	if(it.getkind() != type)
		return false;
	if(!player->match(condition))
		return false;
	if(!it.ismatch(condition))
		return false;
	return true;
}

void foodi::apply(creature* player, const item it, bool interactive) const {
	for(auto& e : chances) {
		auto v = e.value;
		if(e.id.type == Ability && (e.id.value == LifePoints || e.id.value == ManaPoints))
			v = xrand(e.value / 2, e.value);
		else {
			auto hostile = false;
			if(e.id.type == State)
				hostile = bsmeta<statei>::elements[e.id.value].flags.is(Hostile);
			if(v >= 0)
				v = 1;
			else
				v = -1;
			if(!player->usechance(iabs(e.value), hostile, it.getmagic(), it.getquality(), it.getdamage()))
				continue;
		}
		player->add(e.id, it.getkind(), v, interactive, it.getmagic(), it.getquality(), it.getdamage(), 60);
	}
}

bool item::isboost(variant id) const {
	// Simple case, when item have effect
	if(is(KnownPower)) {
		auto e = geteffect();
		if(e == id)
			return true;
	}
	// Check for food
	for(auto& e : elements) {
		if(e.type != type)
			continue;
		for(auto& c : e.chances) {
			if(c.id == id)
				return true;
		}
	}
	return false;
}

bool creature::use(item& it, bool interactive) {
	bool consume = true;
	variant effect;
	switch(it.getitem().slot) {
	case Edible:
		act("%герой съел%а %-1.", it.getname());
		for(auto& e : elements) {
			if(!e.match(this, it))
				continue;
			e.apply(this, it, interactive);
			break;
		}
		break;
	case Drinkable:
		act("%герой выпил%а %-1.", it.getname());
		effect = it.geteffect();
		if(effect) {
			auto v = 1;
			if(effect.type == Ability) {
				switch(effect.value) {
				case ManaPoints:
				case LifePoints:
					v = xrand(8, 16);
					break;
				case Attack: case Deflect: case Protection:
					v = xrand(5, 10);
					break;
				}
			}
			add(effect, it.getkind(), v, true, it.getmagic(), it.getquality(), it.getdamage(), 120);
		} else {
			if(interactive)
				act("Ничего не произошло.");
		}
		break;
	}
	if(consume) {
		dressoff();
		it.clear();
		dresson();
	}
	wait();
	return true;
}
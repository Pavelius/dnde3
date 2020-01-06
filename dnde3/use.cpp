#include "main.h"

struct foodi {
	variant				condition[3];
	chancev				chances;
	//
	void				apply(creature* player, const item it, bool interactive) const;
	bool				match(const creature* player, const item it) const;
};

static foodi elements[] = {{{Ration}, {{LifePoints, 5}, {Poison, 10}, {Sick, 1}}},
{{BreadDwarven, Dwarf}, {{LifePoints, 10}, {Sick, 1}}},
{{BreadDwarven}, {{LifePoints, 3}, {Poison, 10}, {Sick, 1}}},
};

bool foodi::match(const creature* player, const item it) const {
	for(auto v : condition) {
		if(!v)
			break;
		if(!player->match(v))
			return false;
		if(!it.match(v))
			return false;
	}
	return true;
}

void foodi::apply(creature* player, const item it, bool interactive) const {
	for(auto& e : chances)
		player->apply(e.id, e.value, interactive, it.getmagic(), it.getqualityr(), it.getdamage(), 60);
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
		if(effect)
			apply(effect, 0, true, it.getmagic(), it.getqualityr(), it.getdamage(), 120);
		else {
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
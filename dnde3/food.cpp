#include "main.h"

template<> foodi bsmeta<foodi>::elements[] = {{Ration, {Cursed}, {{Sick, 100}}},
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
DECLFULL(foodi);

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
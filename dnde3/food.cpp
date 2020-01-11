#include "main.h"

template<> foodi bsmeta<foodi>::elements[] = {{Ration, {}, 8},
{BreadDwarven, Dwarf, 8},
{BreadDwarven, {}, 4, 0, 0, {}, 10},
{BreadEvlen, {}, 4, 0, 0, {}, 0, -20},
{BreadHalflings, Halfling, 6, 0, 20, Charisma},
{BreadHalflings, {}, 4, 0, 10, Charisma},
{Apple, {}, 2, 2, 25, Strenght},
{Sausage, {}, 6, 0, 20, Constitution},
{Meat, {}, 6, 0, 0, {}, 0, 10},
{Cake, {}, 4},
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
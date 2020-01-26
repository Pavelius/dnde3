#include "main.h"

template<> outdoori bsmeta<outdoori>::elements[] = {{Blocked, "Тига",
"На холме расположилась небольшая деревушка шахтеров.", {ResDecals, 8},
{{AreaCity, 1, 0, {StairsDownRoom, Barracs, Lair}},
{AreaDungeon, 8, -2, {StairsDownRoom, StairsUpRoom}},
{AreaDungeon, 1, -2, {StairsUpRoom}}}},
{Blocked, "Мехер",
"Шумный город Мехер никогда не разочаровывал путишественника. Магазины, таверны и гостиницы - здесь было все высшего качества", {ResDecals, 9},
{{AreaCity, 1, 0, {StairsDownRoom, ShopFood, ShopWeaponAndArmor}},
{AreaDungeon, 8, -2, {StairsDownRoom, StairsUpRoom}},
{AreaDungeon, 1, -2, {StairsUpRoom}}}},
};
DECLFULL(outdoori);

const outdoori* outdoori::find(indext index) {
	for(auto& e : bsmeta<outdoori>()) {
		if(!e)
			continue;
		if(e.getposition() == index)
			return &e;
	}
	return 0;
}

void outdoori::clear() {
	memset(this, 0, sizeof(*this));
	index = Blocked;
}
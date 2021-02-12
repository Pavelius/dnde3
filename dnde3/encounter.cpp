#include "main.h"

BSDATA(encounter) = {
	{Hostile, {}, "Впереди показалась большая группа гоблинов.", {{GoblinWarrior, 5, 12}}},
	{Hostile, {}, "Неболшая группа орков показалась на горизонте.", {{OrcWarrior, 3, 5}}},
};
DECLFULL(encounter);

void encounter::play() {
	static class_s classes[] = {Fighter, Cleric, Ranger, Paladin, Mage, Theif};
	static gender_s genders[] = {Male, Male, Female};
	if(text) {
		sb.add(text);
		auto roll_result = false;
		auto player = creature::getactive();
		if(player)
			roll_result = player->roll(Survival);
		if(roll_result) {
			sb.adds("Хотите избежать встречи?");
			if(player->askyn()) {
				player->addexp(50, true);
				return;
			}
		} else
			player->pause(true);
	}
	game.enter(1, StairsDown);
	auto range = 5;
	auto center_index = loc.get(mmx / 2, mmy / 2);
	auto left_index = loc.get(mmx / 2 - range, mmy / 2);
	auto right_index = loc.get(mmx / 2 + range, mmy / 2);
	auto mosnter_index = right_index;
	for(auto v : monsters) {
		if(v.chance && d100() >= v.chance)
			continue;
		auto count = v.count.roll();
		for(auto i = 0; i < count; i++) {
			creature* m = 0;
			switch(v.monster.type) {
			case Role: m = loc.add(loc.getfree(mosnter_index), (role_s)v.monster.value); break;
			case Race: m = loc.add(loc.getfree(mosnter_index), (race_s)v.monster.value, maprnd(genders), maprnd(classes)); break;
			}
			if(!m)
				continue;
			m->add(state, 1, false);
		}
	}
}

bool encounter::match(variant v) {
	auto def_result = true;
	for(auto e : conditions) {
		if(e.type != v.type)
			continue;
		if(e.value == v.value)
			return true;
		def_result = false;
	}
	return def_result;
}

encounter* encounter::getrandom(tile_s landscape) {
	for(auto& e : bsmeta<encounter>()) {
		auto chance = 30;
		if(d100() >= chance)
			continue;
		if(!e.match(landscape))
			continue;
		return &e;
	}
	return 0;
}
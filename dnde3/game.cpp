#include "main.h"

gamei			game;

static void update_los() {
	auto player = creature::getactive();
	if(!player)
		return;
	// Set fog of war
	auto max_count = mmx * mmy;
	for(auto i = 0; i < max_count; i++)
		loc.remove(i, Visible);
	loc.setlos(player->getposition(), player->getlos());
}

static void move_creatures() {
	for(auto& e : bsmeta<creature>()) {
		if(!e)
			continue;
		e.makemove();
	}
}

void gamei::playactive() {
	const int moves_per_minute = 30;
	bool need_continue = true;
	while(need_continue) {
		need_continue = true;
		for(auto i = 0; i < moves_per_minute; i++) {
			checkcommand();
			if((i % 5) == 0)
				update_los();
			move_creatures();
			auto p = creature::getactive();
			if(!p || p->isbusy() || p->is(Sleep) || command)
				need_continue = false;
		}
		rounds++; // One round is one minute
		applyboost();
		if((rounds % 5)==0)
			applypoison();
	}
}

void gamei::save(playera& players) {

}

void gamei::enter(indext index, int level, map_object_s stairs) {
	static dungeoni meher_dungeon[] = {{AreaCity, 1},
	{AreaDungeon, 16},
	{AreaDungeonLair, 1},
	};
	playera players; save(players);
	setposition(index, level);
	if(!meher_dungeon->create(index, level))
		return;
	update_los();
}

void gamei::checkcommand() {
	if(!command)
		return;
	switch(command) {
	case StairsDown:
		enter(getposition(), getlevel() + 1, StairsUp);
		break;
	case StairsUp:
		enter(getposition(), getlevel() - 1, StairsDown);
		break;
	}
}

bool gamei::checkalive() {
	creaturea source;
	source.select(Friendly);
	if(!source)
		return false;
	source.matchbs(true); // Уберем тех кто занят
	source.match(Sleep, true); // Уберем тех кто спит
	if(source) {
		auto p = creature::getactive();
		if(!p || source.indexof(p) == -1)
			source[0]->activate();
	}
	return true;
}

void gamei::play() {
	while(checkalive())
		playactive();
}

void gamei::applyboost() {
	auto ps = bsmeta<boosti>::elements;
	for(auto& e : bsmeta<boosti>()) {
		if(e.time > rounds) {
			*ps++ = e;
			if(!e.id && e.source.type==Spell) {
				auto player = bsmeta<creature>::elements + e.owner;
				player->suffer((spell_s)e.source.value);
			}
			continue;
		}
		auto player = bsmeta<creature>::elements + e.owner;
		player->add(e.id, e.modifier, player->is(Friendly));
	}
	bsmeta<boosti>::source.setcount(ps - bsmeta<boosti>::elements);
}

void gamei::applypoison() {
	for(auto& e : bsmeta<creature>()) {
		if(e)
			e.checkpoison();
	}
}

void gamei::use(map_object_s v) {
	command = v;
}
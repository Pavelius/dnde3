#include "main.h"

gamei game;

static void update_los() {
	auto player = creature::getactive();
	if(!player)
		return;
	auto loc = location::getactive();
	if(!loc)
		return;
	// Set fog of war
	auto max_count = mmx * mmy;
	for(auto i = 0; i < max_count; i++)
		loc->remove(i, Visible);
	loc->setlos(player->getposition(), player->getlos());
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
			if((i % 5) == 0)
				update_los();
			move_creatures();
			if(!creature::getactive())
				need_continue = false;
		}
		rounds++; // One round is one minute
		applyboost();
	}
}

bool gamei::checkalive() {
	creaturea source;
	source.select(Friendly);
	if(!source)
		return false;
	if(!creature::getactive())
		source[0]->activate();
	return true;
}

void gamei::play() {
	while(checkalive()) {
		playactive();
	}
}

void gamei::applyboost() {
	auto ps = bsmeta<boosti>::elements;
	for(auto& e : bsmeta<boosti>()) {
		if(e.time > rounds) {
			*ps++ = e;
			continue;
		}
		auto player = bsmeta<creature>::elements + e.owner;
		player->add(e.id, e.modifier, player->is(Friendly));
	}
	bsmeta<boosti>::source.setcount(ps - bsmeta<boosti>::elements);
}
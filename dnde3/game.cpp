#include "main.h"

gamei game;

static void updatelos() {
	auto loc = location::getactive();
	if(!loc)
		return;
	// Set fog of war
	auto max_count = mmx * mmy;
	for(auto i = 0; i < max_count; i++)
		loc->remove(i, Visible);
	for(auto& e : bsmeta<creature>()) {
		if(!e)
			continue;
		if(!e.is(Friendly))
			continue;
		loc->setlos(e.getposition(), e.getlos());
	}
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
				updatelos();
			move_creatures();
			if(!creature::getactive())
				need_continue = false;
		}
		rounds++; // One round is one minute
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
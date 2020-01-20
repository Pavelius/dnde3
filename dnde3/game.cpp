#include "main.h"

gamei					game;

class gamestat {
	adat<creature, 8>	players;
	adat<boosti>		boosts;
public:
	gamestat() {
		for(auto& player : bsmeta<creature>()) {
			if(!player)
				continue;
			if(!player.is(Friendly))
				continue;
			// Store player
			auto i = players.getcount();
			player.unlink();
			auto p1 = players.add();
			if(p1) {
				*p1 = player;
				// Store boost
				auto owner_id = player.getid();
				auto pb = bsmeta<boosti>::elements;
				for(auto& b : bsmeta<boosti>()) {
					if(b.owner_id == owner_id) {
						auto pb = boosts.add();
						if(pb) {
							*pb = b;
							pb->owner_id = i;
						}
					} else
						*pb++ = b;
				}
				bsmeta<boosti>::source.setcount(pb - bsmeta<boosti>::elements);
			}
			player.clear();
		}
	}
	~gamestat() {
		auto pb = boosts.begin();
		auto pe = boosts.end();
		for(auto& player : players) {
			auto owner_id = &player - players.data;
			auto p1 = bsmeta<creature>::addz();
			if(p1)
				*p1 = player;
			while(pb < pe && pb->owner_id == owner_id) {
				if(p1) {
					auto p2 = bsmeta<boosti>::add();
					if(p2) {
						*p2 = *pb;
						p2->owner_id = p1->getid();
					}
				}
				pb++;
			}
		}
	}
};

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
		passminute();
	}
}

void gamei::passminute() {
	rounds++; // One round is one minute
	applyboost();
	if((rounds % 5) == 0)
		applypoison();
	if((rounds % 60) == 0)
		applysick();
}

void gamei::enter(indext index, int level, map_object_s stairs) {
	static dungeoni meher_dungeon[] = {{AreaCity, 1},
	{AreaDungeon, 16, -2},
	{AreaDungeonLair, 1, -2},
	};
	if(true) {
		gamestat players;
		if(loc)
			loc.write(getposition(), loc.level);
		setposition(index, level);
		if(!meher_dungeon->create(index, level, false))
			return;
	}
	auto start_position = loc.find(stairs);
	if(start_position != Blocked) {
		for(auto& e : bsmeta <creature>()) {
			if(e && e.is(Friendly))
				e.setposition(loc.getfree(start_position));
		}
	}
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
	command = NoTileObject;
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
		auto player = bsmeta<creature>::elements + e.owner_id;
		if(e.time > rounds) {
			*ps++ = e;
			continue;
		}
		player->add(e.id, e.modifier, player->is(Friendly));
	}
	bsmeta<boosti>::source.setcount(ps - bsmeta<boosti>::elements);
}

void gamei::applysick() {
	for(auto& e : bsmeta<creature>()) {
		if(e)
			e.checksick();
	}
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

void gamei::move(indext index) {
	if(getposition() != index) {
		setposition(index, 0);
		for(auto& e : bsmeta<creature>()) {
			if(!e || !e.is(Friendly))
				continue;
			e.look(index);
			e.setposition(index);
		}
	}
}
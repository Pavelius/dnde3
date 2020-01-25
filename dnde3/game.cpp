#include "main.h"

const auto				OverlandEnergyCost = 24 * 60 * 33 / 7;
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

static void move_creatures_overland() {
	for(auto& e : bsmeta<creature>()) {
		if(!e)
			continue;
		e.makemove();
	}
}

int	gamei::get(skill_s v) const {
	auto total = 0;
	auto count = 0;
	for(auto& e : bsmeta<creature>()) {
		if(!e)
			continue;
		total += e.get(v);
		count++;
	}
	if(count)
		return total / count;
	return 0;
}

void gamei::playoverland() {
	const auto speed = get(Riding);
	while(restore_energy < OverlandEnergyCost) {
		passminute();
		restore_energy += speed;
	}
	auto p = creature::getactive();
	if(!p || p->isbusy() || p->is(Sleep) || command)
		return;
	p->playuioverland();
}

void gamei::playactive() {
	const int moves_per_minute = 30;
	bool need_continue = true;
	while(need_continue) {
		need_continue = true;
		for(auto i = 0; i < moves_per_minute && !command; i++) {
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

static void applysick() {
	for(auto& e : bsmeta<creature>()) {
		if(e)
			e.checksick();
	}
}

static void applyrestore() {
	for(auto& e : bsmeta<creature>()) {
		if(e)
			e.restoration();
	}
}

static void applypoison() {
	for(auto& e : bsmeta<creature>()) {
		if(e)
			e.checkpoison();
	}
}

void gamei::passminute() {
	rounds++; // One round is one minute
	applyboost();
	applyrestore();
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
	overland = false;
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

void gamei::enter() {
	overland = true;
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
	source.matchbs(true); // ������ ��� ��� �����
	source.match(Sleep, true); // ������ ��� ��� ����
	if(source) {
		auto p = creature::getactive();
		if(!p || source.indexof(p) == -1)
			source[0]->activate();
	}
	return true;
}

void gamei::play() {
	while(checkalive()) {
		checkcommand();
		if(overland)
			playoverland();
		else
			playactive();
	}
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

void gamei::use(map_object_s v) {
	command = v;
}

void gamei::move(indext index) {
	auto p = creature::getactive();
	if(!p)
		return;
	if(index == Blocked)
		return;
	// ������ ��������
	auto current_tile = loc.gettile(getposition());
	auto tile = loc.gettile(index);
	switch(tile) {
	case Sea:
		if(current_tile!=Sea && !p->askyn("�� ������������� ������ �������� ����?"))
			return;
		restore_energy -= OverlandEnergyCost * 200 / 100;
		break;
	case Forest:
		restore_energy -= OverlandEnergyCost * 150 / 100;
		break;
	case Mountains:
		if(!find(ClimbingTool)) {
			sb.add("� ��� ���� ������������ ��� ������� �� �����.");
			return;
		}
		restore_energy -= OverlandEnergyCost * 250 / 100;
		break;
	case Hill:
		restore_energy -= OverlandEnergyCost * 125 / 100;
		break;
	case Swamp:
		restore_energy -= OverlandEnergyCost * 200 / 100;
		break;
	default:
		restore_energy -= OverlandEnergyCost * 100 / 100;
		break;
	}
	// ��������� ���

	// ��������
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

void gamei::wait() {
	restore_energy += OverlandEnergyCost;
}

item* gamei::find(item_s v) const {
	for(auto& e : bsmeta<creature>()) {
		if(!e || !e.is(Friendly))
			continue;
		auto r = e.finditem(v);
		if(r)
			return r;
	}
	return 0;
}
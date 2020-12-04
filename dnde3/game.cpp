#include "main.h"

const auto				OverlandEnergyCost = 24 * 60 * 30 / 8;
gamei					game;
static 	map_object_s	command;

class gamestat {

	adat<creature, 8>	players;
	adat<boosti>		boosts;

public:

	void restore() {
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

	void store() {
		auto sp = players.begin();
		auto spe = players.endof();
		auto sb = boosts.begin();
		auto sbe = boosts.end();
		auto spb = sp;
		for(auto& player : bsmeta<creature>()) {
			if(!player)
				continue;
			if(!player.is(Friendly))
				continue;
			// Store player
			player.unlink();
			if(sp < spe)
				*sp = player;
			// Store boost
			auto owner_id = player.getid();
			auto pb = bsmeta<boosti>::elements;
			for(auto& b : bsmeta<boosti>()) {
				if(b.owner_id == owner_id) {
					if(sp < spe && sb < sbe) {
						*sb = b;
						sb->owner_id = sp - spb;
						sb++;
					}
				} else
					*pb++ = b;
			}
			bsmeta<boosti>::source.setcount(pb - bsmeta<boosti>::elements);
			player.clear();
			if(sp < spe)
				sp++;
		}
		players.count = sp - players.data;
		boosts.count = sb - boosts.data;
	}

	void clear() {
		memset(this, 0, sizeof(*this));
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
	if(!p || p->is(Busy) || p->is(Sleep) || command)
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
			if(!p || p->is(Busy) || p->is(Sleep) || command)
				need_continue = false;
		}
		passminute();
	}
}

static void apply(creature::papply p) {
	for(auto& e : bsmeta<creature>()) {
		if(e)
			(e.*p)();
	}
}

void gamei::passminute() {
	rounds++; // One round is one minute
	applyboost();
	apply(&creature::restoration);
	if((rounds % 5) == 0)
		apply(&creature::checkpoison);
	if((rounds % 10) == 0)
		apply(&creature::checkmood);
	if((rounds % 60) == 0)
		apply(&creature::checksick);
	if((rounds % (24 * 60)) == 0)
		loc.growplants();
	if((rounds % (4 * 60)) == 0)
		decoyfood();
}

bool gamei::enter(int level, map_object_s stairs) {
	auto p = getdungeon();
	if(p)
		p = p->find(level);
	if(!p)
		return false;
	gamestat players;
	players.clear();
	players.store();
	write();
	setlevel(level);
	if(isoverland())
		loc.read("game/overland.loc", true);
	else if(!loc.read(getposition(), getlevel())) {
		loc.clear();
		loc.create(*p, level, false, false);
	}
	players.restore();
	if(!creature::getactive()) {
		auto p = creature::getactive(0);
		if(p)
			p->activate();
	}
	if(level > 0) {
		auto start_position = loc.find(stairs);
		if(start_position == Blocked) {
			auto& ei = bsmeta<landscapei>::elements[loc.type];
			auto dir = Right;
			if(creature::getactive())
				dir = creature::getactive()->getdirection();
			start_position = ei.getstart(dir);
		}
		if(start_position == Blocked)
			start_position = loc.get(mmx / 2, mmy / 2);
		for(auto& e : bsmeta <creature>()) {
			if(e && e.is(Friendly))
				e.setposition(loc.getfree(start_position));
		}
		update_los();
	} else
		updatepos();
	return write();
}

void gamei::checkcommand() {
	if(!command)
		return;
	switch(command) {
	case StairsDown:
		enter(getlevel() + 1, StairsUp);
		break;
	case StairsUp:
		if(getlevel() > 0)
			enter(getlevel() - 1, StairsDown);
		break;
	}
	command = NoTileObject;
}

bool gamei::checkalive() {
	creaturea source;
	source.select(Friendly);
	if(!source)
		return false;
	source.match(Busy, true); // Уберем тех кто занят
	source.matchact(Sleep, true); // Уберем тех кто спит
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
		if(isoverland())
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

void gamei::updatepos() {
	auto index = getposition();
	for(auto& e : bsmeta<creature>()) {
		if(!e || !e.is(Friendly))
			continue;
		e.look(index);
		e.setposition(index);
	}
}

void gamei::move(indext index) {
	auto p = creature::getactive();
	if(index == Blocked)
		return;
	// Пункты движения
	auto current_tile = loc.gettile(getposition());
	auto tile = loc.gettile(index);
	auto difficult = 0;
	switch(tile) {
	case Sea:
		if(current_tile != Sea && !p->askyn("Вы действительно хотите пересечь воду?"))
			return;
		difficult += OverlandEnergyCost * 300 / 100;
		break;
	case Forest:
		difficult += OverlandEnergyCost * 200 / 100;
		break;
	case Mountains:
		if(!find(ClimbingTool)) {
			sb.add("У вас нету оборудования для лазания по горам.");
			return;
		}
		difficult += OverlandEnergyCost * 300 / 100;
		for(auto& e : bsmeta<creature>()) {
			if(!e || !e.is(Friendly))
				continue;
			if(!e.roll(Climbing)) {
				e.act("%герой очень долго залазил%а на утес.");
				difficult += OverlandEnergyCost * 50 / 100;
			}
		}
		break;
	case Hill:
		difficult += OverlandEnergyCost * 150 / 100;
		break;
	case Swamp:
		difficult += OverlandEnergyCost * 300 / 100;
		break;
	default:
		difficult += OverlandEnergyCost * 100 / 100;
		break;
	}
	if(loc.is(index, Trailed))
		difficult = difficult * 60 / 100;
	restore_energy -= difficult;
	// Расчитаем еду
	// Движение
	setposition(index);
	updatepos();
	randomencounter();
}

void gamei::setposition(indext v) {
	geoposable::setposition(v);
	outdoor_id = Blocked;
	tile = loc.gettile(v);
	auto p = outdoori::find(v);
	if(p) {
		outdoor_id = p - bsmeta<outdoori>::elements;
		if(p->descriptor)
			sb.add(p->descriptor);
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

const dungeoni* gamei::getdungeon() const {
	if(outdoor_id == Blocked)
		return bsmeta<tilei>::elements[tile].wilderness;
	return bsmeta<outdoori>::elements[outdoor_id].levels;
}

bool gamei::is(variant v) const {
	for(auto& e : bsmeta<creature>()) {
		if(!e || !e.is(Friendly))
			continue;
		if(e.match(v))
			return true;
	}
	return false;
}

void gamei::decoyfood() {
	for(auto& e : bsmeta<creature>()) {
		if(!e)
			continue;
		e.decoyfood();
	}
	for(auto& e : bsmeta<itemground>()) {
		if(!e)
			continue;
		e.decoy();
	}
}

void gamei::randomencounter() {
	if(!isoutdoor())
		return;
	if(d100() >= 15)
		return;
	auto p = encounter::getrandom(tile);
	if(p)
		p->play();
}
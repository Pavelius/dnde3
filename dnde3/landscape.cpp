#include "main.h"

struct vector {
	short unsigned		index;
	direction_s			dir;
};
static vector			rooms[256];
static unsigned char	stack_put, stack_get;
static direction_s		connectors_side[] = {Up, Left, Right, Down};

static slot_s slots_weapons_armor[] = {Melee, Ranged, OffHand, Head, Elbows, Legs, Torso};
static item_s item_treasure[] = {Coin, Coin, Coin, Coin, Coin, Coin, Coin, CoinSP, CoinSP, CoinSP, CoinGP};
static item_s item_food[] = {Ration, Ration, Ration, BreadEvlen, BreadHalflings, BreadDwarven, Sausage};
static item_s item_potion_scrolls[] = {Scroll1, Scroll2, Scroll3,
Scroll1, Scroll2, Scroll3,
Book1, Book2, Book3, Book4, Book5,
Amulet1, Amulet2, Amulet3, Amulet4, Amulet5,
Boot1, Boot2, IronBoot1, IronBoot2, IronBoot3,
Potion1, Potion2, Potion3, Potion4, Potion5,
Wand1, Wand2, Wand3, Wand4, Wand5,
RingRed, RingGreen, RingBlue};

static int compare_rect(const void* p1, const void* p2) {
	auto e1 = (rect*)p1;
	auto e2 = (rect*)p2;
	return e2->width()*e2->height() - e1->width()*e1->height();
}

static void putb(indext index, direction_s dir) {
	auto& e = rooms[stack_put++];
	e.index = index;
	e.dir = dir;
}

static vector& getb() {
	return rooms[stack_get++];
}

static item_s random(aref<item_s> source) {
	if(!source)
		return NoItem;
	return source.data[rand() % source.count];
}

static void show_minimap_step(short unsigned index, bool visualize) {
	if(visualize)
		loc.minimap(index);
}

static void create_big_rooms(const rect& rc, rooma& rooms, bool visualize) {
	if(rc.width() < 4 + 5 || rc.height() < 4 + 3)
		return;
	if(rc.height() < max_building_size * 2 && rc.width() < max_building_size * 2) {
		auto dw = xrand(5, max_building_size - 4);
		if(dw > rc.width() - 4)
			dw = rc.width() - 4;
		auto dh = xrand(3, max_building_size - 7);
		if(dh > rc.height() - 4)
			dh = rc.height() - 4;
		auto dws = (rc.width() - dw - 2);
		auto dhs = (rc.height() - dh - 2);
		if(dws && dhs) {
			auto x1 = rc.x1 + 1 + rand() % dws;
			auto y1 = rc.y1 + 1 + rand() % dhs;
			rooms.add({x1, y1, x1 + dw, y1 + dh});
			if(visualize)
				loc.show(rooms);
		}
	} else if(rc.width() > rc.height()) {
		auto w1 = rc.width() / 2 + (rand() % 8) - 4;
		create_big_rooms({rc.x1, rc.y1, rc.x1 + w1 - 1, rc.y2}, rooms, visualize);
		create_big_rooms({rc.x1 + w1, rc.y1, rc.x2, rc.y2}, rooms, visualize);
	} else {
		auto h1 = rc.height() / 2 + (rand() % 6) - 3;
		create_big_rooms({rc.x1, rc.y1, rc.x2, rc.y1 + h1 - 1}, rooms, visualize);
		create_big_rooms({rc.x1, rc.y1 + h1, rc.x2, rc.y2}, rooms, visualize);
	}
}

static void create_weapon(indext index) {
	static slot_s slots[] = {Melee, Ranged, Amunitions};
	loc.loot(index, slots, loc.level);
}

static void create_armor(indext index) {
	static slot_s slots[] = {Head, Torso, Legs, Elbows, OffHand};
	loc.loot(index, slots, loc.level);
}

static void create_books_and_scrolls(indext index) {
	static slot_s slots[] = {Readable};
	loc.loot(index, slots, loc.level);
}

static void create_potions(indext index) {
	static slot_s slots[] = {Drinkable};
	loc.loot(index, slots, loc.level);
}

static void create_dungeon_item(indext index) {
	static gentileproc chances[] = {create_weapon, create_weapon, create_weapon, create_armor,
		create_armor, create_armor,
		create_books_and_scrolls, create_potions,
	};
	maprnd(chances)(index);
}

static void create_trap(indext index) {
	static trap_s trap_normal[] = {TrapAnimal,
		TrapAcid, TrapCorrupt, TrapCorrosion,
		TrapElectricity, TrapFire, TrapLight, TrapWater,
		TrapArrow, TrapArrow, TrapArrow,
		TrapSpear, TrapSpear,
		TrapPit, TrapPit, TrapPit,
		TrapSpikedPit, TrapBleed};
	loc.set(index, maprnd(trap_normal));
	loc.set(index, Hidden);
}

static void create_treasure(indext index) {
	loc.loot(index, maprnd(item_treasure), loc.level);
}

static void create_monster(indext index) {
	auto p = loc.monster(index);
	p->add(Hostile, 1, false);
	p->setguard(index);
}

static void create_door(indext index) {
	loc.set(index, Door);
	if(d100() < chance_door_closed)
		loc.set(index, Sealed);
}

static void create_corridor_content(indext index) {
	//static gentileproc chances[] = {create_trap};
	static gentileproc chances[] = {create_trap, create_treasure, create_dungeon_item,
		create_monster, create_monster, create_monster,
	};
	maprnd(chances)(index);
}

static void create_connector(short unsigned index, direction_s dir, const rect& correct) {
	auto iterations = xrand(4, 10);
	auto start = Blocked;
	auto tile = Floor;
	for(; iterations > 0; iterations--) {
		auto i0 = loc.to(index, dir); // Forward
		auto i1 = loc.to(i0, dir); // Forward - Forward
		auto i2 = loc.to(i0, loc.to(dir, Left)); // Forward - Left
		auto i3 = loc.to(i0, loc.to(dir, Right)); // Forward - Right
		auto i4 = loc.to(i1, loc.to(dir, Left)); // Forward - Forward - Left
		auto i5 = loc.to(i1, loc.to(dir, Right)); // Forward - Forward - Right
		if(!loc.ismatch(i0, correct) || loc.gettile(i0) == tile)
			return;
		if(iterations == 1
			&& (loc.gettile(i4) == tile || loc.gettile(i5) == tile))
			break;
		index = i0;
		loc.set(index, Floor);
		if(start == Blocked) {
			start = index;
			if(d100() < 60)
				create_door(start);
		} else {
			if(d100() < chance_corridor_content)
				create_corridor_content(index);
		}
		if(loc.gettile(i2) == tile || loc.gettile(i3) == tile)
			return; // Maybe need `break`?
	}
	direction_s rnd[] = {Right, Left, Up};
	zshuffle(rnd, 3);
	int count = 1;
	if(d100() < 50)
		count++;
	if(d100() < 20)
		count++;
	for(auto e : rnd) {
		putb(index, loc.to(dir, e));
		if(--count == 0)
			break;
	}
}

static bool iswall(short unsigned i, direction_s d1, direction_s d2) {
	auto t1 = loc.gettile(loc.to(i, d1));
	auto t2 = loc.gettile(loc.to(i, d2));
	return t1 == Wall && t2 == Wall;
}

static void update_doors() {
	for(short unsigned i = 0; i <= mmx * mmy; i++) {
		if(loc.getobject(i) == Door) {
			if(iswall(i, Left, Right))
				continue;
			if(iswall(i, Up, Down))
				continue;
			loc.set(i, NoTileObject);
		}
	}
}

static void outdoor_floor() {
	loc.create({0, 0, mmx - 1, mmy - 1}, xrand(10, 20), Hill);
	loc.create({0, 0, mmx - 1, mmy - 1}, xrand(4, 12), Swamp);
	loc.create({0, 0, mmx - 1, mmy - 1}, mmx*mmy*dense_forest / 100, Tree);
}

static void create_corridor(int x, int y, int w, int h, direction_s dir) {
	switch(dir) {
	case Up: putb(loc.get(x + rand() % w, y), Up); break;
	case Left: putb(loc.get(x, y + rand() % h), Left); break;
	case Down: putb(loc.get(x + rand() % w, y + h - 1), Down); break;
	default: putb(loc.get(x + w - 1, y + rand() % h), Right); break;
	}
}

static void create_room(int x, int y, int w, int h) {
	for(auto x1 = x; x1 < x + w; x1++) {
		for(auto y1 = y; y1 < y + h; y1++) {
			loc.set(loc.get(x1, y1), Floor);
		}
	}
	auto p = bsmeta<site>::add();
	p->x1 = x;
	p->y1 = y;
	p->x2 = p->x1 + w - 1;
	p->y2 = p->y1 + h - 1;
	p->set(Lair);
}

static void create_dungeon_content(rooma& rooms, bool visualize) {
	rooms.count -= 2; // Two room of lesser size would cutted off
	zshuffle(rooms.data, rooms.count);
	loc.positions[0] = loc.center(rooms[0]);
	loc.set(loc.positions[0], StairsDown);
	loc.positions[1] = loc.center(rooms[1]);
	loc.set(loc.positions[1], StairsUp);
	for(auto& e : rooms)
		create_room(e.x1, e.y1, e.width(), e.height());
	for(auto& e : rooms)
		create_corridor(e.x1, e.y1, e.width(), e.height(), maprnd(connectors_side));
	for(auto& e : rooms)
		create_corridor(e.x1, e.y1, e.width(), e.height(), maprnd(connectors_side));
	rect rc = {2, 2, mmx - 2, mmy - 2};
	while(stack_get != stack_put) {
		auto& e = getb();
		create_connector(e.index, e.dir, rc);
		show_minimap_step(e.index, visualize);
	}
}

template<> landscapei bsmeta<landscapei>::elements[] = {{"Равнина", {}, Plain, {{Tree, 2}, {Water, -16}, {Hill, 1}, {Swamp, -20}}},
{"Лес", {}, Plain, {{Tree, 10}, {Hill, 1}, {Swamp, -20}}},
{"Болото", {}, Plain, {{Tree, -40}, {Swamp, 1}, {Lake, 1}}},
// 
{"Подземелье", {1, 1, 1, 1}, Wall, {}, create_big_rooms, create_dungeon_content},
{"Город", {1, 1, 1, 1}, Plain, {{Tree, 2}, {Water, -16}}},
};

static void fill(const rect& rc, variant id, int value) {
	switch(id.type) {
	case Tile: loc.create(rc, value, (tile_s)id.value); break;
	case Object: loc.create(rc, value, (map_object_s)id.value); break;
	}
}

void location::create(landscape_s landscape, bool explored, bool visualize) {
	auto& ei = bsmeta<landscapei>::elements[landscape];
	clear();
	stack_get = stack_put = 0;
	// Explore all map
	if(explored) {
		auto count = mmx * mmy;
		for(short unsigned i = 0; i < count; i++)
			set(i, Explored);
	}
	rect rc = {0, 0, mmx - 1, mmy - 1};
	rc += ei.border;
	rooma rooms;
	// Initilaize area
	auto count = mmx * mmy;
	for(short unsigned i = 0; i < count; i++)
		loc.set(i, ei.tile);
	for(auto& e : ei.tiles) {
		if(!e.id)
			break;
		if(e.value < 0)
			fill(rc, e.id, xrand(-e.value / 2, -e.value));
		else
			fill(rc, e.id, count*e.value / 100);
	}
	// Create rooms
	if(ei.genarea)
		ei.genarea(rc, rooms, false);
	qsort(rooms.data, rooms.count, sizeof(rooms.data[0]), compare_rect);
	// Object generator (from big to small)
	if(ei.genroom)
		ei.genroom(rooms, visualize && explored);
	// Finish step
	update_doors();
}
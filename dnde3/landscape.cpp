#include "main.h"

struct vector {
	short unsigned		index;
	direction_s			dir;
};
static vector			rooms[256];
static unsigned char	stack_put, stack_get;
static direction_s		connectors_side[] = {Up, Left, Right, Down};
static slot_s			slots_weapons_armor[] = {Melee, Ranged, OffHand, Head, Elbows, Legs, Torso};
static item_s			item_treasure[] = {Coin, Coin, Coin, Coin, Coin, Coin, Coin, CoinSP, CoinSP, CoinSP, CoinGP};

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
		loc.minimap(index, false);
}

static void create_big_rooms(const rect& rc, rooma& rooms, const landscapei& land, bool visualize) {
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
		create_big_rooms({rc.x1, rc.y1, rc.x1 + w1 - 1, rc.y2}, rooms, land, visualize);
		create_big_rooms({rc.x1 + w1, rc.y1, rc.x2, rc.y2}, rooms, land, visualize);
	} else {
		auto h1 = rc.height() / 2 + (rand() % 6) - 3;
		create_big_rooms({rc.x1, rc.y1, rc.x2, rc.y1 + h1 - 1}, rooms, land, visualize);
		create_big_rooms({rc.x1, rc.y1 + h1, rc.x2, rc.y2}, rooms, land, visualize);
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

static void create_corridor(int x, int y, int w, int h, direction_s dir) {
	switch(dir) {
	case Up: putb(loc.get(x + rand() % w, y), Up); break;
	case Left: putb(loc.get(x, y + rand() % h), Left); break;
	case Down: putb(loc.get(x + rand() % w, y + h - 1), Down); break;
	default: putb(loc.get(x + w - 1, y + rand() % h), Right); break;
	}
}

static void create_dungeon_content(const rect& rc, rooma& rooms, const landscapei& land, bool visualize) {
	rooms.count -= 2; // Two room of lesser size would cutted off
	zshuffle(rooms.data, rooms.count);
	int index = 0;
	int index_maximum = sizeof(land.objects) / sizeof(land.objects[0]);
	for(const auto& e : rooms) {
		loc.fill(e, Floor);
		auto t = EmpthyRoom;
		if(index < index_maximum && land.objects[index]) {
			t = land.objects[index];
			loc.addposition(loc.center(e));
			index++;
		}
		auto p = loc.addsite(t, e);
		p->setposition(loc.center(e));
		loc.content(e, t, p);
	}
	for(auto& e : rooms)
		create_corridor(e.x1, e.y1, e.width(), e.height(), maprnd(connectors_side));
	for(auto& e : rooms)
		create_corridor(e.x1, e.y1, e.width(), e.height(), maprnd(connectors_side));
	while(stack_get != stack_put) {
		auto& e = getb();
		create_connector(e.index, e.dir, rc);
		show_minimap_step(e.index, visualize);
	}
}

static void create_road(const rect& rc) {
	loc.fill(rc, Road);
}

static void create_city_buildings(const rect& rc, rooma& rooms, const landscapei& land, bool visualize) {
	rect r1 = rc.getoffset(2, 2);
	auto current = 1;
	auto max_possible_points = rooms.getcount() / 3;
	if(max_possible_points > 25)
		max_possible_points = 25;
	int index = 0;
	int index_maximum = sizeof(land.objects)/ sizeof(land.objects[0]);
	for(auto& e : rooms) {
		auto t = (room_s)xrand(Temple, ShopFood);
		if(current > max_possible_points)
			t = House;
		if(index < index_maximum && land.objects[index]) {
			t = land.objects[index];
			index++;
		}
		auto p = loc.addsite(t, e);
		auto door = loc.building(e);
		rect r2; loc.interior(e, t, door, 0, &r2, p);
		p->setposition(loc.center(r2));
		current++;
		show_minimap_step(p->getposition(), visualize);
	}
}

static void create_city_level(const rect& rc, int level, rooma& rooms, bool visualize) {
	auto w = rc.width();
	auto h = rc.height();
	if(d100() < chance_special_area &&
		w > max_building_size && w<max_building_size * 3 && h>max_building_size && h < max_building_size * 3) {
		switch(rand() % 4) {
		case 1:
			loc.forest({rc.x1, rc.y1, rc.x2 - 3, rc.y2 - 3});
			break;
		default:
			loc.lake({rc.x1 + rand() % 3, rc.y1 + rand() % 3, rc.x2 - 3 * 2, rc.y2 - 3 * 2});
			break;
		}
		return;
	}
	if(w <= max_building_size && h <= max_building_size) {
		if(w > 6 && h > 6)
			rooms.add({rc.x1, rc.y1, rc.x2 - 3, rc.y2 - 3});
		return;
	}
	// Calculate direction
	int m = xrand(30, 60);
	int r = -1;
	if(w / 3 >= h / 2)
		r = 0;
	else if(h / 3 >= w / 2)
		r = 1;
	if(r == -1)
		r = (d100() < 50) ? 0 : 1;
	if(r == 0) {
		int w1 = (w*m) / 100; // horizontal
		create_city_level({rc.x1, rc.y1, rc.x1 + w1, rc.y2}, level + 1, rooms, visualize);
		create_city_level({rc.x1 + w1 + 1, rc.y1, rc.x2, rc.y2}, level + 1, rooms, visualize);
		if(level <= 2) {
			auto r1 = rc;
			if(r1.y2 >= mmy - 3)
				r1.y2 = mmy - 1;
			if(r1.y1 <= 2)
				r1.y1 = 0;
			rect r2 = {r1.x1 + w1 - 2, r1.y1, r1.x1 + w1, r1.y2};
			create_road(r2);
			for(int i = xrand(0, 4); i > 0; i--)
				loc.commoner(loc.getrand(r2));
		}
	} else {
		int h1 = (h*m) / 100; // vertial
		create_city_level({rc.x1, rc.y1, rc.x2, rc.y1 + h1}, level + 1, rooms, visualize);
		create_city_level({rc.x1, rc.y1 + h1 + 1, rc.x2, rc.y2}, level + 1, rooms, visualize);
		if(level <= 2) {
			auto r1 = rc;
			if(r1.x2 >= mmx - 3)
				r1.x2 = mmx - 1;
			if(r1.x1 <= 2)
				r1.x1 = 0;
			rect r2 = {r1.x1, r1.y1 + h1 - 2, r1.x2, r1.y1 + h1};
			create_road(r2);
			for(int i = xrand(0, 4); i > 0; i--)
				loc.commoner(loc.getrand(r2));
		}
	}
}

static void create_city(const rect& rc, rooma& rooms, const landscapei& land, bool visualize) {
	create_city_level(rc, 0, rooms, visualize);
}

template<> landscapei bsmeta<landscapei>::elements[] = {{"Равнина", 0, Plain, {{Tree, 2}, {Water, -16}, {Hill, 1}, {Swamp, -20}}},
{"Лес", 0, Plain, {{Tree, 10}, {Hill, 1}, {Swamp, -20}}},
{"Болото", 0, Plain, {{Tree, -40}, {Swamp, 1}, {Lake, 1}}},
// 
{"Подземелье", 1, Wall, {}, {StairsDownRoom, StairsUpRoom}, create_big_rooms, create_dungeon_content},
{"Логово", 1, Wall, {}, {StairsUpRoom}, create_big_rooms, create_dungeon_content},
{"Город", 1, Plain, {{Tree, 2}, {Water, -16}}, {StairsDownRoom, Barracs, Lair}, create_city, create_city_buildings},
};
assert_enum(landscape, AreaCity);

void location::create(const dungeoni& source, int level, bool explored, bool visualize) {
	clear();
	*static_cast<dungeoni*>(this) = source;
	auto& ei = bsmeta<landscapei>::elements[type];
	this->level = level;
	stack_get = stack_put = 0;
	// Explore all map
	if(explored) {
		auto count = mmx * mmy;
		for(short unsigned i = 0; i < count; i++)
			set(i, Explored);
	}
	rect rc = {0, 0, mmx - 1, mmy - 1};
	rooma rooms;
	// Initilaize area
	auto count = mmx * mmy;
	for(short unsigned i = 0; i < count; i++)
		loc.set(i, ei.tile);
	for(auto& e : ei.tiles) {
		if(!e.id)
			break;
		if(e.value < 0)
			loc.fill(rc, xrand(-e.value / 2, -e.value), e.id);
		else
			loc.fill(rc, count*e.value / 100, e.id);
	}
	rc.x1 += ei.border; rc.y1 += ei.border;
	rc.x2 -= ei.border; rc.y2 -= ei.border;
	// Create rooms
	if(ei.genarea)
		ei.genarea(rc, rooms, ei, visualize);
	qsort(rooms.data, rooms.count, sizeof(rooms.data[0]), compare_rect);
	// Object generator (from big to small)
	if(ei.genroom)
		ei.genroom(rc, rooms, ei, visualize);
	// Finish step
	update_doors();
}
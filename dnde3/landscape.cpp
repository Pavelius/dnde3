#include "main.h"

const bool				visualize_dungeon = false;
const bool				visualize_dungeon_final = true;
const int				chance_line_corridor = 90;
const int				chance_create_door = 10;
const int				chance_hidden_door = 30;
const int				minimum_corridor_lenght = 2;

struct vector {
	short unsigned		index;
	direction_s			dir;
};
static vector			rooms[256];
static unsigned char	stack_put, stack_get;
static direction_s		connectors_side[] = {Up, Left, Right, Down};
static slot_s			slots_weapons_armor[] = {Melee, Ranged, OffHand, Head, Elbows, Legs, Torso};
static item_s			item_treasure[] = {Coin, Coin, Coin, Coin, Coin, Coin, CoinSP, CoinSP, CoinSP, CoinGP};

static int compare_rect(const void* p1, const void* p2) {
	auto e1 = (rect*)p1;
	auto e2 = (rect*)p2;
	return e2->width() * e2->height() - e1->width() * e1->height();
}

static void putb(indext index, direction_s dir) {
	auto& e = rooms[stack_put++];
	e.index = index;
	e.dir = dir;
}

static vector& getb() {
	return rooms[stack_get++];
}

static void show_minimap_step(indext index) {
	if(visualize_dungeon)
		loc.minimap(index, false);
}

static bool check_room(const rect& rc, const rooma& rooms) {
	if(rc.x1 < 2 || rc.y1 < 2)
		return false;
	if(rc.x2 >= mmx - 2 || rc.y2 >= mmy - 2)
		return false;
	for(auto& e : rooms) {
		if(e.intersect(rc))
			return false;
	}
	return true;
}

static void create_dungeon_rooms(const rect& rc, rooma& rooms, const landscapei& land) {
	const auto try_maximum = 1000;
	auto count = xrand(18, 32);
	auto try_threshold = try_maximum;
	for(auto i = 0; i < count; i++) {
		rect room;
		room.x1 = xrand(rc.x1, rc.x2);
		room.x2 = room.x1 + xrand(max_building_size / 2, max_building_size);
		room.y1 = xrand(rc.y1, rc.y2);
		room.y2 = room.y1 + xrand(max_building_size / 2, max_building_size);
		if(!check_room(room, rooms)) {
			i--;
			try_threshold--;
			if(try_threshold == 0) {
				count--;
				try_threshold = try_maximum;
			}
			continue;
		}
		rooms.add(room);
	}
	// Change room size
	for(auto& e : rooms)
		e.offset(1, 1);
}

static void create_big_rooms(const rect& rc, rooma& rooms, const landscapei& land) {
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
			if(visualize_dungeon)
				loc.show(rooms);
		}
	} else if(rc.width() > rc.height()) {
		auto w1 = rc.width() / 2 + (rand() % 8) - 4;
		create_big_rooms({rc.x1, rc.y1, rc.x1 + w1 - 1, rc.y2}, rooms, land);
		create_big_rooms({rc.x1 + w1, rc.y1, rc.x2, rc.y2}, rooms, land);
	} else {
		auto h1 = rc.height() / 2 + (rand() % 6) - 3;
		create_big_rooms({rc.x1, rc.y1, rc.x2, rc.y1 + h1 - 1}, rooms, land);
		create_big_rooms({rc.x1, rc.y1 + h1, rc.x2, rc.y2}, rooms, land);
	}
}

static void create_weapon(indext index) {
	loc.loot(index, {Melee, Ranged, Amunitions}, loc.level, 0, Unknown, 0);
}

static void create_armor(indext index) {
	loc.loot(index, {Head, Torso, Legs, Elbows, OffHand}, loc.level, 0, Unknown, 0);
}

static void create_books_and_scrolls(indext index) {
	loc.loot(index, {Readable}, loc.level, 0, Unknown, 0);
}

static void create_potions(indext index) {
	loc.loot(index, {Drinkable}, loc.level, 0, Unknown, 0);
}

static void create_dungeon_item(indext index) {
	static gentileproc chances[] = {create_weapon, create_weapon, create_weapon,
		create_armor, create_armor, create_armor,
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
	loc.loot(index, maprnd(item_treasure), loc.level, 0, Unknown, 0);
}

static void create_monster(indext index) {
	auto p = loc.monster(index);
	if(!p)
		return;
	p->add(variant(Hostile), 1, false);
	p->setguard(index);
}

static void create_door(indext index, bool hidden) {
	loc.set(index, Door);
	if(d100() < chance_door_closed)
		loc.set(index, Sealed);
	if(hidden)
		loc.hide(index);
}

static void create_corridor_content(indext index) {
	static gentileproc chances[] = {create_trap, create_dungeon_item,
		create_treasure, create_treasure,
		create_monster, create_monster, create_monster,
	};
	maprnd(chances)(index);
}

static bool isallowcorridor(indext index, tile_s tile, const rect& correct, direction_s dir) {
	auto i0 = loc.to(index, dir); // Forward
	auto i1 = loc.to(i0, dir); // Forward - Forward
	if(!loc.ismatch(i0, correct))
		return false;
	if(loc.is(i0, Explored))
		return true;
	if(loc.gettile(i0) == tile)
		return false;
	if(loc.is(i1, Explored))
		return true;
	if(loc.gettile(i1) == tile)
		return false;
	auto d1 = loc.to(dir, Left);
	auto d2 = loc.to(dir, Right);
	auto i2 = loc.to(i0, d1); // Forward - Left
	auto i3 = loc.to(i0, d2); // Forward - Right
	if(loc.gettile(i2) == tile || loc.gettile(i3) == tile)
		return false;
	auto i4 = loc.to(i1, d1); // Forward - Forward - Left
	auto i5 = loc.to(i1, d2); // Forward - Forward - Right
	if(loc.gettile(i4) == tile || loc.gettile(i5) == tile)
		return false;
	return true;
}

static void create_connector(indext index, direction_s dir, const rect& correct) {
	auto start = Blocked;
	auto tile = Floor;
	auto count = 0;
	while(true) {
		if(!isallowcorridor(index, tile, correct, dir))
			break;
		auto i0 = loc.to(index, dir); // Forward
		if(loc.is(i0, Explored))
			return;
		if(loc.gettile(i0) == tile)
			return;
		loc.set(i0, Floor);
		if(start == Blocked) {
			start = i0;
			if(d100() < chance_create_door)
				create_door(i0, d100() < chance_hidden_door);
		} else {
			if(d100() < chance_corridor_content)
				create_corridor_content(i0);
		}
		index = i0;
		if(count >= minimum_corridor_lenght && d100() >= chance_line_corridor)
			break;
		count++;
	}
	show_minimap_step(index);
	direction_s rnd[] = {Right, Left, Up};
	zshuffle(rnd, 3);
	for(auto e : rnd) {
		auto d1 = loc.to(dir, e);
		if(!isallowcorridor(index, tile, correct, d1))
			continue;
		create_connector(index, d1, correct);
	}
}

static bool isboth(short unsigned i, direction_s d1, direction_s d2, tile_s tile) {
	auto t1 = loc.gettile(loc.to(i, d1));
	auto t2 = loc.gettile(loc.to(i, d2));
	return t1 == tile && t2 == tile;
}

static bool isoneof(short unsigned i, direction_s d1, direction_s d2, tile_s tile) {
	auto t1 = loc.gettile(loc.to(i, d1));
	auto t2 = loc.gettile(loc.to(i, d2));
	return t1 == tile || t2 == tile;
}

static bool isoneof(short unsigned i, direction_s d1, direction_s d2, map_object_s v) {
	auto t1 = loc.getobject(loc.to(i, d1));
	auto t2 = loc.getobject(loc.to(i, d2));
	return t1 == v || t2 == v;
}

static void update_doors() {
	for(short unsigned i = 0; i <= mmx * mmy; i++) {
		if(loc.getobject(i) == Door) {
			if(isboth(i, Left, Right, Wall) && !isoneof(i, Up, Down, Wall) && !isoneof(i, Up, Down, Door))
				continue;
			if(isboth(i, Up, Down, Wall) && !isoneof(i, Left, Right, Wall) && !isoneof(i, Left, Right, Door))
				continue;
			loc.set(i, NoTileObject);
		}
	}
}

static void update_creatures() {
	for(auto& e : bsdata<creature>()) {
		if(!e)
			continue;
		auto i = e.getposition();
		if(loc.getobject(i) == Door)
			e.clear();
	}
}

static void update_items() {
	for(auto& e : bsdata<itemground>()) {
		if(!e || e.index == Blocked)
			continue;
		if(loc.getobject(e.index) == Door)
			e.clear();
	}
}

static void create_corridor(int x, int y, int w, int h, direction_s dir, const rect& correct) {
	switch(dir) {
	case Up: create_connector(loc.get(x + rand() % w, y), Up, correct); break;
	case Left: create_connector(loc.get(x, y + rand() % h), Left, correct); break;
	case Down: create_connector(loc.get(x + rand() % w, y + h - 1), Down, correct); break;
	default: create_connector(loc.get(x + w - 1, y + rand() % h), Right, correct); break;
	}
}

static void create_room_door(int x, int y, bool hidden) {
	auto i = loc.get(x, y);
	if(loc.getobject(i) == Door) {
		if(hidden)
			loc.hide(i);
		return;
	}
	if(loc.gettile(i) == Floor)
		create_door(i, hidden);
}

static void create_doors(const rect& rc) {
	auto hidden_room = d100() < chance_hidden_door / 2;
	for(auto x = rc.x1; x <= rc.x2; x++) {
		create_room_door(x, rc.y1, hidden_room);
		create_room_door(x, rc.y2, hidden_room);
	}
	for(auto y = rc.y1; y <= rc.y2; y++) {
		create_room_door(rc.x1, y, hidden_room);
		create_room_door(rc.x2, y, hidden_room);
	}
}

static void create_dungeon_rooms(rooma& rooms) {
	rooms.count -= 2; // Two room of lesser size would cutted off
	zshuffle(rooms.data, rooms.count);
	int index = 0, current = 0;
	int index_maximum = sizeof(loc.rooms) / sizeof(loc.rooms[0]);
	auto max_possible_points = 2 * rooms.getcount() / 3;
	if(max_possible_points > 25)
		max_possible_points = 25;
	for(const auto& e : rooms) {
		loc.fille(e, Floor);
		auto t = (room_s)xrand(RoomOfBlood, RoomOfWind);
		if(current > max_possible_points)
			t = EmpthyRoom;
		if(index < index_maximum && loc.rooms[index]) {
			t = loc.rooms[index];
			index++;
		}
		auto p = loc.addsite(t, e);
		p->setposition(loc.center(e));
		loc.content(e, t, p);
		current++;
	}
}

static void create_dungeon_content(const rect& rc, rooma& rooms, const landscapei& land) {
	create_dungeon_rooms(rooms);
	for(auto& e : rooms) {
		direction_s side[4]; memcpy(side, connectors_side, sizeof(connectors_side));
		zshuffle(side, sizeof(side) / sizeof(side[0]));
		create_corridor(e.x1, e.y1, e.width(), e.height(), side[0], rc);
		if(d100() < 50)
			create_corridor(e.x1, e.y1, e.width(), e.height(), side[1], rc);
		if(d100() < 10)
			create_corridor(e.x1, e.y1, e.width(), e.height(), side[2], rc);
	}
	for(auto& e : rooms) {
		auto r1 = e;
		r1.offset(-1, -1);
		create_doors(r1);
	}
}

static void create_road(const rect& rc) {
	loc.fill(rc, Road);
}

static void create_city_buildings(const rect& rc, rooma& rooms, const landscapei& land) {
	rect r1 = rc.getoffset(2, 2);
	auto current = 1;
	auto max_possible_points = rooms.getcount() / 3;
	if(max_possible_points > 25)
		max_possible_points = 25;
	int index = 0;
	int index_maximum = sizeof(loc.rooms) / sizeof(loc.rooms[0]);
	for(auto& e : rooms) {
		auto t = (room_s)xrand(Temple, ShopFood);
		if(current > max_possible_points)
			t = House;
		if(index < index_maximum && loc.rooms[index]) {
			t = loc.rooms[index];
			index++;
		}
		auto p = loc.addsite(t, e);
		if(t!=House)
			p->set(KnownSite);
		auto door = loc.building(e);
		rect r2; loc.interior(e, t, door, 0, &r2, p);
		p->setposition(loc.center(r2));
		current++;
		show_minimap_step(p->getposition());
	}
}

static void create_city_level(const rect& rc, int level, rooma& rooms) {
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
		int w1 = (w * m) / 100; // horizontal
		create_city_level({rc.x1, rc.y1, rc.x1 + w1, rc.y2}, level + 1, rooms);
		create_city_level({rc.x1 + w1 + 1, rc.y1, rc.x2, rc.y2}, level + 1, rooms);
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
		int h1 = (h * m) / 100; // vertial
		create_city_level({rc.x1, rc.y1, rc.x2, rc.y1 + h1}, level + 1, rooms);
		create_city_level({rc.x1, rc.y1 + h1 + 1, rc.x2, rc.y2}, level + 1, rooms);
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

static void create_city(const rect& rc, rooma& rooms, const landscapei& land) {
	create_city_level(rc, 0, rooms);
}

static indext center_start(direction_s dir) {
	return loc.get(mmx / 2, mmy / 2);
}

static indext road_start(direction_s dir) {
	switch(dir) {
	case Right:
	case RightUp:
	case RightDown:
		return loc.find(Road, {0, 8, 8, mmy - 16});
	case Left:
	case LeftUp:
	case LeftDown:
		return loc.find(Road, {mmx - 8, 8, 8, mmx - 1});
	case Up:
		return loc.find(Road, {0, mmy - 8, mmx - 1, mmy - 1});
	default:
		return loc.find(Road, {0, 0, mmx - 1, 8});
	}
}

template<> landscapei bsdata<landscapei>::elements[] = {
	{"Равнина", 0, Plain, {{Tree, 2}, {Water, -16}, {Hill, 1}, {Swamp, -20}, {Plants, 1}}, 0, 0, center_start},
	{"Лес", 0, Plain, {{Tree, 12}, {Hill, 1}, {Swamp, -20}, {Plants, 1}}, 0, 0, center_start},
	{"Болото", 0, Plain, {{Tree, -40}, {Swamp, 1}, {Lake, 1}}, 0, 0, center_start},
	{"Подземелье", 1, Wall, {}, create_dungeon_rooms, create_dungeon_content, center_start},
	{"Город", 1, Plain, {{Tree, 2}, {Water, -16}}, create_city, create_city_buildings, road_start},
};
assert_enum(landscapei, AreaCity)

void location::create(const dungeoni& source, int level, bool explored) {
	clear();
	*static_cast<dungeoni*>(this) = source;
	auto& ei = bsdata<landscapei>::elements[type];
	this->level = level;
	stack_get = stack_put = 0;
	rect rc = {0, 0, mmx - 1, mmy - 1};
	rooma rooms;
	// Initilaize area
	auto count = mmx * mmy;
	for(short unsigned i = 0; i < count; i++)
		set(i, ei.tile);
	for(auto& e : ei.tiles) {
		if(!e.id)
			break;
		if(e.value < 0)
			fill(rc, xrand(-e.value / 2, -e.value), e.id);
		else
			fill(rc, count * e.value / 100, e.id);
	}
	rc.x1 += ei.border; rc.y1 += ei.border;
	rc.x2 -= ei.border; rc.y2 -= ei.border;
	// Create rooms
	if(ei.genarea)
		ei.genarea(rc, rooms, ei);
	qsort(rooms.data, rooms.count, sizeof(rooms.data[0]), compare_rect);
	// Object generator (from big to small)
	if(ei.genroom)
		ei.genroom(rc, rooms, ei);
	// Finish step
	update_doors();
	update_items();
	update_creatures();
	if(visualize_dungeon_final)
		minimap(Blocked, false);
	// Explore all map
	if(explored) {
		auto count = mmx * mmy;
		for(short unsigned i = 0; i < count; i++)
			set(i, Explored);
	} else {
		auto count = mmx * mmy;
		for(short unsigned i = 0; i < count; i++)
			remove(i, Explored);
	}
}
#include "main.h"

const int chance_generate_room = 40;
const int chance_special_area = 5;
const int chance_corridor_content = 10;
const int chance_door_closed = 30;
const int dense_forest = 15;
const int max_building_size = 15;

struct vector {
	short unsigned		index;
	direction_s			dir;
	char				chance;
};
static vector			rooms[256];
static unsigned char	stack_put, stack_get;
static direction_s		connectors_side[] = {Up, Left, Right, Down};

static slot_s slots_weapons_armor[] = {Melee, Ranged, OffHand, Head, Elbows, Legs, Torso};
static item_s item_treasure[] = {Coin, Coin, Coin, Coin, RingRed};
static item_s item_food[] = {Ration, Ration, Ration, BreadEvlen, BreadHalflings, BreadDwarven, Sausage};
static item_s item_potion_scrolls[] = {Scroll1, Scroll2, Scroll3,
Scroll1, Scroll2, Scroll3,
Book1, Book2, Book3, Book4, Book5,
Amulet1, Amulet2, Amulet3, Amulet4, Amulet5,
Boot1, Boot2, IronBoot1, IronBoot2, IronBoot3,
Potion1, Potion2, Potion3, Potion4, Potion5,
Wand1, Wand2, Wand3, Wand4, Wand5,
RingRed, RingGreen, RingBlue};

static void putb(indext index, direction_s dir) {
	auto& e = rooms[stack_put++];
	e.index = index;
	e.dir = dir;
}

static vector& getb() {
	return rooms[stack_get++];
}

void location::dungeonc(rooma& rooms) {
	rooms.count -= 2; // Two room of lesser size would cutted off
	zshuffle(rooms.data, rooms.count);
	positions[0] = center(rooms[0]);
	set(positions[0], StairsDown);
	positions[1] = center(rooms[1]);
	set(positions[1], StairsUp);
	for(auto& e : rooms)
		room(e);
	for(auto& e : rooms)
		corridor(e, maprnd(connectors_side));
	for(auto& e : rooms)
		corridor(e, maprnd(connectors_side));
	rect rc = {2, 2, mmx - 2, mmy - 2};
	while(stack_get != stack_put) {
		auto& e = getb();
		//connector(e.index, e.dir, rc);
	}
}

void location::corridor(const rect& rc, direction_s dir) {
	switch(dir) {
	case Up: putb(get(rc.x1 + rand() % rc.width(), rc.y1), Up); break;
	case Left: putb(get(rc.x1, rc.y1 + rand() % rc.height()), Left); break;
	case Down: putb(get(rc.x1 + rand() % rc.width(), rc.y2 - 1), Down); break;
	default: putb(get(rc.x2 - 1, rc.y1 + rand() % rc.height()), Right); break;
	}
}

void location::connector(indext index, direction_s dir, const rect& correct) {
	auto iterations = xrand(4, 10);
	auto start = Blocked;
	auto tile = Floor;
	for(; iterations > 0; iterations--) {
		auto i0 = to(index, dir); // Forward
		auto i1 = to(i0, dir); // Forward - Forward
		auto i2 = to(i0, to(dir, Left)); // Forward - Left
		auto i3 = to(i0, to(dir, Right)); // Forward - Right
		auto i4 = to(i1, to(dir, Left)); // Forward - Forward - Left
		auto i5 = to(i1, to(dir, Right)); // Forward - Forward - Right
		if(!ismatch(i0, correct) || gettile(i0) == tile)
			return;
		if(iterations == 1
			&& (gettile(i4) == tile || gettile(i5) == tile))
			break;
		index = i0;
		set(index, Floor);
		if(start == Blocked) {
			start = index;
			if(d100() < 60)
				set(start, Door);
		} else {
			//if(d100() < chance_corridor_content)
			//	create_corridor_content(index);
		}
		if(gettile(i2) == tile || gettile(i3) == tile)
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
		putb(index, to(dir, e));
		if(--count == 0)
			break;
	}
}
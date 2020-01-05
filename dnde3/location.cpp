#include "main.h"

static location*		current_location;
static short unsigned	movements[mmx*mmy];
static short unsigned	stack[256 * 256];
static direction_s		all_aroud[] = {Left, Right, Up, Down, LeftDown, LeftUp, RightDown, RightUp};

static const direction_s orientations_7b7[49] = {
	LeftUp, LeftUp, Up, Up, Up, RightUp, RightUp,
	LeftUp, LeftUp, LeftUp, Up, RightUp, RightUp, RightUp,
	Left, Left, LeftUp, Up, RightUp, Right, Right,
	Left, Left, Left, Center, Right, Right, Right,
	Left, Left, LeftDown, Down, RightDown, Right, Right,
	LeftDown, LeftDown, LeftDown, Down, RightDown, RightDown, RightDown,
	LeftDown, LeftDown, Down, Down, Down, RightDown, RightDown,
};

void location::activate() {
	current_location = this;
}

location* location::getactive() {
	return current_location;
}

int location::getindex(indext i, tile_s e) const {
	static direction_s dir[] = {Up, Down, Left, Right};
	auto m = 0;
	auto f = 1;
	for(auto d : dir) {
		auto i1 = to(i, d);
		if(i1 == Blocked)
			m |= f;
		else {
			auto n = gettile(i1);
			if(n != e)
				m |= f;
		}
		f = f << 1;
	}
	return m;
}

indext location::to(indext index, direction_s id) {
	switch(id) {
	case Left:
		if(getx(index) == 0)
			return Blocked;
		return index - 1;
	case Right:
		if(getx(index) >= mmx - 1)
			return Blocked;
		return index + 1;
	case Up:
		if(gety(index) == 0)
			return Blocked;
		return index - mmx;
	case Down:
		if(gety(index) >= mmy - 1)
			return Blocked;
		return index + mmx;
	case LeftUp:
		if(getx(index) == 0)
			return Blocked;
		if(gety(index) == 0)
			return Blocked;
		return index - 1 - mmx;
	case LeftDown:
		if(getx(index) == 0)
			return Blocked;
		if(gety(index) >= mmy - 1)
			return Blocked;
		return index - 1 + mmx;
	case RightUp:
		if(getx(index) >= mmx - 1)
			return -1;
		if(gety(index) == 0)
			return Blocked;
		return index + 1 - mmx;
	case RightDown:
		if(getx(index) >= mmx - 1)
			return Blocked;
		if(gety(index) >= mmy - 1)
			return Blocked;
		return index + 1 + mmx;
	case Center:
		return index;
	default:
		return Blocked;
	}
}

void location::clear() {
	memset(this, 0, sizeof(*this));
	for(auto& e : random)
		e = rand() % 256;
}

static void correct(rect& rc) {
	if(rc.x1 < 0)
		rc.x1 = 0;
	if(rc.y1 < 0)
		rc.y1 = 0;
	if(rc.x2 > mmx - 1)
		rc.x2 = mmx - 1;
	if(rc.y2 > mmy - 1)
		rc.y2 = mmy - 1;
}

tile_s location::gettile(indext i) const {
	if(i == Blocked)
		return Wall;
	return tiles[i];
}

void location::drop(indext i, item v) {
	itemground* pi = 0;
	for(auto& e : bsmeta<itemground>()) {
		if(e)
			continue;
		pi = &e;
	}
	if(!pi)
		pi = bsmeta<itemground>::add();
	*static_cast<item*>(pi) = v;
	pi->index = i;
}

void location::addinfo(indext i, stringbuilder& sb) const {
	static const char* blooded[] = {"запачконое", "запачканный", "запачканна€", "запачканные"};
	static const char* webbed[] = {"покрытое", "покрытый", "покрыта€", "покрытые"};
	if(i == Blocked)
		return;
	auto t = gettile(i);
	sb.add("Ёто");
	if(true) {
		auto count = 0;
		if(is(i, Blooded)) {
			sb.adds("%1 кровью", blooded[bsmeta<tilei>::elements[t].gender]);
			count++;
		}
		if(is(i, Webbed)) {
			sb.adds("%1 паутиной", webbed[bsmeta<tilei>::elements[t].gender]);
			count++;
		}
		sb.adds("%-1.", getstr(t));
	}
	auto o = getobject(i);
	if(o)
		sb.adds("«десь находитс€ %1.", getstr(o));
	auto p = creature::find(i);
	if(p)
		sb.adds("«десь стоит %1.", p->getname());
}

direction_s	location::getdirection(point s, point d) {
	const int osize = 7;
	int dx = d.x - s.x;
	int dy = d.y - s.y;
	int st = (2 * imax(iabs(dx), iabs(dy)) + osize - 1) / osize;
	if(!st)
		return Center;
	int ax = dx / st;
	int ay = dy / st;
	return orientations_7b7[(ay + (osize / 2))*osize + ax + (osize / 2)];
}

direction_s	location::getdirection(indext from, indext to) {
	short x1 = getx(from); short y1 = gety(from);
	short x2 = getx(to); short y2 = gety(to);
	return getdirection({x1, y1}, {x2, y2});
}

void location::setcamera(indext i) {
	if(i != Blocked)
		setcamera(getx(i), gety(i));
}

void location::create(const rect& rc, int count, map_object_s v) {
	for(int i = 0; i < count; i++) {
		int x1 = xrand(rc.x1, rc.x2);
		int y1 = xrand(rc.y1, rc.y2);
		set(get(x1, y1), v);
	}
}

void location::create(const rect& rc, int count, tile_s v) {
	for(int i = 0; i < count; i++) {
		int x1 = xrand(rc.x1, rc.x2);
		int y1 = xrand(rc.y1, rc.y2);
		set(get(x1, y1), v);
	}
}

void location::set(indext index, tile_s v, int width, int height) {
	if(index == Blocked)
		return;
	auto x1 = getx(index);
	auto y1 = gety(index);
	auto x2 = x1 + width;
	auto y2 = y1 + height;
	if(x2 > mmx)
		x2 = mmx;
	if(y2 > mmy)
		y2 = mmy;
	for(auto y = y1; y < y2; y++) {
		for(auto x = x1; x < x2; x++)
			set(get(x, y), v);
	}
}

void location::ellipse(rect rc, tile_s object) {
	int a = iabs(rc.width()), b = iabs(rc.height()), b1 = b & 1;
	long dx = 4 * (1 - a)*b*b, dy = 4 * (b1 + 1)*a*a;
	long err = dx + dy + b1 * a*a, e2; /* error of 1.step */
	if(rc.x1 > rc.x2) {
		rc.x1 = rc.x2;
		rc.x2 += a;
	} /* if called with swapped points */
	if(rc.y1 > rc.y2)
		rc.y1 = rc.y2; /* .. exchange them */
	rc.y1 += (b + 1) / 2;
	rc.y2 = rc.y1 - b1;   /* starting pixel */
	a *= 8 * a; b1 = 8 * b*b;
	do {
		set(get(rc.x1, rc.y1), object, rc.x2 - rc.x1, 1);
		set(get(rc.x1, rc.y2), object, rc.x2 - rc.x1, 1);
		e2 = 2 * err;
		if(e2 <= dy) {
			rc.y1++;
			rc.y2--;
			err += dy += a;
		}  /* y step */
		if(e2 >= dx || 2 * err > dy) {
			rc.x1++; rc.x2--;
			err += dx += b1;
		} /* x step */
	} while(rc.x1 <= rc.x2);
	while(rc.y1 - rc.y2 < b) {  /* too early stop of flat ellipses a=1 */
		set(get(rc.x1 - 1, rc.y1), object); /* -> finish tip of ellipse */
		set(get(rc.x2 + 1, rc.y1++), object);
		set(get(rc.x1 - 1, rc.y2), object);
		set(get(rc.x2 + 1, rc.y2--), object);
	}
}

indext location::bpoint(indext index, int width, int height, direction_s dir) const {
	auto x = getx(index);
	auto y = gety(index);
	switch(dir) {
	case Left: return get(x, y + xrand(1, height - 2));
	case Right: return get(x + width - 1, y + xrand(1, height - 2));
	case Up: return get(x + xrand(1, width - 2), y);
	default: return get(x + xrand(1, width - 2), y + height - 1);
	}
}

indext location::building(indext index, int w, int h, direction_s dir) {
	static direction_s rdir[] = {Right, Left, Up, Down};
	if(index == Blooded)
		return index;
	// ѕреобразуем координаты с учетом
	auto x = getx(index);
	auto y = gety(index);
	x = imin(imax((short)1, x), short(mmx - w - 1));
	y = imin(imax((short)1, y), short(mmy - h - 1));
	auto i = get(x, y);
	// —тены и пол
	set(i, Wall, w, h);
	set(to(i, RightDown), Floor, w - 2, h - 2);
	// ƒвери
	if(dir == Center)
		dir = maprnd(rdir);;
	auto door = bpoint(i, w, h, dir);
	set(door, Floor);
	set(door, Door);
	auto m1 = to(door, dir);
	auto t1 = gettile(m1);
	if(t1 != Floor) {
		if(t1 != Plain && t1 != Road)
			set(m1, Plain);
	}
	set(m1, NoTileObject);
	return door;
}

// Set horizontal wall in interior room
indext location::setiwh(int x, int y, int s, tile_s o, map_object_s r, bool locked_doors) {
	if(s <= 2)
		return Blocked;
	set(get(x, y), o, s, 1);
	auto i = get(x + xrand(1, s - 2), y);
	set(i, Floor);
	set(i, r);
	remove(i, Opened);
	if(locked_doors)
		set(i, Sealed);
	return i;
}

// Set vertical wall in interior room
indext location::setiwv(int x, int y, int s, tile_s o, map_object_s r, bool locked_doors) {
	if(s <= 2)
		return Blocked;
	set(get(x, y), o, 1, s);
	auto i = get(x, y + xrand(1, s - 2));
	set(i, Floor);
	set(i, r);
	remove(i, Opened);
	if(locked_doors)
		set(i, Sealed);
	return i;
}

void location::lake(int x, int y, int w, int h) {
	int w2 = w / 2;
	int h2 = h / 2;
	for(int i = 0; i < 5; i++) {
		int x1 = x + xrand(0, w2);
		int y1 = y + xrand(0, h2);
		int w1 = w2 - xrand(0, 2);
		int h1 = h2 - xrand(0, 2);
		ellipse({x1, y1, x1 + w1, y1 + h1}, Water);
	}
}

bool location::isfree(indext i) const {
	if(i == Blocked)
		return false;
	if(tiles[i] == Wall)
		return false;
	switch(objects[i]) {
	case Tree:
		return false;
	case Door:
		return is(i, Opened);
	default:
		break;
	}
	return true;
}

bool location::isfreenc(indext i) const {
	if(!isfree(i))
		return false;
	if(creature::find(i))
		return false;
	return true;
}

indext location::getfree(indext i, procis proc, int radius_maximum) const {
	if(i == Blocked)
		return i;
	if((this->*proc)(i))
		return i;
	auto x = getx(i);
	auto y = gety(i);
	for(auto r = 1; r < radius_maximum; r++) {
		if(rand() % 2) {
			for(auto x1 = x - r; x1 <= x + r; x1++) {
				if(x1 < 0)
					continue;
				if(x1 >= mmx)
					break;
				for(auto y1 = y - r; y1 <= y + r; y1++) {
					if(y1 < 0)
						continue;
					if(y1 >= mmy)
						break;
					auto i1 = get(x1, y1);
					if((this->*proc)(i1))
						return i1;
				}
			}
		} else {
			for(auto y1 = y - r; y1 <= y + r; y1++) {
				if(y1 < 0)
					continue;
				if(y1 >= mmy)
					break;
				for(auto x1 = x - r; x1 <= x + r; x1++) {
					if(x1 < 0)
						continue;
					if(x1 >= mmx)
						break;
					auto i1 = get(x1, y1);
					if((this->*proc)(i1))
						return i1;
				}
			}
		}
	}
	return Blocked;
}

creature* location::add(indext index, role_s role) {
	auto p = bsmeta<creature>::addz();
	p->create(role);
	p->setposition(getfree(index));
	return p;
}

creature* location::add(indext index, race_s race, gender_s gender, class_s type) {
	auto p = bsmeta<creature>::addz();
	p->create(race, gender, type);
	p->setposition(getfree(index));
	return p;
}

int	location::getrange(indext i1, indext i2) {
	if(i1 == Blocked || i2 == Blocked)
		return Blocked;
	auto x1 = getx(i1), y1 = gety(i1);
	auto x2 = getx(i2), y2 = gety(i2);
	auto dx = iabs(x1 - x2);
	auto dy = iabs(y1 - y2);
	return (dx > dy) ? dx : dy;
}

void location::clearblock() {
	memset(movements, 0, sizeof(movements));
}

void location::makewave(indext index) {
	if(index == Blocked)
		return;
	auto start = index;
	short unsigned push = 0;
	short unsigned pop = 0;
	stack[push++] = start;
	movements[start] = 0;
	while(push != pop) {
		auto n = stack[pop++];
		auto w = ((n==start) ? 0 : movements[n]) + 1;
		for(auto d : all_aroud) {
			auto i = to(n, d);
			if(i==Blocked || movements[i] == Blocked)
				continue;
			if(!movements[i] || movements[i] > w) {
				movements[i] = w;
				stack[push++] = i;
			}
		}
	}
}

void location::blockcreatures() {
	for(auto& e : bsmeta<creature>()) {
		if(!e)
			continue;
		auto i = e.getposition();
		if(i == Blocked)
			continue;
		movements[i] = Blocked;
	}
}

void location::blockwalls(bool water) {
	for(indext i = 0; i < mmx*mmy; i++) {
		switch(tiles[i]) {
		case Water:
			if(water)
				movements[i] = Blocked;
			break;
		default:
			if(!isfree(i))
				movements[i] = Blocked;
			break;
		}
	}
}

indext location::stepto(indext index) {
	auto current_index = Blocked;
	auto current_value = Blocked;
	for(auto d : all_aroud) {
		auto i = to(index, d);
		if(i==Blocked || movements[i]==Blocked || !movements[i])
			continue;
		if(movements[i] < current_value) {
			current_value = movements[i];
			current_index = i;
		}
	}
	return current_index;
}

indext location::stepfrom(indext index) {
	auto current_index = Blocked;
	auto current_value = 0;
	for(auto d : all_aroud) {
		auto i = to(index, d);
		if(i == Blocked || movements[i] == Blocked)
			continue;
		if(movements[i] > current_value) {
			current_value = movements[i];
			current_index = i;
		}
	}
	return current_index;
}
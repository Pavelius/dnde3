#include "main.h"

location				loc;
static short unsigned	movements[mmx * mmy];
static indext			stack[256 * 256];
static direction_s		all_aroud[] = {Left, Right, Up, Down, LeftDown, LeftUp, RightDown, RightUp};
static direction_s		all_rows[] = {Left, Right, Up, Down};

static const direction_s orientations_7b7[49] = {
	LeftUp, LeftUp, Up, Up, Up, RightUp, RightUp,
	LeftUp, LeftUp, LeftUp, Up, RightUp, RightUp, RightUp,
	Left, Left, LeftUp, Up, RightUp, Right, Right,
	Left, Left, Left, Center, Right, Right, Right,
	Left, Left, LeftDown, Down, RightDown, Right, Right,
	LeftDown, LeftDown, LeftDown, Down, RightDown, RightDown, RightDown,
	LeftDown, LeftDown, Down, Down, Down, RightDown, RightDown,
};

int location::getindex(indext i, map_flag_s e) const {
	static direction_s dir[] = {Up, Down, Left, Right};
	auto m = 0;
	auto f = 1;
	for(auto d : dir) {
		auto i1 = to(i, d);
		if(i1 == Blocked || is(i1, e))
			m |= f;
		f = f << 1;
	}
	return m;
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

int location::getindex3(indext i, tile_s e) const {
	static direction_s dir[] = {Up, Down, Left, Right};
	auto m = 0;
	auto f = 1;
	for(auto d : dir) {
		auto i1 = to(i, d);
		if(i1 != Blocked) {
			auto n = gettile(i1);
			if(n != e)
				m |= f;
		}
		f = f << 1;
	}
	return m;
}

int location::getindex2(indext i, tile_s t, int r) const {
	struct element {
		direction_s	dir;
		int			frame;
	};
	static element data[] = {{RightDown, 16},
		{LeftDown, 17},
		{LeftUp, 18},
		{RightUp, 19},
	};
	if(!r) {
		for(auto& e : data) {
			auto i1 = to(i, e.dir);
			if(i1 == Blocked)
				continue;
			if(gettile(i1) != t)
				return e.frame;
		}
	}
	return -1;
}

direction_s location::to(direction_s from, direction_s side) {
	switch(side) {
	case Up:
		return from;
	case RightUp:
		switch(from) {
		case Left: return LeftUp;
		case LeftUp: return Up;
		case Up: return RightUp;
		case RightUp: return Right;
		case Right: return RightDown;
		case RightDown: return Down;
		case Down: return LeftDown;
		case LeftDown: return Left;
		default: return Center;
		}
	case LeftUp:
		switch(from) {
		case Left: return LeftDown;
		case LeftDown: return Down;
		case Down: return RightDown;
		case RightDown: return Right;
		case Right: return RightUp;
		case RightUp: return Up;
		case Up: return LeftUp;
		case LeftUp: return Left;
		default: return Center;
		}
	case Down:
		switch(from) {
		case Left: return Right;
		case Right: return Left;
		case Up: return Down;
		case Down: return Up;
		default: return Center;
		}
	case Left:
		switch(from) {
		case Left: return Down;
		case Right: return Up;
		case Up: return Left;
		case Down: return Right;
		default: return Center;
		}
	case Right:
		switch(from) {
		case Left: return Up;
		case Right: return Down;
		case Up: return Right;
		case Down: return Left;
		default: return Center;
		}
	default:
		return Center;
	}
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
	bsdata<creature>::source.clear();
	bsdata<site>::source.clear();
	bsdata<boosti>::source.clear();
	bsdata<itemground>::source.clear();
}

tile_s location::gettile(indext i) const {
	if(i == Blocked)
		return Wall;
	return tiles[i];
}

trap_s location::gettrap(indext i) const {
	if(i == Blocked)
		return NoTrap;
	if(objects[i] == Trap)
		return (trap_s)getrand(i);
	return NoTrap;
}

void location::set(indext i, trap_s v) {
	if(i == Blocked)
		return;
	objects[i] = Trap;
	random[i] = v;
}

void location::drop(indext i, item v) {
	itemground* pi = 0;
	for(auto& e : bsdata<itemground>()) {
		if(e)
			continue;
		pi = &e;
	}
	if(!pi)
		pi = bsdata<itemground>::add();
	*static_cast<item*>(pi) = v;
	pi->index = i;
}

void location::addinfo(indext i, stringbuilder& sb) const {
	static const char* blooded[] = {"запачконое", "запачканный", "запачканная", "запачканные"};
	static const char* webbed[] = {"покрытое", "покрытый", "покрытая", "покрытые"};
	if(i == Blocked)
		return;
	auto t = gettile(i);
	sb.add("Это");
	if(true) {
		auto count = 0;
		if(is(i, Blooded)) {
			sb.adds("%1 кровью", blooded[bsdata<tilei>::elements[t].gender]);
			count++;
		}
		if(is(i, Webbed)) {
			sb.adds("%1 паутиной", webbed[bsdata<tilei>::elements[t].gender]);
			count++;
		}
		sb.adds("%-1.", getstr(t));
	}
	addobject(i, sb);
	auto p = creature::find(i);
	if(p)
		sb.adds("Здесь стоит %1.", p->getname());
	additems(i, sb);
}

void location::addobject(indext i, stringbuilder& sb) const {
	if(is(i, Hidden))
		return;
	auto o = getobject(i);
	if(o == Trap)
		sb.adds("Тут %-1.", bsdata<trapi>::elements[gettrap(i)].name);
	else if(o)
		sb.adds("Здесь находится %-1.", getstr(o));
}

int location::getitemscount(indext i) const {
	auto result = 0;
	for(auto& e : bsdata<itemground>()) {
		if(!e || e.index != i)
			continue;
		result++;
	}
	return result;
}

void location::additems(indext i, stringbuilder& sb) const {
	auto maximum_count = getitemscount(i);
	auto count = 0;
	for(auto& e : bsdata<itemground>()) {
		if(!e || e.index != i)
			continue;
		if(!count) {
			if(maximum_count > 1 || e.getcount() > 1)
				sb.adds("На полу лежат");
			else
				sb.adds("На полу лежит");
		} else if((count + 1) == maximum_count)
			sb.adds("и");
		else
			sb.add(",");
		e.getname(sb, true);
		count++;
	}
	if(count > 0)
		sb.add(".");
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
	return orientations_7b7[(ay + (osize / 2)) * osize + ax + (osize / 2)];
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

void location::fill(const rect& rc, int count, map_object_s v) {
	for(int i = 0; i < count; i++) {
		int x1 = xrand(rc.x1, rc.x2);
		int y1 = xrand(rc.y1, rc.y2);
		set(get(x1, y1), v);
	}
}

void location::fill(const rect& rc, int count, map_flag_s v) {
	for(int i = 0; i < count; i++) {
		int x1 = xrand(rc.x1, rc.x2);
		int y1 = xrand(rc.y1, rc.y2);
		set(get(x1, y1), v);
	}
}

void location::fill(const rect& rc, int count, tile_s v) {
	for(int i = 0; i < count; i++) {
		int x1 = xrand(rc.x1, rc.x2);
		int y1 = xrand(rc.y1, rc.y2);
		set(get(x1, y1), v);
	}
}

void location::fill(const rect& rc, int count, variant id) {
	switch(id.type) {
	case Tile: fill(rc, count, (tile_s)id.value); break;
	case Object: fill(rc, count, (map_object_s)id.value); break;
	case ObjectFlags: fill(rc, count, (map_flag_s)id.value); break;
	}
}

void location::fill(const rect& rc, tile_s v) {
	auto r1 = normalize(rc);
	for(auto y = r1.y1; y <= r1.y2; y++)
		for(auto x = r1.x1; x <= r1.x2; x++)
			set(get(x, y), v);
}

void location::fille(const rect& rc, tile_s v) {
	auto r1 = normalize(rc);
	for(auto y = r1.y1; y <= r1.y2; y++)
		for(auto x = r1.x1; x <= r1.x2; x++) {
			auto i = get(x, y);
			set(i, v);
			set(i, Explored);
		}
}

void location::fill(const rect& r1, map_flag_s v) {
	for(auto y = r1.y1; y <= r1.y2; y++)
		for(auto x = r1.x1; x <= r1.x2; x++) {
			auto i = get(x, y);
			if(i == Blocked || !isfree(i))
				continue;
			set(get(x, y), v);
		}
}

void location::fill(const rect& rc, map_object_s v) {
	auto r1 = normalize(rc);
	for(auto y = r1.y1; y <= r1.y2; y++)
		for(auto x = r1.x1; x <= r1.x2; x++)
			set(get(x, y), v);
}

rect location::getrect(indext i, int rx, int ry) {
	if(i == Blocked)
		return {};
	rect rc;
	rc.x1 = getx(i) - rx;
	rc.y1 = gety(i) - ry;
	rc.x2 = rc.x1 + rx * 2;
	rc.y2 = rc.y1 + ry * 2;
	normalize(rc);
	return rc;
}

void location::set(indext i, tile_s v) {
	switch(v) {
	case Lake:
		lake(getrect(i, 3, 3));
		break;
	default:
		tiles[i] = v;
		objects[i] = NoTileObject;
		break;
	}
}

void location::ellipse(rect rc, tile_s object) {
	int a = iabs(rc.width()), b = iabs(rc.height()), b1 = b & 1;
	long dx = 4 * (1 - a) * b * b, dy = 4 * (b1 + 1) * a * a;
	long err = dx + dy + b1 * a * a, e2; /* error of 1.step */
	if(rc.x1 > rc.x2) {
		rc.x1 = rc.x2;
		rc.x2 += a;
	} /* if called with swapped points */
	if(rc.y1 > rc.y2)
		rc.y1 = rc.y2; /* .. exchange them */
	rc.y1 += (b + 1) / 2;
	rc.y2 = rc.y1 - b1;   /* starting pixel */
	a *= 8 * a; b1 = 8 * b * b;
	do {
		fill({rc.x1, rc.y1, rc.x2, rc.y1}, object);
		fill({rc.x1, rc.y2, rc.x2, rc.y2}, object);
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

indext location::getpoint(const rect& rc, direction_s dir) const {
	switch(dir) {
	case Left: return get(rc.x1, xrand(rc.y1 + 1, rc.y2 - 1));
	case Right: return get(rc.x2, xrand(rc.y1 + 1, rc.y2 - 1));
	case Up: return get(xrand(rc.x1 + 1, rc.x2 - 1), rc.y1);
	default: return get(xrand(rc.x1 + 1, rc.x2 - 1), rc.y2);
	}
}

indext location::building(const rect& rc, direction_s dir) {
	static direction_s rdir[] = {Right, Left, Up, Down};
	// Стены и пол
	fill(rc, Wall);
	fill(rc.getoffset(1, 1), Floor);
	// Двери
	if(dir == Center)
		dir = maprnd(rdir);;
	auto door = getpoint(rc, dir);
	set(door, Floor);
	set(door, Door);
	auto m1 = to(door, dir);
	auto t1 = gettile(m1);
	if(t1 != Floor && t1 != Plain && t1 != Road)
		set(m1, Plain);
	set(m1, NoTileObject);
	return door;
}

// Set horizontal wall in interior room
indext location::setiwh(int x, int y, int s, tile_s o, map_object_s r, bool locked_doors) {
	if(s <= 2)
		return Blocked;
	fill({x, y, x + s, y}, o);
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
	fill({x, y, x, y + s}, o);
	auto i = get(x, y + xrand(1, s - 2));
	set(i, Floor);
	set(i, r);
	remove(i, Opened);
	if(locked_doors)
		set(i, Sealed);
	return i;
}

void location::lake(const rect& rc) {
	int w2 = rc.width() / 2;
	int h2 = rc.height() / 2;
	for(int i = 0; i < 5; i++) {
		int x1 = rc.x1 + xrand(0, w2);
		int y1 = rc.y1 + xrand(0, h2);
		int w1 = w2 - xrand(0, 2);
		int h1 = h2 - xrand(0, 2);
		ellipse({x1, y1, x1 + w1, y1 + h1}, Water);
	}
}

void location::forest(const rect& rc) {
	loc.fill(rc, rc.width() * rc.height() * dense_forest / 100, Tree);
}

bool location::isfreelt(indext i) const {
	if(i == Blocked)
		return false;
	if(tiles[i] == Wall)
		return false;
	auto v = objects[i];
	switch(v) {
	case Door:
		return is(i, Opened);
	default:
		return !bsdata<map_objecti>::elements[v].flags.is(BlockLight);
	}
	return true;
}

bool location::isfree(indext i) const {
	if(i == Blocked)
		return false;
	if(tiles[i] == Wall)
		return false;
	auto v = objects[i];
	switch(v) {
	case Door:
		return is(i, Opened);
	default:
		return !bsdata<map_objecti>::elements[v].flags.is(BlockMovement);
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

bool location::isfreenw(indext i) const {
	switch(tiles[i]) {
	case Water:
		return false;
	default:
		return isfreenc(i);
	}
	return true;
}

static tile_s tile_value;

bool location::istile(indext i) const {
	return gettile(i) == tile_value;
}

indext location::movernd(indext start, indext exclude) {
	indext directions[16];
	auto p = directions;
	for(auto d : all_aroud) {
		auto i1 = to(start, d);
		if(i1 == exclude)
			continue;
		if(i1 == Blocked)
			return false;
		*p++ = i1;
	}
	auto count = p - directions;
	if(!count)
		return start;
	return directions[rand() % count];
}

bool location::istile2(indext i) const {
	if(gettile(i) != tile_value)
		return false;
	for(auto d : all_aroud) {
		auto i1 = to(i, d);
		if(i1 == Blocked)
			return false;
		if(gettile(i1) != tile_value)
			return false;
	}
	return true;
}

indext location::getfreex(int x1, int x2, int y, procis proc) const {
	if(y < 0 || y >= mmy)
		return Blocked;
	if(x1 < 0)
		x1 = 0;
	if(x2 >= mmx)
		x2 = mmx - 1;
	for(; x1 <= x2; x1++) {
		auto i1 = get(x1, y);
		if((this->*proc)(i1))
			return i1;
	}
	return Blocked;
}

indext location::getfreey(int x, int y1, int y2, procis proc) const {
	if(x < 0 || x >= mmx)
		return Blocked;
	if(y1 < 0)
		y1 = 0;
	if(y2 >= mmy)
		y2 = mmy - 1;
	for(; y1 <= y2; y1++) {
		auto i1 = get(x, y1);
		if((this->*proc)(i1))
			return i1;
	}
	return Blocked;
}

indext location::getfree(indext i, procis proc, int radius_maximum) const {
	if(i == Blocked)
		return i;
	if((this->*proc)(i))
		return i;
	auto x = getx(i);
	auto y = gety(i);
	indext i1;
	for(auto r = 1; r < radius_maximum; r++) {
		if(rand() % 2) {
			i1 = getfreex(x - r, x + r, y + r, proc);
			if(i1 != Blocked)
				return i1;
			i1 = getfreex(x - r, x + r, y - r, proc);
			if(i1 != Blocked)
				return i1;
			i1 = getfreey(x - r, y - r, y + r, proc);
			if(i1 != Blocked)
				return i1;
			i1 = getfreey(x + r, y - r, y + r, proc);
			if(i1 != Blocked)
				return i1;
		} else {
			i1 = getfreey(x - r, y - r, y + r, proc);
			if(i1 != Blocked)
				return i1;
			i1 = getfreey(x + r, y - r, y + r, proc);
			if(i1 != Blocked)
				return i1;
			i1 = getfreex(x - r, x + r, y + r, proc);
			if(i1 != Blocked)
				return i1;
			i1 = getfreex(x - r, x + r, y - r, proc);
			if(i1 != Blocked)
				return i1;
		}
	}
	return Blocked;
}

creature* location::add(indext index, role_s role) {
	auto p = bsdata<creature>::addz();
	p->create(role);
	p->setposition(getfree(index));
	if(*p)
		return p;
	return 0;
}

creature* location::add(indext index, race_s race, gender_s gender, class_s type) {
	auto p = bsdata<creature>::addz();
	p->create(race, gender, type);
	p->setposition(getfree(index));
	if(*p)
		return p;
	return 0;
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
		auto w = ((n == start) ? 0 : movements[n]) + 1;
		for(auto d : all_aroud) {
			auto i = to(n, d);
			if(i == Blocked || movements[i] == Blocked)
				continue;
			if(!movements[i] || movements[i] > w) {
				movements[i] = w;
				stack[push++] = i;
			}
		}
	}
}

void location::blockcreatures() {
	for(auto& e : bsdata<creature>()) {
		if(!e)
			continue;
		if(e.is(Friendly))
			continue;
		auto i = e.getposition();
		if(i == Blocked)
			continue;
		movements[i] = Blocked;
	}
}

void location::blockwalls(bool water) {
	for(indext i = 0; i < mmx * mmy; i++) {
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
		if(i == Blocked || movements[i] == Blocked || !movements[i])
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

static bool linelossv(int x0, int y0, int x1, int y1) {
	int dx = iabs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = iabs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, e2;
	for(;;) {
		if(x0 >= 0 && x0 < mmx && y0 >= 0 && y0 < mmy) {
			auto i = loc.get(x0, y0);
			loc.set(i, Visible);
			loc.set(i, Explored);
			if(!loc.isfreelt(i))
				return false;
		}
		if(x0 == x1 && y0 == y1)
			return true;
		e2 = err;
		if(e2 > -dx) {
			err -= dy;
			x0 += sx;
		}
		if(e2 < dy) {
			err += dx;
			y0 += sy;
		}
	}
}

bool location::linelos(int x0, int y0, int x1, int y1) const {
	int dx = iabs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = iabs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, e2;
	for(;;) {
		if(x0 >= 0 && x0 < mmx && y0 >= 0 && y0 < mmy) {
			auto i = get(x0, y0);
			if(!isfreelt(i))
				return false;
		}
		if(x0 == x1 && y0 == y1)
			return true;
		e2 = err;
		if(e2 > -dx) {
			err -= dy;
			x0 += sx;
		}
		if(e2 < dy) {
			err += dx;
			y0 += sy;
		}
	}
}

void location::setlos(indext index, int r) {
	auto x0 = getx(index);
	auto y0 = gety(index);
	for(auto x = x0 - r; x <= x0 + r; x++) {
		linelossv(x0, y0, x, y0 - r);
		linelossv(x0, y0, x, y0 + r);
	}
	for(auto y = y0 - r; y <= y0 + r; y++) {
		linelossv(x0, y0, x0 - r, y);
		linelossv(x0, y0, x0 + r, y);
	}
}

bool location::cansee(indext i1, indext i2) const {
	if(i1 == Blocked || i2 == Blocked)
		return false;
	return linelos(getx(i1), gety(i1), getx(i2), gety(i2));
}

indext location::center(int x, int y, int w, int h) {
	return get(x + w / 2, y + (h / 2));
}

indext location::center(const rect& rc) {
	return get(rc.x1 + rc.width() / 2, rc.y1 + rc.height() / 2);
}

site& location::room(const rect& rc) {
	for(auto x = rc.x1; x < rc.x2; x++) {
		for(auto y = rc.y1; y < rc.y2; y++)
			set(get(x, y), Floor);
	}
	auto p = bsdata<site>::add();
	*((rect*)p) = rc;
	p->set(Lair);
	return *p;
}

bool location::ismatch(indext index, const rect& rectangle) const {
	if(index == Blocked)
		return false;
	auto x = getx(index), y = gety(index);
	if(x < rectangle.x1 || x > rectangle.x2 || y < rectangle.y1 || y > rectangle.y2)
		return false;
	return true;
}

creature* location::shopkeeper(indext index) {
	return loc.add(index, Shopkeeper);
}

creature* location::adventurer(indext index) {
	static race_s races[] = {Human, Elf, Halfling, Dwarf};
	static gender_s genders[] = {Male, Female, Male};
	static class_s classes[] = {Cleric, Cleric, Fighter, Fighter, Mage, Mage, Paladin, Ranger, Theif, Theif};
	return add(index, maprnd(races), maprnd(genders), maprnd(classes));
}

creature* location::monster(indext index) {
	adat<role_s> monsters;
	auto cr = level / 2;
	auto n1 = imax(0, cr - 1);
	auto n2 = cr + 1;
	for(auto i = GoblinWarrior; i < Character; i = (role_s)(i + 1)) {
		auto& ei = bsdata<rolei>::elements[i];
		if(ei.type == Commoner)
			continue;
		auto n = ei.getcr();
		if(n<n1 || n > n2)
			continue;
		monsters.add(i);
	}
	auto r = GoblinWarrior;
	if(monsters)
		r = monsters[rand() % monsters.getcount()];
	return add(index, r);
}

bool location::ismatch(indext index, variant v) const {
	switch(v.type) {
	case Object:
		if(objects[index] == v.value)
			return true;
		break;
	case Tile:
		if(tiles[index] == v.value)
			return true;
		break;
	case ObjectFlags:
		return is(index, (map_flag_s)v.value);
	}
	return false;
}

void location::loot(indext index, item_s type, int level, char chance_bigger_price, identify_s identify, char chance_curse) {
	if(index == Blocked || type == NoItem)
		return;
	item it;
	auto chance_artifact = level / 2;
	auto chance_magic = 10 + level * 2;
	it.create(type, chance_artifact, chance_magic, chance_curse);
	it.set(identify);
	if(it.is(Coinable))
		it.setcount(xrand(1 * level, 6 * level));
	if(chance_bigger_price > 0) {
		if(d100() < chance_bigger_price) {
			it.set(Sale150);
			if(d100() < chance_bigger_price / 2) {
				it.set(Sale200);
				if(d100() < chance_bigger_price / 3) {
					it.set(Sale250);
					if(d100() < chance_bigger_price / 4)
						it.set(Sale300);
				}
			}
		} else
			it.set(Sale100);
	} else if(chance_bigger_price < 0) {
		chance_bigger_price = -chance_bigger_price;
		if(d100() < chance_bigger_price) {
			it.set(Sale80);
			if(d100() < chance_bigger_price / 2)
				it.set(Sale65);
		} else
			it.set(Sale100);
	}
	if(it.is(Artifact))
		loc.artifacts++;
	if(it.is(Blessed))
		loc.magic_items++;
	loc.drop(index, it);
}

void location::loot(indext index, slot_s slots, int level, char chance_bigger_price, identify_s identify, char chance_curse) {
	variantc source;
	source.additems(slots, level);
	source.match(Natural, true);
	if(chance_bigger_price)
		source.matchp(1, true);
	loot(index, (item_s)source.random().value, level, chance_bigger_price, identify, chance_curse);
}

void location::loot(indext index, slota slots, int level, char chance_bigger_price, identify_s identify, char chance_curse) {
	variantc source;
	source.additems(slots, level);
	source.match(Natural, true);
	if(chance_bigger_price)
		source.matchp(1, true);
	loot(index, (item_s)source.random().value, level, chance_bigger_price, identify, chance_curse);
}

void location::loot(const rect& rc, const variantc& source, int chance, int level, char chance_bigger_price, identify_s identify, char chance_curse) {
	if(!source)
		return;
	for(auto y = rc.y1; y <= rc.y2; y++) {
		if(y < 0 || y >= mmy)
			continue;
		for(auto x = rc.x1; x <= rc.x2; x++) {
			if(x < 0 || x >= mmx)
				continue;
			if(d100() < chance)
				loot(get(x, y), (item_s)source.random().value, level, chance_bigger_price, identify, chance_curse);
		}
	}
}

void location::content(const rect& rc, room_s type, site* p) {
	if(!rc)
		return;
	auto& ei = bsdata<roomi>::elements[type];
	auto index = center(rc);
	if(ei.heart)
		set(index, ei.heart);
	if(p && ei.keeper)
		p->setowner(loc.add(index, ei.keeper));
	for(auto& e : ei.shops) {
		variantc source;
		source.additems(e.slot, level);
		source.match(Natural, true);
		if(e.price)
			source.matchp(1, true);
		loc.loot(rc, source, e.chance, loc.level + e.quality, e.price, KnownPower, e.cursed);
	}
	switch(type) {
	case Temple:
		if(p) {
			p->setparam((diety_s)xrand(GodBane, GodTyr));
			p->setowner(p->priest());
		}
		break;
	case Tavern:
		for(auto i = xrand(1, 3); i > 0; i--)
			loc.adventurer(index);
		break;
	case Barracs:
		for(auto i = xrand(2, 4); i > 0; i--)
			loc.add(index, HumanGuard);
		break;
	case CityHall:
		break;
	}
}

void location::interior(const rect& rc, room_s type, indext entrance, int level, rect* result_rect, site* ps) {
	if(rc.width() < 5 && rc.height() < 5) {
		rect r2 = rc.getoffset(1, 1);
		if(level == 0) {
			if(result_rect)
				*result_rect = r2;
		}
		content(r2, type, ps);
		return;
	}
	if(entrance == Blocked)
		return;
	const auto w = rc.width();
	const auto h = rc.height();
	auto dx = loc.getx(entrance);
	auto dy = loc.gety(entrance);
	rect r1 = rc, r2 = rc;
	if(w > h && w >= 5) {
		r1.x2 = r1.x1 + r1.width() / 2 + 1 - (rand() % 3);
		// Дверь может попасть прямо на линию
		if(r1.x2 == dx) {
			if(r1.x2 <= (rc.x1 + rc.width() / 2))
				r1.x2++;
			else
				r1.x2--;
		}
		r2.x1 = r1.x2;
		if(r1.width() < 2 || r2.width() <= 2)
			return;
		entrance = setiwv(r1.x2, r1.y1, r1.height(), Wall, Door, true);
		if(r1.x2 < dx)
			iswap(r1, r2);
	} else if(h >= 5) {
		r1.y2 = r1.y1 + r1.height() / 2 + 1 - (rand() % 3);
		// Дверь может попасть прямо на линию
		if(r1.y2 == dy) {
			if(r1.y2 <= (rc.y1 + rc.height() / 2))
				r1.y2++;
			else
				r1.y2--;
		}
		r2.y1 = r1.y2;
		if(r1.height() < 2 || r2.height() <= 2)
			return;
		entrance = setiwh(r1.x1, r1.y2, r1.width(), Wall, Door, true);
		if(r1.y2 < dy)
			iswap(r1, r2);
	}
	if(level == 0) {
		if(result_rect)
			*result_rect = r1.getoffset(1, 1);
	}
	content(r1.getoffset(1, 1), type, ps);
	switch(type) {
	case ShopPotions:
	case ShopScrolls:
	case ShopWeaponAndArmor:
		interior(r2, TreasureRoom, entrance, level + 1, 0, ps);
		break;
	default:
		interior(r2, EmpthyRoom, entrance, level + 1, 0, ps);
		break;
	}
}

rect location::normalize(const rect& rc) {
	rect result = rc;
	if(result.x1 < 0)
		result.x1 = 0;
	if(result.y1 < 0)
		result.y1 = 0;
	if(result.x2 >= mmx)
		result.x2 = mmx - 1;
	if(result.y2 >= mmy)
		result.y2 = mmy - 1;
	return result;
}

creature* location::commoner(indext index) {
	static role_s commoners[] = {HumanMale, HumanMale, HumanFemale, HumanChild};
	return add(index, maprnd(commoners));
}

indext location::getrand(const rect& rc) const {
	auto r1 = normalize(rc);
	return get(xrand(r1.x1, rc.x2), xrand(r1.y1, r1.y2));
}

indext location::find(map_object_s v) const {
	const indext count = mmx * mmy;
	for(indext i = 0; i < count; i++) {
		if(objects[i] == v)
			return i;
	}
	return Blocked;
}

indext location::find(tile_s v, const rect& rc) const {
	auto x = xrand(rc.x1, rc.x2);
	auto y = xrand(rc.y1, rc.y2);
	tile_value = v;
	return loc.getfree(get(x, y), &location::istile, 32);
}

site* location::addsite(room_s type, const rect& rc) {
	auto p = bsdata<site>::add();
	p->clear();
	p->set(type);
	p->set(rc);
	p->randomname();
	return p;
}

void location::remove(indext i) {
	auto p = outdoori::find(i);
	if(p)
		p->index = Blocked;
}

void location::trail(indext i) {
	if(is(i, Trailed))
		remove(i, Trailed);
	else
		set(i, Trailed);
}

void location::growplants() {
	int n, r;
	for(indext i = 0; i < mmx * mmy; i++) {
		auto t = getobject(i);
		if(t != Plants)
			continue;
		n = getplantgrow(i);
		random[i] += xrand(1, 4);
		r = getplantgrow(i);
		if(n != r && r >= 3)
			random[i] = 0;
	}
}

int location::getplantgrow(indext i) const {
	return random[i] / 60;
}

void location::set(indext i, map_object_s v) {
	objects[i] = v;
	switch(v) {
	case Plants:
		random[i] = xrand(0, 60 * 3 - 1);
		break;
	}
}

bool location::use(indext index, variant id, creature& player, int level, int order, bool run) {
	switch(id.type) {
	case Spell: return use(index, (spell_s)id.value, player, level, order, run);
	case Skill: return use(index, (skill_s)id.value, player, level, order, run);
	default: return false;
	}
}

void location::generate(indext start, tile_s tile, int chance, int count) {
	short unsigned push = 0;
	short unsigned pop = 0;
	set(start, tile);
	stack[push++] = start;
	while(push != pop) {
		auto n = stack[pop++];
		for(auto d : all_rows) {
			auto i = to(n, d);
			if(i == Blocked)
				continue;
			auto t = gettile(i);
			if(t == Water || t == tile)
				continue;
			if(start == n || (d100() < chance)) {
				set(i, tile);
				if(--count <= 0)
					return;
				stack[push++] = i;
			}
		}
	}
}

bool location::ishidden(indext i) const {
	if(i == Blocked)
		return false;
	return tiles[i] == Wall && objects[i] != NoTileObject;
}

void location::reveal(indext i) {
	if(i == Blocked)
		return;
	tiles[i] = Floor;
}

void location::hide(indext i) {
	if(i == Blocked)
		return;
	tiles[i] = Wall;
}
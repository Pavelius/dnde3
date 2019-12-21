#include "main.h"

static location* current_location;

void location::activate() {
	current_location = this;
}

location* location::getlocation() {
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
		if(getx(index)==0)
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

void location::fill(rect rc, tile_s v) {
	correct(rc);
	for(auto y = rc.y1; y <= rc.y2; y++) {
		for(auto x = rc.x1; x <= rc.x2; x++)
			set(get(x, y), v);
	}
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
	static const char* blooded[] = {"запачконое", "запачканный", "запачканная", "запачканные"};
	static const char* webbed[] = {"покрытое", "покрытый", "покрытая", "покрытые"};
	if(i == Blocked)
		return;
	auto t = gettile(i);
	sb.add("Это");
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
		sb.adds("Здесь находится %1.", getstr(o));
}

direction_s	location::getdirection(indext from, indext to) {
	return Left;
}
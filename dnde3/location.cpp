#include "main.h"

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
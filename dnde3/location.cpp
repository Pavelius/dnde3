#include "main.h"

int location::getindex(indext i, tile_s e) const {
	auto m = 0;
	auto n = gettile(to(i, Up));
	if(n != e && n)
		m |= 1;
	n = gettile(to(i, Down));
	if(n != e && n)
		m |= 2;
	n = gettile(to(i, Left));
	if(n != e && n)
		m |= 4;
	n = gettile(to(i, Right));
	if(n != e && n)
		m |= 8;
	return m;
}

indext location::to(indext index, direction_s id) {
	switch(id) {
	case Left:
		if((index%mmx) == 0)
			return Blocked;
		return index - 1;
	case Right:
		if((index%mmx) >= mmx - 1)
			return Blocked;
		return index + 1;
	case Up:
		if((index / mmx) == 0)
			return Blocked;
		return index - mmx;
	case Down:
		if((index / mmx) >= mmy - 1)
			return Blocked;
		return index + mmx;
	case LeftUp:
		if((index%mmx) == 0)
			return Blocked;
		if((index / mmx) == 0)
			return Blocked;
		return index - 1 - mmx;
	case LeftDown:
		if((index%mmx) == 0)
			return Blocked;
		if((index / mmx) >= mmy - 1)
			return Blocked;
		return index - 1 + mmx;
	case RightUp:
		if((index%mmx) >= mmx - 1)
			return -1;
		if((index / mmx) == 0)
			return Blocked;
		return index + 1 - mmx;
	case RightDown:
		if((index%mmx) >= mmx - 1)
			return Blocked;
		if((index / mmx) >= mmy - 1)
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
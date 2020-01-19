#include "main.h"

const dungeoni* dungeoni::find(int level) const {
	auto n = 0;
	for(auto p = this; *p; p++) {
		n += p->level;
		if(n >= level)
			return p;
	}
	return 0;
}

bool dungeoni::create(indext index, int level, bool visualize) const {
	auto p = find(level);
	if(!p)
		return false;
	if(!loc.read(index, level)) {
		loc.create(*p, level, p->is(Explored), visualize);
		loc.write(index, level);
	}
	return true;
}
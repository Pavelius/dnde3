#include "main.h"

const dungeoni* dungeoni::find(int level) const {
	auto n = 0;
	for(auto p = this; *p; p++) {
		n += p->levels;
		if(n >= level)
			return p;
	}
	return 0;
}

bool dungeoni::create(indext index, int level) const {
	auto p = find(level);
	if(!p)
		return false;
	if(!loc.read(index, level)) {
		loc.create(p->type, is(Explored), is(Visible));
		loc.write(index, level);
	}
	return true;
}
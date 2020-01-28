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
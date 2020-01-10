#include "main.h"

const char* descriptioni::get(variant v1, variant v2) const {
	auto p = this;
	while(p->v1 || p->v2) {
		if(p->v1 == v1 && p->v2 == v2)
			return p->text;
		if(!p->v1 && p->v2 == v2)
			return p->text;
		if(!p->v2 && p->v1 == v1)
			return p->text;
		p++;
	}
	return p->text;
}
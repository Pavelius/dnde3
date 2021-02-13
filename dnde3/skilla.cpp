#include "main.h"

static int compare_skills(const void* p1, const void* p2) {
	auto e1 = *((skill_s*)p1);
	auto e2 = *((skill_s*)p2);
	return strcmp(bsmeta<skilli>::elements[e1].name, bsmeta<skilli>::elements[e2].name);
}

void skilla::match(target_flag_s v, bool remove) {
	auto ps = data;
	for(auto i : *this) {
		if(bsmeta<skilli>::elements[i].target.flags.is(v) == remove)
			continue;
		*ps++ = i;
	}
	count = ps - data;
}

void skilla::removent() {
	auto ps = data;
	for(auto i : *this) {
		if(!bsmeta<skilli>::elements[i].target)
			continue;
		*ps++ = i;
	}
	count = ps - data;
}

void skilla::select(const creature& e) {
	e.select(*this);
}

void skilla::sort() {
	qsort(data, count, sizeof(data[0]), compare_skills);
}

skillu::skillu() {
	memset(cap, 0, sizeof(cap));
}
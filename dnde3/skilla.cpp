#include "main.h"

static int compare_skills(const void* p1, const void* p2) {
	auto e1 = *((skill_s*)p1);
	auto e2 = *((skill_s*)p2);
	return strcmp(bsmeta<skilli>::elements[e1].name, bsmeta<skilli>::elements[e2].name);
}

void skilla::select(const creature& e) {
	e.select(*this);
}

void skilla::sort() {
	qsort(data, count, sizeof(data[0]), compare_skills);
}

skillu::skillu(creature* player) : player(player) {
	memset(cap, 0, sizeof(cap));
}

void skillu::setcaps() {
	for(auto e : *this) {
		auto n = player->get(e);
		setcap(e, n + 10);
	}
}
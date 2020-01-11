#include "main.h"

static int compare_skills(const void* p1, const void* p2) {
	auto e1 = *((skill_s*)p1);
	auto e2 = *((skill_s*)p2);
	return strcmp(bsmeta<skilli>::elements[e1].name, bsmeta<skilli>::elements[e2].name);
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

skillu::skillu(creature* player) : player(player) {
	memset(cap, 0, sizeof(cap));
}

void skillu::setcaps() {
	for(auto e : *this) {
		auto n = player->getbasic(e);
		setcap(e, n + 10);
	}
}

int skillu::getcap(skill_s i, creature& player) const {
	const auto& ei = bsmeta<skilli>::elements[i];
	return cap[i] + player.get(ei.abilities[0]) + player.get(ei.abilities[1]);
}
#include "main.h"

void skilla::select(const creature& e) {
	e.select(*this);
}

void skilla::sort() {
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
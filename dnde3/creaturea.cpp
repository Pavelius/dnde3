#include "main.h"

void creaturea::match(creature& player, variant v, bool remove) {
	if(!v)
		return;
	auto ps = data;
	for(auto p : *this) {
		if(v.type == State && v.value == Hostile) {
			if(remove) {
				if(player.isenemy(p))
					continue;
			} else {
				if(!player.isenemy(p))
					continue;
			}
		} else if(v.type == State && v.value == Friendly) {
			if(remove) {
				if(!player.isenemy(p))
					continue;
			} else {
				if(player.isenemy(p))
					continue;
			}
		} else {
			if(remove) {
				if(p->ismatch(v))
					continue;
			} else {
				if(!p->ismatch(v))
					continue;
			}
		}
		*ps++ = p;
	}
	count = ps - data;
}

void creaturea::match(variant v, bool remove) {
	if(!v)
		return;
	auto ps = data;
	for(auto p : *this) {
		if(remove) {
			if(p->ismatch(v))
				continue;
		} else {
			if(!p->ismatch(v))
				continue;
		}
		*ps++ = p;
	}
	count = ps - data;
}

void creaturea::matchr(indext index, int range) {
	auto ps = data;
	for(auto p : *this) {
		auto i = p->getposition();
		if(loc.getrange(i, index) > range)
			continue;
		*ps++ = p;
	}
	count = ps - data;
}

void creaturea::select() {
	auto ps = data;
	auto pe = endof();
	for(auto& e : bsmeta<creature>()) {
		if(!e)
			continue;
		if(ps < pe)
			*ps++ = &e;
	}
	count = ps - data;
}

void creaturea::select(state_s v) {
	auto ps = data;
	auto pe = endof();
	for(auto& e : bsmeta<creature>()) {
		if(!e)
			continue;
		if(!e.is(v))
			continue;
		if(ps < pe)
			*ps++ = &e;
	}
	count = ps - data;
}

void creaturea::select(indext start, int distance) {
	auto ps = data;
	auto pe = endof();
	for(auto& e : bsmeta<creature>()) {
		if(!e)
			continue;
		auto i = e.getposition();
		if(loc.getrange(i, start) > distance)
			continue;
		if(!loc.cansee(i, start))
			continue;
		if(ps < pe)
			*ps++ = &e;
	}
	count = ps - data;
}

creature* creaturea::choose(bool interactive, const char* format) {
	indexa source;
	for(auto p : *this)
		source.add(p->getposition());
	auto i = source.choose(interactive, format);
	if(i == -1)
		return 0;
	return data[i];
}

static indext compare_index;

static int compare_distace(const void* v1, const void* v2) {
	auto p1 = *((creature**)v1);
	auto p2 = *((creature**)v2);
	auto d1 = location::getrange(p1->getposition(), compare_index);
	auto d2 = location::getrange(p2->getposition(), compare_index);
	return d1 - d2;
}

void creaturea::sort(indext start) {
	compare_index = start;
	qsort(data, count, sizeof(data[0]), compare_distace);
}
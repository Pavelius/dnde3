#include "main.h"

void indexa::select(indext index, int distance) {
	auto ps = data;
	auto pe = endof();
	auto x0 = loc.getx(index);
	auto y0 = loc.gety(index);
	for(auto y = y0 - distance; y <= y + distance; y++) {
		if(y < 0)
			continue;
		if(y >= mmy)
			break;
		for(auto x = x0 - distance; x <= x + distance; x++) {
			if(x < 0)
				continue;
			if(x >= mmx)
				break;
			auto i = loc.get(x, y);
			auto v = loc.getobject(i);
			if(!v)
				continue;
			if(ps < pe)
				*ps++ = i;
		}
	}
	count = ps - data;
}

void indexa::match(variant v, bool remove) {
	if(!v)
		return;
	auto ps = data;
	for(auto i : *this) {
		if(remove) {
			if(loc.ismatch(i, v))
				continue;
		} else {
			if(!loc.ismatch(i, v))
				continue;
		}
		*ps++ = i;
	}
	count = ps - data;
}

void indexa::match(creature& player, variant id, int v) {
	if(!id)
		return;
	auto ps = data;
	for(auto i : *this) {
		if(!player.usei(i, id, v, false))
			continue;
		*ps++ = i;
	}
	count = ps - data;
}

void indexa::matchr(indext index, int range) {
	auto ps = data;
	for(auto i : *this) {
		if(loc.getrange(i, index)>range)
			continue;
		*ps++ = i;
	}
	count = ps - data;
}

static indext compare_index;

static int compare_distance(const void* v1, const void* v2) {
	auto p1 = *((indext*)v1);
	auto p2 = *((indext*)v2);
	auto d1 = location::getrange(p1, compare_index);
	auto d2 = location::getrange(p2, compare_index);
	return d1 - d2;
}

void indexa::sort(indext start) {
	compare_index = start;
	qsort(data, count, sizeof(data[0]), compare_distance);
}
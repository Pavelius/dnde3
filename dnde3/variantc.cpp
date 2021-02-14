#include "main.h"

variant variantc::random() const {
	if(!count)
		return variant();
	return data[rand() % count].value;
}

void variantc::additems(slot_s v, int level) {
	for(auto i = item_s(1); i < ManyItems; i = (item_s)(i + 1)) {
		auto& ei = bsdata<itemi>::elements[i];
		if(!ei.is(v))
			continue;
		add(i);
	}
}

void variantc::additems(slota source, int level) {
	for(auto i = item_s(1); i < ManyItems; i = (item_s)(i + 1)) {
		auto& ei = bsdata<itemi>::elements[i];
		if(!ei.level || ei.level > level)
			continue;
		if(!ei.is(source))
			continue;
		add(i);
	}
}

void variantc::match(slot_s v, bool remove) {
	auto ps = data;
	for(auto& e : *this) {
		bool result;
		switch(e.type) {
		case Item:
			result = bsdata<itemi>::elements[e.value].is(v);
			break;
		default:
			continue;
		}
		if(result == remove)
			continue;
		*ps++ = e;
	}
	count = ps - data;
}

void variantc::matchp(int value, bool greater) {
	auto ps = data;
	for(auto& e : *this) {
		if(e.type != Item)
			continue;
		if(greater) {
			if(bsdata<itemi>::elements[e.value].cost < value)
				continue;
		} else {
			if(bsdata<itemi>::elements[e.value].cost > value)
				continue;
		}
		*ps++ = e;
	}
	count = ps - data;
}

void variantc::match(itemflag_s v, bool remove) {
	auto ps = data;
	for(auto& e : *this) {
		if(e.type != Item)
			continue;
		if(remove) {
			if(bsdata<itemi>::elements[e.value].flags.is(v))
				continue;
		} else {
			if(!bsdata<itemi>::elements[e.value].flags.is(v))
				continue;
		}
		*ps++ = e;
	}
	count = ps - data;
}
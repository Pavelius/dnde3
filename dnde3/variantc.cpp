#include "main.h"

int	variantc::getweight() const {
	auto r = 0;
	for(auto& e : *this)
		r += e.value;
	return r;
}

variant variantc::random() const {
	auto w = getweight();
	if(!w)
		return {};
	auto n = rand() % w;
	auto r = 0;
	for(auto& e : *this) {
		r += e.value;
		if(r > n)
			return e.id;
	}
	return data[rand() % count].value;
}

bool variantc::is(variant v) const {
	for(auto& e : *this) {
		if(e.id == v)
			return true;
	}
	return false;
}

void variantc::add(variant v, rarity_s r) {
	if(is(v))
		return;
	auto rw = bsmeta<rarityi>::elements[r].weight;
	if(!rw)
		return;
	auto p = adat::add();
	p->id = v;
	p->value = rw;
}

void variantc::add(variant v) {
	auto r = Unique;
	switch(v.type) {
	case Item: r = bsmeta<itemi>::elements[v.value].rarity; break; break;
	default: break;
	}
	if(r!=Unique)
		add(v, r);
}

void variantc::additems(const aref<slot_s>& source) {
	for(auto i = item_s(1); i < ManyItems; i = (item_s)(i + 1)) {
		auto& ei = bsmeta<itemi>::elements[i];
		if(!ei.is(source))
			continue;
		add(i, ei.rarity);
	}
}

void variantc::match(slot_s v, bool remove) {
	auto ps = data;
	for(auto& e : *this) {
		if(e.id.type != Item)
			continue;
		if(remove) {
			if(bsmeta<itemi>::elements[e.id.value].is(v))
				continue;
		} else {
			if(!bsmeta<itemi>::elements[e.id.value].is(v))
				continue;
		}
		*ps++ = e;
	}
	count = ps - data;
}

void variantc::matchp(int value, bool greater) {
	auto ps = data;
	for(auto& e : *this) {
		if(e.id.type != Item)
			continue;
		if(greater) {
			if(bsmeta<itemi>::elements[e.id.value].cost < value)
				continue;
		} else {
			if(bsmeta<itemi>::elements[e.id.value].cost > value)
				continue;
		}
		*ps++ = e;
	}
	count = ps - data;
}

void variantc::match(item_flag_s v, bool remove) {
	auto ps = data;
	for(auto& e : *this) {
		if(e.id.type != Item)
			continue;
		if(remove) {
			if(bsmeta<itemi>::elements[e.id.value].flags.is(v))
				continue;
		} else {
			if(!bsmeta<itemi>::elements[e.id.value].flags.is(v))
				continue;
		}
		*ps++ = e;
	}
	count = ps - data;
}
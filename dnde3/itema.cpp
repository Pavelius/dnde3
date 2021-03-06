#include "main.h"

void itema::select(creature& e) {
	e.select(*this, Head, LastWear, false);
}

void itema::selecta(creature& e) {
	e.select(*this, Backpack, LastWear, true);
}

void itema::selectb(creature& e) {
	e.select(*this, Backpack, LastBackpack, true);
}

void itema::selectg(variant v) {
	auto ps = begin();
	auto pe = end();
	for(auto p : *this) {
		if(!p)
			continue;
		switch(v.type) {
		case Sale:
			if(v.value < p->getsale())
				continue;
			break;
		}
		if(ps < pe)
			*ps++ = p;
	}
	count = ps - data;
}

void itema::select(site& ei) {
	auto ps = begin();
	auto pe = endof();
	for(auto& e : bsdata<itemground>()) {
		if(!e)
			continue;
		point pt;
		pt.x = loc.getx(e.index);
		pt.y = loc.gety(e.index);
		if(!pt.in(ei))
			continue;
		if(ps < pe)
			*ps++ = &e;
	}
	count = ps - data;
}

void itema::select(indext index, bool extend) {
	if(index == Blocked)
		return;
	auto ps = data;
	if(extend)
		ps += count;
	auto pe = endof();
	for(auto& e : bsdata<itemground>()) {
		if(!e)
			continue;
		if(e.index != index)
			continue;
		if(ps < pe)
			*ps++ = &e;
	}
	count = ps - data;
}

void itema::matchboost(variant v) {
	auto ps = data;
	for(auto p : *this) {
		if(!p->isboost(v))
			continue;
		*ps++ = p;
	}
	count = ps - data;
}

void itema::match(variant v, bool remove) {
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

void itema::match(item& v, bool remove) {
	auto ps = data;
	for(auto p : *this) {
		if(remove) {
			if(p==&v)
				continue;
		} else {
			if(p!=&v)
				continue;
		}
		*ps++ = p;
	}
	count = ps - data;
}

static const char* addweight(stringbuilder& sb, int v) {
	sb.clear();
	sb.add("%1i.%2i", v / 100, (v / 10) % 10);
	return sb;
}

void itema::footer(stringbuilder& sb) const {
	auto player = creature::getactive();
	if(!player)
		return;
	char temp[24]; stringbuilder s1(temp);
	sb.add("����� ��� ����� �������� [%1] ��.", addweight(s1, player->getweight()));
}

void itema::matcha(creature& player, variant id, int v) {
	if(!id)
		return;
	auto ps = data;
	for(auto pi : *this) {
		if(!pi->apply(player, id, v, 0, false))
			continue;
		*ps++ = pi;
	}
	count = ps - data;
}
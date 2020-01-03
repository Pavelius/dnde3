#include "main.h"

void creaturea::match(state_s v) {
	auto ps = data;
	for(auto p : *this) {
		if(!p->is(v))
			continue;
		*ps++ = p;
	}
	count = ps - data;
}

void creaturea::match(const racea& v) {
	if(!v)
		return;
	auto ps = data;
	for(auto p : *this) {
		if(!v.is(p->getrace()))
			continue;
		*ps++ = p;
	}
	count = ps - data;
}

void creaturea::match(const alignmenta& v) {
	if(!v)
		return;
	auto ps = data;
	for(auto p : *this) {
		if(!v.is(p->getrace()))
			continue;
		*ps++ = p;
	}
	count = ps - data;
}

void creaturea::remove(state_s v) {
	auto ps = data;
	for(auto p : *this) {
		if(p->is(v))
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

creature* creaturea::choose(bool interactive, const char* format) {
	indexa source;
	for(auto p : *this)
		source.add(p->getposition());
	auto i = source.choose(interactive, format);
	if(i == -1)
		return 0;
	return data[i];
}
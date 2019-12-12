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
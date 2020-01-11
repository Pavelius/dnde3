#include "main.h"

item_s itemia::random() const {
	if(!count)
		return NoItem;
	return data[rand() % count];
}

void itemia::select() {
	auto ps = data;
	auto pe = endof();
	for(auto i = item_s(1); i < ManyItems; i = (item_s)(i + 1)) {
		if(ps < pe)
			*ps++ = i;
	}
	count = ps - data;
}

void itemia::addx(const aref<slot_s>& source) {
	for(auto i = item_s(1); i < ManyItems; i = (item_s)(i + 1)) {
		if(!bsmeta<itemi>::elements[i].is(source))
			continue;
		add(i);
	}
}

void itemia::match(slot_s v, bool remove) {
	auto ps = data;
	for(auto i : *this) {
		if(remove) {
			if(bsmeta<itemi>::elements[i].is(v))
				continue;
		} else {
			if(!bsmeta<itemi>::elements[i].is(v))
				continue;
		}
		*ps++ = i;
	}
	count = ps - data;
}
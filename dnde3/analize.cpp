#include "main.h"

static int range_value[] = {0, 1, 3, 5, 7};

analize::analize(creature& player, creaturea& opponents) :
	target(SingleTarget), type(Creature), player(player), creatures(opponents), items(), indecies() {
}

analize::analize(creature& player) :
	target(SingleTarget), type(Creature), player(player), creatures(player), items(), indecies() {
}

void analize::set(variant_s v) {
	if(v == type)
		return;
	switch(v) {
	case Item:
		items.select(player);
		break;
	case Object: case Tile:
		indecies.select(player.getposition(), player.getlos());
		break;
	default:
		return;
	}
	type = v;
}

void analize::match(variant v) {
	switch(v.type) {
	case Variant:
		set(variant_s(v.value));
		break;
	case Range:
		switch(type) {
		case Creature:
			creatures.matchr(player.getposition(), range_value[v.value]);
			break;
		case Object:
			indecies.match(player.getposition(), range_value[v.value]);
			break;
		}
		break;
	case Target:
		target = (target_s)v.value;
		break;
	case State:
		creatures.match(player, v, false);
		break;
	case Slot:
	case Item:
	case ItemType:
		set(Item);
		items.match(v, false);
		break;
	case Object:
		set(Object);
		indecies.match(v, false);
		break;
	}
}

void analize::remove(variant v) {
	switch(v.type) {
	case State:
		creatures.match(player, v, true);
		break;
	case Slot:
	case Item:
	case ItemType:
		set(Item);
		items.match(v, true);
		break;
	case Object:
		set(Object);
		indecies.match(v, true);
		break;
	default:
		switch(type) {
		case Creature: creatures.match(v, true); break;
		case Item: items.match(v, true); break;
		case Object: indecies.match(v, true); break;
		}
		break;
	}
}

variant analize::select(const varianta& source) {
	auto pe = source + sizeof(source)/ sizeof(source[0]);
	for(auto ps = source; ps < pe; ps++) {
		auto pn = ps + 1;
		if((pn==pe) || !(*pn)) {
			remove(*ps);
			return *ps;
		}
		match(*ps);
	}
	return variant();
}

void analize::apply(const varianta& source, int value) {
	auto v = select(source);
	auto count = 1;
	switch(target) {
	case RandomTarget:
		switch(type) {
		case Creature: creatures.shuffle(); break;
		case Item: items.shuffle(); break;
		default: indecies.shuffle(); break;
		}
		break;
	case NearestTarget:
		switch(type) {
		case Creature: creatures.sort(player.getposition()); break;
		case Item: items.shuffle(); break;
		default: indecies.sort(player.getposition()); break;
		}
		break;
	case AllTargets:
		switch(type) {
		case Creature: count = creatures.getcount(); break;
		case Item: count = items.getcount(); break;
		default: count = indecies.getcount(); break;
		}
		break;
	}
	switch(type) {
	case Creature:
		if(count > creatures.getcount())
			count = creatures.getcount();
		break;
	case Item:
		if(count > items.getcount())
			count = items.getcount();
		break;
	default:
		if(count > indecies.getcount())
			count = indecies.getcount();
		break;
	}
	for(auto i = 0; i < count; i++) {
		switch(type) {
		case Creature:
			creatures[i]->add(v, value);
			break;
		}
	}
}
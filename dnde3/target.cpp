#include "main.h"

static int range_value[] = {0, 1, 3, 5, 7};

void targeti::select(creature& player, creaturea& creatures, itema& items, indexa& indecies, variant id, int v) const {
	auto r = range_value[range];
	auto los = player.getlos();
	if(r > los)
		r = los;
	switch(type) {
	case Item:
		items.select(player);
		for(auto v : conditions)
			items.match(v, false);
		break;
	case Object:
		indecies.select(player.getposition(), r);
		for(auto v : conditions)
			indecies.match(v, false);
		indecies.match(player, id, v);
		break;
	case Creature:
		creatures.matchr(player.getposition(), r);
		for(auto v : conditions)
			creatures.match(player, v, false);
		break;
	}
}

unsigned targeti::getcount(creaturea& creatures, itema& items, indexa& indecies) const {
	switch(type) {
	case Item: return items.getcount();
	case Creature: return creatures.getcount();
	default: return indecies.getcount();
	}
}

bool targeti::apply(creature& player, creaturea& source, variant id, int v) const {
	creaturea creatures = source;
	itema items;
	indexa indecies;
	select(player, creatures, items, indecies, id, v);
	auto maximum_count = getcount(creatures, items, indecies);
	if(!maximum_count)
		return false;
	unsigned count = 1;
	switch(target) {
	case AllTargets:
		count = maximum_count;
		break;
	case RandomTarget:
		switch(type) {
		case Item: items.shuffle(); break;
		case Creature: creatures.shuffle(); break;
		default: indecies.shuffle(); break;
		}
		break;
	case SingleTarget:
		if(maximum_count > 1) {
			// Allow interactive choose
		}
		break;
	}
	switch(type) {
	case Creature:
		for(auto p : creatures)
			p->add(id, v, true);
		break;
	case Item:
		for(auto p : items) {
		}
		break;
	default:
		for(auto i : indecies)
			player.usei(i, id, v, true);
		break;
	}
	return true;
}
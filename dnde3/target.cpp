#include "main.h"

unsigned targeti::getcount(creaturea& creatures, itema& items, indexa& indecies) const {
	switch(type) {
	case Item: return items.getcount();
	case Creature: return creatures.getcount();
	default: return indecies.getcount();
	}
}

bool targeti::prepare(creature& player, creaturea& creatures, itema& items, indexa& indecies, variant id, int v) const {
	static int range_value[] = {0, 1, 3, 5, 7};
	auto r = range_value[range];
	auto los = player.getlos();
	if(r > los)
		r = los;
	switch(type) {
	case Item:
		items.select(player);
		items.matcha(player, id, v);
		break;
	case Object:
		indecies.select(player.getposition(), r);
		indecies.matcha(player, id, v);
		indecies.sort(player.getposition());
		break;
	case Creature:
		creatures.matchr(player.getposition(), r);
		if(flags.is(Enemies))
			creatures.match(player, Hostile, false);
		else if(flags.is(Friends))
			creatures.match(player, Friendly, false);
		if(flags.is(NotYou))
			creatures.remove(creatures.indexof(&player), 1);
		creatures.matcha(player, id, v, false);
		creatures.sort(player.getposition());
		break;
	}
	return getcount(creatures, items, indecies) > 0;
}

void targeti::use(creature& player, creaturea& source, creaturea& creatures, itema& items, indexa& indecies, variant id, int v) const {
	auto maximum_count = getcount(creatures, items, indecies);
	unsigned count = 1;
	switch(target) {
	case AllTargets:
		count = maximum_count;
		break;
	case NearestTarget:
		switch(type) {
		case Item: items.shuffle(); break;
		}
		break;
	case SingleTarget:
		if(maximum_count > 1) {
			const char* interactive = 0;
			if(player.isactive())
				interactive = "Нет подходящей цели";
			// Allow interactive choose
			if(type == Creature) {
				auto p = creatures.choose(interactive, "Выбирайте цель");
				if(p) {
					auto i = creatures.indexof(p);
					iswap(creatures[0], creatures[i]);
				}
			} else if(type == Item) {
				auto p = items.choose(interactive, "Выбирайте цель", 0, NoSlotName);
				if(p) {
					auto i = items.indexof(p);
					iswap(items[0], items[i]);
				}
			}
		}
		break;
	}
	if(count > maximum_count)
		count = maximum_count;
	switch(type) {
	case Creature:
		for(unsigned i = 0; i < count; i++)
			creatures[i]->apply(player, id, v, i, true);
		break;
	case Item:
		for(unsigned i = 0; i < count; i++)
			items[i]->apply(player, id, v, i, true);
		break;
	default:
		for(unsigned i = 0; i < count; i++)
			loc.apply(player, indecies[i], id, v, i, true);
		break;
	}
}

bool targeti::use(creature& player, creaturea& source, variant id, int v) const {
	creaturea creatures = source;
	itema items;
	indexa indecies;
	if(!prepare(player, creatures, items, indecies, id, v))
		return false;
	use(player, source, creatures, items, indecies, id, v);
	return true;
}
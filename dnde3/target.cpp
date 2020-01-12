#include "main.h"

unsigned targeti::getcount(creaturea& creatures, itema& items, indexa& indecies) const {
	switch(type) {
	case Item: return items.getcount();
	case Creature: return creatures.getcount();
	default: return indecies.getcount();
	}
}

bool targeti::prepare(creature& player, creaturea& creatures, itema& items, indexa& indecies, variant id, int v, bool show_errors) const {
	static int range_value[] = {0, 1, 2, 4, 7};
	auto r = range_value[range];
	auto los = player.getlos();
	if(r > los)
		r = los;
	switch(type) {
	case Item:
		items.selecta(player);
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
	auto result = getcount(creatures, items, indecies) > 0;
	if(!result && show_errors && player.isactive()) {
		switch(type) {
		case Creature: sb.adds("Вокруг нет никого, на кого это могло бы подействовать."); break;
		case Item: sb.adds("У вас нету подходящего предмета, на которое это подействует."); break;
		default: sb.adds("Рядом нет объекта, на который это может подействовать."); break;
		}
	}
	return result;
}

void targeti::use(creature& player, creaturea& source, creaturea& creatures, itema& items, indexa& indecies, variant id, int v) const {
	auto maximum_count = getcount(creatures, items, indecies);
	unsigned count = 1;
	auto interactive = this->interactive;
	if(is(TwoTargets))
		count += 1;
	else if(is(ThreeTargets))
		count += 2;
	else if(is(AllTargets)) {
		count = maximum_count;
		interactive = 0;
	}
	if(count > maximum_count)
		count = maximum_count;
	if(is(RandomTargets)) {
		switch(type) {
		case Creature: creatures.shuffle(); break;
		case Item: items.shuffle(); break;
		default: indecies.shuffle(); break;
		}
		interactive = 0;
	}
	if(interactive && (maximum_count > 1 || is(AlwaysChoose))) {
		if(!player.isactive())
			interactive = 0;
		if(type == Creature) {
			auto p = creatures.choose(interactive);
			if(p) {
				auto i = creatures.indexof(p);
				iswap(creatures[0], creatures[i]);
			}
		} else if(type == Item) {
			auto p = items.choose(interactive, 0, NoSlotName, false, false);
			if(p) {
				auto i = items.indexof(p);
				iswap(items[0], items[i]);
			}
		} else {
			auto i = indecies.choose(interactive);
			if(i != -1)
				iswap(indecies[0], indecies[i]);
		}
	}
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

bool targeti::use(creature& player, creaturea& source, variant id, int v, bool show_errors) const {
	creaturea creatures = source;
	itema items;
	indexa indecies;
	if(!prepare(player, creatures, items, indecies, id, v, show_errors))
		return false;
	use(player, source, creatures, items, indecies, id, v);
	return true;
}
#include "main.h"

void targeti::select(creature& player, creaturea& creatures, itema& items, indexa& indecies) const {
	switch(type) {
	case Item:
		items.select(player);
		break;
	case Object:
		indecies.select(player.getposition(), player.getlos());
		break;
	default:
		break;
	}
}

void targeti::match(creature& player, creaturea& creatures, itema& items, indexa& indecies) const {
	for(auto v : conditions) {
		if(!v)
			break;
		switch(type) {
		case Item:
			items.match(v, false);
			break;
		case Creature:
			creatures.match(player, v, false);
			break;
		default:
			indecies.match(v, false);
			break;
		}
	}
}
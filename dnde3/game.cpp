#include "main.h"

gamei game;

static void move_creatures() {
	for(auto& e : bsmeta<creature>()) {
		if(!e)
			continue;
		e.makemove();
	}
}

void gamei::play() {
	const int moves_per_minute = 30;
	while(true) {
		for(auto i = 0; i < moves_per_minute; i++)
			move_creatures();
		rounds++; // One round is one minute
	}
}
#include "main.h"

BSDATA(conditioni) = {
	{"Anger"},
	{"Berserking"},
	{"Busy"},
	{"Guardian"},
	{"Happy"},
	{"Wounded"},
	{"MissHits"},
	{"MissMana"},
	{"MissHalfHits"},
	{"MissHalfMana"},
	{"MissAlmostAllHits"},
	{"MissAlmostAllMana"},
	{"Owner"},
};
assert_enum(conditioni, Owner)

bool creature::is(condition_s v) const {
	switch(v) {
	case Anger: return mood < 30;
	case Berserking: return mood < 60;
	case Busy: return restore_energy <= -(StandartEnergyCost * 4) && is(Unaware);
	case Guardian: return guard != Blocked;
	case Happy: return mood > 40;
	case MissHits: return gethits() < get(LifePoints);
	case MissHalfHits: return gethits() < get(LifePoints) / 2;
	case MissAlmostAllHits: return gethits() < get(LifePoints) / 5;
	case MissMana: return getmana() < get(ManaPoints);
	case MissHalfMana: return getmana() < get(ManaPoints) / 2;
	case MissAlmostAllMana: return getmana() < get(ManaPoints) / 5;
	case Owner: return getsite() && getsite()->getowner() == this;
	case Wounded: return wounds > 0;
	default: return false;
	}
}
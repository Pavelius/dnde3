#include "main.h"

enum duration_s : unsigned char {
	Instant,
	Minute, FewMinutes,
	Hour, FewHours,
	Day, FewDays,
};

typedef adat<casev<variant>, 8> chancev;

struct foodi {
	variant				condition[3];
	char				chance;
	chancev				chances;
	void				apply(creature* player, const item it, bool interactive) const;
	bool				match(const creature* player, const item it) const;
};

static foodi elements[] = {{{Ration}, 0, {{LifePoints, 5}, {Poison, 10}, {Sick, 1}}},
{{BreadDwarven, Dwarf}, 0, {{LifePoints, 10}, {Sick, 1}}},
{{BreadDwarven}, 0, {{LifePoints, 3}, {Poison, 10}, {Sick, 1}}},
};

bool foodi::match(const creature* player, const item it) const {
	for(auto v : condition) {
		if(!v)
			break;
		if(!player->match(v))
			return false;
		if(!it.match(v))
			return false;
	}
	return true;
}

void foodi::apply(creature* player, const item it, bool interactive) const {
	for(auto& e : chances)
		player->apply(e.id, e.value, interactive, it.getmagic(), it.getqualityr(), it.getdamage(), 60);
}

bool creature::eat(item it, bool interactive) {
	auto& ei = it.getitem();
	if(ei.slot != Edible) {
		if(interactive) {
			static const char* text[] = {
				"Это не съедобно.",
				"Такое не употребляю.",
				"Съесть это выше моих сил.",
			};
			say(maprnd(text));
		}
		return false;
	}
	act("%герой съел%а %-1.", it.getname());
	for(auto& e : elements) {
		if(!e.match(this, it))
			continue;
		if(e.chance && d100() < e.chance)
			continue;
		e.apply(this, it, interactive);
		break;
	}
	wait();
	return true;
}
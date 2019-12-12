#include "main.h"

itemi bsmeta<itemi>::elements[] = {{""},
{"������ �����", 850, 5 * GP, Male, Iron, {0, D4n9, -2}, {}, {}, {}, Melee, WeaponFocusAxes},
{"������", 1000, 5 * CP, Female, Wood, {}, {}, {}, {}, Melee},
{"������", 50, 2 * GP, Male, Iron, {}, {}, {}, {}, Melee, WeaponFocusBlades},
{"������", 800, 2 * GP, Male, Wood, {}, {}, {}, {}, Melee},
{"������", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"�����", 700, 8 * GP, NoGender, Iron, {}, {}, {}, {}, Melee},
{"�����", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"���", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"�������� ���", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"��������� ���", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
//
{"�������", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"������� �������", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"������� ���", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"�������� ���", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"����", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"�����", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
//
{"�����", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"������", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"�����", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
//
};

creature* item::getwearer() const {
	auto i = bsmeta<creature>::source.indexof((creature*)(this));
	if(i == -1)
		return 0;
	return (creature*)bsmeta<creature>::source.ptr(i);
}

bool item::is(slot_s v) const {
	switch(v) {
	case LeftFinger:
		return bsmeta<itemi>::elements[type].slot == RightFinger;
	case Melee:
		return bsmeta<itemi>::elements[type].slot == OffHand || bsmeta<itemi>::elements[type].slot == Melee;
	default:
		return bsmeta<itemi>::elements[type].slot == v;
	}
}
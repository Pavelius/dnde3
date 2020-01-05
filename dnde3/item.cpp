#include "main.h"

itemi bsmeta<itemi>::elements[] = {{"", 0, 0, NoGender, Organic, {}, {}, {}, {}, Melee},
{"������ �����", 850, 5 * GP, Male, Iron, {-4, D1n8, Slashing}, {}, {}, {Versatile}, Melee, FightAxes},
{"������", 1000, 5 * CP, Female, Wood, {-6, D1n4, Bludgeon}, {}, {}, {}, Melee},
{"������", 50, 2 * GP, Male, Iron, {-2, D1n3, Piercing}, {}, {}, {}, Melee},
{"�����", 800, 2 * GP, Male, Wood, {-3, D1n4, Bludgeon}, {}, {}, {}, Melee},
{"������", 700, 8 * GP, Female, Iron, {-3, D2n7, Bludgeon}, {}, {}, {}, Melee},
{"�����", 700, 8 * GP, NoGender, Iron, {-5, D1n8, Piercing}, {}, {}, {}, Melee},
{"�����", 700, 8 * GP, Female, Iron, {-1, D1n6, Bludgeon}, {}, {}, {TwoHanded}, Melee},
{"������� ���", 700, 8 * GP, Female, Iron, {-4, D1n8, Slashing}, {}, {}, {Versatile}, Melee, FightSwords},
{"�������� ���", 700, 8 * GP, Female, Iron, {-3, D1n8, Slashing}, {}, {}, {}, Melee, FightSwords},
{"��������� ���", 700, 8 * GP, Female, Iron, {-8, D2n12, Slashing}, {}, {}, {TwoHanded}, Melee, FightTwohanded},
//
{"�������", 700, 40 * GP, Male, Wood, {}, {}, {}, {}, Ranged},
{"������� �������", 1200, 80 * GP, Male, Wood, {}, {}, {}, {}, Ranged},
{"������� ���", 500, 60 * GP, Male, Wood, {-8, D1n8, Piercing, 0, Arrow}, {}, {}, {}, Ranged, Archery},
{"�������� ���", 300, 30 * GP, Male, Wood, {-4, D1n6, Piercing, 0, Arrow}, {}, {}, {}, Ranged, Archery},
{"����", 30, 1 * SP, Male, Wood, {}, {}, {}, {}, Melee},
{"�����", 50, 1 * SP, Female, Leather, {}, {}, {}, {}, Melee},
//
{"�����", 20, 0, Male, Stone, {}, {}, {}, {}, Amunitions},
{"������", 2, 2 * CP, Female, Wood, {}, {}, {}, {}, Amunitions},
{"�����", 3, 1 * CP, Male, Iron, {}, {}, {}, {}, Amunitions},
//
{"�������� �����", 1000, 5 * GP, Female, Leather, {}, {10, 1}, {}, {}, Torso},
{"��������� �����", 1500, 15 * GP, Female, Leather, {}, {15, 1}, {}, {}, Torso},
{"���������� ������", 2500, 30 * GP, Male, Iron, {}, {25, 2}, {}, {}, Torso},
{"��������", 2600, 50 * GP, Female, Iron, {}, {25, 3}, {}, {}, Torso},
{"������", 3000, 200 * GP, Male, Iron, {}, {30, 4}, {}, {}, Torso},
{"����", 3500, 800 * GP, Female, Iron, {}, {40, 5}, {}, {}, Torso},
//
{"���", 0, 0 * GP, Female, Iron, {}, {}, {}, {}, OffHand},
{"����", 0, 0 * GP, Female, Iron, {}, {}, {}, {}, Head},
{"������", 0, 0 * GP, Female, Iron, {}, {}, {}, {}, Elbows},
{"�������� ������", 0, 0 * GP, Female, Iron, {}, {}, {}, {}, Elbows},
//
{"����", 0, 0 * GP, Male, Leather, {}, {}, {}, {}, TorsoBack},
{"����", 0, 0 * GP, Male, Leather, {}, {}, {}, {}, TorsoBack},
{"����", 0, 0 * GP, Female, Leather, {}, {}, {}, {}, TorsoBack},
{"����", 0, 0 * GP, Female, Leather, {}, {}, {}, {}, TorsoBack},
{"����", 0, 0 * GP, Female, Leather, {}, {}, {}, {}, TorsoBack},
//
{"������", 0, 0 * GP, NoGender, Leather, {}, {}, {}, {}, Legs},
{"������", 0, 0 * GP, NoGender, Leather, {}, {}, {}, {}, Legs},
{"������", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}, Legs},
{"������", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}, Legs},
{"������", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}, Legs},
//
{"���", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {}},
{"���� ��������", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {}},
{"���� ������", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {}},
{"���� ������", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {}},
{"��������", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {}},
{"�������", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {}},
{"����", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {}},
//
{"������", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
//
{"����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
//
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
//
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
//
{"������", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
//
{"������", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
//
{"����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
//
{"������", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"����������� ������", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"������� ������", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
//
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {Natural}},
{"������", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {Natural}},
{"����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {Natural}},
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {Natural}},
{"���", 0, 0 * GP, NoGender, Paper, {}, {20}, {}, {Natural}, Torso},
//
{"����� ���������"},
};
assert_enum(item, ManyItems);
static_assert(sizeof(item) == sizeof(int), "Struct 'item' must be sizeof(int)");

creature* item::getwearer() const {
	auto i = bsmeta<creature>::source.indexof((creature*)(this));
	if(i == -1)
		return 0;
	return (creature*)bsmeta<creature>::source.ptr(i);
}

slot_s item::getwearerslot() const {
	return getwearer()->getwearerslot(this);
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

void item::getname(stringbuilder& sb) const {
	auto& ei = getitem();
	if(identify_cab)
		sb.adds(bsmeta<item_typei>::elements[magic].name[ei.gender]);
	sb.adds("%-1", getname());
}
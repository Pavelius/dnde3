#include "main.h"

itemi bsmeta<itemi>::elements[] = {{"����", 0, 0, NoGender, Organic, {0, D1n3, Bludgeon, 4}, {}, {}, {}, Melee},
{"������ �����", 850, 5 * GP, Male, Iron, {-4, D1n8, Slashing}, {}, {}, {Versatile}, Melee, FocusAxes},
{"������", 1000, 5 * CP, Female, Wood, {-6, D1n4, Bludgeon}, {}, {}, {}, Melee},
{"������", 50, 2 * GP, Male, Iron, {-2, D1n3, Piercing, 2}, {}, {}, {Light}, Melee},
{"�����", 800, 2 * GP, Male, Wood, {-3, D1n4, Bludgeon}, {}, {}, {}, Melee},
{"������", 700, 8 * GP, Female, Iron, {-3, D2n7, Bludgeon}, {}, {}, {}, Melee},
{"�����", 700, 8 * GP, NoGender, Iron, {-5, D1n8, Piercing}, {}, {}, {}, Melee},
{"�����", 700, 8 * GP, Female, Iron, {-2, D1n6, Bludgeon, 1}, {}, {}, {TwoHanded}, Melee},
{"������� ���", 700, 8 * GP, Female, Iron, {-4, D1n8, Slashing}, {}, {}, {Versatile}, Melee, FocusSwords},
{"�������� ���", 700, 8 * GP, Female, Iron, {-3, D1n6, Slashing}, {}, {}, {Light}, Melee, FocusSwords},
{"��������� ���", 700, 8 * GP, Female, Iron, {-9, D2n12, Slashing, -2}, {}, {}, {TwoHanded}, Melee, FocusTwohanded},
//
{"�������", 700, 40 * GP, Male, Wood, {-3, D1n8, Piercing, -2, Bolt}, {}, {}, {}, Ranged},
{"������� �������", 1200, 80 * GP, Male, Wood, {-10, D1n12, Piercing, -6, Bolt}, {}, {}, {}, Ranged},
{"������� ���", 500, 60 * GP, Male, Wood, {-10, D1n8, Piercing, 1, Arrow}, {}, {}, {}, Ranged, FocusBows},
{"�������� ���", 300, 30 * GP, Male, Wood, {-6, D1n6, Piercing, 1, Arrow}, {}, {}, {}, Ranged, FocusBows},
{"����", 30, 1 * SP, Male, Wood, {-4, D1n3, Piercing, 3}, {}, {}, {}, Ranged},
{"�����", 50, 1 * SP, Female, Leather, {-6, D1n4, Bludgeon, 0, Rock}, {}, {}, {}, Ranged},
//
{"�����", 20, 0, Male, Stone, {}, {}, {}, {}, Amunitions, Bargaining, 30},
{"������", 3, 2 * CP, Female, Wood, {}, {}, {}, {}, Amunitions, Bargaining, 20},
{"�����", 2, 1 * CP, Male, Iron, {}, {}, {}, {}, Amunitions, Bargaining, 20},
//
{"�������� �����", 1000, 5 * GP, Female, Leather, {-5}, {10, 1, 15}, {}, {}, Torso},
{"��������� �����", 1500, 15 * GP, Female, Leather, {-7}, {15, 1, 15}, {}, {}, Torso},
{"���������� ������", 2500, 30 * GP, Male, Iron, {-12}, {25, 2, 30}, {}, {}, Torso},
{"��������", 2600, 50 * GP, Female, Iron, {-10}, {25, 3, 25}, {}, {}, Torso},
{"������", 3000, 200 * GP, Male, Iron, {-15}, {30, 4, 35}, {}, {}, Torso},
{"����", 3500, 800 * GP, Female, Iron, {-20}, {40, 5, 35}, {}, {}, Torso},
//
{"���", 0, 0 * GP, Female, Iron, {-5}, {10, 0, 25}, {}, {}, OffHand},
{"����", 0, 0 * GP, Female, Iron, {-1}, {5, 0, 20}, {}, {}, Head},
{"������", 0, 0 * GP, Female, Iron, {-1}, {5, 0, 15}, {}, {}, Elbows},
{"�������� ������", 0, 0 * GP, Female, Iron, {-3}, {5, 1, 20}, {}, {}, Elbows},
//
{"����", 0, 0 * GP, Male, Leather, {}, {3, 0, 2}, {}, {}, TorsoBack},
{"����", 0, 0 * GP, Male, Leather, {}, {3, 0, 2}, {}, {}, TorsoBack},
{"����", 0, 0 * GP, Female, Leather, {}, {3, 0, 2}, {}, {}, TorsoBack},
{"����", 0, 0 * GP, Female, Leather, {}, {5, 0, 2}, {}, {}, TorsoBack},
{"����", 0, 0 * GP, Female, Leather, {}, {5, 0, 2}, {}, {}, TorsoBack},
//
{"������", 0, 0 * GP, NoGender, Leather, {}, {3, 0, 10}, {}, {}, Legs},
{"������", 0, 0 * GP, NoGender, Leather, {}, {3, 0, 10}, {}, {}, Legs},
{"������", 0, 0 * GP, NoGender, Iron, {}, {5, 0, 15}, {}, {}, Legs},
{"������", 0, 0 * GP, NoGender, Iron, {}, {3, 1, 10}, {}, {}, Legs},
{"������", 0, 0 * GP, NoGender, Iron, {}, {3, 1, 10}, {}, {}, Legs},

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
{"����", 0, 0 * GP, NoGender, Wood, {}, {}, {}, {}},
{"����", 0, 0 * GP, NoGender, Wood, {}, {}, {}, {}},
{"����", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
{"����", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
{"����", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
//
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
//
{"�����", 0, 0 * GP, NoGender, Glass, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Glass, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Glass, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Glass, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Glass, {}, {}, {}, {}},
//
{"������", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
//
{"������", 0, 0 * GP, NoGender, Wood, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Wood, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
//
{"����", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
//
{"������", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
{"����������� ������", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
{"������� ������", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
//
{"�����", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {Natural}, Melee},
{"������", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {Natural}, Melee},
{"����", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {Natural}, Melee},
{"�����", 0, 0 * GP, NoGender, Leather, {-10}, {}, {30, 2, 40}, {Natural}, Torso},
{"���", 0, 0 * GP, NoGender, Leather, {-3}, {20, 0, 20}, {}, {Natural}, Torso},
//
{"����� ���������"},
};
assert_enum(item, ManyItems);
static_assert(sizeof(item) == sizeof(int), "Struct 'item' must be sizeof(int)");

item::item(item_s type) {
	clear();
	this->type = type;
	auto& ei = getitem();
	if(ei.count > 0)
		setcount(ei.count);
}

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
	auto& ei = getitem();
	switch(v) {
	case LeftFinger: return ei.slot == RightFinger;
	case OffHand: return ei.slot == OffHand || (ei.slot == Melee && ei.flags.is(Light));
	default: return ei.slot == v;
	}
}

void item::getname(stringbuilder& sb) const {
	auto& ei = getitem();
	//if(identify_cab)
	//	sb.adds(bsmeta<item_typei>::elements[magic].name[ei.gender]);
	sb.adds("%-1", getname());
	auto n = getcount();
	if(n>1)
		sb.adds("%1i��.", n);
}

item& item::setcount(int count) {
	if(!count) {
		auto p = getwearer();
		if(p)
			p->dressoff();
		clear();
		if(p)
			p->dresson();
	} else if(iscountable()) {
		auto mc = getitem().count;
		if(count > mc)
			count = mc;
		this->count = count - 1;
	}
	return *this;
}

bool item::use() {
	auto c = getcount();
	setcount(getcount() - 1);
	return c > 1;
}
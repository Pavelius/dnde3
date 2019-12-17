#include "main.h"

itemi bsmeta<itemi>::elements[] = {{""},
{"������ �����", 850, 5 * GP, Male, Iron, {-3, D4n9, Slashing}, {}, {}, {}, Melee, WeaponFocusAxes},
{"������", 1000, 5 * CP, Female, Wood, {-6, D1n4, Bludgeon}, {}, {}, {}, Melee},
{"������", 50, 2 * GP, Male, Iron, {-6, D1n3, Piercing}, {}, {}, {}, Melee, WeaponFocusBlades},
{"�����", 800, 2 * GP, Male, Wood, {-3, D3n8, Bludgeon}, {}, {}, {}, Melee},
{"������", 700, 8 * GP, Female, Iron, {-3, D2n7, Bludgeon}, {}, {}, {}, Melee},
{"�����", 700, 8 * GP, NoGender, Iron, {-3, D3n8, Piercing}, {}, {}, {}, Melee},
{"�����", 700, 8 * GP, Female, Iron, {-3, D1n6, Bludgeon}, {}, {}, {}, Melee},
{"���", 700, 8 * GP, Female, Iron, {-4, D5n10, Slashing}, {}, {}, {}, Melee, WeaponFocusBlades},
{"�������� ���", 700, 8 * GP, Female, Iron, {-3, D3n8, Slashing}, {}, {}, {}, Melee, WeaponFocusBlades},
{"��������� ���", 700, 8 * GP, Female, Iron, {-9, D4n14, Slashing}, {}, {}, {}, Melee, WeaponFocusBlades},
//
{"�������", 700, 40 * GP, Male, Wood, {}, {}, {}, {}, Melee},
{"������� �������", 1200, 80 * GP, Male, Wood, {}, {}, {}, {}, Melee},
{"������� ���", 500, 60 * GP, Male, Wood, {}, {}, {}, {}, Melee, WeaponFocusBows},
{"�������� ���", 300, 30 * GP, Male, Wood, {}, {}, {}, {}, Melee, WeaponFocusBows},
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
};

static_assert(sizeof(item) == sizeof(int), "Struct 'item' must be sizeof(int)");

creature* item::getwearer() const {
	auto i = bsmeta<creature>::source.indexof((creature*)(this));
	if(i == -1)
		return 0;
	return (creature*)bsmeta<creature>::source.ptr(i);
}

slot_s item::getslot() const {
	return getwearer()->getslot(this);
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

void item::get(attacki& result) const {
	result = getitem().weapon;
}

//{"������ �����", 850, 5 * GP, Male, Iron, {1, {1, 8, Slashing}}, {5}, {Versatile}, {Melee}, WeaponFocusAxes, axe_effect},
//{"������", 1000, 5 * CP, Female, Wood, {2, {1, 6}}, {4}, {}, {Melee}, NoSkill, bludgeon_effect},
//{"������", 50, 2 * GP, Male, Iron, {3, {1, 4, Piercing}, 5}, {5}, {}, {Melee, OffHand}, WeaponFocusBlades, swords_effect},
//{"�����", 800, 2 * GP, Male, Wood, {1, {2, 5}}, {5}, {}, {Melee}, WeaponFocusAxes, bludgeon_effect},
//{"������", 700, 8 * GP, Female, Iron, {1, {1, 7}}, {5}, {}, {Melee}, WeaponFocusAxes, bludgeon_effect},
//{"�����", 250, 8 * SP, NoGender, Wood, {1, {1, 8, Piercing}}, {5}, {Versatile}, {Melee}, NoSkill, pierce_effect},
//{"�����", 200, 1 * SP, Male, Wood, {2, {1, 6}}, {6}, {TwoHanded}, {Melee}, NoSkill, staff_spells, NoItem, 0, 50},
//{"������� ���", 200, 15 * GP, Male, Iron, {1, {1, 8, Slashing}}, {6}, {}, {Melee}, WeaponFocusBlades, swords_effect},
//{"�������� ���", 150, 10 * GP, Male, Iron, {1, {1, 6, Slashing}}, {6}, {}, {Melee, OffHand}, WeaponFocusBlades, swords_effect},
//{"��������� ���", 1500, 50 * GP, Male, Iron, {0, {2, 12, Slashing}}, {6}, {TwoHanded}, {Melee}, WeaponFocusBlades, swords_effect},

//{"�������", 700, 40 * GP, Male, Wood, {0, {2, 7, Piercing}, 1}, {5}, {}, {Ranged}, NoSkill, pierce_effect, Bolt},
//{"������� �������", 1200, 80 * GP, Male, Wood, {0, {3, 9, Piercing}, 5}, {5}, {}, {Ranged}, NoSkill, pierce_effect, Bolt},
//{"������� ���", 500, 60 * GP, Male, Wood, {1, {1, 8, Piercing}}, {5}, {}, {Ranged}, WeaponFocusBows, pierce_effect, Arrow},
//{"���", 300, 30 * GP, Male, Wood, {2, {1, 6, Piercing}}, {5}, {}, {Ranged}, WeaponFocusBows, pierce_effect, Arrow},
//{"������", 30, 1 * SP, Male, Wood, {3, {1, 3, Piercing}}, {5}, {}, {Ranged}},
//{"�����", 50, 1 * SP, Female, Leather, {2, {1, 4}}, {5}, {}, {Ranged}, NoSkill, pierce_effect, Rock},
////
//{"������", 20, 0, Male, Stone, {1, {1, 3}}, {}, {}, {Amunitions, Ranged}, NoSkill, {}, NoItem, 20},
//{"������", 2, 2 * CP, Female, Wood, {}, {}, {}, {Amunitions}, NoSkill, {}, NoItem, 20},
//{"����", 3, 1 * CP, Male, Iron, {}, {}, {}, {Amunitions}, NoSkill, {}, NoItem, 20},
////
//{"�������� �����", 1000, 5 * GP, Female, Leather, {0, {}, 0, {10}, 4}, {}, {}, {Torso}, NoSkill, armor_effect},
//{"��������� �����", 1500, 15 * GP, Female, Leather, {0, {}, 0, {15}, 4}, {}, {}, {Torso}, NoSkill, armor_effect},
//{"���������� ������", 2500, 30 * GP, Male, Iron, {0, {}, 0, {25}, 5}, {}, {}, {Torso}, NoSkill, armor_effect},
//{"��������", 2600, 50 * GP, Female, Iron, {0, {}, 0, {25, 1}, 5}, {}, {}, {Torso}, NoSkill, armor_effect},
//{"������", 3000, 200 * GP, Male, Iron, {0, {}, 0, {30, 2}, 5}, {}, {}, {Torso}, NoSkill, armor_effect},
//{"����", 3500, 800 * GP, Female, Iron, {0, {}, 0, {40, 3}, 5}, {}, {}, {Torso}, NoSkill, armor_effect},
////
//{"���", 1500, 20 * GP, Male, Iron, {0, {}, {}, {12}, 5}, {}, {}, {OffHand}, NoSkill, shield_effect},
//{"����", 300, 5 * GP, Male, Iron, {0, {}, {}, {3}, 2}, {}, {}, {Head}, NoSkill, helm_effect},
//{"������", 200, 3 * GP, They, Leather, {0, {}, {}, {3}, 2}, {}, {}, {Elbows}, NoSkill, bracers_effect},
//{"������", 400, 8 * GP, They, Iron, {0, {}, {}, {5, 1}, 2}, {}, {}, {Elbows}, NoSkill, bracers_effect},
////
//{"����", 200, 5 * GP, Male, Leather, {0, {}, {}, {1, 0}, 1}, {}, {}, {TorsoBack}, NoSkill, cloack_effect},
//{"����", 200, 6 * GP, Male, Leather, {0, {}, {}, {2, 0}, 1}, {}, {}, {TorsoBack}, NoSkill, cloack_effect},
//{"����", 200, 5 * GP, Male, Leather, {0, {}, {}, {1, 0}, 2}, {}, {}, {TorsoBack}, NoSkill, cloack_effect},
//{"����", 200, 5 * GP, Male, Leather, {0, {}, {}, {1, 0}, 1}, {}, {}, {TorsoBack}, NoSkill, cloack_effect},
//{"����", 200, 6 * GP, Male, Leather, {0, {}, {}, {2, 0}, 1}, {}, {}, {TorsoBack}, NoSkill, cloack_effect},
////
//{"������", 300, 6 * GP, They, Leather, {0, {}, {}, {1, 0}, 1}, {}, {}, {Legs}, NoSkill},
//{"������", 300, 6 * GP, They, Leather, {0, {}, {}, {2, 0}, 1}, {}, {}, {Legs}, NoSkill},
//{"������", 350, 6 * GP, They, Leather, {0, {}, {}, {3, 0}, 1}, {}, {}, {Legs}, NoSkill},
//{"������", 400, 6 * GP, They, Leather, {0, {}, {}, {4, 0}, 1}, {}, {}, {Legs}, NoSkill},
//{"������", 450, 6 * GP, They, Leather, {0, {}, {}, {4, 0}, 2}, {}, {}, {Legs}, NoSkill},
////
//{"�������", 100, 3 * SP, Male, Organic, {}, {}, {}, {}, NoSkill, {}, NoItem, 0, 0, {5, 0, {1, 0, 1, 0, 0, 0}, 10 * Minute}},
//{"������", 10, 1 * SP, NoGender, Organic, {}, {}, {}, {}, NoSkill, {}, NoItem, 0, 0, {1, 0, {2, 0, 0, 0, 0, 0}, 2 * Minute}},
//{"���� ��������", 20, 5 * SP, Male, Organic, {}, {}, {}, {}, NoSkill, {}, NoItem, 0, 0, {3, 0, {0, 3, 2, 0, 0, 0}, 4 * Minute, 10}},
//{"���� ������", 20, 10 * SP, Male, Organic, {}, {}, {}, {}, NoSkill, {}, NoItem, 0, 0, {4, 0, {0, 4, 0, 1, 0, 0}, 5 * Minute, 20}},
//{"���� ������", 25, 2 * SP, Male, Organic, {}, {}, {}, {}, NoSkill, {}, NoItem, 0, 0, {2, 0, {1, 0, 4, 0, 0, 0}, 5 * Minute, 0, 10}},
//{"�������", 10, 1 * SP, Male, Organic, {}, {}, {}, {}, NoSkill, {}, NoItem, 0, 0, {1, 0, {0, 0, 0, 1, 0, 1}, Minute}},
//{"�������", 40, 8 * SP, Female, Organic, {}, {}, {}, {}, NoSkill, {}, NoItem, 0, 0, {4, 0, {1, 1, 1, 0, 0, 0}, 5 * Minute}},
//{"����", 50, 5 * SP, NoGender, Organic, {}, {}, {}, {}, NoSkill, {}, NoItem, 0, 0, {2, 0, {2, 0, 0, 0, 0, 0}, 3 * Minute}},
////
//{"������", 10, 5 * GP, Male, Paper, {}, {15, 5}, {Readable}, {}, NoSkill, scroll_spells},
//{"������", 10, 6 * GP, Male, Paper, {}, {10, 10}, {Readable}, {}, NoSkill, scroll_spells},
//{"������", 10, 7 * GP, Male, Paper, {}, {5, 10}, {Readable}, {}, NoSkill, scroll_spells},
////
//{"�������", 5, 50 * GP, Female, Wood, {}, {}, {}, {}, NoSkill, wand_spells, NoItem, 0, 20},
//{"�������", 5, 70 * GP, Female, Wood, {}, {}, {}, {}, NoSkill, wand_spells, NoItem, 0, 30},
//{"�������", 5, 90 * GP, Female, Iron, {}, {}, {}, {}, NoSkill, wand_spells, NoItem, 0, 40},
//{"�������", 5, 50 * GP, Female, Iron, {}, {}, {}, {}, NoSkill, wand_spells, NoItem, 0, 20},
//{"�������", 5, 60 * GP, Female, Iron, {}, {}, {}, {}, NoSkill, wand_spells, NoItem, 0, 25},
////
//{"�����", 300, 80 * GP, Female, Paper, {}, {80, -20, 65}, {Readable, Tome}, {}, NoSkill, wand_spells},
//{"������", 350, 100 * GP, Male, Paper, {}, {55, -15, 50}, {Readable, Tome}, {}, NoSkill, manual_skills},
//{"�����", 300, 110 * GP, Female, Paper, {}, {70, -10, 55}, {Readable, Tome}, {}, NoSkill, wand_spells},
//{"�����", 300, 90 * GP, Female, Paper, {}, {75, -15, 50}, {Readable, Tome}, {}, NoSkill, wand_spells},
//{"���", 300, 130 * GP, Male, Paper, {}, {60, -15, 35}, {Readable, Tome}, {}, NoSkill, wand_spells},
////
//{"�����", 40, 20 * GP, NoGender, Glass, {}, {}, {}, {}, NoSkill, potion_state, NoItem},
//{"�����", 40, 25 * GP, NoGender, Glass, {}, {}, {}, {}, NoSkill, potion_state, NoItem},
//{"�����", 40, 30 * GP, NoGender, Glass, {}, {}, {}, {}, NoSkill, potion_state, NoItem},
//{"�����", 40, 20 * GP, NoGender, Glass, {}, {}, {}, {}, NoSkill, potion_state, NoItem},
//{"�����", 40, 25 * GP, NoGender, Glass, {}, {}, {}, {}, NoSkill, potion_state, NoItem},
////
//{"������", 2, 60 * GP, NoGender, Iron, {}, {}, {}, {}, NoSkill, ring_effect},
//{"������", 2, 70 * GP, NoGender, Iron, {}, {}, {}, {}, NoSkill, ring_effect},
//{"������", 2, 80 * GP, NoGender, Iron, {}, {}, {}, {}, NoSkill, ring_effect},
////
//{"������", 2, 50 * GP, Male, Iron, {}, {}, {}, {}, NoSkill, ring_effect},
//{"������", 2, 50 * GP, Male, Iron, {}, {}, {}, {}, NoSkill, ring_effect},
//{"������", 2, 50 * GP, Male, Iron, {}, {}, {}, {}, NoSkill, ring_effect},
//{"������", 2, 50 * GP, Male, Iron, {}, {}, {}, {}, NoSkill, ring_effect},
//{"������", 2, 50 * GP, Male, Iron, {}, {}, {}, {}, NoSkill, ring_effect},
////
//{"����", 10, 0, Male, Iron},
//{"������", 1, 1 * CP, Female, Iron, {}, {}, {}, {}, NoSkill, {}, NoItem, 50},
//{"������", 1, 1 * SP, Female, Iron, {}, {}, {}, {}, NoSkill, {}, NoItem, 50},
//{"������", 1, 1 * GP, Female, Iron, {}, {}, {}, {}, NoSkill, {}, NoItem, 50},
////
//{"�����", 0, 0, They, Organic, {4, {1, 3, Slashing}}, {}, {Natural}, {Melee}, NoSkill, bite_effect},
//{"����", 0, 0, Male, Organic, {0, {2, 7}}, {}, {Natural}, {Melee}, NoSkill, bite_effect},
//{"����", 0, 0, Male, Organic, {2, {2, 5, Piercing}}, {}, {Natural}, {Melee}, NoSkill, bite_effect},
//{"�����", 0, 0, Male, Organic, {0, {}, 0, {20}, 4}, {}, {Natural}, {Torso}, NoSkill, armor_effect},
//{"���", 0, 0, Male, Organic, {0, {}, 0, {12}, 4}, {}, {Natural}, {Torso}, NoSkill, armor_effect},
#include "main.h"

itemi bsmeta<itemi>::elements[] = {{""},
{"Боевой топор", 850, 5 * GP, Male, Iron, {-3, D4n9, Slashing}, {}, {}, {}, Melee, WeaponFocusAxes},
{"Дубина", 1000, 5 * CP, Female, Wood, {-6, D1n4, Bludgeon}, {}, {}, {}, Melee},
{"Кинжал", 50, 2 * GP, Male, Iron, {-6, D1n3, Piercing}, {}, {}, {}, Melee, WeaponFocusBlades},
{"Молот", 800, 2 * GP, Male, Wood, {-3, D3n8, Bludgeon}, {}, {}, {}, Melee},
{"Булава", 700, 8 * GP, Female, Iron, {-3, D2n7, Bludgeon}, {}, {}, {}, Melee},
{"Копье", 700, 8 * GP, NoGender, Iron, {-3, D3n8, Piercing}, {}, {}, {}, Melee},
{"Посох", 700, 8 * GP, Female, Iron, {-3, D1n6, Bludgeon}, {}, {}, {}, Melee},
{"Меч", 700, 8 * GP, Female, Iron, {-4, D5n10, Slashing}, {}, {}, {}, Melee, WeaponFocusBlades},
{"Короткий меч", 700, 8 * GP, Female, Iron, {-3, D3n8, Slashing}, {}, {}, {}, Melee, WeaponFocusBlades},
{"Двуручный меч", 700, 8 * GP, Female, Iron, {-9, D4n14, Slashing}, {}, {}, {}, Melee, WeaponFocusBlades},
//
{"Арбалет", 700, 40 * GP, Male, Wood, {}, {}, {}, {}, Melee},
{"Тяжелый арбалет", 1200, 80 * GP, Male, Wood, {}, {}, {}, {}, Melee},
{"Длинный лук", 500, 60 * GP, Male, Wood, {}, {}, {}, {}, Melee, WeaponFocusBows},
{"Короткий лук", 300, 30 * GP, Male, Wood, {}, {}, {}, {}, Melee, WeaponFocusBows},
{"Дарт", 30, 1 * SP, Male, Wood, {}, {}, {}, {}, Melee},
{"Праща", 50, 1 * SP, Female, Leather, {}, {}, {}, {}, Melee},
//
{"Камни", 20, 0, Male, Stone, {}, {}, {}, {}, Amunitions},
{"Стрелы", 2, 2 * CP, Female, Wood, {}, {}, {}, {}, Amunitions},
{"Болты", 3, 1 * CP, Male, Iron, {}, {}, {}, {}, Amunitions},
//
{"Кожанная броня", 1000, 5 * GP, Female, Leather, {}, {10, 1}, {}, {}, Torso},
{"Клепанная броня", 1500, 15 * GP, Female, Leather, {}, {15, 1}, {}, {}, Torso},
{"Чешуйчатый доспех", 2500, 30 * GP, Male, Iron, {}, {25, 2}, {}, {}, Torso},
{"Кольчуга", 2600, 50 * GP, Female, Iron, {}, {25, 3}, {}, {}, Torso},
{"Бахрец", 3000, 200 * GP, Male, Iron, {}, {30, 4}, {}, {}, Torso},
{"Латы", 3500, 800 * GP, Female, Iron, {}, {40, 5}, {}, {}, Torso},
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

//{"Боевой топор", 850, 5 * GP, Male, Iron, {1, {1, 8, Slashing}}, {5}, {Versatile}, {Melee}, WeaponFocusAxes, axe_effect},
//{"Дубина", 1000, 5 * CP, Female, Wood, {2, {1, 6}}, {4}, {}, {Melee}, NoSkill, bludgeon_effect},
//{"Кинжал", 50, 2 * GP, Male, Iron, {3, {1, 4, Piercing}, 5}, {5}, {}, {Melee, OffHand}, WeaponFocusBlades, swords_effect},
//{"Молот", 800, 2 * GP, Male, Wood, {1, {2, 5}}, {5}, {}, {Melee}, WeaponFocusAxes, bludgeon_effect},
//{"Булава", 700, 8 * GP, Female, Iron, {1, {1, 7}}, {5}, {}, {Melee}, WeaponFocusAxes, bludgeon_effect},
//{"Копье", 250, 8 * SP, NoGender, Wood, {1, {1, 8, Piercing}}, {5}, {Versatile}, {Melee}, NoSkill, pierce_effect},
//{"Посох", 200, 1 * SP, Male, Wood, {2, {1, 6}}, {6}, {TwoHanded}, {Melee}, NoSkill, staff_spells, NoItem, 0, 50},
//{"Длинный меч", 200, 15 * GP, Male, Iron, {1, {1, 8, Slashing}}, {6}, {}, {Melee}, WeaponFocusBlades, swords_effect},
//{"Короткий меч", 150, 10 * GP, Male, Iron, {1, {1, 6, Slashing}}, {6}, {}, {Melee, OffHand}, WeaponFocusBlades, swords_effect},
//{"Двуручный меч", 1500, 50 * GP, Male, Iron, {0, {2, 12, Slashing}}, {6}, {TwoHanded}, {Melee}, WeaponFocusBlades, swords_effect},

//{"Арбалет", 700, 40 * GP, Male, Wood, {0, {2, 7, Piercing}, 1}, {5}, {}, {Ranged}, NoSkill, pierce_effect, Bolt},
//{"Тяжелый арбалет", 1200, 80 * GP, Male, Wood, {0, {3, 9, Piercing}, 5}, {5}, {}, {Ranged}, NoSkill, pierce_effect, Bolt},
//{"Длинный лук", 500, 60 * GP, Male, Wood, {1, {1, 8, Piercing}}, {5}, {}, {Ranged}, WeaponFocusBows, pierce_effect, Arrow},
//{"Лук", 300, 30 * GP, Male, Wood, {2, {1, 6, Piercing}}, {5}, {}, {Ranged}, WeaponFocusBows, pierce_effect, Arrow},
//{"Дротик", 30, 1 * SP, Male, Wood, {3, {1, 3, Piercing}}, {5}, {}, {Ranged}},
//{"Пращя", 50, 1 * SP, Female, Leather, {2, {1, 4}}, {5}, {}, {Ranged}, NoSkill, pierce_effect, Rock},
////
//{"Камень", 20, 0, Male, Stone, {1, {1, 3}}, {}, {}, {Amunitions, Ranged}, NoSkill, {}, NoItem, 20},
//{"Стрела", 2, 2 * CP, Female, Wood, {}, {}, {}, {Amunitions}, NoSkill, {}, NoItem, 20},
//{"Болт", 3, 1 * CP, Male, Iron, {}, {}, {}, {Amunitions}, NoSkill, {}, NoItem, 20},
////
//{"Кожанная броня", 1000, 5 * GP, Female, Leather, {0, {}, 0, {10}, 4}, {}, {}, {Torso}, NoSkill, armor_effect},
//{"Клепанная броня", 1500, 15 * GP, Female, Leather, {0, {}, 0, {15}, 4}, {}, {}, {Torso}, NoSkill, armor_effect},
//{"Чешуйчатый доспех", 2500, 30 * GP, Male, Iron, {0, {}, 0, {25}, 5}, {}, {}, {Torso}, NoSkill, armor_effect},
//{"Кольчуга", 2600, 50 * GP, Female, Iron, {0, {}, 0, {25, 1}, 5}, {}, {}, {Torso}, NoSkill, armor_effect},
//{"Бахрец", 3000, 200 * GP, Male, Iron, {0, {}, 0, {30, 2}, 5}, {}, {}, {Torso}, NoSkill, armor_effect},
//{"Латы", 3500, 800 * GP, Female, Iron, {0, {}, 0, {40, 3}, 5}, {}, {}, {Torso}, NoSkill, armor_effect},
////
//{"Щит", 1500, 20 * GP, Male, Iron, {0, {}, {}, {12}, 5}, {}, {}, {OffHand}, NoSkill, shield_effect},
//{"Шлем", 300, 5 * GP, Male, Iron, {0, {}, {}, {3}, 2}, {}, {}, {Head}, NoSkill, helm_effect},
//{"Наручи", 200, 3 * GP, They, Leather, {0, {}, {}, {3}, 2}, {}, {}, {Elbows}, NoSkill, bracers_effect},
//{"Наручи", 400, 8 * GP, They, Iron, {0, {}, {}, {5, 1}, 2}, {}, {}, {Elbows}, NoSkill, bracers_effect},
////
//{"Плащ", 200, 5 * GP, Male, Leather, {0, {}, {}, {1, 0}, 1}, {}, {}, {TorsoBack}, NoSkill, cloack_effect},
//{"Плащ", 200, 6 * GP, Male, Leather, {0, {}, {}, {2, 0}, 1}, {}, {}, {TorsoBack}, NoSkill, cloack_effect},
//{"Плащ", 200, 5 * GP, Male, Leather, {0, {}, {}, {1, 0}, 2}, {}, {}, {TorsoBack}, NoSkill, cloack_effect},
//{"Плащ", 200, 5 * GP, Male, Leather, {0, {}, {}, {1, 0}, 1}, {}, {}, {TorsoBack}, NoSkill, cloack_effect},
//{"Плащ", 200, 6 * GP, Male, Leather, {0, {}, {}, {2, 0}, 1}, {}, {}, {TorsoBack}, NoSkill, cloack_effect},
////
//{"Сапоги", 300, 6 * GP, They, Leather, {0, {}, {}, {1, 0}, 1}, {}, {}, {Legs}, NoSkill},
//{"Сапоги", 300, 6 * GP, They, Leather, {0, {}, {}, {2, 0}, 1}, {}, {}, {Legs}, NoSkill},
//{"Сапоги", 350, 6 * GP, They, Leather, {0, {}, {}, {3, 0}, 1}, {}, {}, {Legs}, NoSkill},
//{"Сапоги", 400, 6 * GP, They, Leather, {0, {}, {}, {4, 0}, 1}, {}, {}, {Legs}, NoSkill},
//{"Сапоги", 450, 6 * GP, They, Leather, {0, {}, {}, {4, 0}, 2}, {}, {}, {Legs}, NoSkill},
////
//{"Сухпаек", 100, 3 * SP, Male, Organic, {}, {}, {}, {}, NoSkill, {}, NoItem, 0, 0, {5, 0, {1, 0, 1, 0, 0, 0}, 10 * Minute}},
//{"Яблоко", 10, 1 * SP, NoGender, Organic, {}, {}, {}, {}, NoSkill, {}, NoItem, 0, 0, {1, 0, {2, 0, 0, 0, 0, 0}, 2 * Minute}},
//{"Хлеб хоббитов", 20, 5 * SP, Male, Organic, {}, {}, {}, {}, NoSkill, {}, NoItem, 0, 0, {3, 0, {0, 3, 2, 0, 0, 0}, 4 * Minute, 10}},
//{"Хлеб эльфов", 20, 10 * SP, Male, Organic, {}, {}, {}, {}, NoSkill, {}, NoItem, 0, 0, {4, 0, {0, 4, 0, 1, 0, 0}, 5 * Minute, 20}},
//{"Хлеб гномов", 25, 2 * SP, Male, Organic, {}, {}, {}, {}, NoSkill, {}, NoItem, 0, 0, {2, 0, {1, 0, 4, 0, 0, 0}, 5 * Minute, 0, 10}},
//{"Печенье", 10, 1 * SP, Male, Organic, {}, {}, {}, {}, NoSkill, {}, NoItem, 0, 0, {1, 0, {0, 0, 0, 1, 0, 1}, Minute}},
//{"Колбаса", 40, 8 * SP, Female, Organic, {}, {}, {}, {}, NoSkill, {}, NoItem, 0, 0, {4, 0, {1, 1, 1, 0, 0, 0}, 5 * Minute}},
//{"Мясо", 50, 5 * SP, NoGender, Organic, {}, {}, {}, {}, NoSkill, {}, NoItem, 0, 0, {2, 0, {2, 0, 0, 0, 0, 0}, 3 * Minute}},
////
//{"Свиток", 10, 5 * GP, Male, Paper, {}, {15, 5}, {Readable}, {}, NoSkill, scroll_spells},
//{"Свиток", 10, 6 * GP, Male, Paper, {}, {10, 10}, {Readable}, {}, NoSkill, scroll_spells},
//{"Свиток", 10, 7 * GP, Male, Paper, {}, {5, 10}, {Readable}, {}, NoSkill, scroll_spells},
////
//{"Палочка", 5, 50 * GP, Female, Wood, {}, {}, {}, {}, NoSkill, wand_spells, NoItem, 0, 20},
//{"Палочка", 5, 70 * GP, Female, Wood, {}, {}, {}, {}, NoSkill, wand_spells, NoItem, 0, 30},
//{"Палочка", 5, 90 * GP, Female, Iron, {}, {}, {}, {}, NoSkill, wand_spells, NoItem, 0, 40},
//{"Палочка", 5, 50 * GP, Female, Iron, {}, {}, {}, {}, NoSkill, wand_spells, NoItem, 0, 20},
//{"Палочка", 5, 60 * GP, Female, Iron, {}, {}, {}, {}, NoSkill, wand_spells, NoItem, 0, 25},
////
//{"Книга", 300, 80 * GP, Female, Paper, {}, {80, -20, 65}, {Readable, Tome}, {}, NoSkill, wand_spells},
//{"Мануал", 350, 100 * GP, Male, Paper, {}, {55, -15, 50}, {Readable, Tome}, {}, NoSkill, manual_skills},
//{"Книга", 300, 110 * GP, Female, Paper, {}, {70, -10, 55}, {Readable, Tome}, {}, NoSkill, wand_spells},
//{"Книга", 300, 90 * GP, Female, Paper, {}, {75, -15, 50}, {Readable, Tome}, {}, NoSkill, wand_spells},
//{"Том", 300, 130 * GP, Male, Paper, {}, {60, -15, 35}, {Readable, Tome}, {}, NoSkill, wand_spells},
////
//{"Зелье", 40, 20 * GP, NoGender, Glass, {}, {}, {}, {}, NoSkill, potion_state, NoItem},
//{"Зелье", 40, 25 * GP, NoGender, Glass, {}, {}, {}, {}, NoSkill, potion_state, NoItem},
//{"Зелье", 40, 30 * GP, NoGender, Glass, {}, {}, {}, {}, NoSkill, potion_state, NoItem},
//{"Зелье", 40, 20 * GP, NoGender, Glass, {}, {}, {}, {}, NoSkill, potion_state, NoItem},
//{"Зелье", 40, 25 * GP, NoGender, Glass, {}, {}, {}, {}, NoSkill, potion_state, NoItem},
////
//{"Кольцо", 2, 60 * GP, NoGender, Iron, {}, {}, {}, {}, NoSkill, ring_effect},
//{"Кольцо", 2, 70 * GP, NoGender, Iron, {}, {}, {}, {}, NoSkill, ring_effect},
//{"Кольцо", 2, 80 * GP, NoGender, Iron, {}, {}, {}, {}, NoSkill, ring_effect},
////
//{"Амулет", 2, 50 * GP, Male, Iron, {}, {}, {}, {}, NoSkill, ring_effect},
//{"Амулет", 2, 50 * GP, Male, Iron, {}, {}, {}, {}, NoSkill, ring_effect},
//{"Амулет", 2, 50 * GP, Male, Iron, {}, {}, {}, {}, NoSkill, ring_effect},
//{"Амулет", 2, 50 * GP, Male, Iron, {}, {}, {}, {}, NoSkill, ring_effect},
//{"Амулет", 2, 50 * GP, Male, Iron, {}, {}, {}, {}, NoSkill, ring_effect},
////
//{"Ключ", 10, 0, Male, Iron},
//{"Монета", 1, 1 * CP, Female, Iron, {}, {}, {}, {}, NoSkill, {}, NoItem, 50},
//{"Монета", 1, 1 * SP, Female, Iron, {}, {}, {}, {}, NoSkill, {}, NoItem, 50},
//{"Монета", 1, 1 * GP, Female, Iron, {}, {}, {}, {}, NoSkill, {}, NoItem, 50},
////
//{"Когти", 0, 0, They, Organic, {4, {1, 3, Slashing}}, {}, {Natural}, {Melee}, NoSkill, bite_effect},
//{"Удар", 0, 0, Male, Organic, {0, {2, 7}}, {}, {Natural}, {Melee}, NoSkill, bite_effect},
//{"Укус", 0, 0, Male, Organic, {2, {2, 5, Piercing}}, {}, {Natural}, {Melee}, NoSkill, bite_effect},
//{"Хитин", 0, 0, Male, Organic, {0, {}, 0, {20}, 4}, {}, {Natural}, {Torso}, NoSkill, armor_effect},
//{"Мех", 0, 0, Male, Organic, {0, {}, 0, {12}, 4}, {}, {Natural}, {Torso}, NoSkill, armor_effect},
#include "main.h"

itemi bsmeta<itemi>::elements[] = {{""},
{"Боевой топор", 850, 5 * GP, Male, Iron, {0, D4n9, -2}, {}, {}, {}, Melee, WeaponFocusAxes},
{"Дубина", 1000, 5 * CP, Female, Wood, {}, {}, {}, {}, Melee},
{"Кинжал", 50, 2 * GP, Male, Iron, {}, {}, {}, {}, Melee, WeaponFocusBlades},
{"Молото", 800, 2 * GP, Male, Wood, {}, {}, {}, {}, Melee},
{"Булава", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"Копье", 700, 8 * GP, NoGender, Iron, {}, {}, {}, {}, Melee},
{"Посох", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"Меч", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"Короткий меч", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"Двуручный меч", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
//
{"Арбалет", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"Тяжелый арбалет", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"Длинный лук", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"Короткий лук", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"Дарт", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"Праща", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
//
{"Камни", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"Стрелы", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
{"Болты", 700, 8 * GP, Female, Iron, {}, {}, {}, {}, Melee},
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
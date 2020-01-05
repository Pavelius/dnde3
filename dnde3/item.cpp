#include "main.h"

itemi bsmeta<itemi>::elements[] = {{"", 0, 0, NoGender, Organic, {}, {}, {}, {}, Melee},
{"Боевой топор", 850, 5 * GP, Male, Iron, {-4, D1n8, Slashing}, {}, {}, {Versatile}, Melee, FightAxes},
{"Дубина", 1000, 5 * CP, Female, Wood, {-6, D1n4, Bludgeon}, {}, {}, {}, Melee},
{"Кинжал", 50, 2 * GP, Male, Iron, {-2, D1n3, Piercing}, {}, {}, {}, Melee},
{"Молот", 800, 2 * GP, Male, Wood, {-3, D1n4, Bludgeon}, {}, {}, {}, Melee},
{"Булава", 700, 8 * GP, Female, Iron, {-3, D2n7, Bludgeon}, {}, {}, {}, Melee},
{"Копье", 700, 8 * GP, NoGender, Iron, {-5, D1n8, Piercing}, {}, {}, {}, Melee},
{"Посох", 700, 8 * GP, Female, Iron, {-1, D1n6, Bludgeon}, {}, {}, {TwoHanded}, Melee},
{"Длинный меч", 700, 8 * GP, Female, Iron, {-4, D1n8, Slashing}, {}, {}, {Versatile}, Melee, FightSwords},
{"Короткий меч", 700, 8 * GP, Female, Iron, {-3, D1n8, Slashing}, {}, {}, {}, Melee, FightSwords},
{"Двуручный меч", 700, 8 * GP, Female, Iron, {-8, D2n12, Slashing}, {}, {}, {TwoHanded}, Melee, FightTwohanded},
//
{"Арбалет", 700, 40 * GP, Male, Wood, {}, {}, {}, {}, Ranged},
{"Тяжелый арбалет", 1200, 80 * GP, Male, Wood, {}, {}, {}, {}, Ranged},
{"Длинный лук", 500, 60 * GP, Male, Wood, {-8, D1n8, Piercing, 0, Arrow}, {}, {}, {}, Ranged, Archery},
{"Короткий лук", 300, 30 * GP, Male, Wood, {-4, D1n6, Piercing, 0, Arrow}, {}, {}, {}, Ranged, Archery},
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
//
{"Щит", 0, 0 * GP, Female, Iron, {}, {}, {}, {}, OffHand},
{"Шлем", 0, 0 * GP, Female, Iron, {}, {}, {}, {}, Head},
{"Наручи", 0, 0 * GP, Female, Iron, {}, {}, {}, {}, Elbows},
{"Железные наручи", 0, 0 * GP, Female, Iron, {}, {}, {}, {}, Elbows},
//
{"Плащ", 0, 0 * GP, Male, Leather, {}, {}, {}, {}, TorsoBack},
{"Плащ", 0, 0 * GP, Male, Leather, {}, {}, {}, {}, TorsoBack},
{"Плащ", 0, 0 * GP, Female, Leather, {}, {}, {}, {}, TorsoBack},
{"Плащ", 0, 0 * GP, Female, Leather, {}, {}, {}, {}, TorsoBack},
{"Плащ", 0, 0 * GP, Female, Leather, {}, {}, {}, {}, TorsoBack},
//
{"Сапоги", 0, 0 * GP, NoGender, Leather, {}, {}, {}, {}, Legs},
{"Сапоги", 0, 0 * GP, NoGender, Leather, {}, {}, {}, {}, Legs},
{"Сапоги", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}, Legs},
{"Сапоги", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}, Legs},
{"Сапоги", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}, Legs},
//
{"Еда", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {}},
{"Яблоко", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {}},
{"Хлеб хоббитов", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {}},
{"Хлеб эльфов", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {}},
{"Хлеб гномов", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {}},
{"Пирожное", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {}},
{"Колбаса", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {}},
{"Мясо", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {}},
//
{"Свиток", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Свиток", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Свиток", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
//
{"Жезл", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Жезл", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Жезл", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Жезл", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Жезл", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
//
{"Книга", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Книга", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Книга", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Книга", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Книга", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
//
{"Зелье", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Зелье", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Зелье", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Зелье", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Зелье", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
//
{"Кольцо", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Кольцо", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Кольцо", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
//
{"Амулет", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Амулет", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Амулет", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Амулет", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Амулет", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
//
{"Ключ", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
//
{"Монета", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Серебрянная монета", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Золотая монета", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
//
{"Когти", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {Natural}},
{"Кулаки", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {Natural}},
{"Укус", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {Natural}},
{"Хитин", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {Natural}},
{"Мех", 0, 0 * GP, NoGender, Paper, {}, {20}, {}, {Natural}, Torso},
//
{"Много предметов"},
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
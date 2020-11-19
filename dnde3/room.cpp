#include "main.h"

static const char* shop_end[] = {"из-за океана",
"высокого качества",
"по небольшим ценам",
"с изюменкой",
"для королей",
"дешевле некуда",
};
static const char* shop_weapon[] = {"Кузница %2",
"Мечи и доспехи %2",
"Оружие %2",
"Мечи %2",
};
static const char* shop_potions[] = {"Аптека %2",
"Зелья и элексиры %2",
"Алхимия %2",
};
static const char* shop_scrolls[] = {"Свитки %2",
"Тома и заклинания %2",
"Магия %2",
"Волшебные папирусы %2",
};
static const char* shop_food[] = {"Припасы %2",
"Магазин еды %2",
"Еда %2",
"Лавка %4",
};
static const char* temple[] = {"Храм %6",
"Святилище %6",
"Молебня %6",
};

BSDATA(roomi) = {{""},
{"Сокровищница", {}, {}, NoTileObject, {}, {{60, Coinable}}},
{"Спуск ниже", {}, {}, StairsDown},
{"Подъем наверх", {}, {}, StairsUp},
{"Дом"},
{"Логово"},
{0, temple, {}, Altar},
{"Таверна \"%1 %2\"", {}, {}, NoTileObject, Bartender},
{"Бараки"},
{"Магистратура"},
{0, shop_weapon, shop_end, NoTileObject, Shopkeeper, {{30, Torso, 10}, {70, Melee, 10}}},
{0, shop_potions, shop_end, NoTileObject, Shopkeeper, {{80, Drinkable, 20}}},
{0, shop_scrolls, shop_end, NoTileObject, Shopkeeper, {{70, Readable, 30}}},
{0, shop_food, shop_end, NoTileObject, Shopkeeper, {{90, Edible, 5}}},
};
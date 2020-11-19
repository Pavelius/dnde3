#include "main.h"

static const char* shop_end[] = {"��-�� ������",
"�������� ��������",
"�� ��������� �����",
"� ���������",
"��� �������",
"������� ������",
};
static const char* shop_weapon[] = {"������� %2",
"���� � ������� %2",
"������ %2",
"���� %2",
};
static const char* shop_potions[] = {"������ %2",
"����� � �������� %2",
"������� %2",
};
static const char* shop_scrolls[] = {"������ %2",
"���� � ���������� %2",
"����� %2",
"��������� �������� %2",
};
static const char* shop_food[] = {"������� %2",
"������� ��� %2",
"��� %2",
"����� %4",
};
static const char* temple[] = {"���� %6",
"��������� %6",
"������� %6",
};

BSDATA(roomi) = {{""},
{"������������", {}, {}, NoTileObject, {}, {{60, Coinable}}},
{"����� ����", {}, {}, StairsDown},
{"������ ������", {}, {}, StairsUp},
{"���"},
{"������"},
{0, temple, {}, Altar},
{"������� \"%1 %2\"", {}, {}, NoTileObject, Bartender},
{"������"},
{"������������"},
{0, shop_weapon, shop_end, NoTileObject, Shopkeeper, {{30, Torso, 10}, {70, Melee, 10}}},
{0, shop_potions, shop_end, NoTileObject, Shopkeeper, {{80, Drinkable, 20}}},
{0, shop_scrolls, shop_end, NoTileObject, Shopkeeper, {{70, Readable, 30}}},
{0, shop_food, shop_end, NoTileObject, Shopkeeper, {{90, Edible, 5}}},
};
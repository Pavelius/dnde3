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

static const char* addlooktemple(const void* object, stringbuilder& sb) {
	auto p = (site*)object;
	auto v = p->getparam();
	if(v.type != God)
		return 0;
	auto& ei = bsdata<dietyi>::elements[v.value];
	sb.add("������ ��� ���� ������� � ��������������� �������� � ������� ��������. ��������� ��������� ������, �� ������� ��������� ����: ");
	sb.add(ei.symbol);
	sb.add(".");
	return sb.begin();
}

BSDATA(roomi) = {
	{""},
	{"������������", "�� ���� ����� ���� ������ ��������� ��������, � �� ���� ������ ��������� ������� �����.", {}, {}, NoTileObject, {}, {{60, Coinable}}},
	{"����� ����", 0, {}, {}, StairsDown},
	{"������ ������", 0, {}, {}, StairsUp},
	{"���"},
	{"������"},
	{0, 0, temple, {}, Altar, {}, {}, addlooktemple},
	{"������� \"%1 %2\"", "������ ���� ������ ���������� �����, �� ������� ���� �������. �� ������ ������ ����, ���� � ���.", {}, {}, NoTileObject, Bartender},
	{"������", "������ ���� ������ ��������� ��������� � �������. ������ ������ ���������� ������� �� ����."},
	{"������������", "��� � ����� ����� ������ �������� �� ��������� �������� ����� � ���� ������� ���������� �������. ����� ������ ����� � ��������� �������."},
	{0, "������ ���� ������ ��������� ����� � ������ �� ������� ���� ��������� ������ � �������.", shop_weapon, shop_end, NoTileObject, Shopkeeper, {{30, Torso, 10}, {70, Melee, 10}}},
	{0, "������ ���� ������ ��������� ������ �� ������� ������ ��������� ��������� ������.", shop_potions, shop_end, NoTileObject, Shopkeeper, {{80, Drinkable, 20}}},
	{0, 0, shop_scrolls, shop_end, NoTileObject, Shopkeeper, {{70, Readable, 30}}},
	{0, 0, shop_food, shop_end, NoTileObject, Shopkeeper, {{90, Edible, 5}}},
};
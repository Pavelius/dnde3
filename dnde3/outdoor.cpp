#include "main.h"

template<> outdoori bsmeta<outdoori>::elements[] = {{Blocked, "����",
"�� ����� ������������� ��������� ��������� ��������.", {ResDecals, 8},
{{AreaCity, 1, 0, {StairsDownRoom, Barracs, Lair}},
{AreaDungeon, 8, -2, {StairsDownRoom, StairsUpRoom}},
{AreaDungeon, 1, -2, {StairsUpRoom}}}},
{Blocked, "�����",
"������ ����� ����� ������� �� ������������� ���������������. ��������, ������� � ��������� - ����� ���� ��� ������� ��������", {ResDecals, 9},
{{AreaCity, 1, 0, {StairsDownRoom, ShopFood, ShopWeaponAndArmor}},
{AreaDungeon, 8, -2, {StairsDownRoom, StairsUpRoom}},
{AreaDungeon, 1, -2, {StairsUpRoom}}}},
{Blocked, "����������� �����",
"� ����� ��������� ����������� �����. ������ ������ ����, � �������. ������ ����� ����� ������ �� ����.", {ResDecals, 8},
{{AreaDungeon, 10, -2, {StairsDownRoom, StairsUpRoom}},
{AreaDungeon, 1, -2, {StairsUpRoom}}}},
};
assert_enum(outdoor, LastOutdoor);

const outdoori* outdoori::find(indext index) {
	for(auto& e : bsmeta<outdoori>()) {
		if(!e)
			continue;
		if(e.getposition() == index)
			return &e;
	}
	return 0;
}

void outdoori::clear() {
	memset(this, 0, sizeof(*this));
	index = Blocked;
}
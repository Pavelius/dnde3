#include "main.h"

BSDATA(outdoori) = {{1349, "����", "town",
"�� ����� ������������� ��������� ��������� ��������.",
{{AreaCity, 1, 0, {Barracs, Lair}},
}},
{Blocked, "�����", "borderland_entry",
"������ ����� ����� ������� �� ������������� ���������������. ��������, ������� � ��������� - ����� ���� ��� ������� ��������",
{{AreaCity, 1, 0, {StairsDownRoom, ShopFood, ShopWeaponAndArmor}},
{AreaDungeon, 8, -2, {StairsDownRoom, StairsUpRoom}},
{AreaDungeon, 1, -2, {StairsUpRoom}}}},
{774, "����������� �����", "tunnel",
"� ����� ��������� ����������� �����. ������ ������ ����, � �������. ������ ����� ����� ������ �� ����.",
{{AreaDungeon, 5, -2, {StairsDownRoom, StairsUpRoom}},
{AreaDungeon, 1, -2, {StairsUpRoom}}}},
};
assert_enum(outdoori, LastOutdoor)

int	outdoori::getid() const {
	return this - bsdata<outdoori>::elements;
}

outdoori* outdoori::find(indext index) {
	for(auto& e : bsdata<outdoori>()) {
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

outdoori* outdoori::choose() {
	answeri an;
	for(auto& e : bsdata<outdoori>()) {
		if(e)
			continue;
		an.add((int)&e, e.name);
	}
	an.sort();
	return (outdoori*)an.choosev(true, false, false, "��������� ���������");
}
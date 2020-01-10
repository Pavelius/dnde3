#include "main.h"

abilityi bsmeta<abilityi>::elements[] = {{"Strenght", "����", "��", "����", "��������", "�������", "������", {}, 0, 1, "%+1i", 2},
{"Dexterity", "��������", "��", "��������", "�����������", "�����", "��������", {}, 0, 1, "%+1i", 2},
{"Constitution", "������������", "��", "������������", "�������", "��������", "��������", {}, 0, 1, "%+1i", 1},
{"Intellegence", "���������", "��", "����������", "��������", "�����", "������", {}, 0, 1, "%+1i", 1},
{"Wisdow", "��������", "��", "��������", "���������", "������", "�������", {}, 0, 1, "%+1i", 1},
{"Charisma", "�������", "��", "�������", "��������", "�������������", "���������", {}, 0, 1, "%+1i", 2},
//
{"Attack", "���� ���������", "��", "���������", "�������", "�������", "������", {Dexterity, Multiply2, -20}, 0, 4, "%+1i%%", 1},
{"Damage", "����", "��", "�����", "�������", "�������", "������", {Strenght, Divide2, -5}, 0, 1, "%+1i", 3},
{"Pierce", "������������", "��", "����������", "����������", "�������", "������", {}, 0, 1, "%+1i", 1},
{"Protection", "������", "��", "������", "����������", "����������", "������", {Acrobatics, Divide4}, 0, 5, "%+1i%%", 1},
{"Armor", "�����", "��", "�����", "", "���������", "��������", {}, 0, 1, "%+1i", 3},
{"Deflect", "���������", "��", "���������", "����������", "����������", "��������", {}, 0, 1, "%+1i%%", 2},
{"Speed", "��������", "��", "��������", "����������", "�������", "���������", {95, Athletics, Divide10, Dexterity, Divide2}, 0, 1, "%+1i", 2},
{"Movement", "������������", "��", "��������", "����������", "���������", "�����������", {}, 0, 1, "%+1i", 1},
{"Visibility", "�����", "��", "������", "�������", "�����", "����", {6}, 0, 1, "%+1i", 2},
//
{"Resist Acid", "������������� �������", "", "������ �� �������", "���������� � �������", "����������", "��������", {}, 20, 10, "%+1i%%", 1},
{"Resist Charm", "������������� �����", "", "������ �� �����", "���������� � �����", "����������", "��������", {Intellegence, Wisdow}, 15, 10, "%+1i%%", 1},
{"Resist Ice", "������������� ������", "", "������ �� ������", "���������� � ������", "����������", "��������", {}, 10, 10, "%+1i%%", 1},
{"Resist Lighting", "������������� �������������", "", "������ �� ������", "���������� � ������", "����������", "��������", {}, 20, 10, "%+1i%%", 1},
{"Resist Fire", "������������� ����", "", "������ �� ����", "���������� � ����", "����������", "��������", {}, 20, 15, "%+1i%%", 1},
{"Resist Paralize", "������������� ��������", "", "������ �� ��������", "���������� � ��������", "����������", "��������", {}, 20, 10, "%+1i%%", 0},
{"Resist Poison", "������������� ���", "", "������ �� ���", "���������� � ���", "����������", "��������", {}, 20, 15, "%+1i%%", 1},
{"Resist Water", "������� �����", "", "������ �� ����", "���������� � ����", "����������", "��������", {}, 20, 20, "%+1i%%", 0},
//
{"Level", "�������", "", "�����", "", "�������", "������", {}, 0, 0, 0},
{"LifePoints", "�����", "", "�������", "���", "�����", "����", {Constitution}, 0, 5, "%+1i", 0},
{"LifeRate", "�������������� �����", "", "�����������", "����������", "�����", "����", {Healing}, 0, 40, "%+1i", 1},
{"ManaPoints", "����", "", "��������������", "�����������", "�����", "����", {Wisdow}, 0, 5, "%+1i", 0},
{"ManaRate", "�������������� ����", "", "�������������", "��������", "�����", "����", {Concetration}, 0, 50, "%+1i", 1},
};
assert_enum(ability, ManaRate);

ability_s abilityi::getid() const {
	return ability_s(this - bsmeta<abilityi>::elements);
}

int	abilityi::getbonus(int v) const {
	if(!v)
		return 0;
	if(v > 0)
		v = bonus_base + bonus_multiplier*v;
	else
		v = bonus_multiplier*v;
	return v;
}
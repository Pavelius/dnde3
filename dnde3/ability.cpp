#include "main.h"

abilityi bsmeta<abilityi>::elements[] = {{"Strenght", "����", "��", "����", "��������", "�������", "������", {}, 0, 1, "%+1i"},
{"Dexterity", "��������", "��", "��������", "�����������", "�����", "��������", {}, 0, 1, "%+1i"},
{"Constitution", "������������", "��", "������������", "�������", "��������", "��������", {}, 0, 1, "%+1i"},
{"Intellegence", "���������", "��", "����������", "��������", "�����", "������", {}, 0, 1, "%+1i"},
{"Wisdow", "��������", "��", "��������", "���������", "������", "�������", {}, 0, 1, "%+1i"},
{"Charisma", "�������", "��", "�������", "��������", "�������������", "���������", {}, 0, 1, "%+1i"},
//
{"Attack", "���� ���������", "��", "���������", "������������", "�������", "������", {Dexterity, Multiply2, -20}, 0, 1, "%+1i%%"},
{"Damage", "����", "��", "�����", "�������", "�������", "������", {Strenght, Divide2, -5}, 0, 1, "%+1i"},
{"Pierce", "������������", "��", "����������", "����������", "�������", "������", {}, 0, 1, "%+1i"},
{"Protection", "������", "��", "������", "����������", "����������", "������", {Acrobatics, Divide4}, 0, 5, "%+1i%%"},
{"Armor", "�����", "��", "�����", "", "���������", "��������", {}, 0, 1, "%+1i"},
{"Deflect", "���������", "��", "���������", "����������", "����������", "��������", {}, 0, 1, "%+1i%%"},
{"Speed", "��������", "��", "��������", "����������", "�������", "���������", {95, Athletics, Divide10, Dexterity, Divide2}, 0, 1, "%+1i"},
{"Visibility", "�����", "��", "������", "�������", "�����", "����", {6}, 0, 1, "%+1i"},
//
{"Resist Acid", "������������� �������", "", "������ �� �������", "���������� � �������", "����������", "��������", {}, 20, 10, "%+1i%%"},
{"Resist Charm", "������������� �����", "", "������ �� �����", "���������� � �����", "����������", "��������", {}, 20, 10, "%+1i%%"},
{"Resist Ice", "������������� ������", "", "������ �� ������", "���������� � ������", "����������", "��������", {}, 20, 10, "%+1i%%"},
{"Resist Lighting", "������������� �������������", "", "������ �� ������", "���������� � ������", "����������", "��������", {}, 20, 10, "%+1i%%"},
{"Resist Fire", "������������� ����", "", "������ �� ����", "���������� � ����", "����������", "��������", {}, 20, 10, "%+1i%%"},
{"Resist Paralize", "������������� ��������", "", "������ �� ��������", "���������� � ��������", "����������", "��������", {}, 20, 10, "%+1i%%"},
{"Resist Poison", "������������� ���", "", "������ �� ���", "���������� � ���", "����������", "��������", {}, 20, 10, "%+1i%%"},
{"Resist Water", "������� �����", "", "������ �� ����", "���������� � ����", "����������", "��������", {}, 20, 10, "%+1i%%"},
//
{"Level", "�������", "", "�����", "", "�������", "������", {}, 0, 0},
{"LifePoints", "�����", "", "��������", "��������� ��������", "�����", "����", {Constitution}, 0, 5, "%+1i"},
{"LifeRate", "�������������� �����", "", "�����������", "����������", "�����", "����", {Healing}, 0, 40, "%+1i"},
{"ManaPoints", "����", "", "����", "��������� ����", "�����", "����", {Wisdow}, 0, 5, "%+1i"},
{"ManaRate", "�������������� ����", "", "�������������", "��������", "�����", "����", {Concetration}, 0, 50, "%+1i"},
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
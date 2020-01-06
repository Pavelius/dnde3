#include "main.h"

abilityi bsmeta<abilityi>::elements[] = {{"Strenght", "����", "��", "����", "��������"},
{"Dexterity", "��������", "��", "��������", "�����������"},
{"Constitution", "������������", "��", "������������", "�������"},
{"Intellegence", "���������", "��", "����������", "��������"},
{"Wisdow", "��������", "��", "��������", "���������"},
{"Charisma", "�������", "��", "�������", "��������"},
//
{"AttackMelee", "���� �������� �����", "", "���������", "������������", {Strenght, Multiply2, -20}},
{"AttackRanged", "���� �������� ��������", "", "��������", "����������", {Dexterity, Multiply2, -20}},
{"DamageMelee", "���� ����������", "", "�����", "�������", {Strenght, Divide2, -5}},
{"DamageRanged", "���� ���������", "", "�����", "�������"},
{"Pierce", "������������", "", "����������", "����������"},
{"Deflect", "���������", "", "���������", "����������", {Acrobatics, Divide4}},
{"Armor", "�����", "", "�����", ""},
{"Speed", "��������", "", "��������", "����������", {95, Athletics, Divide10, Dexterity, Divide2}},
{"Visibility", "�����", "", "������", "�������", {6}},
//
{"Level", "�������", "", "�����", ""},
{"LifePoints", "�����", "", "��������", "", {Constitution}},
{"LifeRate", "�������������� �����", "", "���������", "", {Healing}},
{"ManaPoints", "����", "", "�����������", "", {Wisdow}},
{"ManaRate", "�������������� ����", "", "�����������", "", {Concetration}},
};
assert_enum(ability, ManaRate);

ability_s abilityi::getid() const {
	return ability_s(this - bsmeta<abilityi>::elements);
}
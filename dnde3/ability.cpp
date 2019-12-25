#include "main.h"

abilityi bsmeta<abilityi>::elements[] = {{"Strenght", "����", "��", "����", "��������"},
{"Dexterity", "��������", "��", "��������", "�����������"},
{"Constitution", "������������", "��", "������������", "�������"},
{"Intellegence", "���������", "��", "����������", "��������"},
{"Wisdow", "��������", "��", "��������", "���������"},
{"Charisma", "�������", "��", "�������", "��������"},
//
{"AttackMelee", "���� �������� �����", "", "���������", "������������", {40}},
{"AttackRanged", "���� �������� ��������", "", "��������", "����������", {40, Dexterity}},
{"DamageMelee", "���� ����������", "", "�����", "�������", {Strenght, Divide4, -2}},
{"DamageRanged", "���� ���������", "", "�����", "�������"},
{"Pierce", "������������", "", "����������", "����������"},
{"Deflect", "���������", "", "���������", "", {Acrobatics, Divide4}},
{"Armor", "�����", "", "�����", ""},
{"Speed", "��������", "", "��������", "����������"},
{"Visibility", "�����", "", "������", "�������"},
//
{"Level", "�������", "", "�����", ""},
{"LifePoints", "�����", "", "��������", "", {Constitution}},
{"LifeRate", "�������������� �����", "", "�����", ""},
{"ManaPoints", "����", "", "�����������", "", {Intellegence, Concetration, Divide4}},
{"ManaRate", "�������������� ����", "", "�����������", ""},
};
assert_enum(ability, ManaRate);

ability_s abilityi::getid() const {
	return ability_s(this - bsmeta<abilityi>::elements);
}
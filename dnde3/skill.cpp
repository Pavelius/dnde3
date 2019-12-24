#include "main.h"

static const char* talk_subjects[] = {"������", "��������", "������", "�������", "�������", "�������", "�������", "���� ������"};
static const char* talk_object[] = {"���������", "��������� ������", "��������� ��������", "����", "�������� ������", "���������� ����������"};
static const char* talk_location[] = {"����������", "������", "�������", "�������", "����"};
static const char* talk_games[] = {"������", "�����", "���������", "������"};

skilli bsmeta<skilli>::elements[] = {{"��������", "��������", {Charisma, Intellegence}},
{"����", "������", {Charisma, Dexterity}},
{"����������", "����������", {Charisma, Wisdow}},
//
{"����������", "����������", {Dexterity, Dexterity}},
{"��������������", "��������������", {Wisdow, Dexterity}},
{"��������", "��������", {Strenght, Dexterity}},
{"��������", "��������", {Dexterity, Dexterity}},
{"������������", "�����������", {Wisdow, Wisdow}},
{"����������� �������", "�������", {Dexterity, Intellegence}},
{"������� �����", "�����", {Wisdow, Intellegence}},
{"��������� � ����", "����������", {Dexterity, Dexterity}},
{"������� �����", "������", {Dexterity, Intellegence}},
{"�������� �������", "���������", {Dexterity, Charisma}},
{"�������", "�������", {Intellegence, Intellegence}},
{"�����", "������", {Dexterity, Charisma}},
{"���������� ����", "���������", {Intellegence, Intellegence}},
{"�������� ����", "�������� ���", {Charisma, Dexterity}},
{"�������", "�������", {Intellegence, Intellegence}},
{"�������", "�������", {Wisdow, Intellegence}},
{"�����������", "������ � ������", {Intellegence, Intellegence}},
{"���������� ����", "����������� ����", {Strenght, Constitution}},
{"���� ������", "���� ������", {Dexterity, Constitution}},
{"��������� ����", "���������� ����", {Strenght, Intellegence}},
{"���������", "���������", {Wisdow, Constitution}},
{"��������", "��������", {Strenght, Constitution}},
//
{"�������� �����", "�������� �� ����", {Dexterity, Dexterity}},
{"�������� �����", "�������� �� �����", {Strenght, Dexterity}},
{"�������� �������", "�������� �� �������", {Strenght, Constitution}},
{"�������� ����� ��������", "�������� ������", {Strenght, Dexterity}},
//
{"������������� �������", "�������", {Dexterity, Constitution}},
{"������������� �����", "������� � �����", {Wisdow, Wisdow}},
{"������������� ������", "������", {Constitution, Strenght}},
{"������������� �������������", "������", {Dexterity, Dexterity}},
{"������������� ����", "����", {Constitution, Dexterity}},
{"������������� ��������", "��������", {Constitution, Dexterity}},
{"������������� ���", "���", {Constitution, Constitution}},
{"������� �����", "����", {Strenght, Constitution}},
};
assert_enum(skill, ResistWater);

skill_s	skilli::getid() const {
	return skill_s(this - bsmeta<skilli>::elements);
}
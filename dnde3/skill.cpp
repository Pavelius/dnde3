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
{"�������", "�������", {Dexterity, Strenght}},
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
{"��������� ���", "��������", {Wisdow, Constitution}},
{"�������������", "�����������", {Wisdow, Intellegence}},
{"�����������", "������ � ������", {Intellegence, Intellegence}},
{"���������� ����", "����������� ����", {Strenght, Constitution}},
{"���� ������", "���� ������", {Dexterity, Constitution}},
{"��������� ����", "���������� ����", {Strenght, Intellegence}},
{"���������", "���������", {Wisdow, Constitution}},
{"��������", "��������", {Strenght, Constitution}},
//
{"�������� �����", "�������� �� ����", {Dexterity, Dexterity}, {3, 30}},
{"�������� �����", "�������� �� �����", {Strenght, Dexterity}, {4, 20}},
{"�������� �������", "�������� �� �������", {Strenght, Constitution}, {5, 15}},
{"�������� ����� ��������", "�������� ������", {Strenght, Dexterity}, {5, 30}},
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

const char* skilli::getusetext() const {
	switch(getid()) {
	case Healing: return "������������ ������������� �������������� 1 ���� ����� �� ������������ ���������� �������.";
	case Concetration: return "������������ ������������� �������������� 1 ���� ���� �� ������������ ���������� �������.";
	default:
		if(isweapon())
			return "����� �������� ������� ������ �� ���� ���������, ��������� ����, �������� ���������� ����� � ������������ ������������� ��� ��� ���������.";
		return "���� ����� ������������ �������������.";
	}
}
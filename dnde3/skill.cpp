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
{"����������� �������", "�������", {Dexterity, Intellegence}, {Trap}},
{"���� � �������� �����", "������ ������", {Dexterity, Intellegence}},
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
{"�������� �����", "�������� �� ����", {Dexterity, Dexterity}, {3, 30, 30}},
{"�������� �����", "�������� �� �����", {Strenght, Dexterity}, {4, 20, 25}},
{"�������� �������", "�������� �� �������", {Strenght, Constitution}, {5, 15, 30}},
{"�������� ��������� �������", "���������� ������", {Strenght, Strenght}, {4, 10, 35}},
{"�������� ����� ��������", "�������� ������", {Strenght, Dexterity}, {5, 30, 15}},
};
assert_enum(skill, LastSkill);

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
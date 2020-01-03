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
{"�������� �����", "�������� �� ����", {Dexterity, Dexterity}, {40, 2}},
{"�������� �������", "�������� ������", {Strenght, Strenght}, {30, 1}},
{"�������� �����", "�������� �� �����", {Strenght, Dexterity}, {35, 1}},
{"�������� �������", "�������� �� �������", {Strenght, Constitution}, {30, 1}},
{"�������� ������� �������", "����������", {Dexterity, Dexterity}, {40, 2}},
{"�������� ��������� �������", "�������� ��������� �������", {Strenght, Strenght}, {30, 1}},
{"�������� �������", "�������� �������", {Strenght, Dexterity}, {40, 2}},
{"���������� ���", "����������� ���", {Strenght, Dexterity}, {60, 3}},
{"�������� ����� ��������", "�������� ������", {Strenght, Dexterity}, {40, 2}},
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
	default: return "���� ����� ������������ �������������.";
	}
}

int	skilli::weaponi::get(int v) const {
	auto r = base;
	switch(divider) {
	case 0: break;
	case 1: r += (2 * v) / 3; break;
	default: r += v / divider; break;
	}
	return r;
}
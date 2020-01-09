#include "main.h"

static const char* talk_subjects[] = {"������", "��������", "������", "�������", "�������", "�������", "�������", "���� ������"};
static const char* talk_object[] = {"���������", "��������� ������", "��������� ��������", "����", "�������� ������", "���������� ����������"};
static const char* talk_location[] = {"����������", "������", "�������", "�������", "����"};
static const char* talk_games[] = {"������", "�����", "���������", "������"};

skilli bsmeta<skilli>::elements[] = {{"��������", "��������", {Charisma, Intellegence}, {}, {Creature, {NotYou}, SingleTarget, Close}},
{"����", "������", {Charisma, Dexterity}, {}, {Creature, {NotYou}, SingleTarget, Close}},
{"����������", "����������", {Charisma, Wisdow}, {}, {Creature, {NotYou}, SingleTarget, Close}},
//
{"����������", "����������", {Dexterity, Dexterity}},
{"��������������", "��������������", {Wisdow, Dexterity}},
{"��������", "��������", {Strenght, Dexterity}, {}, {Object, {}, SingleTarget, Close}},
{"��������", "��������", {Dexterity, Dexterity}, {}, {Creature, {NotYou, Friends}, SingleTarget, Close}},
{"�������", "�������", {Dexterity, Strenght}},
{"������������", "�����������", {Wisdow, Wisdow}},
{"����������� �������", "�������", {Dexterity, Intellegence}, {}, {Object, {}, SingleTarget, Close}},
{"���� � �������� �����", "������ ������", {Dexterity, Intellegence}},
{"������� �����", "�����", {Wisdow, Intellegence}},
{"��������� � ����", "����������", {Dexterity, Dexterity}, {}, {Creature}},
{"������� �����", "������", {Dexterity, Intellegence}, {}, {Object, {}, SingleTarget, Close}},
{"�������� �������", "���������", {Dexterity, Charisma}, {}, {Creature, {NotYou}, SingleTarget, Close}},
{"�������", "�������", {Intellegence, Intellegence}},
{"�����", "������", {Dexterity, Charisma}},
{"���������� ����", "���������", {Intellegence, Intellegence}},
{"�������� ����", "�������� ���", {Charisma, Dexterity}, {}, {Creature, {Friends}, SingleTarget, Close}},
{"�������", "�������", {Intellegence, Intellegence}, {}, {Object, {}, SingleTarget, Reach}},
{"��������� ���", "��������", {Wisdow, Constitution}},
{"�������������", "�����������", {Wisdow, Intellegence}, {}, {Object, {}, SingleTarget, Close}},
{"�����������", "������ � ������", {Intellegence, Intellegence}},
{"���������� ����", "����������� ����", {Strenght, Constitution}, {}, {Object, {}, SingleTarget, Close}},
{"���� ������", "���� ������", {Dexterity, Constitution}},
{"��������� ����", "���������� ����", {Strenght, Intellegence}, {}, {Object, {}, SingleTarget, Close}},
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
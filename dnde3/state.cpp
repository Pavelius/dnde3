#include "main.h"

statei bsmeta<statei>::elements[] = {{"Anger", "����", "������", true, "%����� ��������%���.", "%����� ��������%� �������."},
{"Dazzled", "���������", "�����������", true, 0, "%����� �������� �� ��������������."},
{"Drunken", "������", "���������", true, "%����� �������%� � �����.", "%����� ������� ����������%�."},
{"Fear", "����", "������", true, "%����� �������%� �������� ����.", "%����� ������� ��������%���."},
{"Friendly", "������������", "������", false},
{"Hostile", "���������", "������", true},
{"Invisible", "�������", "�����������", false, "�������� %����� �����%�� �� ����.", "%����� ������%��� �� ��������."},
{"Poisoned", "��������", "����������", true, "%����� �����������%� ���� ������� ����.", "%����� �����������%� ���� ������� �����."},
{"Sick", "�����", "�������", true, "%����� �����������%� ���� ������� ����.", "%����� �����������%� ���� ������� �����."},
{"Sleeped", "����", "���", true, "%����� �������%���."},
{"Wounded", "�����", "����������", true, 0, "������������ � %����� ������������."},
};
assert_enum(state, LastState);
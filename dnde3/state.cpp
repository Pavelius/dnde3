#include "main.h"

statei bsmeta<statei>::elements[] = {{"�����������", "������� ������", {Friendly}},
{"���������", "�����������", {Hostile}, 0, "%����� �������� �� ��������������."},
{"������", "���������", {Hostile}, "%����� �������%� � �����.", "%����� ������� ����������%�."},
{"����", "������", {Hostile}, "%����� �������%� �������� ����.", "%����� ������� ��������%���."},
{"������������", "������", {Friendly}},
{"���������", "������", {Hostile}},
{"�������", "�����������", {}, "�������� %����� �����%�� �� ����.", "%����� ������%��� �� ��������."},
{"��������", "���", {Hostile}, "%����� ������������%� ����������.", "%����� �����������%� ���� �����."},
{"�����", "�������", {Hostile}, "%����� �������%�.", "%����� �����������%� ���� ������� �����."},
{"�������", "���������", {Hostile}, "%����� ������%��� �� ��������.", "%����� ������%� � �������."},
{"��������", "����������", {Hostile}},
{"�����", "����������", {Hostile}, 0, "������������ � %����� ������������."},
};
assert_enum(state, LastState);
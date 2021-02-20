#include "main.h"

BSDATA(dietyi) = {
	{"����", "�����", Male, {Mace, LeatherArmor, BracersLeather, BracersIron}, "������ ����� �� �������� ������� ������� ����"},
	{"����", "�����", Male, {Dagger, LeatherArmor}, "������ ����� � ��������� ������� ������ �����"},
	{"�����", "������", Male, {Spear, ChainMail}, "�������� ����"},
	{"����", "�����", Male, {HammerWar, ChainMail, PlateMail}, "�������� ������"},
	{"������", "������", Female, {Staff, Wand1, Wand2, Wand3, Wand4, Wand5}, "���� �� ���� ����-����� ����� � ������� ����� ������ �� ������"},
	{"������", "�������", Male, {AxeBattle, SwordLong, SwordTwoHanded, SwordShort, Shield}, "���������� ����������� ��� �� ������� ������� ����"},
	{"���", "����", Male, {SwordLong}, "���� ����������� �� �����"},
};
assert_enum(dietyi, GodTyr)

void dietyi::act(const char* format, ...) const {
	actv(sb, format, xva_start(format));
}

void dietyi::actv(stringbuilder& st, const char* format, const char* format_param) const {
	string sb = st;
	sb.name = name;
	sb.gender = gender;
	if(true)
		sb.addsep(' ');
	sb.addv(format, format_param);
	st = sb;
}
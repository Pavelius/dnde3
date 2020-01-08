#include "main.h"

static void choose_children(stringbuilder& sb, manual& mn, answeri& an) {
	for(auto& e : bsmeta<manual>()) {
		if(e.parent == mn.value)
			an.add((int)&e, e.getname());
	}
}

static void ability_skills(stringbuilder& sb, manual& mn, answeri& an) {
	skilla source;
	for(auto& e : bsmeta<skilli>()) {
		if(e.isresist())
			continue;
		if(e.abilities[0] == mn.value.value || e.abilities[1] == mn.value.value)
			source.add(e.getid());
	}
	if(!source.count)
		return;
	source.sort();
	sb.addn("[������]: ");
	auto p = sb.get();
	for(auto i : source) {
		if(p[0])
			sb.add(", ");
		sb.add(bsmeta<skilli>::elements[i].name);
	}
	sb.add(".");
}

static manual::proc ability_list[] = {choose_children};
static manual::proc ability_procs[] = {ability_skills};
static manual::proc manual_list[] = {choose_children};
static manual::proc skill_list[] = {choose_children};
manual bsmeta<manual>::elements[] = {{NoVariant, Variant, "������", "�������� ���������� ���������� �� �������� ����.", manual_list},
{Variant, Ability, "����������� ����������", "������ �������� ����� [6] ������� ������������, ������� ��� �������������.", ability_list},
{Ability, Strenght, 0, "���� �������� ����� ���������� ����, ���������� ��������� � ������������ ���������. ��� ����������� �������� ������� ��� ������, ��� ��� ��� ������ ���� ��������, ����� ������ ������� � ������� ������� �������.", ability_procs},
{Ability, Dexterity, 0, "�������� ������������� ��������� ���������� ����������� ���������, ������� ����������� ��������, ����������, �������� �������, �������� � ������������� �������. �������� ������ �� ������� ��������� �� ��������� ��� �������������, �� ��� ������ ���������� � ����������� ������� � ������, � ����� �� ��� ������������ �� ������.", ability_procs},
{Ability, Constitution, 0, "������������ ��������� ������������� ��� ���������� ������, ������������� ����������, �������� � ������������ � �����������, �������� � ������������.", ability_procs},
{Ability, Intellegence, 0, "��������� ������������� ������, ���������������� ��������� � ��� ����������� � ��������, ������� ������� ��� ����, ��� ����� �������� �������� ������.", ability_procs},
{Ability, Wisdow, 0, "�������� ������������� ������������ ��������������, ����������������, ���� ����, �������� ������ � �������� ���������.", ability_procs},
{Ability, Charisma, 0, "������� ������������� ����������� ��������� � ���������, ��� ������ ����������������� � ��������� ��������. ��� �� �������� ���������� �����������������, ���� ���, ����������, ����� ������ ����. ������� ����� ��� ���� ����������, � �������� ��� ���, ��� ���������� ����� ���� � ���������� �����������, �������� �������, ������� � ��������� ���������.", ability_procs},
{Skill, Acrobatics, 0, "��������� [++1%%] � ����� ��������� �� ��������� �� ������ 4 �������."},
{Skill, Riding, 0, "��������� ������������� ������� �� ���������� �����."},
{Skill, FindWeakness, 0, "���� ������� � �������� ����� ����� � ������� [++30%%] �����, � ����� � ����������� �� ���� ������ �������������� ������:\n* ������� ������ ��������� +40%% ����� ������ +30%% � ���������� �����.\n* ������� ������ ���������� ���������� �������� ������\n* ������� ������ ������� ������������� ���� ����� �������� �������� ������ ������ � ���� ����� ������ ������.\n�� ����� ������� � �������� ����� ���������� ���������� ����� ������."},
{Variant, TrapVariant, "�������", "� ����� ���������� ��������� ��������� �������. ��-��������� ��� ������� ������ � ����� ����� ���������� �� ������ � �������� �������� ������������ ������ [��������������] c ������� [--20%%]. ���� ���� ������ ����� ������������ ������� � �������� ������� �����. ����� ����������� ������ ������� � ��� ���������� �������.\n��� ��������� ���������� �� ������� ����� �������� ���� ��������������, �� � ������� [++15%%]. ���� ���� ������ ����� �������� ������� ������� (����, ������� ��, ����� �� ��������)."},
{Variant, Skill, "������ ����������", "������ �������� ����� ������������ ���������� �������. �� ����� ������� �� ������ � ����.", skill_list},
};
DECLFULL(manual);

const char* manual::getname() const {
	if(name)
		return name;
	return value.getname();
}

void gamei::help() {
	adat<manual*, 32> pages;
	auto pm = bsmeta<manual>::elements;
	while(pm) {
		char temp[2048]; stringbuilder sb(temp); answeri an;
		sb.add(pm->text);
		for(auto p : pm->procs)
			p(sb, *pm, an);
		if(pages.getcount() == pages.getmaximum())
			pages.remove(0);
		pages.add(pm);
		pm = (manual*)an.dialogv(true, pm->getname(), sb);
		if(!pm && pages.getcount()>1) {
			pages.remove(pages.getcount() - 1);
			pm = pages[pages.getcount() - 1];
			pages.remove(pages.getcount() - 1);
		}
	}
}
#include "main.h"

static int compare_skills(const void* p1, const void* p2) {
	auto e1 = *((skill_s*)p1);
	auto e2 = *((skill_s*)p2);
	return strcmp(bsmeta<skilli>::elements[e1].name, bsmeta<skilli>::elements[e2].name);
}

static void choose_children(stringbuilder& sb, manual& mn, answeri& an) {
	for(auto& e : bsmeta<manual>()) {
		if(e.parent == mn.value)
			an.add((int)&e, e.getname());
	}
}

static void ability_skills(stringbuilder& sb, manual& mn, answeri& an) {
	adat<skill_s, LastResist + 1> source;
	for(auto& e : bsmeta<skilli>()) {
		if(e.abilities[0] == mn.value.value || e.abilities[1] == mn.value.value)
			source.add(e.getid());
	}
	qsort(source.data, source.count, sizeof(source.data[0]), compare_skills);
	if(!source.count)
		return;
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
manual bsmeta<manual>::elements[] = {{Variant, Ability, "����������� ����������", "������ �������� ����� **6** ������� ������������, ������� ��� �������������.", ability_list},
{Ability, Strenght, 0, "���� �������� ����� ���������� ����, ���������� ��������� � ������������ ���������. ��� ����������� �������� ������� ��� ������, ��� ��� ��� ������ ���� ��������, ����� ������ ������� � ������� ������� �������.", ability_procs},
{Ability, Dexterity, 0, "�������� ������������� ��������� ���������� ����������� ���������, ������� ����������� ��������, ����������, �������� �������, �������� � ������������� �������. �������� ������ �� ������� ��������� �� ��������� ��� �������������, �� ��� ������ ���������� � ����������� ������� � ������, � ����� �� ��� ������������ �� ������.", ability_procs},
{Ability, Constitution, 0, "������������ ��������� ������������� ��� ���������� ������, ������������� ����������, �������� � ������������ � �����������, �������� � ������������.", ability_procs},
{Ability, Intellegence, 0, "��������� ������������� ������, ���������������� ��������� � ��� ����������� � ��������, ������� ������� ��� ����, ��� ����� �������� �������� ������.", ability_procs},
{Ability, Wisdow, 0, "�������� ������������� ������������ ��������������, ����������������, ���� ����, �������� ������ � �������� ���������.", ability_procs},
{Ability, Charisma, 0, "������� ������������� ����������� ��������� � ���������, ��� ������ ����������������� � ��������� ��������. ��� �� �������� ���������� �����������������, ���� ���, ����������, ����� ������ ����. ������� ����� ��� ���� ����������, � �������� ��� ���, ��� ���������� ����� ���� � ���������� �����������, �������� �������, ������� � ��������� ���������.", ability_procs},
{Skill, Acrobatics, 0, "��������� **1%%** � ����� ��������� �� ��������� �� ������ 4 �������. ����� ��������� ������������� � �������."},
{Skill, Riding, 0, "��������� ������������� ������� �� ���������� �����."},
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
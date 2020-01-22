#include "main.h"

static void choose_children(stringbuilder& sb, manual& mn, answeri& an) {
	for(auto& e : bsmeta<manual>()) {
		if(e.parent == mn.value)
			an.add((int)&e, e.getname());
	}
}

static void add_children(stringbuilder& sb, manual& mn, answeri& an) {
	for(auto& e : bsmeta<manual>()) {
		if(e.parent == mn.value) {
			sb.addn("[%1]: ", e.name);
			sb.adds(e.text);
		}
	}
}

static void race_ability(stringbuilder& sb, manual& mn, answeri& an) {
	if(mn.value.type != Race)
		return;
	sb.addn("[��������]: ");
	auto& ei = bsmeta<racei>::elements[mn.value.value];
	for(auto i = Strenght; i <= Charisma; i = ability_s(i+1)) {
		if(i!=Strenght)
			sb.add(", ");
		sb.add("%1i %2", ei.abilities[i-Strenght], getstr(i));
	}
	sb.add(".");
}

static void ability_example(stringbuilder& sb, manual& mn, answeri& an) {
	if(mn.value.type != Ability)
		return;
	sb.addn("[��������]: ");
	auto count = 0;
	for(auto i = 5; i <= 20; i++) {
		for(auto& e : bsmeta<racei>()) {
			if(e.abilities[mn.value.value] != i)
				continue;
			if(count > 0)
				sb.add(", ");
			sb.add("%1i %-2", i, e.name);
			count++;
			break;
		}
	}
	sb.add(".");
}

static void ability_skills(stringbuilder& sb, manual& mn, answeri& an) {
	skilla source;
	for(auto& e : bsmeta<skilli>()) {
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
static manual::proc ability_procs[] = {ability_example, ability_skills, add_children};
static manual::proc general_list[] = {choose_children};
static manual::proc manual_list[] = {choose_children};
static manual::proc skill_list[] = {choose_children};
static manual::proc state_list[] = {choose_children};
static manual::proc race_ability_list[] = {race_ability};
manual bsmeta<manual>::elements[] = {{NoVariant, Variant, "������", "�������� ���������� ���������� �� �������� ����.", manual_list},
{Variant, Ability, "����������� ����������", "������ �������� ����� [6] ������� ������������, ������� ��� �������������.", ability_list},
{Ability, Strenght, 0, "���� �������� ����� ���������� ����, ���������� ��������� � ������������ ���������. ��� ����������� �������� ������� ��� ������, ��� ��� ��� ������ ���� ��������, ����� ������ ������� � ������� ������� �������.", ability_procs},
{Ability, Dexterity, 0, "�������� ������������� ��������� ���������� ����������� ���������, ������� ����������� ��������, ����������, �������� �������, �������� � ������������� �������. �������� ������ �� ������� ��������� �� ��������� ��� �������������, �� ��� ������ ���������� � ����������� ������� � ������, � ����� �� ��� ������������ �� ������.", ability_procs},
{Ability, Constitution, 0, "������������ ��������� ������������� ��� ���������� ������, ������������� ����������, �������� � ������������ � �����������, �������� � ������������.", ability_procs},
{Ability, Intellegence, 0, "��������� ������������� ������, ���������������� ��������� � ��� ����������� � ��������, ������� ������� ��� ����, ��� ����� �������� �������� ������.", ability_procs},
{Ability, Wisdow, 0, "�������� ������������� ������������ ��������������, ����������������, ���� ����, �������� ������ � �������� ���������.", ability_procs},
{Ability, Charisma, 0, "������� ������������� ����������� ��������� � ���������, ��� ������ ����������������� � ��������� ��������. ��� �� �������� ���������� �����������������, ���� ���, ����������, ����� ������ ����. ������� ����� ��� ���� ����������, � �������� ��� ���, ��� ���������� ����� ���� � ���������� �����������, �������� �������, ������� � ��������� ���������.", ability_procs},
{Intellegence, 0, "�������������� ����", "�������� �������� [++1%%] ����� �� ������ ������� ���������� ����� **10** � [--1%%] ����� �� ������ ������� ���������� ����� 10."},
{Intellegence, 0, "��������� �������", "�������� �������� **��������/2** ��������� �������� �� ������ ������."},
{Intellegence, 0, "�������", "�� ����� ������� ��� ��������� ��������� � ��������� ��� ��������� ������ �������� **����������2-20**."},
{Variant, Trap, "�������", "� ����� ���������� ��������� ��������� �������. ��-��������� ��� ������� ������ � ����� ����� ���������� �� ������ � �������� �������� ������������ ������ [��������������] c ������� [--20%%]. ���� ���� ������ ����� ������������ ������� � �������� ������� �����. ����� ����������� ������ ������� � ��� ���������� �������.\n��� ��������� ���������� �� ������� ����� �������� ���� ��������������, �� � ������� [++15%%]. ���� ���� ������ ����� �������� ������� ������� (����, ������� ��, ����� �� ��������)."},
{Variant, Skill, "������ ����������", "������ �������� ����� ������������ ���������� �������. �� ����� ������� �� ������ � ����.", skill_list},
{Variant, State, "��������� ����������", "�� ����� ����������� ��������� ������������ � ���������� ����������.", state_list},
{Skill, Acrobatics, 0, "��������� [++1%%] � ����� ��������� �� ��������� �� ������ 4 �������."},
{Skill, Alchemy, 0, "��������� ���������� �����. ����� ����� ������������� ����� �� ��������."},
{Skill, Backstabbing, 0, "����������� � ����� ��������� � ����������� ���� � ��������� ������ ���������� ������, ���� ����� �������� �� ���������� ���������. ����� ����� ����� �� ����������� �������."},
{Skill, Riding, 0, "��������� ������������� ������� �� ���������� �����."},
{Skill, FindWeakness, 0, "���� ������� � �������� ����� ����� � ������� [++30%%] �����, � ����� � ����������� �� ���� ������ �������������� ������:\n* ������� ������ ��������� +40%% ����� ������ +30%% � ���������� �����.\n* ������� ������ ���������� ���������� �������� ������\n* ������� ������ ������� ������������� ���� ����� �������� �������� ������ ������ � ���� ����� ������ ������.\n�� ����� ������� � �������� ����� ���������� ���������� ����� ������."},
{Skill, Healing, 0, "�������� �������������� �������� ���������."},
{Skill, Concetration, 0, "�������� �������������� ���� ���������."},
{Skill, HearNoises, 0, "���� �������� �������� ���������� ����� � ������� ��� �������."},
{Skill, MoveSilently, 0, "���� ��������� ���� ��������� ������������ � ��������� ���������."},
{Skill, HideInShadow, 0, "��������� ���������� � ���������� �����������. �� ������ ��������� �� ���� � ���� �� ����."},
{State, Sick, 0, "������� �������� �� ��������������� ���� ����� ��������� �����. ��� ������� ���������� ����������������� ����� ������� �� ���."},
{State, Poisoned, 0, "����������� �������� ������ �� ������ ���� ����� ������ 5 ����� ���� �� �������� ������������� ���. ��� ������� ������������� ����������� ������� ���."},
{Variant, Race, "����", "�� ��������� ���� ������� ��������� �������� � ������� ���������� ��������� ����������� �� [--2] �� [++2].", general_list},
{Race, Human, 0, "���� - ����� ���������������� ���� � ������� ������������. �� ������� ������������ ����� ����� ���������� ������ ��, ��� ��� ����� ������� ����� ����� � �������������. � ������� ��� ����� ����� 70 ��� � ��������� ����� 180 ��.", race_ability_list},
{Race, Dwarf, 0, "������ � ������, ���������� �����, �� ����� ������ �� ������� � ������. � ������� ��� �� 4 �� 4 1/2 ����� ������. � ��� ������� ����, ����� ����� � ����� ������. � �������� ������ ����� 350 ���. ������ ������ ������ � ��������������. ��� ���������� ������ �������, �� ���� ������� ����. ��� ������� � �������. ��� ����� ����, ���, �� � ����� ������� �������. �� ������� ������, ������, ����������� ������, � ��������� ������. ��� ����� ���������, �������� ������, � ������������ ����� (����� ��������). ������� �������� ����� � �� �������� ����. ��� �� ������� ����� ������ � ���������� ����� ��������� � ����� � ��������. �� ������ ���������� ���� �� ��������� ������ �� ������� ��� ������ ������� ������� (���� �� ���� ��� ������� �� ���������� ����������), � ��� ��������� � ����� ����������� � ������������� � ���� ���������.��� ������ ��������� � ����� � ���� �������� � ���, �� ������������ ����, ������� ���������� � �������� ��������, ������ ��� ���������� ����.", race_ability_list},
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
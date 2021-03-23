#include "main.h"

static void choose_children(stringbuilder& sb, manual& mn, answeri& an) {
	for(auto& e : bsdata<manual>()) {
		if(e.parent == mn.value)
			an.add((int)&e, e.getname());
	}
}

static void choose_children_sort(stringbuilder& sb, manual& mn, answeri& an) {
	choose_children(sb, mn, an);
	an.sort();
}

static void add_children(stringbuilder& sb, manual& mn, answeri& an) {
	for(auto& e : bsdata<manual>()) {
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
	auto& ei = bsdata<racei>::elements[mn.value.value];
	for(auto i = Strenght; i <= Charisma; i = ability_s(i + 1)) {
		if(i != Strenght)
			sb.add(", ");
		sb.add("%1i %2", ei.abilities[i - Strenght], getstr(i));
	}
	sb.add(".");
}

static bool have(const varianta& source, variant v) {
	for(auto e : source) {
		if(e == v)
			return true;
	}
	return false;
}

static void race_skills(stringbuilder& sb, manual& mn, answeri& an) {
	auto count = 0;
	if(mn.value.type == Skill) {
		const char* header = "������������";
		auto skill = (skill_s)mn.value.value;
		for(auto& ei : bsdata<racei>()) {
			if(!ei.is(skill))
				continue;
			if(!count)
				sb.addn("[%1]: ", header);
			else
				sb.add(", ");
			sb.add("%+1", ei.name);
			count++;
		}
		for(auto& ei : bsdata<classi>()) {
			if(!have(ei.bonuses, skill))
				continue;
			if(!count)
				sb.addn("[%1]: ", header);
			else
				sb.add(", ");
			sb.add("%+1", ei.name);
			count++;
		}
	} else if(mn.value.type == Race) {
		const char* header = "������";
		auto race = (race_s)mn.value.value;
		for(auto v : bsdata<racei>::elements[race].bonuses) {
			if(!count)
				sb.addn("[%1]: ", header);
			else
				sb.add(", ");
			sb.add("%+1", v.getname());
			count++;
		}
	}
	if(count)
		sb.add(".");
}

static void item_example(stringbuilder& sb, manual& mn, answeri& an) {
	auto count = 0;
	if(mn.value.type == Skill) {
		auto id = (skill_s)mn.value.value;
		const char* header = "���������";
		for(auto& ei : bsdata<itemi>()) {
			if(ei.skill != id)
				continue;
			if(!count)
				sb.addn("[%1]: ", header);
			else
				sb.add(", ");
			sb.add("%+1", ei.name);
			count++;
		}
	}
}

static void weapon_skill_ability(stringbuilder& sb, manual& mn, answeri& an) {
	auto count = 0;
	if(mn.value.type == Skill) {
		auto id = (skill_s)mn.value.value;
		auto& ei = bsdata<skilli>::elements[id];
		if(ei.weapon.attack)
			sb.addn("* ���� +1%% � ����� ������� �� ������ [%1i%%] ������.", ei.weapon.attack);
		if(ei.weapon.damage)
			sb.addn("* ���� +1 � ������������ �� ������ [%1i%%] ������.", ei.weapon.damage);
		if(ei.weapon.speed)
			sb.addn("* ���� +1 � �������� ����� �� ������ [%1i%%] ������.", ei.weapon.speed);
	}
}

static void ability_example(stringbuilder& sb, manual& mn, answeri& an) {
	if(mn.value.type != Ability)
		return;
	sb.addn("[��������]: ");
	auto count = 0;
	for(auto i = 5; i <= 20; i++) {
		for(auto& e : bsdata<racei>()) {
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
	for(auto& e : bsdata<skilli>()) {
		if(e.is(ability_s(mn.value.value)))
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
		sb.add(bsdata<skilli>::elements[i].name);
	}
	sb.add(".");
}

static void ability_list(stringbuilder& sb, manual& mn, answeri& an) {
	choose_children(sb, mn, an);
}

static void ability_procs(stringbuilder& sb, manual& mn, answeri& an) {
	add_children(sb, mn, an);
	ability_example(sb, mn, an);
	ability_skills(sb, mn, an);
}

static void general_list(stringbuilder& sb, manual& mn, answeri& an) {
	choose_children(sb, mn, an);
}

static void manual_list(stringbuilder& sb, manual& mn, answeri& an) {
	choose_children(sb, mn, an);
}

static void skill_list(stringbuilder& sb, manual& mn, answeri& an) {
	choose_children_sort(sb, mn, an);
}

static void skill_proc(stringbuilder& sb, manual& mn, answeri& an) {
	race_skills(sb, mn, an);
}

static void weapon_skill(stringbuilder& sb, manual& mn, answeri& an) {
	weapon_skill_ability(sb, mn, an);
	item_example(sb, mn, an);
	race_skills(sb, mn, an);
}

static void state_list(stringbuilder& sb, manual& mn, answeri& an) {
	choose_children_sort(sb, mn, an);
}

static void race_ability_list(stringbuilder& sb, manual& mn, answeri& an) {
	race_ability(sb, mn, an);
	race_skills(sb, mn, an);
}

BSDATA(manual) = {
	{NoVariant, Variant, "������", "�������� ���������� ���������� �� �������� ����.", manual_list},
	{Variant, Ability, "����������� ����������", "������ �������� ����� [6] ������� ������������, ������� ��� �������������. �� ������� ��������� ������� ��������� �������������, ���� ����� �� ������� ���������� **����** ��������. ���� ������ ����� ���� (� ���������) ����� ���������� �������� �������� ���� ��������� ������������.", ability_list},
	{Ability, Strenght, 0, "���� �������� ����� ���������� ����, ���������� ��������� � ������������ ���������. ��� ����������� �������� ������� ��� ������, ��� ��� ��� ������ ���� ��������, ����� ������ ������� � ������� ������� �������.", ability_procs},
	{Ability, Dexterity, 0, "�������� ������������� ��������� ���������� ����������� ���������, ������� ����������� ��������, ����������, �������� �������, �������� � ������������� �������. �������� ������ �� ������� ��������� �� ��������� ��� �������������, �� ��� ������ ���������� � ����������� ������� � ������, � ����� �� ��� ������������ �� ������.", ability_procs},
	{Ability, Constitution, 0, "������������ ��������� ������������� ��� ���������� ������, ������������� ����������, �������� � ������������ � �����������, �������� � ������������.", ability_procs},
	{Ability, Intellegence, 0, "��������� ������������� ������, ���������������� ��������� � ��� ����������� � ��������, ������� ������� ��� ����, ��� ����� �������� �������� ������.", ability_procs},
	{Ability, Wisdow, 0, "�������� ������������� ������������ ��������������, ����������������, ���� ����, �������� ������ � �������� ���������.", ability_procs},
	{Ability, Charisma, 0, "������� ������������� ����������� ��������� � ���������, ��� ������ ����������������� � ��������� ��������. ��� �� �������� ���������� �����������������, ���� ���, ����������, ����� ������ ����. ������� ����� ��� ���� ����������, � �������� ��� ���, ��� ���������� ����� ���� � ���������� �����������, �������� �������, ������� � ��������� ���������.", ability_procs},
	{Dexterity, 0, "��������", "����������� � ����� ������� �� ����� ������������� �������."},
	{Dexterity, 0, "���� �������� �������", "���� �� ��������� �� ������� �������� ���� ��������. � ������ ������, �� ������� �� ������."},
	{Dexterity, 0, "����� ���������", "��� ������� � ������ ����������, ��������� [++1%%] �� ������ 2 ������� � ����� �������� �����, ����� �� ��� ���� ����."},
	{Intellegence, 0, "�������������� ����", "�������� �������� [++1%%] ����� �� ������ ������� ���������� ����� **10** � [--1%%] ����� �� ������ ������� ���������� ����� 10."},
	{Intellegence, 0, "��������� �������", "�������� �������� **��������/2** ��������� �������� �� ������ ������."},
	{Intellegence, 0, "�������", "�� ����� ������� ��� ��������� ��������� � ��������� ��� ��������� ������ �������� **����������2-20**."},
	{Variant, Trap, "�������", "� ����� ���������� ��������� ��������� �������. ��-��������� ��� ������� ������ � ����� ����� ���������� �� ������ � �������� �������� ������������ [��������]. ���� ���� ������ ����� ������������ ������� � �������� �� �������. ����� ����������� ������ ������� � ��� ���������� �������.\n��� ��������� ���������� �� ������� ����� �������� ��� �� ����, �� � ������� [++40%%]."},
	{Variant, Skill, "������ ����������", "������ �������� ����� ������������ ���������� �������. �� ����� ������� �� ������ � ����.", skill_list},
	{Variant, State, "��������� ����������", "�� ����� ����������� ��������� ������������ � ���������� ����������.", state_list},
	{Skill, Acrobatics, 0, "��������� [++1%%] � ����� ��������� �� ��������� �� ������ 4 �������.", skill_proc},
	{Skill, Alertness, 0, "��������� [++1%%] � ����� ���������� ������� �� ������ 4 �������. ��������� [++1%%] � ����� ���������� ������� ����� �� ������ 3 ������� ��� �������� ������.", skill_proc},
	{Skill, Alchemy, 0, "��������� ���������� �����. ����� ����� ������������� ����� �� ��������.", skill_proc},
	{Skill, Athletics, 0, "�������� � ���� ������� �������� ����� � �����. ��� ��������� ������ ������� ���� �������� **��������** ��� **����**. ����� �������� �������� �������� ��������� � �������.", skill_proc},
	{Skill, Backstabbing, 0, "����������� � ����� ��������� � ����������� ���� � ��������� ������ ���������� ������, ���� ����� �������� �� ���������� ���������. ����� ����� ����� �� ����������� �������.", skill_proc},
	{Skill, Climbing, 0, "��� �������� �� ����� �������� ���� � ���� �� �� ������� ������� ������� �� �������� ������������� �� 50%%. ����� ���� �������� ��� ������� ����� � ������. ����� ���� ����������� ��� ������� ������� �� **�������** ��� ������� **���**.", skill_proc},
	{Skill, Concetration, 0, "�������� �������������� ���� ���������.", skill_proc},
	{Skill, DisarmTraps, 0, "������� ������� � ���� ������� �����.", skill_proc},
	{Skill, FindWeakness, 0, "���� ������� � �������� ����� ����� � ������� [++30%%] �����, � ����� � ����������� �� ���� ������ �������������� ������:\n* ������� ������ ��������� +40%% ����� ������ +30%% � ���������� �����.\n* ������� ������ ���������� ���������� �������� ������\n* ������� ������ ������� ������������� ���� ����� �������� �������� ������ ������ � ���� ����� ������ ������.\n�� ����� ������� � �������� ����� ���������� ���������� ����� ������.", skill_proc},
	{Skill, Gambling, 0, "���� �������� ������ � �������� ���� (������, �����, �����). ����� ������ ������ � ��������� � ������ � ��������� ����������, � �������� ����� ������� ������ �����.", skill_proc},
	{Skill, Healing, 0, "�������� �������������� �������� ���������.", skill_proc},
	{Skill, HearNoises, 0, "���� �������� �������� ���������� ����� � ������� ��� �������.", skill_proc},
	{Skill, Herbalism, 0, "���� ������ ������� ������ � ������ � �������� ������������ �������� � ���� ������������. ���������� ��� ���������� �������� ������������ ��������� �������.", skill_proc},
	{Skill, HideInShadow, 0, "��������� ���������� � ���������� �����������. �� ������ ��������� �� ���� � ���� �� ����.", skill_proc},
	{Skill, Literacy, 0, "����� ������ ����� ��� ����. ����� �� �������� ������ ��� ����� ���������� ���� �������� � ������� ��, ��� �� �����. ����� ��������� ���� ������� ���������� - ������ ��� ���� ��� ������� � ����������, ��� ������� �� ����� ����� ����� ���������� ���-�� �����.", skill_proc},
	{Skill, Lockpicking, 0, "��������� �������� ����� � ���� ������� �����.", skill_proc},
	{Skill, MoveSilently, 0, "���� ��������� ���� ��������� ������������ � ��������� ���������.", skill_proc},
	{Skill, PickPockets, 0, "� ����� ��������� ������ ����� ��������� �������� ����������� ����������.", skill_proc},
	{Skill, Religion, 0, "���������� ������ ������� � �������������� ����� ����. �� ������� ������� ���������� ����� ���� ���������.", skill_proc},
	{Skill, Riding, 0, "��������� ������������� ������� �� ���������� �����.", skill_proc},
	{Skill, FocusAxes, 0, "��������� ��������� ������� ������ � ����� ������� ��� �����.", weapon_skill},
	{Skill, FocusSwords, 0, "��������� ����� ���������� ��������� �� �����.", weapon_skill},
	{Skill, FocusBows, 0, "��������� ����� ���������� �������� �� ����.", weapon_skill},
	{Skill, FocusTwohanded, 0, "��������� ����� ���������� ��������� ��������� �������.", weapon_skill},
	{Skill, TwoWeaponFighting, 0, "��������� ��������� ����� ����� �� ����������, ��� �������� � ������� � ������ ����.", skill_proc},
	{State, Poisoned, 0, "����������� �������� ������ �� ������ ���� ����� ������ 5 ����� ���� �� �������� ������������� ���. ��� ������� ������������� ����������� ������� ���."},
	{State, Sick, 0, "������� �������� �� ��������������� ���� ����� ��������� �����. ��� ������� ���������� ����������������� ����� ������� �� ���."},
	{Variant, Race, "����", "�� ��������� ���� ������� ��������� �������� � ������� ���������� ��������� ����������� �� [--2] �� [++2].", general_list},
	{Race, Human, 0, "���� - ����� ��������������� ���� � ������� ������������. �� ������� ������������ ����� ����� ���������� ������ ��, ��� ��� ����� ������� ����� ����� � �������������. � ������� ��� ����� ����� 70 ��� � ��������� ����� 180 ��.", race_ability_list},
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
	auto pm = bsdata<manual>::elements;
	while(pm) {
		char temp[2048]; stringbuilder sb(temp); answeri an;
		sb.add(pm->text);
		if(pm->description)
			pm->description(sb, *pm, an);
		if(pages.getcount() == pages.getmaximum())
			pages.remove(0);
		pages.add(pm);
		pm = (manual*)an.dialogv(true, pm->getname(), sb);
		if(!pm && pages.getcount() > 1) {
			pages.remove(pages.getcount() - 1);
			pm = pages[pages.getcount() - 1];
			pages.remove(pages.getcount() - 1);
		}
	}
}
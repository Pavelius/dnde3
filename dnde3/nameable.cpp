#include "main.h"

struct nameablei {
	race_s			race;
	gender_s		gender;
	const char*		name;
};
nameablei bsmeta<nameablei>::elements[] = {{Human, Male, "�����"},
{Human, Male, "�������"},
{Human, Male, "������"},
{Human, Female, "�����"},
{Human, Male, "�������"},
{Human, Male, "������"},
{Human, Female, "�����"},
{Human, Female, "����"},
{Human, Female, "������"},
{Human, Male, "�����"},
{Human, Male, "���"},
{Halfling, Male, "�������"},
{Halfling, Female, "������"},
{Halfling, Male, "��������"},
{Halfling, Male, "�������"},
{Halfling, Male, "������"},
{Halfling, Male, "�������"},
{Halfling, Female, "�����"},
{Elf, Male, "�������"},
{Elf, Female, "�������"},
{Elf, Male, "������"},
{Elf, Male, "�������"},
{Elf, Male, "������"},
{Elf, Male, "������"},
{Elf, Female, "��������"},
{Elf, Female, "��������"},
{Dwarf, Male, "�����"},
{Dwarf, Male, "������"},
{Dwarf, Female, "���������"},
{Dwarf, Female, "������"},
{Dwarf, Male, "������"},
{Dwarf, Female, "������"},
{Dwarf, Male, "���"},
{Dwarf, Male, "�������"},
{Dwarf, Male, "����"},
{Dwarf, Male, "������"},
};

static unsigned short getrandomname(race_s race, gender_s gender) {
	const auto max_count = sizeof(bsmeta<nameablei>::elements) / sizeof(bsmeta<nameablei>::elements[0]);
	unsigned short data[max_count];
	auto p = data;
	for(unsigned i = 0; i < max_count; i++) {
		if(bsmeta<nameablei>::elements[i].race == race && bsmeta<nameablei>::elements[i].gender == gender)
			*p++ = i;
	}
	unsigned count = p - data;
	if(!count)
		return Blocked;
	return data[rand() % count];
}

void nameable::setname(race_s race, gender_s gender) {
	name = getrandomname(race, gender);
}

gender_s nameable::getgender() const {
	if(name == Blocked)
		return Male;
	return bsmeta<nameablei>::elements[name].gender;
}

race_s nameable::getrace() const {
	if(name == Blocked)
		return Human;
	return bsmeta<nameablei>::elements[name].race;
}

const char* nameable::getname() const {
	if(name == Blocked)
		return "�����";
	return bsmeta<nameablei>::elements[name].name;
}

void nameable::actv(stringbuilder& st, const char* format, const char* param) const {
	string sb = st;
	sb.name = getname();
	sb.gender = getgender();
	sb.addv(format, param);
	st = sb;
}

void nameable::actv(stringbuilder& st, nameable& e, const char* format, const char* param) const {
	string sb = st;
	sb.name = getname();
	sb.gender = getgender();
	sb.opponent_name = e.getname();
	sb.opponent_gender = e.getgender();
	sb.addv(format, param);
	st = sb;
}
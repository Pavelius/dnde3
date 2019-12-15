#include "main.h"

struct nameablei {
	race_s			race;
	gender_s		gender;
	const char*		name;
};
nameablei bsmeta<nameablei>::elements[] = {{Human, Male, "Хавки"},
{Human, Male, "Рудигер"},
{Human, Male, "Грегор"},
{Human, Female, "Бриан"},
{Human, Male, "Вальтон"},
{Human, Male, "Кастор"},
{Human, Female, "Шанна"},
{Human, Female, "Соня"},
{Human, Female, "Солнце"},
{Human, Male, "Айакс"},
{Human, Male, "Хоб"},
{Halfling, Male, "Финганн"},
{Halfling, Female, "Оливия"},
{Halfling, Male, "Рэндольф"},
{Halfling, Male, "Батлбай"},
{Halfling, Male, "Аурбей"},
{Halfling, Male, "Балдвин"},
{Halfling, Female, "Бэкки"},
{Elf, Male, "Эйлохир"},
{Elf, Female, "Харасез"},
{Elf, Male, "Хазрич"},
{Elf, Male, "Шеварал"},
{Elf, Male, "Кадиус"},
{Elf, Male, "Эльдар"},
{Elf, Female, "Котораса"},
{Elf, Female, "Фелианна"},
{Dwarf, Male, "Озрук"},
{Dwarf, Male, "Суртур"},
{Dwarf, Female, "Брундилла"},
{Dwarf, Female, "Анника"},
{Dwarf, Male, "Джанос"},
{Dwarf, Female, "Гретта"},
{Dwarf, Male, "Дим"},
{Dwarf, Male, "Рундриг"},
{Dwarf, Male, "Жарл"},
{Dwarf, Male, "Ксоток"},
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
		return "Павел";
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
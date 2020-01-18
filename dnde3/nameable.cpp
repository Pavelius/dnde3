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
	if(ischaracter()) {
		if(name == Blocked)
			return Male;
		return bsmeta<nameablei>::elements[name].gender;
	}
	return bsmeta<rolei>::elements[value].gender;
}

race_s nameable::getrace() const {
	if(ischaracter()) {
		if(name == Blocked)
			return Human;
		return bsmeta<nameablei>::elements[name].race;
	}
	return bsmeta<rolei>::elements[value].race;
}

const char* nameable::getname() const {
	if(ischaracter()) {
		if(name == Blocked)
			return "Павел";
		return bsmeta<nameablei>::elements[name].name;
	}
	return bsmeta<rolei>::elements[value].name;
}

bool nameable::cansee() const {
	auto player = creature::getactive();
	if(!player)
		return false;
	if(player != this) {
		auto start = player->getposition();
		if(start == Blocked)
			return false;
		auto target = getposition();
		if(target == Blocked)
			return false;
		if(loc.getrange(start, target) > player->getlos())
			return false;
		if(!loc.cansee(start, target))
			return false;
	}
	return true;
}

void nameable::actv(stringbuilder& st, const char* format, const char* param) const {
	if(!format)
		return;
	if(!cansee())
		return;
	string sb = st;
	sb.name = getname();
	sb.gender = getgender();
	sb.addsep(' ');
	sb.addv(format, param);
	st = sb;
}

void nameable::actv(stringbuilder& st, nameable& e, const char* format, const char* param) const {
	if(!format)
		return;
	if(!cansee() && e.cansee())
		return;
	string sb = st;
	sb.name = getname();
	sb.gender = getgender();
	sb.opponent_name = e.getname();
	sb.opponent_gender = e.getgender();
	sb.addsep(' ');
	sb.addv(format, param);
	st = sb;
}

void nameable::act(const char* format, ...) const {
	actv(sb, format, xva_start(format));
}

void nameable::sayv(stringbuilder& st, const char* format, const char* param) const {
	if(!format)
		return;
	if(!cansee())
		return;
	string sb = st;
	sb.name = getname();
	sb.gender = getgender();
	sb.addsep(' ');
	sb.add("[%герой:]");
	sb.add("\"");
	sb.addv(format, param);
	sb.add("\"");
	st = sb;
}
#include "main.h"

template<> adjectivei bsmeta<adjectivei>::elements[] = {{"шальное", "шальной", "шальная"},
{"волшебное", "вольшебный", "волшебная"},
{"смешное", "смешной", "смешная"},
{"черное", "черный", "черная"},
{"белое", "белый", "белая"},
{"стальное", "стальной", "стальная"},
{"золотое", "золотой", "золотая"},
};
DECLFULL(adjectivei);

template<> objectivei bsmeta<objectivei>::elements[] = {{"гном", Male},
{"орк", Male},
{"единорог", Male},
{"волк", Male},
{"рысь", Female},
{"дракон", Male},
{"волк", Male},
{"перстень", Male},
{"сокровище", NoGender},
{"река", Female},
{"гора", Female},
{"лес", Male},
{"дуб", Male},
};
DECLFULL(objectivei);

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
{Ratling, Male, "Хуч"},
{Ratling, Male, "Чша"},
{Ratling, Female, "Лун"},
{Ratling, Female, "Кан"},
{Ratling, Male, "Раз"},
{Ratling, Female, "Джиз"},
{Ratling, Male, "Кин"},
{Ratling, Male, "Хон"},
{Ratling, Male, "Лин"},
{Ratling, Male, "Чук"},
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

void nameable::randomname() {
	roomi* ri;
	switch(type) {
	case Role:
		break;
	case Room:
		ri = bsmeta<roomi>::elements + value;
		if(ri->name) {
			name[0] = bsmeta<adjectivei>::source.random();
			name[1] = bsmeta<objectivei>::source.random();
		} else {
			if(ri->name1)
				name[0] = rand() % ri->name1.getcount();
			if(ri->name2)
				name[1] = rand() % ri->name2.getcount();
		}
		break;
	}
}

void nameable::setname(race_s race, gender_s gender) {
	name[0] = getrandomname(race, gender);
}

gender_s nameable::getgender() const {
	if(ischaracter()) {
		if(name[0] == Blocked)
			return Male;
		return bsmeta<nameablei>::elements[name[0]].gender;
	}
	return bsmeta<rolei>::elements[value].gender;
}

race_s nameable::getrace() const {
	if(ischaracter()) {
		if(name[0] == Blocked)
			return Human;
		return bsmeta<nameablei>::elements[name[0]].race;
	}
	return bsmeta<rolei>::elements[value].race;
}

const char* nameable::getname() const {
	switch(type) {
	case Role:
		if(ischaracter()) {
			if(name[0] == Blocked)
				return "Павел";
			return bsmeta<nameablei>::elements[name[0]].name;
		}
		return bsmeta<rolei>::elements[value].name;
	case Room: return bsmeta<roomi>::elements[value].name;
	default: return "Что-то";
	}
}

void nameable::getname(stringbuilder& sb) const {
	roomi* ri;
	switch(type) {
	case Room:
		ri = bsmeta<roomi>::elements + value;
		if(ri->name) {
			sb.add(ri->name,
				bsmeta<adjectivei>::elements[name[0]].get(bsmeta<objectivei>::elements[name[1]].gender),
				bsmeta<objectivei>::elements[name[1]].name);
		} else {
			const char* p2 = "";
			const char* n1 = "";
			const char* n2 = "";
			const char* v1 = "";
			const char* v2 = "";
			if(ri->name2)
				p2 = ri->name2[name[1]];
			auto ps = getsite();
			if(ps) {
				auto owner = ps->getowner();
				if(owner) {
					n1 = owner->getname();
					n2 = owner->getnameof();
				}
				auto param = ps->getparam();
				if(param) {
					v1 = param.getname();
					v2 = param.getnameof();
				}
			}
			sb.add(ri->name1[name[0]], "", p2, n1, n2, v1, v2);
		}
		break;
	default:
		sb.add(getname());
		break;
	}
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

void nameable::actev(stringbuilder& st, const char* format, const char* param) const {
	string sb = st;
	sb.name = getname();
	sb.gender = getgender();
	sb.addsep(' ');
	sb.addv(format, param);
	st = sb;
}

void nameable::actv(stringbuilder& st, const char* format, const char* param) const {
	if(!format)
		return;
	if(!cansee())
		return;
	actev(st, format, param);
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

bool nameable::askv(stringbuilder& st, const nameable& opponent, const char* format, const char* param) const {
	if(!format)
		return false;
	if(!cansee() || !opponent.cansee())
		return true;
	string sb = st;
	sb.name = getname();
	sb.gender = getgender();
	sb.opponent_name = opponent.getname();
	sb.opponent_gender = opponent.getgender();
	sb.addsep(' ');
	sb.add("[%герой:]");
	sb.add("\"");
	sb.addv(format, param);
	sb.add("\"");
	st = sb;
	auto player = creature::getactive();
	if(player)
		return player->askyn();
	return true;
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

const char* adjectivei::get(gender_s v) const {
	switch(v) {
	case Male: return name_male;
	case Female: return name_female;
	default: return name;
	}
}

bool nameable::isactive() const {
	if(!this)
		return false;
	return creature::getactive() == this;
}

site* nameable::getsite() const {
	auto i = bsmeta<site>::source.indexof(this);
	if(i == -1)
		return 0;
	return bsmeta<site>::elements + i;
}
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
};

static const char* shop_end[] = {"из-за океана",
"высокого качества",
"по небольшим ценам",
"с изюменкой",
"для королей",
"дешевле некуда",
};
static const char* shop_weapon[] = {"Кузница %1",
"Мечи и доспехи %1",
"Оружие %1",
"Мечи %1",
};
static const char* shop_potions[] = {"Аптека %1",
"Зелья и элексиры %1",
"Алхимия %1",
};
static const char* shop_scrolls[] = {"Свитки %1",
"Тома и заклинания %1",
"Магия %1",
"Волшебные папирусы %1",
};
static const char* temple[] = {"Храм %1",
"Святилище %1",
"Молебня %1",
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
	switch(type) {
	case Role:
		break;
	case Room:
		switch(value) {
		case ShopWeaponAndArmor:
			name[0] = rand() % (sizeof(shop_weapon) / sizeof(shop_weapon[0]));
			name[1] = rand() % (sizeof(shop_end) / sizeof(shop_end[0]));
			break;
		case ShopPotions:
			name[0] = rand() % (sizeof(shop_potions) / sizeof(shop_potions[0]));
			name[1] = rand() % (sizeof(shop_end) / sizeof(shop_end[0]));
			break;
		case ShopScrolls:
			name[0] = rand() % (sizeof(shop_scrolls) / sizeof(shop_scrolls[0]));
			name[1] = rand() % (sizeof(shop_end) / sizeof(shop_end[0]));
			break;
		case Temple:
			name[0] = rand() % (sizeof(temple) / sizeof(temple[0]));
			break;
		default:
			name[0] = bsmeta<adjectivei>::source.random();
			name[1] = bsmeta<objectivei>::source.random();
			break;
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
	switch(type) {
	case Room:
		switch(value) {
		case ShopWeaponAndArmor: sb.add(shop_weapon[name[0]], shop_end[name[1]]); break;
		case ShopPotions: sb.add(shop_potions[name[0]], shop_end[name[1]]); break;
		case ShopScrolls: sb.add(shop_scrolls[name[0]], shop_end[name[1]]); break;
		case Temple: sb.add(temple[name[0]], getnameof(), variant::getname()); break;
		default:
			sb.add(bsmeta<roomi>::elements[value].title,
				bsmeta<adjectivei>::elements[name[0]].get(bsmeta<objectivei>::elements[name[1]].gender),
				bsmeta<objectivei>::elements[name[1]].name);
			break;
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
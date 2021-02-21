#include "main.h"

struct nameablei {
	race_s			race;
	gender_s		gender;
	const char*		name;
};

BSDATA(adjectivei) = {{"�������", "�������", "�������"},
{"���������", "����������", "���������"},
{"�������", "�������", "�������"},
{"������", "������", "������"},
{"�����", "�����", "�����"},
{"��������", "��������", "��������"},
{"�������", "�������", "�������"},
};
DECLFULL(adjectivei)

BSDATA(objectivei) = {{"����", Male},
{"���", Male},
{"��������", Male},
{"����", Male},
{"����", Female},
{"������", Male},
{"����", Male},
{"��������", Male},
{"���������", NoGender},
{"����", Female},
{"����", Female},
{"���", Male},
{"���", Male},
};
DECLFULL(objectivei);

BSDATA(nameablei) = {{Human, Male, "�����"},
{Human, Male, "�������"},
{Human, Male, "������"},
{Human, Male, "�������"},
{Human, Male, "������"},
{Human, Male, "�����"},
{Human, Male, "���"},
{Human, Male, "���"},
{Human, Male, "������"},
{Human, Male, "����"},
{Human, Male, "�����"},
{Human, Male, "����"},
{Human, Male, "����"},
{Human, Male, "����"},
{Human, Male, "���"},
{Human, Male, "������"},
{Human, Female, "������"},
{Human, Female, "�����"},
{Human, Female, "�����"},
{Human, Female, "����"},
{Human, Female, "������"},
{Human, Female, "������"},
{Human, Female, "�����"},
{Human, Female, "�����"},
{Human, Female, "����"},
{Human, Female, "����"},
{Halfling, Male, "������"},
{Halfling, Male, "������"},
{Halfling, Male, "���"},
{Halfling, Male, "�����"},
{Halfling, Male, "�������"},
{Halfling, Male, "��������"},
{Halfling, Male, "������"},
{Halfling, Male, "�����"},
{Halfling, Male, "�������"},
{Halfling, Male, "������"},
{Halfling, Male, "�������"},
{Halfling, Female, "������"},
{Halfling, Female, "�����"},
{Halfling, Female, "�������"},
{Halfling, Female, "������"},
{Halfling, Female, "�����"},
{Elf, Male, "�����"},
{Elf, Male, "�����"},
{Elf, Male, "������"},
{Elf, Male, "�����"},
{Elf, Male, "��������"},
{Elf, Male, "������"},
{Elf, Male, "�����"},
{Elf, Male, "�����"},
{Elf, Male, "�������"},
{Elf, Male, "��������"},
{Elf, Male, "�������"},
{Elf, Male, "����"},
{Elf, Male, "�����"},
{Elf, Male, "������"},
{Elf, Female, "�����"},
{Elf, Female, "������"},
{Elf, Female, "�����������"},
{Elf, Female, "��������"},
{Elf, Female, "�������"},
{Elf, Female, "��������"},
{Elf, Female, "������"},
{Elf, Female, "�������"},
{Elf, Female, "����"},
{Elf, Female, "���"},
{Elf, Female, "�������"},
{Elf, Female, "�������"},
{Elf, Female, "�������"},
{Elf, Female, "�������"},
{Elf, Female, "��������"},
{Elf, Female, "�������"},
{Elf, Female, "���"},
{Elf, Female, "����"},
{Elf, Female, "�������"},
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
{Ratling, Male, "���"},
{Ratling, Male, "���"},
{Ratling, Female, "���"},
{Ratling, Female, "���"},
{Ratling, Male, "���"},
{Ratling, Female, "����"},
{Ratling, Male, "���"},
{Ratling, Male, "���"},
{Ratling, Male, "���"},
{Ratling, Male, "���"},
{Orc, Male, "����"},
{Orc, Male, "����"},
{Orc, Male, "����"},
{Orc, Male, "����"},
{Orc, Male, "����"},
{Orc, Male, "���"},
{Orc, Male, "���"},
{Orc, Male, "�����"},
{Orc, Male, "�������"},
{Orc, Male, "����"},
{Orc, Male, "����"},
{Orc, Female, "�����"},
{Orc, Female, "����"},
{Orc, Female, "����"},
{Orc, Female, "���"},
{Orc, Female, "����"},
{Orc, Female, "����"},
{Orc, Female, "�����"},
{Orc, Female, "�����"},
{Orc, Female, "�����"},
{Orc, Female, "�������"},
};

static unsigned short getrandomname(race_s race, gender_s gender) {
	const auto max_count = sizeof(bsdata<nameablei>::elements) / sizeof(bsdata<nameablei>::elements[0]);
	unsigned short data[max_count];
	auto p = data;
	for(unsigned i = 0; i < max_count; i++) {
		if(bsdata<nameablei>::elements[i].race == race && bsdata<nameablei>::elements[i].gender == gender)
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
		ri = bsdata<roomi>::elements + value;
		if(ri->name) {
			name[0] = bsdata<adjectivei>::source.random();
			name[1] = bsdata<objectivei>::source.random();
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
		return bsdata<nameablei>::elements[name[0]].gender;
	}
	return bsdata<rolei>::elements[value].gender;
}

race_s nameable::getrace() const {
	if(ischaracter()) {
		if(name[0] == Blocked)
			return Human;
		return bsdata<nameablei>::elements[name[0]].race;
	}
	return bsdata<rolei>::elements[value].race;
}

const char* nameable::getname() const {
	switch(type) {
	case Role:
		if(ischaracter()) {
			if(name[0] == Blocked)
				return "�����";
			return bsdata<nameablei>::elements[name[0]].name;
		}
		return bsdata<rolei>::elements[value].name;
	case Room: return bsdata<roomi>::elements[value].name;
	default: return "���-��";
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

void nameable::actev(stringbuilder& st, const char* format, const char* param, bool add_sep) const {
	string sb = st;
	sb.name = getname();
	sb.gender = getgender();
	if(add_sep)
		sb.addsep(' ');
	sb.addv(format, param);
	st = sb;
}

void nameable::actv(stringbuilder& st, const char* format, const char* param) const {
	if(!format)
		return;
	if(!cansee())
		return;
	actev(st, format, param, true);
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

void nameable::infov(stringbuilder& st, const char* format, const char* param) const {
	if(this != creature::getactive())
		return;
	string sb = st;
	sb.name = getname();
	sb.gender = getgender();
	sb.addsep(' ');
	sb.addv(format, param);
	st = sb;
}

void nameable::act(const char* format, ...) const {
	actv(sb, format, xva_start(format));
}

void nameable::info(const char* format, ...) const {
	infov(sb, format, xva_start(format));
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
	sb.add("[%�����:]");
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
	sb.add("[%�����:]");
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
	auto i = bsdata<site>::source.indexof(this);
	if(i == -1)
		return 0;
	return bsdata<site>::elements + i;
}

void nameable::getname(stringbuilder& sb) const {
	roomi* ri;
	switch(type) {
	case Room:
		if(getsite() && !getsite()->is(KnownSite))
			break;
		ri = bsdata<roomi>::elements + value;
		if(ri->name) {
			sb.add(ri->name,
				bsdata<adjectivei>::elements[name[0]].get(bsdata<objectivei>::elements[name[1]].gender),
				bsdata<objectivei>::elements[name[1]].name);
		} else if(ri->name1) {
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
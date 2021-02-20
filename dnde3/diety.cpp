#include "main.h"

BSDATA(dietyi) = {
	{"Бейн", "Бейна", Male, {Mace, LeatherArmor, BracersLeather, BracersIron}, "сжатый кулак из которого исходят зеленые лучи"},
	{"Баал", "Баала", Male, {Dagger, LeatherArmor}, "черный череп в окружении красных капель крови"},
	{"Грумш", "Грумша", Male, {Spear, ChainMail}, "октрытый глаз"},
	{"Хелм", "Хелма", Male, {HammerWar, ChainMail, PlateMail}, "открытая ладонь"},
	{"Мистра", "Мистры", Female, {Staff, Wand1, Wand2, Wand3, Wand4, Wand5}, "круг из семи сине-белых звезд с красной мглой идущей из центра"},
	{"Темпус", "Темпуса", Male, {AxeBattle, SwordLong, SwordTwoHanded, SwordShort, Shield}, "сверкающий серебрянный меч на кроваво красном щите"},
	{"Тир", "Тира", Male, {SwordLong}, "весы опирающиеся на молот"},
};
assert_enum(dietyi, GodTyr)

void dietyi::act(const char* format, ...) const {
	actv(sb, format, xva_start(format));
}

void dietyi::actv(stringbuilder& st, const char* format, const char* format_param) const {
	string sb = st;
	sb.name = name;
	sb.gender = gender;
	if(true)
		sb.addsep(' ');
	sb.addv(format, format_param);
	st = sb;
}
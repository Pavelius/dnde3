#include "main.h"

BSDATA(rolei) = {{"Гоблин", "гоблина", "0", Goblin, Male, Chaotic, Monster, 0, {SwordShort}},
{"Гоблин", "гоблина", "4", Goblin, Male, Chaotic, Monster, 0, {Rock, LeatherArmor}},
{"Орк", "орка", "1", Orc, Male, Chaotic, Monster, 1, {SwordLong, StuddedLeatherArmor}},
{"Летучая мышь", "летучей мыши", "2", Animal, Female, Chaotic, Monster, 0, {Bite, Dexterity}},
{"Крыса", "крысы", "3", Animal, Female, Chaotic, Monster, 0, {Bite, Dexterity}},
{"Крестьянин", "крестьянина", "282", Human, Male, Neutral, Commoner, 1},
{"Охранник", "охранника", "57", Human, Male, Neutral, Fighter, 2, {Spear, SplintMail, Helmet}},
{"Ребенок", "ребенка", "283", Human, Male, Neutral, Commoner},
{"Крестьянка", "крестьянки", "284", Human, Male, Neutral, Commoner},
{"Владелец магазина", "владельца магазина", "84", Human, Male, Neutral, Commoner, 1, {Bargaining}},
{"Кузнец", "кузнеца", "95", Dwarf, Male, Neutral, Commoner, 1, {HammerWar}},
{"Бартендер", "бартендера", "130", Dwarf, Male, Neutral, Commoner, 1, },
{"Скелет", "скелета", "8", Undead, Male, Chaotic, Monster, 1, {Spear, Dexterity}},
{"Зомби", "зомби", "16", Undead, Male, Chaotic, Monster, 2, {Dagger, Strenght}},
{"Кобольд", "кобольда", "5", Kobold, Male, Chaotic, Monster, 0, {BowShort, Dagger}},
{"Кобольд-шаман", "кобольда-шамана", "14", Kobold, Male, Chaotic, Mage, 3, {Staff, Wand1, MagicMissile}},
{"Собака", "собаки", "6", Animal, Female, Neutral, Monster, 1, {Bite, Dexterity}},
{"Рысь", "рыси", "7", Animal, Female, Chaotic, Monster, 3, {Bite, Fur, Dexterity, Strenght, Constitution}},
{"Жаба", "жабы", "9", Animal, Female, Neutral, Monster, 1, {Bite, Dexterity}},
{"Муравей", "муравья", "10", Insect, Male, Chaotic, Monster, 0, {Bite, Hitin}},
{"Муравей-воин", "муравья-воина", "11", Insect, Male, Chaotic, Monster, 1, {Bite, Hitin, Strenght}},
{"Матка муравьев", "матки муравья", "12", Insect, Female, Chaotic, Monster, 5, {Bite, Hitin, Strenght, Strenght, Constitution}, {AntWorker, AntWarrior}},
{"Гнолл", "гнолла", "13", Gnoll, Male, Chaotic, Monster, 2, {AxeBattle, Fur}},
{"Гоблин", "гоблина", "15", Goblin, Male, Chaotic, Monster, 1, {Rock, StuddedLeatherArmor}},
{"Гоблиниха", "гоблинихи", "15_f", Goblin, Female, Chaotic, Monster, 1, {Rock, StuddedLeatherArmor, Meat}},
{"Орк", "орка", "17", Orc, Male, Chaotic, Monster, 2, {Spear, StuddedLeatherArmor}},
{"Орчиха", "орчихи", "17_f", Orc, Female, Chaotic, Monster, 2, {Spear, StuddedLeatherArmor}},
{"Летучая мышь", "летучей мыши", "18", Animal, Female, Chaotic, Monster, 1, {Bite, Dexterity}},
{"Летучая мышь", "летучей мыши", "19", Animal, Female, Chaotic, Monster, 2, {Bite, Dexterity}},
{"Пчела", "пчелы", "20", Animal, Female, Neutral, Monster, 0, {Bite}},
{"Пчела", "пчелы", "21", Animal, Female, Neutral, Monster, 1, {Bite}},
{"Пчела", "пчелы", "22", Animal, Female, Neutral, Monster, 1, {Bite}},
{"Огр", "огра", "23", Ogre, Male, Chaotic, Monster, 5, {Slam}},
{"Огриха", "огрихи", "23_f", Ogre, Female, Chaotic, Monster, 5, {Slam}},
{"Медвежук", "медвежука", "24", Goblin, Male, Chaotic, Monster, 3, {Fur}},
{"Медвежучиха", "медвежучихи", "24_f", Goblin, Male, Chaotic, Monster, 3, {Fur}},
{"Персонаж", "персонажа", 0, Human, Male, Neutral, Commoner},
};
assert_enum(role, Character)

void creature::create(role_s type) {
	clear();
	auto& ei = bsmeta<rolei>::elements[type];
	this->variant::type = Role;
	this->variant::value = type;
	this->kind = ei.type;
	applyabilities();
	for(auto v : ei.features)
		add(v, 4, false);
	while(abilities[Level] < ei.level)
		raiselevel();
	finish();
}

int	rolei::getcr() const {
	auto b = 0;
	for(auto v : features) {
		if(v.type == Ability)
			b += bsmeta<abilityi>::elements[v.value].cost;
		else if(v)
			b++;
	}
	return level + b / 4;
}
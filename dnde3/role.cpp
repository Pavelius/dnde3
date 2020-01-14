#include "main.h"

rolei bsmeta<rolei>::elements[] = {{"Гоблин", "0", Goblin, Male, Chaotic, Monster, 0, {SwordShort}},
{"Гоблин", "4", Goblin, Male, Chaotic, Monster, 0, {Rock, LeatherArmor}},
{"Орк", "1", Orc, Male, Chaotic, Monster, 1, {SwordLong, StuddedLeatherArmor}},
{"Летучая мышь", "2", Animal, Female, Chaotic, Monster, 0, {Bite, Dexterity}},
{"Крыса", "3", Animal, Female, Chaotic, Monster, 0, {Bite, Dexterity}},
{"Крестьянин", "282", Human, Male, Neutral, Commoner, 1},
{"Охранник", "57", Human, Male, Neutral, Fighter, 2, {Spear, SplintMail, Helmet}},
{"Ребенок", "283", Human, Male, Neutral, Commoner},
{"Крестьянка", "284", Human, Male, Neutral, Commoner},
{"Владелец магазина", "84", Human, Male, Neutral, Commoner, 1, {Bargaining}},
{"Кузнец", "95", Dwarf, Male, Neutral, Commoner, 1, {HammerWar}},
{"Бартендер", "130", Dwarf, Male, Neutral, Commoner, 1, },
{"Скелет", "8", Undead, Male, Chaotic, Monster, 1, {Spear, Dexterity}},
{"Зомби", "16", Undead, Male, Chaotic, Monster, 2, {Dagger, Strenght}},
{"Кобольд", "5", Kobold, Male, Chaotic, Monster, 0, {BowShort, Dagger}},
{"Кобольд-шаман", "14", Kobold, Male, Chaotic, Mage, 3, {Staff, Wand1, MagicMissile}},
{"Собака", "6", Animal, Female, Neutral, Monster, 1, {Bite, Dexterity}},
{"Рысь", "7", Animal, Female, Chaotic, Monster, 3, {Bite, Fur, Dexterity, Strenght, Constitution}},
{"Жаба", "9", Animal, Female, Neutral, Monster, 1, {Bite, Dexterity}},
{"Муравей", "10", Insect, Male, Chaotic, Monster, 0, {Bite, Hitin}},
{"Муравей-воин", "11", Insect, Male, Chaotic, Monster, 1, {Bite, Hitin, Strenght}},
{"Матка муравьев", "12", Insect, Female, Chaotic, Monster, 5, {Bite, Hitin, Strenght, Strenght, Constitution}, {AntWorker, AntWorker, AntWarrior, AntWarrior}},
{"Гнолл", "13", Gnoll, Male, Chaotic, Monster, 2, {AxeBattle, Fur}},
{"Гоблин", "15", Goblin, Male, Chaotic, Monster, 1, {Rock, StuddedLeatherArmor}},
{"Гоблиниха", "15_f", Goblin, Female, Chaotic, Monster, 1, {Rock, StuddedLeatherArmor, Meat}},
{"Орк", "17", Orc, Male, Chaotic, Monster, 2, {Rock, Spear, StuddedLeatherArmor}},
{"Орчиха", "17_f", Orc, Female, Chaotic, Monster, 2, {Rock, Spear, StuddedLeatherArmor}},
{"Летучая мышь", "18", Animal, Female, Chaotic, Monster, 1, {Bite, Dexterity}},
{"Летучая мышь", "19", Animal, Female, Chaotic, Monster, 2, {Bite, Dexterity}},
{"Пчела", "20", Animal, Female, Neutral, Monster, 0, {Bite}},
{"Пчела", "21", Animal, Female, Neutral, Monster, 1, {Bite}},
{"Пчела", "22", Animal, Female, Neutral, Monster, 1, {Bite}},
{"Огр", "23", Ogre, Male, Chaotic, Monster, 5, {Slam}},
{"Огриха", "23_f", Ogre, Female, Chaotic, Monster, 5, {Slam}},
{"Медвежук", "24", Goblin, Male, Chaotic, Monster, 3, {Fur}},
{"Медвежучиха", "24_f", Goblin, Male, Chaotic, Monster, 3, {Fur}},
{"Персонаж", 0, Human, Male, Neutral, Commoner},
};
assert_enum(role, Character);

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
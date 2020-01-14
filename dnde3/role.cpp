#include "main.h"

rolei bsmeta<rolei>::elements[] = {{"������", "0", Goblin, Male, Chaotic, Monster, 0, {SwordShort}},
{"������", "4", Goblin, Male, Chaotic, Monster, 0, {Rock, LeatherArmor}},
{"���", "1", Orc, Male, Chaotic, Monster, 1, {SwordLong, StuddedLeatherArmor}},
{"������� ����", "2", Animal, Female, Chaotic, Monster, 0, {Bite, Dexterity}},
{"�����", "3", Animal, Female, Chaotic, Monster, 0, {Bite, Dexterity}},
{"����������", "282", Human, Male, Neutral, Commoner, 1},
{"��������", "57", Human, Male, Neutral, Fighter, 2, {Spear, SplintMail, Helmet}},
{"�������", "283", Human, Male, Neutral, Commoner},
{"����������", "284", Human, Male, Neutral, Commoner},
{"�������� ��������", "84", Human, Male, Neutral, Commoner, 1, {Bargaining}},
{"������", "95", Dwarf, Male, Neutral, Commoner, 1, {HammerWar}},
{"���������", "130", Dwarf, Male, Neutral, Commoner, 1, },
{"������", "8", Undead, Male, Chaotic, Monster, 1, {Spear, Dexterity}},
{"�����", "16", Undead, Male, Chaotic, Monster, 2, {Dagger, Strenght}},
{"�������", "5", Kobold, Male, Chaotic, Monster, 0, {BowShort, Dagger}},
{"�������-�����", "14", Kobold, Male, Chaotic, Mage, 3, {Staff, Wand1, MagicMissile}},
{"������", "6", Animal, Female, Neutral, Monster, 1, {Bite, Dexterity}},
{"����", "7", Animal, Female, Chaotic, Monster, 3, {Bite, Fur, Dexterity, Strenght, Constitution}},
{"����", "9", Animal, Female, Neutral, Monster, 1, {Bite, Dexterity}},
{"�������", "10", Insect, Male, Chaotic, Monster, 0, {Bite, Hitin}},
{"�������-����", "11", Insect, Male, Chaotic, Monster, 1, {Bite, Hitin, Strenght}},
{"����� ��������", "12", Insect, Female, Chaotic, Monster, 5, {Bite, Hitin, Strenght, Strenght, Constitution}, {AntWorker, AntWorker, AntWarrior, AntWarrior}},
{"�����", "13", Gnoll, Male, Chaotic, Monster, 2, {AxeBattle, Fur}},
{"������", "15", Goblin, Male, Chaotic, Monster, 1, {Rock, StuddedLeatherArmor}},
{"���������", "15_f", Goblin, Female, Chaotic, Monster, 1, {Rock, StuddedLeatherArmor, Meat}},
{"���", "17", Orc, Male, Chaotic, Monster, 2, {Rock, Spear, StuddedLeatherArmor}},
{"������", "17_f", Orc, Female, Chaotic, Monster, 2, {Rock, Spear, StuddedLeatherArmor}},
{"������� ����", "18", Animal, Female, Chaotic, Monster, 1, {Bite, Dexterity}},
{"������� ����", "19", Animal, Female, Chaotic, Monster, 2, {Bite, Dexterity}},
{"�����", "20", Animal, Female, Neutral, Monster, 0, {Bite}},
{"�����", "21", Animal, Female, Neutral, Monster, 1, {Bite}},
{"�����", "22", Animal, Female, Neutral, Monster, 1, {Bite}},
{"���", "23", Ogre, Male, Chaotic, Monster, 5, {Slam}},
{"������", "23_f", Ogre, Female, Chaotic, Monster, 5, {Slam}},
{"��������", "24", Goblin, Male, Chaotic, Monster, 3, {Fur}},
{"�����������", "24_f", Goblin, Male, Chaotic, Monster, 3, {Fur}},
{"��������", 0, Human, Male, Neutral, Commoner},
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
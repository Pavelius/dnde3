#include "main.h"

rolei bsmeta<rolei>::elements[] = {{"������", Goblin, Male, Chaotic, Monster, 0, {SwordShort}},
{"������", Goblin, Male, Chaotic, Monster, 0, {Rock, LeatherArmor}},
{"���", Orc, Male, Chaotic, Monster, 1, {SwordLong, StuddedLeatherArmor}},
{"������� ����", Animal, Female, Chaotic, Monster, 0, {Bite, Dexterity}},
{"�����", Animal, Female, Chaotic, Monster, 0, {Bite, Dexterity}},
{"����������", Human, Male, Neutral, Commoner, 1},
{"��������", Human, Male, Neutral, Fighter, 2, {Spear, SplintMail, Helmet}},
{"�������", Human, Male, Neutral, Commoner},
{"����������", Human, Male, Neutral, Commoner},
{"�������� ��������", Human, Male, Neutral, Commoner, 1, {Bargaining}},
{"������", Dwarf, Male, Neutral, Commoner, 1, {HammerWar}},
{"���������", Dwarf, Male, Neutral, Commoner, 1, },
{"������", Undead, Male, Chaotic, Monster, 1, {Spear, Dexterity}},
{"�����", Undead, Male, Chaotic, Monster, 2, {Dagger, Strenght}},
{"�������", Kobold, Male, Chaotic, Monster, 0, {BowShort, Dagger}},
{"�������-�����", Kobold, Male, Chaotic, Mage, 3, {Staff, Wand1, MagicMissile}},
{"������", Animal, Female, Neutral, Monster, 1, {Bite, Dexterity}},
{"����", Animal, Female, Chaotic, Monster, 3, {Bite, Fur, Dexterity, Strenght, Constitution}},
{"�������", Animal, Female, Neutral, Monster, 1, {Bite, Dexterity}},
{"�������", Insect, Male, Chaotic, Monster, 0, {Bite, Hitin}},
{"�������-����", Insect, Male, Chaotic, Monster, 1, {Bite, Hitin, Strenght}},
{"����� ��������", Insect, Female, Chaotic, Monster, 5, {Bite, Hitin, Strenght, Strenght, Constitution}, {AntWorker, AntWorker, AntWarrior, AntWarrior}},
{"�����", Gnoll, Male, Chaotic, Monster, 2, {AxeBattle, Fur}},
{"������", Goblin, Male, Chaotic, Monster, 1, {Strenght, Rock, StuddedLeatherArmor}},
{"��������", Human, Male, Neutral, Commoner},
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
		else
			b++;
	}
	return level + b / 3;
}
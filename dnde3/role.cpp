#include "main.h"

rolei bsmeta<rolei>::elements[] = {{"Гоблин", Goblin, Male, Chaotic, Monster, 0, {SwordShort}},
{"Гоблин", Goblin, Male, Chaotic, Monster, 0, {Rock}},
{"Орк", Orc, Male, Chaotic, Monster, 1, {SwordLong, StuddedLeatherArmor}},
{"Летучая мышь", Animal, Female, Chaotic, Monster, 0, {Bite, Dexterity}},
{"Крыса", Animal, Female, Chaotic, Monster, 0, {Bite, Dexterity}},
{"Крестьянин", Human, Male, Neutral, Commoner, 1},
{"Охранник", Human, Male, Neutral, Fighter, 1, {Spear}},
{"Ребенок", Human, Male, Neutral, Commoner},
{"Крестьянка", Human, Male, Neutral, Commoner, 1},
{"Владелец магазина", Human, Male, Neutral, Commoner, 1, {Bargaining}},
{"Кузнец", Dwarf, Male, Neutral, Commoner, 1, {HammerWar}},
{"Бартендер", Dwarf, Male, Neutral, Commoner, 1, },
{"Скелет", Undead, Male, Chaotic, Monster, 1, {Spear, Dexterity}},
{"Зомби", Undead, Male, Chaotic, Monster, 2, {Dagger, Strenght}},
{"Кобольд", Kobold, Male, Chaotic, Monster, 0, {BowShort, Dagger}},
{"Кобольд-шаман", Kobold, Male, Chaotic, Mage, 3, {Staff, Wand1, MagicMissile}},
{"Собака", Animal, Female, Neutral, Monster, 2, {Bite, Dexterity}},
{"Рысь", Animal, Female, Chaotic, Monster, 4, {Bite, Fur, Dexterity, Strenght, Constitution}},
{"Лягушка", Animal, Female, Neutral, Monster, 1, {Bite, Dexterity}},
{"Муравей", Insect, Male, Chaotic, Monster, 0, {Bite, Hitin}},
{"Муравей-воин", Insect, Male, Chaotic, Monster, 1, {Bite, Hitin, Strenght}},
{"Матка муравьев", Insect, Female, Chaotic, Monster, 5, {Bite, Hitin, Strenght, Strenght, Constitution}, {AntWorker, AntWorker, AntWarrior, AntWarrior}},
{"Гнолл", Gnoll, Male, Chaotic, Monster, 2, {AxeBattle, Fur}},
{"Персонаж", Human, Male, Neutral, Commoner},
};
assert_enum(role, Character);

void creature::create(role_s type) {
	clear();
	auto& ei = bsmeta<rolei>::elements[type];
	this->variant::type = Role;
	this->variant::value = type;
	this->kind = ei.type;
	auto& ci = getclass();
	abilities[Level] = ei.level;
	applyabilities();
	for(auto v : ei.features)
		add(v, 4);
	for(auto i = 1; i < abilities[Level]; i++) {
		if(ci.hp)
			add(LifePoints, xrand(1, ci.hp));
		if(ci.mp)
			add(ManaPoints, xrand(1, getclass().mp));
		raiseskills();
	}
	finish();
}
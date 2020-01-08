#include "main.h"

static creature* create(location& loc, race_s race, gender_s gender, class_s cls) {
	auto p = loc.add(loc.positions[1], race, gender, cls);
	p->add(Friendly, 1, false);
	return p;
}

static creature* create(location& loc, role_s type) {
	auto p = loc.add(loc.get(10, 10), type);
	p->add(Hostile, 1, false);
	return p;
}

static void create(creature* p1, item_s type) {
	item it;
	it.create(type, 3, 20, 6, 30);
	it.set(KnownPower);
	p1->add(it, true, false);
}

static void create(creature* p1, item_s type, variant effect) {
	item it;
	it.create(type, 5, 20, 6, 40);
	it.set(KnownPower);
	it.seteffect(effect);
	p1->add(it, true, false);
}

static void test_worldmap() {
	location loc;
	loc.clear();
	indext position = loc.get(10, 10);
	auto i0 = loc.get(5, 4);
	auto i1 = loc.get(5, 5);
	auto i2 = loc.get(5, 6);
	loc.set(loc.get(0, 0), Sea, mmx, 2);
	loc.set(loc.get(0, 0), Sea, 2, mmy);
	loc.set(loc.get(mmx - 2, 0), Sea, 2, mmy);
	loc.set(loc.get(0, mmy - 2), Sea, mmx, 2);
	loc.set(i0, Mountains);
	loc.set(i0 + 1, Mountains);
	loc.set(i1, Sea);
	loc.set(i1 + 1, Sea);
	loc.set(i1 - 1, Sea);
	loc.set(i2, Sea);
	loc.write("test.loc");
	loc.editor();
}

static bool test_spells() {
	auto& loc = bsmeta<spelli>::elements[0];
	return loc.target.type != Creature;
}

static void test_answers() {
	sb.addn("##Создание персонажа");
	sb.addn("У вас есть преимущество при выборе данных этого элемента.");
	answeri an;
	an.add(1, "Тестовый выбор для строки");
	an.add(1, "Тестовый выбор для второй строки");
	an.choosev(true, false, true, sb);
}

static void modify_weapon(creature* p1) {
	item* pi = (item*)&p1->get(Melee);
	pi->seteffect(Attack);
	pi->set(Cursed);
	//pi->setquality(3);
	//pi->setidentify(1);
}

static void test_indoor() {
	loc.clear();
	loc.positions[1] = loc.get(3, 3);
	loc.building(loc.get(5, 5), 7, 5);
	loc.lake(10, 10, 20, 20);
	loc.drop(loc.get(5, 4), SwordShort);
	loc.drop(loc.get(4, 6), SwordTwoHanded);
	loc.drop(loc.get(3, 3), Staff);
	loc.set(loc.get(4, 3), Tree);
	loc.set(loc.get(2, 6), Hill);
	loc.set(loc.get(3, 7), Hill);
	loc.set(loc.get(3, 8), Water);
	loc.set(loc.get(4, 8), Water);
	//loc.set(loc.get(5, 8), Water);
	loc.set(loc.get(3, 7), Hill);
	loc.set(loc.get(3, 8), Altar);
	loc.set(loc.get(3, 5), Plants);
	loc.set(loc.get(2, 6), Blooded); loc.set(loc.get(3, 6), Blooded); loc.set(loc.get(4, 6), Blooded);
	loc.set(loc.get(3, 6), Webbed); loc.set(loc.get(4, 6), Webbed); loc.set(loc.get(4, 7), Webbed);
	auto p1 = create(loc, Human, Male, Ranger);
	auto p2 = create(loc, Dwarf, Male, Cleric);
	auto p3 = create(loc, Elf, Male, Fighter);
	create(loc, GoblinWarrior);
	create(loc, GoblinWarrior);
	create(loc, GnollWarrior);
	p1->activate();
	p1->damage(6, Bludgeon, 100);
	modify_weapon(p1);
	create(p1, Potion1);
	create(p1, Potion1, Dexterity);
	create(p1, BracersLeather);
	create(p1, Boot1);
	create(p1, Helmet);
	create(p1, RingRed);
	create(p1, RingBlue);
	modify_weapon(p1);
	game.play();
}

static void test_dungeon() {
	loc.clear();
	loc.level = 5;
	loc.create(true, false);
	auto p1 = create(loc, Human, Male, Ranger);
	auto p2 = create(loc, Dwarf, Male, Cleric);
	auto p3 = create(loc, Elf, Male, Fighter);
	p1->activate();
	modify_weapon(p1);
	create(p1, Potion1);
	create(p1, Potion1, Dexterity);
	create(p1, BracersLeather);
	create(p1, Boot1);
	create(p1, Helmet);
	create(p1, RingRed);
	create(p1, RingBlue);
	game.play();
}

static void item_choose() {
	auto p1 = bsmeta<creature>::addz();
	p1->create(Human, Male, Theif);
	p1->inventory();
}

static bool test_formula() {
	creature c1;
	c1.create(GnollWarrior);
	c1.set(Strenght, 18);
	c1.set(Dexterity, 8);
	c1.set(Athletics, 4);
	auto r = c1.calculate(bsmeta<abilityi>::elements[Speed].formula);
	return r == 102;
}

int main(int argc, char* argv[]) {
	auto s1 = sizeof(outdoor);
	auto s2 = sizeof(creature);
	auto s3 = sizeof(item);
	if(!test_formula())
		return false;
	game.intialize();
	//game.setnextlayer(test_worldmap);
	//game.layer();
	test_spells();
	//test_answers();
	//item_choose();
	//test_worldmap();
	//test_indoor();
	test_dungeon();
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	srand((unsigned)time(0));
	//srand(12200);
	return main(0, 0);
}
#include "main.h"

static creature* create(location& loc, race_s race, gender_s gender, class_s cls) {
	auto p = loc.add(loc.get(3,3), race, gender, cls);
	p->set(Friendly);
	return p;
}

static creature* create(location& loc, role_s type) {
	auto p = loc.add(loc.get(10, 10), type);
	p->set(Hostile);
	return p;
}

static void create(creature* p1, item_s type) {
	item it(type, 5, 20, 6, 40);
	it.setidentify(true);
	p1->add(it, true, false);
}

static void test_worldmap() {
	location e;
	e.clear();
	indext position = e.get(10, 10);
	auto i0 = e.get(5, 4);
	auto i1 = e.get(5, 5);
	auto i2 = e.get(5, 6);
	e.set(e.get(0, 0), Sea, mmx, 2);
	e.set(e.get(0, 0), Sea, 2, mmy);
	e.set(e.get(mmx - 2, 0), Sea, 2, mmy);
	e.set(e.get(0, mmy - 2), Sea, mmx, 2);
	e.set(i0, Mountains);
	e.set(i0 + 1, Mountains);
	e.set(i1, Sea);
	e.set(i1 + 1, Sea);
	e.set(i1 - 1, Sea);
	e.set(i2, Sea);
	e.write("test.loc");
	e.editor();
}

static bool test_spells() {
	auto& e = bsmeta<spelli>::elements[0];
	return e.target.type != Creature;
}

static void test_answers() {
	sb.addn("##Создание персонажа");
	sb.addn("У вас есть преимущество при выборе данных этого элемента.");
	answeri an;
	an.add(1, "Тестовый выбор для строки");
	an.add(1, "Тестовый выбор для второй строки");
	an.choosev(true, false, true, sb);
}

static void random_bless(creature* p1) {
	itema source;
	source.selecta(*p1);
	source[1]->set(Blessed);
	source[1]->setidentify(true);
}

static void test_indoor() {
	location e;
	e.clear();
	e.building(e.get(5, 5), 7, 5);
	e.lake(10, 10, 20, 20);
	e.drop(e.get(5, 4), SwordShort);
	e.drop(e.get(4, 6), SwordTwoHanded);
	e.drop(e.get(3, 3), Staff);
	e.drop(e.get(3, 3), LeatherArmor);
	e.set(e.get(2, 2), Tree);
	e.set(e.get(4, 3), Tree);
	e.set(e.get(2, 6), Hill);
	e.set(e.get(3, 7), Hill);
	e.set(e.get(3, 8), Water);
	e.set(e.get(4, 8), Water);
	//e.set(e.get(5, 8), Water);
	e.set(e.get(3, 7), Hill);
	e.set(e.get(3, 8), Altar);
	e.set(e.get(3, 5), Plants);
	e.set(e.get(2, 6), Blooded); e.set(e.get(3, 6), Blooded); e.set(e.get(4, 6), Blooded);
	e.set(e.get(3, 6), Webbed); e.set(e.get(4, 6), Webbed); e.set(e.get(4, 7), Webbed);
	auto p1 = create(e, Human, Male, Ranger);
	auto p2 = create(e, Dwarf, Male, Cleric);
	auto p3 = create(e, Elf, Male, Fighter);
	create(e, GoblinWarrior);
	create(e, GoblinWarrior);
	create(e, GnollWarrior);
	random_bless(p1);
	e.activate();
	p1->activate();
	p1->damage(6, Bludgeon, 100);
	create(p1, Potion1);
	create(p1, BracersLeather);
	create(p1, Boot1);
	create(p1, Helmet);
	game.play();
}

static void item_choose() {
	auto p1 = bsmeta<creature>::addz();
	p1->create(Human, Male, Theif);
	random_bless(p1);
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
	test_indoor();
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	//srand((unsigned)time(0));
	srand(12200);
	return main(0, 0);
}
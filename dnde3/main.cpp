#include "main.h"

static creature* create(race_s race, gender_s gender, class_s cls) {
	auto p = loc.add(loc.positions[1], race, gender, cls);
	p->add(Friendly, 1, false);
	return p;
}

static creature* create(role_s type) {
	auto p = loc.add(loc.positions[2], type);
	p->add(Hostile, 1, false);
	return p;
}

static void create(creature* p1, item_s type) {
	item it;
	it.create(type, 3, 20, 6, 30);
	//it.set(KnownPower);
	p1->equip(it);
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
	//pi->decoy(Bludgeon, true);
	//pi->set(Cursed);
	//pi->setquality(3);
	//pi->setidentify(1);
}

static creature* create_indoor(bool enemies = true) {
	static slot_s weapons[] = {Melee, Ranged};
	loc.clear();
	loc.positions[1] = loc.get(3, 3);
	loc.positions[2] = loc.get(12, 12);
	if(true) {
		rect rc = {5, 5, 16, 11};
		auto door = loc.building(rc);
		rc.offset(1, 1);
		//loc.set(door, Sealed);
		loc.loot(rc, weapons, 80, 30, 0, KnownPower);
	}
	loc.lake(10, 10, 20, 20);
	loc.drop(loc.get(5, 4), item(SwordShort, 5));
	loc.drop(loc.get(4, 6), item(SwordTwoHanded, 10));
	loc.drop(loc.get(3, 3), item(Staff, 20));
	loc.set(loc.get(4, 3), Tree);
	loc.set(loc.get(2, 6), Hill);
	loc.set(loc.get(3, 7), Hill);
	loc.set(loc.get(3, 8), Water);
	loc.set(loc.get(3, 7), Hill);
	loc.set(loc.get(3, 8), Altar);
	loc.set(loc.get(3, 5), Plants);
	loc.set(loc.get(2, 6), Blooded); loc.set(loc.get(3, 6), Blooded); loc.set(loc.get(4, 6), Blooded);
	loc.set(loc.get(3, 6), Webbed); loc.set(loc.get(4, 6), Webbed); loc.set(loc.get(4, 7), Webbed);
	auto p1 = create(Human, Male, Theif);
	auto p2 = create(Dwarf, Male, Cleric);
	auto p3 = create(Elf, Male, Fighter);
	if(enemies) {
		create(GoblinWarrior);
		create(GoblinWarrior);
		create(GnollWarrior);
	}
	p1->activate();
	p1->damage(6, Bludgeon, 100);
	create(p1, Potion1);
	create(p1, Potion1, Dexterity);
	create(p1, BracersLeather);
	create(p1, Boot1);
	create(p1, Helmet);
	create(p1, RingRed);
	create(p1, RingBlue);
	return p1;
}

static void test_indoor(bool enemies = true) {
	create_indoor(enemies);
	game.play();
}

static void test_analize() {
	auto p1 = create_indoor();
}

static void test_stack(creature* p1) {
	item i1(Arrow, 1);
	item i2 = i1;
	i1.setcount(40000);
	i2.setcount(40000);
	p1->add(i1, true, false);
	p1->add(i2, true, false);
}

static void test_pause() {
	sb.add("Текст, который написан и который будет выведен с паузой.");
	creature::pause();
}

static void test_dungeon() {
	loc.clear();
	loc.level = 1;
	loc.create(false, false);
	loc.setdungeon(true);
	loc.setlight(-2);
	auto p1 = create(Human, Male, Theif);
	auto p2 = create(Dwarf, Male, Cleric);
	auto p3 = create(Human, Male, Fighter);
	p1->activate();
	create(p1, Potion3, Level);
	create(p1, Potion3, Level);
	create(p1, Amulet1, Lockpicking);
	create(p1, Boot1);
	create(p1, Helmet);
	create(p1, RingRed);
	create(p1, Wand1, Sleep);
	create(p1, Book1);
	create(p3, BracersLeather);
	create(p3, Wand1);
	modify_weapon(p1);
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
	//test_answers();
	//item_choose();
	//test_worldmap();
	test_indoor(true);
	//test_analize();
	//test_dungeon();
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	srand((unsigned)time(0));
	//srand(12200);
	return main(0, 0);
}
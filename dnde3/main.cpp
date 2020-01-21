#include "main.h"

static creature* create(race_s race, gender_s gender, class_s cls) {
	auto p = loc.add(loc.positions[0], race, gender, cls);
	p->add(Friendly, 1, false);
	return p;
}

static creature* create(role_s type) {
	auto p = loc.add(loc.positions[1], type);
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
	auto i0 = loc.get(5, 4);
	auto i1 = loc.get(5, 5);
	auto i2 = loc.get(5, 6);
	loc.clear();
	loc.fill({0, 0, mmx - 1, 1}, Sea);
	loc.fill({0, 0, 1, mmy - 1}, Sea);
	loc.fill({mmx - 2, 0, mmx - 1, mmy - 1}, Sea);
	loc.fill({0, mmy - 2, mmx - 1, mmy - 1}, Sea);
	loc.set(i0, Mountains);
	loc.set(i0 + 1, Mountains);
	loc.set(i1, Sea);
	loc.set(i1 + 1, Sea);
	loc.set(i1 - 1, Sea);
	loc.set(i2, Sea);
	loc.write("game/overland.loc", true);
	loc.editor();
}

static void test_adventure() {
	loc.read("game/overland.loc", true);
	auto p1 = create(Elf, Female, Mage);
	auto p2 = create(Dwarf, Male, Cleric);
	auto p3 = create(Elf, Male, Fighter);
	p1->activate();
	game.move(loc.get(10, 10));
	p1->playuioverland();
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

static void create_indoor(landscape_s area) {
	if(!game.read()) {
		game.enter(loc.get(20, 20), 1, StairsDown);
		auto p1 = create(Elf, Female, Mage);
		auto p2 = create(Dwarf, Male, Cleric);
		auto p3 = create(Elf, Male, Fighter);
		p1->activate();
		p1->damage(6, Bludgeon, 100);
		create(p1, Potion2, PoisonSpell);
	}
	game.play();
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
	static dungeoni dungeon = {AreaDungeon, 5, -2};
	loc.create(dungeon, 1, true, false);
	loc.setdungeon(true);
	loc.setlight(-2);
	auto p1 = create(Human, Female, Mage);
	auto p2 = create(Dwarf, Male, Cleric);
	auto p3 = create(Human, Male, Fighter);
	p1->activate();
	create(p1, Potion3, Level);
	create(p1, Amulet1, Lockpicking);
	create(p1, Boot1);
	create(p1, Helmet);
	create(p1, RingRed);
	create(p1, Wand1, Sleep);
	create(p1, Book1);
	create(p1, Guitar, Damage);
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

void util_main();

int main(int argc, char* argv[]) {
	util_main();
	if(!test_formula())
		return false;
	game.intialize();
	//test_answers();
	//item_choose();
	//test_worldmap();
	test_adventure();
	//test_analize();
	//test_dungeon();
	//create_indoor(AreaCity);
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	srand((unsigned)time(0));
	//srand(12200);
	return main(0, 0);
}
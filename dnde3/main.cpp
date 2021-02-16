#include "main.h"

static creature* create(race_s race, gender_s gender, class_s cls) {
	auto p = loc.add(Blocked, race, gender, cls);
	p->add(variant(Friendly), 1, false);
	return p;
}

static void create(creature* p1, item_s type) {
	item it;
	it.create(type, 3, 20, 6);
	p1->equip(it);
}

static void createp(creature* p1, item_s type) {
	item it;
	it.create(type, 3, 20, 6);
	it.setpersonal(true);
	p1->equip(it);
}

static void create(creature* p1, item_s type, variant effect) {
	item it;
	it.create(type, 5, 20, 6);
	it.set(KnownPower);
	it.seteffect(effect);
	p1->add(it, true, false);
}

static void test_adventure() {
	bsdata<outdoori>::elements[CityMeher].index = loc.get(4, 8);
	if(!game.read()) {
		auto p1 = create(Elf, Female, Mage);
		auto p2 = create(Dwarf, Male, Cleric);
		auto p3 = create(Elf, Male, Fighter);
		create(p1, AlchemyReceipt);
		create(p1, AlchemySet);
		createp(p1, Staff);
		p1->activate();
		game.setposition(loc.get(8, 8));
		if(!game.enter(0, NoTileObject))
			return;
	}
	game.play();
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
	loc.create(dungeon, 1, true);
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
	auto p1 = bsdata<creature>::addz();
	p1->create(Human, Male, Theif);
	p1->inventory();
}

static bool test_formula() {
	creature c1;
	auto s1 = sizeof(c1);
	c1.create(GnollWarrior);
	c1.set(Dexterity, 7);
	c1.set(Athletics, 12);
	auto r = c1.calculate(bsdata<abilityi>::elements[Speed].formula);
	return r == 99;
}

static void continue_game() {
	if(!game.read())
		return;
	game.play();
}

static void random_landscape() {
	auto i1 = loc.get(10, 10);
	loc.generate(i1, Forest, 70, 10 * 12);
	loc.generate(i1, Mountains, 50, 10 * 8);
	loc.generate(i1, CloudPeaks, 70, 10);
	loc.choose(i1, false, true, true);
}

static void create_overland() {
	loc.clear();
	loc.fill({0, 0, mmx - 1, 1}, Sea);
	loc.fill({0, 0, 1, mmy - 1}, Sea);
	loc.fill({mmx - 2, 0, mmx - 1, mmy - 1}, Sea);
	loc.fill({0, mmy - 2, mmx - 1, mmy - 1}, Sea);
}

static void begin_game() {
	if(!game.read()) {
		create_overland();
		auto p1 = create(Elf, Female, Mage);
		auto p2 = create(Dwarf, Male, Cleric);
		auto p3 = create(Ratling, Male, Fighter);
		create(p1, AlchemyReceipt);
		p1->raise(Dancing);
		p1->add(variant(FireBall), 1, false);
		create(p2, Potion1, DrunkenSpell);
		create(p1, AlchemySet);
		createp(p1, Staff);
		p1->addexp(10000, false);
		game.setposition(loc.get(8, 8));
		if(!game.enter(0, NoTileObject))
			return;
	}
	game.play();
}

static void run_editor() {
	if(!loc.read("game/overland.loc", true))
		create_overland();
	loc.editor();
	loc.write("game/overland.loc", true);
}

static void exit_game() {
	exit(0);
}

static void test_game() {
	game.begin();
}

static stageproc choose_stage() {
	answeri an;
	an.add((int)begin_game, "Начать игру");
	an.add((int)continue_game, "Продолжить игру");
	an.add((int)run_editor, "Редактор карт");
	an.add((int)test_game, "Тестировать");
	an.add(0, "Выйти из игры");
	return (stageproc)an.menuv(true, "Главное меню");
}

static void main_menu() {
	while(true) {
		auto stage = choose_stage();
		if(!stage)
			break;
		stage();
		game.setnobackground();
	}
}

void util_main();

int main(int argc, char* argv[]) {
	if(!test_formula())
		return false;
	game.intialize();
	util_main();
	main_menu();
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	srand((unsigned)time(0));
	//srand(12200);
	return main(0, 0);
}
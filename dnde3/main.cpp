#include "main.h"

static void test_worldmap() {
	location e;
	e.clear();
	indext position = e.get(10, 10);
	auto i0 = e.get(5, 4);
	auto i1 = e.get(5, 5);
	auto i2 = e.get(5, 6);
	e.fill({0, 0, mmx, 1}, Sea);
	e.fill({0, 0, 1, mmy}, Sea);
	e.fill({mmx-2, 0, mmx, mmy}, Sea);
	e.fill({0, mmy-2, mmx, mmy}, Sea);
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
	answeri an;
	an.add(1, "Тестовый выбор для строки");
	an.add(1, "Тестовый выбор для второй строки");
	an.choosev(true, false, true, "##Создание персонажа\nУ вас есть преимущество при выборе данных этого элемента.");
}

static void test_indoor() {
	location e;
	e.clear();
	e.fill({5, 5, 10, 8}, Wall);
	e.drop(e.get(4, 3), SwordShort);
	e.drop(e.get(3, 3), Staff);
	e.drop(e.get(3, 3), LeatherArmour);
	e.adventure();
}

int main(int argc, char* argv[]) {
	auto s = sizeof(outdoor);
	game.intialize();
	//game.setnextlayer(test_worldmap);
	//game.layer();
	test_spells();
	//test_answers();
	test_indoor();
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	srand((unsigned)time(0));
	//srand(1000);
	return main(0, 0);
}
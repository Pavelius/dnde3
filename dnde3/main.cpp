#include "main.h"

static void test_worldmap() {
	location e;
	indext position = e.get(10, 10);
	auto i0 = e.get(5, 4);
	auto i1 = e.get(5, 5);
	auto i2 = e.get(5, 6);
	e.clear();
	e.set(i0, Mountains);
	e.set(i0 + 1, Mountains);
	e.set(i1, Sea);
	e.set(i1 + 1, Sea);
	e.set(i1 - 1, Sea);
	e.set(i2, Sea);
	e.editor();
}

int main(int argc, char* argv[]) {
	game.intialize();
	test_worldmap();
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	srand((unsigned)time(0));
	//srand(1000);
	return main(0, 0);
}
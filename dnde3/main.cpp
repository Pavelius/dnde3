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

static void test_answers() {
	answeri an;
	an.add(1, "�������� ����� ��� ������");
	an.add(1, "�������� ����� ��� ������ ������");
	an.choosev(true, false, true, "##�������� ���������\n� ��� ���� ������������ ��� ������ ������ ����� ��������.");
}

int main(int argc, char* argv[]) {
	auto s = sizeof(outdoor);
	game.intialize();
	//game.setnextlayer(test_worldmap);
	//game.layer();
	test_answers();
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	srand((unsigned)time(0));
	//srand(1000);
	return main(0, 0);
}
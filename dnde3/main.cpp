#include "main.h"

int main(int argc, char* argv[]) {
	game.intialize();
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	srand((unsigned)time(0));
	//srand(1000);
	return main(0, 0);
}
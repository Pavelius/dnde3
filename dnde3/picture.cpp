#include "main.h"

void picture::set(indext i) {
	set(location::getx(i), location::gety(i));
}

void picture::setcursor(indext i, int size) {
	clear();
	set(i);
	img = ResUI;
	frame = size;
}
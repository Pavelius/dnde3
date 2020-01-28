#include "main.h"

short unsigned deck::take() {
	auto r = data[0];
	remove(0, 1);
	return r;
}

void deck::drop(short unsigned v) {
	add(v);
}
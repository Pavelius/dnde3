#include "main.h"

struct intellegencei {
	const char*		name;
	dicei			range;
	intellegence_s	getid() const { return intellegence_s(this - bsmeta<intellegencei>::elements); }
};

BSDATA(intellegencei) = {{"���������� ��������", {0}},
{"��������", {1, 1}},
{"������������ ��������", {2, 4}},
{"������ ��������", {5, 7}},
{"��� ��� ������", {8, 10}},
{"����� ��������", {11, 12}},
{"������ ���������������� ���������", {13, 14}},
{"������������� ���������������� ���������", {15, 16}},
{"���������� ���������", {17, 18}},
{"����� ���������� ���������", {19, 21}},
{"����������� ���������������� ���������", {22, 100}},
};
assert_enum(intellegence, GodInt)

intellegence_s creature::getint() const {
	auto v = get(Intellegence);
	for(auto& e : bsmeta<intellegencei>()) {
		if(v <= e.range.min)
			return e.getid();
	}
	return GodInt;
}

bool creature::is(intellegence_s v) const {
	auto i = get(Intellegence);
	auto& e = bsmeta<intellegencei>::elements[v];
	return i >= e.range.min;
}
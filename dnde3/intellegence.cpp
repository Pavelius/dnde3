#include "main.h"

struct intellegencei {
	const char*		name;
	dicei			range;
	intellegence_s	getid() const { return intellegence_s(this - bsmeta<intellegencei>::elements); }
};

BSDATA(intellegencei) = {{"неразумное существо", {0}},
{"животное", {1, 1}},
{"полуразумное существо", {2, 4}},
{"глупое существо", {5, 7}},
{"как все вокруг", {8, 10}},
{"умное существо", {11, 12}},
{"высоко интеллектуальный индивидум", {13, 14}},
{"исключительно интеллектуальный индивидум", {15, 16}},
{"гениальный индивидум", {17, 18}},
{"супер гениальный индивидум", {19, 21}},
{"божественно интеллектуальный индивидум", {22, 100}},
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
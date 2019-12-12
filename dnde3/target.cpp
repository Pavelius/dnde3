#include "main.h"

targeti::targeti(const std::initializer_list<variant>& source) : targeti() {
	for(auto& e : source) {
		switch(e.type) {
		case Alignment:
			alignments.set(e.value);
			break;
		case Race:
			races.set(e.value);
			break;
		case Range:
			range = range_s(e.value);
			break;
		case State:
			states.set(e.value);
			break;
		case Target:
			target = target_s(e.value);
			break;
		case Variant:
			type = variant_s(e.value);
			break;
		}
	}
}
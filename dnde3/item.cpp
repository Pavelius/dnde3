#include "main.h"

itemi bsmeta<itemi>::elements[] = {{""},
{"Боевой топор", 850, 5 * GP, Male, Iron},
{"Дубина"},
{"Кинжал"},
{"Молото"},
{"Булава"},
};

creature* item::getwearer() const {
	auto i = bsmeta<creature>::source.indexof((creature*)(this));
	if(i == -1)
		return 0;
	return (creature*)bsmeta<creature>::source.ptr(i);
}
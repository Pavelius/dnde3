#include "main.h"

creature* item::getwearer() const {
	auto i = bsmeta<creature>::source.indexof((creature*)(this));
	if(i == -1)
		return 0;
	return (creature*)bsmeta<creature>::source.ptr(i);
}
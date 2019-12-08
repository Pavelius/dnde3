#include "main.h"

variant::variant(const creature* v) : type(v ? Creature : NoVariant), value(v - bsmeta<creature>::elements)
{}
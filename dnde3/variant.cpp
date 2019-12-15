#include "main.h"

variant::variant(const creature* v) : type(v ? Creature : NoVariant), value(v - bsmeta<creature>::elements)
{}

const char*	variant::getname() const {
	switch(type) {
	case Ability: return bsmeta<abilityi>::elements[value].name;
	case Skill: return bsmeta<skilli>::elements[value].name;
	default: return "Нет варианта";
	}
}
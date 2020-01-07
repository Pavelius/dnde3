#include "main.h"

variant::variant(const creature* v) : type(v ? Creature : NoVariant), value(v - bsmeta<creature>::elements)
{}

const char*	variant::getname() const {
	switch(type) {
	case Ability: return bsmeta<abilityi>::elements[value].name;
	case Role: return bsmeta<rolei>::elements[value].name;
	case Skill: return bsmeta<skilli>::elements[value].name;
	default: return "Нет варианта";
	}
}

const char*	variant::getnameof() const {
	switch(type) {
	case Ability: return bsmeta<abilityi>::elements[value].nameof;
	case State: return bsmeta<statei>::elements[value].name;
	case Skill: return bsmeta<skilli>::elements[value].name_tome;
	default: return "Нет варианта";
	}
}

const char*	variant::getnameofc() const {
	switch(type) {
	case Ability: return bsmeta<abilityi>::elements[value].cursedof;
	case State: return bsmeta<statei>::elements[value].nameof;
	case Skill: return bsmeta<skilli>::elements[value].name_tome;
	default: return "Нет варианта";
	}
}
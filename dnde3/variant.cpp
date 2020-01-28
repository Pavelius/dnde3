#include "main.h"

variant::variant(const creature* v) : type(v ? Creature : NoVariant), value(v - bsmeta<creature>::elements)
{}

const char*	variant::getname() const {
	switch(type) {
	case Ability: return bsmeta<abilityi>::elements[value].name;
	case Object: return bsmeta<map_objecti>::elements[value].name;
	case Race: return bsmeta<racei>::elements[value].name;
	case Role: return bsmeta<rolei>::elements[value].name;
	case Skill: return bsmeta<skilli>::elements[value].name;
	case Spell: return bsmeta<spelli>::elements[value].name;
	case State: return bsmeta<statei>::elements[value].name;
	default: return "Нет варианта";
	}
}

const char*	variant::getnameof() const {
	switch(type) {
	case Ability: return bsmeta<abilityi>::elements[value].nameof;
	case Skill: return bsmeta<skilli>::elements[value].name_tome;
	case Spell: return bsmeta<spelli>::elements[value].nameof;
	case State: return bsmeta<statei>::elements[value].nameof;
	default: return "Нет варианта";
	}
}

const char*	variant::getnameofc() const {
	switch(type) {
	case Ability: return bsmeta<abilityi>::elements[value].cursedof;
	case Skill: return bsmeta<skilli>::elements[value].name_tome;
	case State: return bsmeta<statei>::elements[value].nameof;
	default: return "Нет варианта";
	}
}
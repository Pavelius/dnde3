#include "main.h"

variant::variant(const creature* v) : type(v ? Creature : NoVariant), value(v - bsdata<creature>::elements)
{}

const char*	variant::getname() const {
	switch(type) {
	case Ability: return bsdata<abilityi>::elements[value].name;
	case God: return bsdata<dietyi>::elements[value].name;
	case Object: return bsdata<map_objecti>::elements[value].name;
	case Race: return bsdata<racei>::elements[value].name;
	case Role: return bsdata<rolei>::elements[value].name;
	case Skill: return bsdata<skilli>::elements[value].name;
	case Spell: return bsdata<spelli>::elements[value].name;
	case State: return bsdata<statei>::elements[value].name;
	default: return "Нет варианта";
	}
}

const char*	variant::getnameof() const {
	switch(type) {
	case Ability: return bsdata<abilityi>::elements[value].nameof;
	case God: return bsdata<dietyi>::elements[value].nameof;
	case Role: return bsdata<rolei>::elements[value].nameof;
	case Skill: return bsdata<skilli>::elements[value].nameof;
	case Spell: return bsdata<spelli>::elements[value].nameof;
	case State: return bsdata<statei>::elements[value].nameof;
	default: return "Нет варианта";
	}
}

const char*	variant::getnameofc() const {
	switch(type) {
	case Ability: return bsdata<abilityi>::elements[value].cursedof;
	case Skill: return bsdata<skilli>::elements[value].nameof;
	case State: return bsdata<statei>::elements[value].nameof;
	default: return "Нет варианта";
	}
}
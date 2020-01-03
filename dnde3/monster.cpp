#include "main.h"

void creature::create(role_s type) {
	clear();
	this->role = type;
	this->type = Fighter;
	abilities[Level] = 1;
	applyabilities();
	if(abilities[Intellegence] >= 9)
		raise(Literacy);
	for(auto e : getclass().spells)
		set(e, 1);
	// Повысим навыки
	auto skill_checks = get(Intellegence) / 2;
	raiseskills(skill_checks);
	// Восполним хиты
	add(LifePoints, getclass().hp);
	add(ManaPoints, getclass().mp);
	// Generate name
	hp = get(LifePoints);
	mp = get(ManaPoints);
}
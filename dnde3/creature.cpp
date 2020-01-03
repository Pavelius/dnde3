#include "main.h"

DECLDATA(creature, 256);

static int			skill_level[] = {20, 50, 75, 90};
static const char*	skill_names[] = {"Начальный", "Продвинутый", "Экспертный", "Мастерский"};
static creature*	current_player;

void creature::clear() {
	memset(this, 0, sizeof(*this));
	charmer = horror = Blocked;
	guard = Blocked;
	location_id = Blocked;
	site_id = Blocked;
}

creature* creature::getactive() {
	return current_player;
}

short unsigned creature::getid() const {
	if(!this)
		return Blocked;
	return this - bsmeta<creature>::elements;
}

void creature::add(variant id, int v) {
	switch(id.type) {
	case Ability: abilities[id.value] += v; break;
	case Item: equip(item_s(id.value)); break;
	}
}

void creature::delayed(variant id, int modifier, unsigned rounds) {
	auto p = bsmeta<boosti>::add();
	p->id = id;
	p->modifier = modifier;
	p->time = rounds;
	p->owner = getid();
}

void creature::add(variant id, int v, unsigned time) {
	add(id, v);
	delayed(id, -v, time);
}

creature* creature::getobject(short unsigned v) {
	if(v == Blocked)
		return 0;
	return bsmeta<creature>::elements + v;
}

void creature::isolate() {
	auto id = getid();
	for(auto& e : bsmeta<creature>()) {
		if(!e)
			continue;
		if(e.charmer == id)
			e.charmer = 0;
		if(e.horror == id)
			e.horror = 0;
	}
}

void creature::dressoff() {
	if(!this)
		return;
	dresswp(-1);
	dresswr(-1);
}

void creature::dresson() {
	if(!this)
		return;
	dresswr(1);
	dresswp(1);
}

void creature::dresswp(int m) {
	abilities[AttackMelee] += m * wears[Melee].getmagic() * 3;
	abilities[DamageMelee] += m * (wears[Melee].getmagic() / 2);
	abilities[AttackRanged] += m * wears[Ranged].getmagic() * 4;
	abilities[DamageRanged] += m * (wears[Ranged].getmagic() / 2);
}

void creature::dresswr(int m) {
	for(auto i = Head; i <= Legs; i = (slot_s)(i + 1)) {
		if(!wears[i])
			continue;
		auto mi = wears[i].getmagic();
		auto& ei = wears[i].getitem();
		if(i != Melee && i != Ranged) {
			auto a = m * ei.weapon.attack;
			abilities[AttackMelee] += a;
			abilities[AttackRanged] += a;
		}
		abilities[Deflect] += m * (ei.armor.deflect + mi*ei.armor.multiplier);
		abilities[Armor] += m * ei.armor.armor;
	}
}

const char* creature::getlevelname(skill_s v) const {
	auto n = getlevel(v);
	return maptbl(skill_names, n);
}

int creature::getlevel(skill_s v) const {
	auto r = skills[v];
	auto n = 0;
	for(auto a : skill_level) {
		if(r < a)
			break;
		n++;
	}
	return n;
}

dice_s creature::getraise(skill_s v) const {
	dice_s source[] = {D3n13, D1n6, D1n4};
	auto n = getlevel(v);
	return maptbl(source, n);
}

void creature::raise(skill_s value) {
	const auto& dice = bsmeta<dicei>::elements[getraise(value)];
	skills[value] += dice.roll();
}

void creature::equip(item it, slot_s id) {
	dressoff();
	wears[id] = it;
	dresson();
}

bool creature::add(item v, bool run) {
	// Second place item to backpack
	for(auto i = FirstBackpack; i <= LastBackpack; i = (slot_s)(i + 1)) {
		if(wears[i])
			continue;
		if(run)
			equip(v, i);
		return true;
	}
	return false;
}

bool creature::equip(item v) {
	// First try to dress this item
	for(auto i = Head; i <= Amunitions; i = (slot_s)(i + 1)) {
		if(!v.is(i))
			continue;
		if(wears[i])
			continue;
		equip(v, i);
		return true;
	}
	return add(v, true);
}

void creature::raiseskills(int number) {
	skill_s source[sizeof(skills) / sizeof(skills[0])];
	auto pb = source;
	for(auto i = Bargaining; i <= TwoWeaponFighting; i = (skill_s)(i + 1)) {
		if(skills[i])
			*pb++ = i;
	}
	unsigned count = pb - source;
	if(!count)
		return;
	zshuffle(source, count);
	unsigned index = 0;
	while(number >= 0) {
		if(index >= count)
			index = 0;
		raise(source[index++]);
		number--;
	}
}

static spell_s choose_spells(creature* p) {
	spell_s source[64];
	auto pb = source;
	auto pe = pb + sizeof(source) / sizeof(source[0]);
	for(auto e = FirstSpell; e <= LastSpell; e = (spell_s)(e + 1)) {
		if(p->get(e))
			continue;
		if(pb < pe)
			*pb++ = e;
	}
	auto count = pb - source;
	if(!count)
		FirstSpell;
	return source[rand() % count];
}

static void start_equipment(creature& e) {
	for(auto& ei : bsmeta<equipmenti>()) {
		if((!ei.race || ei.race == e.getrace()) && e.is(ei.type)) {
			for(auto ef : ei.features)
				e.add(ef, 1);
			break;
		}
	}
	e.equip(Ration);
	e.setmoney(e.getmoney() + xrand(3, 18)*GP);
}

void creature::applyabilities() {
	const auto& ri = bsmeta<racei>::elements[getrace()];
	const auto& ci = getclass();
	// Generate abilities
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1))
		abilities[i] = ri.abilities[i] + (rand() % 5) - 3;
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1))
		abilities[i] += ci.ability[i];
	for(auto i = Strenght; i <= Charisma; i = (ability_s)(i + 1)) {
		if(abilities[i] < 0)
			abilities[i] = 0;
	}
	// Generate skills
	for(auto e : ri.skillvs)
		skills[e.id] += e.value;
	for(auto e : ri.skills)
		raise(e);
	for(auto e : ci.skills)
		raise(e);
	dresson();
}

void creature::create(race_s race, gender_s gender, class_s type) {
	clear();
	this->type = type;
	role = Character;
	abilities[Level] = 1;
	setname(race, gender);
	applyabilities();
	if(abilities[Intellegence] >= 9)
		raise(Literacy);
	for(auto e : getclass().spells)
		set(e, 1);
	start_equipment(*this);
	// Повысим навыки
	raise(UnarmedFighting);
	raise(Climbing);
	auto skill_checks = get(Intellegence) / 2;
	raiseskills(skill_checks);
	// Восполним хиты
	add(LifePoints, getclass().hp);
	add(ManaPoints, getclass().mp);
	// Generate name
	hp = get(LifePoints);
	mp = get(ManaPoints);
}

void creature::getfullname(stringbuilder& sb) const {
	sb.add(getname());
	sb.adds("%-3-%-1 %2i уровня", getstr(type), get(Level), getstr(getrace()));
}

attacki creature::getattack(slot_s id) const {
	attacki result = {0};
	auto& weapon = wears[id];
	auto attack_ability = AttackMelee;
	auto damage_ability = DamageMelee;
	auto skill = weapon.getitem().skill;
	if(id == Ranged) {
		attack_ability = AttackRanged;
		damage_ability = DamageRanged;
	}
	if(weapon) {
		result = weapon.getattack();
		result.dice = bsmeta<dicei>::elements[result.damage];
	} else if(id == Melee) {
		result.type = Bludgeon;
		result.dice.min = 0;
		result.dice.max = 4;
	}
	if(!result.dice.max)
		return result;
	result.attack += bsmeta<skilli>::elements[skill].weapon.get(get(skill)); // Basic chance to hit
	result.attack += get(attack_ability); // Basic chance to hit
	result.dice.max += get(damage_ability);
	// RULE: Versatile weapon if used two-handed made more damage.
	if(id == Melee) {
		result.dice.max += (get(Strenght) - 10) / 2;
		if(weapon.is(Versatile) && !wears[OffHand]) {
			result.dice.min += 1;
			result.dice.max += 1;
		}
	}
	return result;
}

int creature::getbasic(ability_s id) const {
	return abilities[id];
}

void creature::activate() {
	current_player = this;
}

creature* creature::getactive(int n) {
	auto p = bsmeta<creature>::elements + n;
	if(!(*p))
		return 0;
	return p;
}

const variant* creature::calculate(const variant* p, int& result) const {
	switch(p->type) {
	case Ability: result = get((ability_s)p->value); p++; break;
	case Skill: result = get((skill_s)p->value); p++; break;
	default: return 0;
	}
	while(p->type == Formula) {
		switch(p->value) {
		case Negative: result = -result; break;
		case Divide2: result = result / 2; break;
		case Divide3: result = result / 3; break;
		case Divide4: result = result / 4; break;
		default: return 0;
		}
		p++;
	}
	return p;
}

int	creature::calculate(const variant* formula) const {
	int result = 0;
	while(formula) {
		int a = 0;
		formula = calculate(formula, a);
		result += a;
	}
	return result;
}

slot_s creature::getwearerslot(const item* p) const {
	if(this
		&& p >= wears
		&& p < (wears + sizeof(wears) / sizeof(wears[0])))
		return slot_s(p - wears);
	return FirstBackpack;
}

void creature::select(itema& a, slot_s i1, slot_s i2, bool filled_only) {
	auto ps = a.data;
	for(auto i = i1; i <= i2; i = (slot_s)(i + 1)) {
		if(filled_only && !wears[i])
			continue;
		*ps++ = &wears[i];
	}
	a.count = ps - a.data;
}

void creature::dropdown(item& item) {
	if(!remove(item, false))
		return;
	auto p = location::getactive();
	if(p) {
		p->drop(getposition(), item);
		remove(item, true);
		consume(StandartEnergyCost / 4);
	}
}

void creature::dropdown() {
	itema items; items.selectb(*this);
	auto pi = items.choose(true, "Выбросить предмет", 0, NoSlotName);
	if(pi)
		dropdown(*pi);
}

void creature::pickup() {
	itema items; items.select(getposition());
	auto pi = items.choose(true, "Поднять предмет", 0, NoSlotName);
	if(pi) {
		if(add(*pi, true)) {
			pi->clear();
			consume(StandartEnergyCost / 4);
		}
	}
}

void creature::inventory() {
	while(true) {
		itema items; items.select(*this);
		auto pi = items.choose(true, "Инвенторий", 0, SlotName);
		if(!pi)
			break;
		auto slot = pi->getwearerslot();
		if(*pi) {
			if(!remove(*pi, false)) {
				//say("Я не могу это снять.");
				continue;
			}
			if(!add(*pi, false)) {
				//say("У меня уже нету места.");
				continue;
			}
			add(*pi, true);
			remove(*pi, true);
		} else {
			if(slot >= Head && slot <= Amunitions) {
				items.clear();
				items.selectb(*this);
				items.match(slot);
				auto p2 = items.choose(true, "Рюкзак", 0, NoSlotName);
				if(p2) {
					dressoff();
					auto pc = *pi;
					*pi = *p2;
					*p2 = pc;
					dresson();
				}
			}
		}
	}
}

bool creature::remove(item& e, bool run) {
	if(!e)
		return false;
	if(run) {
		dressoff();
		e.clear();
		dresson();
	}
	return true;
}

int	creature::getweight() const {
	auto r = 0;
	for(auto& e : wears)
		r += e.getweight();
	return r;
}

int	creature::get(skill_s v) const {
	return skills[v]
		+ get(bsmeta<skilli>::elements[v].abilities[0])
		+ get(bsmeta<skilli>::elements[v].abilities[1]);
}

void creature::select(skilla& a) const {
	auto ps = a.data;
	for(auto i = FirstSkill; i <= LastSkill; i = (skill_s)(i + 1)) {
		if(!skills[i])
			continue;
		*ps++ = i;
	}
	a.count = ps - a.data;
}

void creature::useskills() {
	bool cancel = false;
	skillu source(this);
	source.select(*this);
	source.sort();
	source.setcaps();
	auto s = source.choose(true, "Какой навык использовать?", &cancel);
	if(cancel)
		return;
	auto ps = bsmeta<skilli>::elements[s].getusetext();
	sb.add(ps);
}

void creature::cantmovehere() const {
	if(isactive())
		act("Сюда не пройти.");
}

void creature::move(indext index) {
	auto loc = location::getactive();
	if(!loc)
		return;
	if(index == Blocked) {
		cantmovehere();
		return;
	}
	auto d1 = location::getdirection(getposition(), index);
	if(d1 != Down && d1 != Up)
		direction = d1;
	if(loc->gettile(index) == Wall) {
		cantmovehere();
		return;
	}
	switch(loc->getobject(index)) {
	case Door:
		if(!loc->is(index, Opened)) {
			if(loc->is(index, Sealed))
				say("Здесь заперто.");
			else
				loc->set(index, Opened);
			wait();
			return;
		}
		break;
	case Tree:
		cantmovehere();
		return;
	}
	auto p = find(index);
	if(p) {
		if(isenemy(p)) {
			meleeattack(*p);
			return;
		} else if(!isactive()) {
			// Монстры и другие персонажи не меняются
			wait();
			return;
		} else if(p->isguard()) {
			static const char* talk[] = {
				"Я охраняю это место.",
				"Здесь не пройти.",
				"Не толкайся, я не отойду.",
			};
			p->say(maprnd(talk));
			wait();
			return;
		} else {
			// Игрок меняется позицией с неигроком
			auto psite = site::find(p->getposition());
			if(psite && psite->getowner() == p) {
				// С владельцами области не поменяешся местами
				static const char* talk[] = {
					"Не толкай меня в моем заведении.",
					"Держи руки на виду.",
					"Я за тобой слежу",
				};
				p->say(maprnd(talk));
				return;
			}
			p->setposition(getposition());
			p->wait();
			if(d100() < 50) {
				static const char* talk[] = {
					"Эй! Не толкайся.",
					"Давай, проходи.",
					"Куда ты так спешишь?",
				};
				p->say(maprnd(talk));
			}
		}
	}
	setposition(index);
	wait();
}

creature* creature::find(indext i) {
	for(auto& e : bsmeta<creature>()) {
		if(!e)
			continue;
		if(e.getposition() == i)
			return &e;
	}
	return 0;
}

void creature::consume(int v) {
	energy -= v;
}

int	creature::getspeed() const {
	auto r = 100 + abilities[Speed];
	r += get(Athletics) / 10;
	r += get(Dexterity) / 2 - 5;
	return r;
}

void creature::attack(creature& enemy, slot_s id, int bonus) {
	auto ai = getattack(id);
	if(!rollv(ai.attack + bonus, 0))
		act("%герой промазал%а.");
	auto parry = enemy.getparry();
	if((id == Melee || id == OffHand) && parry > 0) {
		if(enemy.rollv(parry, 0)) {
			act("%герой, но %оппонент отбил%А удар.");
			return;
		}
	}
	auto dv = ai.dice.roll();
	damage(dv, ai.type, isinteractive());
}

void creature::meleeattack(creature& enemy, int bonus) {
	int energy = StandartEnergyCost;
	if(wears[OffHand]) {
		attack(enemy, Melee, bonus - 10);
		attack(enemy, OffHand, bonus - 25);
		energy += StandartEnergyCost / 2;
	} else
		attack(enemy, Melee, bonus);
	consume(energy);
}

bool creature::isenemy(const creature* target) const {
	return is(Hostile) != target->is(Hostile);
}

void creature::say(const char* format, ...) const {
	sayv(sb, format, xva_start(format));
}

void creature::makemove() {
	if(energy < StandartEnergyCost) {
		energy += getspeed();
		return;
	}
	if(isactive()) {
		auto start = energy;
		while(start == energy) {
			location::setcamera(getposition());
			playui();
		}
	} else {

	}
}

bool creature::roll(skill_s v, int bonus, int divider) const {
	auto r = get(v) + bonus;
	if(divider > 1)
		r /= divider;
	return rollv(r, 0);
}

bool creature::rollv(int v, int* r) {
	auto result = d100();
	if(v <= 0)
		return false;
	if(v < 5)
		v = 5;
	if(r)
		*r = result;
	return result < v;
}

void creature::damage(int value, attack_s type, bool interactive) {

}

bool creature::isinteractive() const {
	return true;
}
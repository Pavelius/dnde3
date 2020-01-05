#include "main.h"

DECLDATA(creature, 256);

static int			skill_level[] = {20, 50, 75, 90};
static const char*	skill_names[] = {"Начальный", "Продвинутый", "Экспертный", "Мастерский"};
static creature*	current_player;
const int			chance_act = 40;
const int			chance_blood_when_dead = 70;

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

void creature::unlink() {
	auto id = getid();
	for(auto& e : bsmeta<creature>()) {
		if(!e)
			continue;
		if(e.charmer == id)
			e.charmer = Blocked;
		if(e.horror == id)
			e.horror = Blocked;
	}
	if(current_player == this)
		current_player = 0;
}

void creature::dressoff() {
	if(!this)
		return;
	dresssa(-1);
	dress(-1);
}

void creature::dresson() {
	if(!this)
		return;
	dress(1);
	dresssa(1);
}

void creature::dress(int m) {
	for(auto i = Head; i <= Legs; i = (slot_s)(i + 1)) {
		if(!wears[i])
			continue;
		auto mi = wears[i].getmagic();
		auto& ei = wears[i].getitem();
		auto wa = m * ei.weapon.attack;
		switch(i) {
		case Melee:
			abilities[AttackMelee] += m * mi * 3;
			abilities[DamageMelee] += m * mi / 2;
			break;
		case Ranged:
			abilities[AttackMelee] += m * mi * 4;
			abilities[DamageMelee] += m * mi / 2;
			break;
		default:
			abilities[AttackMelee] += wa;
			abilities[AttackRanged] += wa;
			break;
		}
		abilities[Deflect] += m * (ei.armor.deflect + mi*ei.armor.multiplier);
		abilities[Armor] += m * ei.armor.armor;
	}
}

void creature::dresssk(int m) {
	for(auto i = FirstSkill; i <= LastSkill; i = (skill_s)(i + 1)) {
		auto& ei = bsmeta<skilli>::elements[i];
		skills[i] += m*(get(ei.abilities[0]) + get(ei.abilities[1]));
	}
}

void creature::dresssa(int m) {
	for(auto i = AttackMelee; i <= ManaRate; i = (ability_s)(i + 1)) {
		auto& ei = bsmeta<abilityi>::elements[i];
		if(ei.formula)
			abilities[i] += m*calculate(ei.formula);
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
	dice_s source[] = {D2n12, D1n6, D1n4};
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

bool creature::add(item v, bool run, bool talk) {
	// Second place item to backpack
	for(auto i = FirstBackpack; i <= LastBackpack; i = (slot_s)(i + 1)) {
		if(wears[i])
			continue;
		if(run)
			equip(v, i);
		return true;
	}
	if(talk) {
		static const char* text[] = {
			"Ну и куда мне это засунуть?",
			"У меня уже нету места.",
			"Некуда класть. Надо что-нибудь выбросить из рюкзака.",
		};
		say(maprnd(text));
	}
	return false;
}

bool creature::equip(item v) {
	if(!isallow(v.getkind()))
		return false;
	// First try to dress this item
	for(auto i = Head; i <= Amunitions; i = (slot_s)(i + 1)) {
		if(!v.is(i))
			continue;
		if(wears[i])
			continue;
		equip(v, i);
		return true;
	}
	return add(v, true, false);
}

bool creature::equip(item& v1, item& v2, bool run) {
	if(!isallow(v2.getkind())) {
		if(run) {
			static const char* text[] = {
				"Да ну, это не мой размер.",
				"Я не знаю как этим пользоваться.",
				"Я такое носить не буду.",
			};
			say(maprnd(text));
		}
		return false;
	}
	dressoff();
	auto v = v1;
	v1 = v2;
	v2 = v;
	dresson();
	return true;
}

void creature::raiseskills(int number) {
	skillu source(this);
	source.select(*this);
	source.shuffle();
	source.setcaps();
	unsigned index = 0;
	unsigned count = source.getcount();
	while(number > 0) {
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
	// Class spells
	for(auto e : ci.spells)
		set(e, 1);
	// Hits
	add(LifePoints, getclass().hp);
	add(ManaPoints, getclass().mp);
	dresson();
}

void creature::finish() {
	hp = get(LifePoints);
	mp = get(ManaPoints);
}

void creature::create(race_s race, gender_s gender, class_s type) {
	clear();
	this->variant::type = Role;
	this->variant::value = Character;
	this->kind = type;
	abilities[Level] = 1;
	setname(race, gender);
	applyabilities();
	raise(Climbing);
	if(abilities[Intellegence] >= 9)
		raise(Literacy);
	raiseskills();
	start_equipment(*this);
	finish();
}

void creature::getfullname(stringbuilder& sb) const {
	sb.add(getname());
	sb.adds("%-3-%-1 %2i уровня", getstr(kind), get(Level), getstr(getrace()));
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
	auto& ci = getclass();
	result.attack += ci.weapon.base;
	result.attack += get(Level) * ci.weapon.multiplier;
	if(id == Ranged)
		result.attack += (get(Dexterity) - 10) * 2;
	else
		result.attack += (get(Strenght) - 10) * 2;
	if(skill && skills[skill]) {
		auto& ei = bsmeta<skilli>::elements[skill];
		result.attack += get(skill) / ei.weapon.attack; // Basic chance to hit
		if(ei.weapon.damage) {
			result.dice.min += get(skill) / ei.weapon.damage;
			result.dice.max += get(skill) / ei.weapon.damage;
		}
	}
	result.attack += get(attack_ability); // Basic chance to hit
	result.dice.max += get(damage_ability);
	if(id == Melee) {
		result.dice.max += (get(Strenght) - 10) / 2;
		if(weapon.is(Versatile) && !wears[OffHand]) {
			// RULE: Versatile weapon if used two-handed made more damage.
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
	case Number: result = p->value; p++; break;
	default: return 0;
	}
	while(p->type == Formula) {
		switch(p->value) {
		case Negative: result = -result; break;
		case Divide2: result = result / 2; break;
		case Divide3: result = result / 3; break;
		case Divide4: result = result / 4; break;
		case Divide10: result = result / 10; break;
		case Multiply2: result = result * 2; break;
		case Multiply3: result = result * 3; break;
		case Multiply4: result = result * 4; break;
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
	if(!remove(item, false, true))
		return;
	auto loc = location::getactive();
	if(loc) {
		loc->drop(getposition(), item);
		remove(item, true, true);
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
		if(add(*pi, true, true)) {
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
			if(!remove(*pi, false, true)) {
				pause();
				continue;
			}
			if(!add(*pi, false, true)) {
				pause();
				continue;
			}
			add(*pi, true, false);
			remove(*pi, true, false);
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

bool creature::remove(item& e, bool run, bool talk) {
	if(!e)
		return false;
	if(e.iscursed()) {
		if(talk) {
			static const char* text[] = {
				"Уберите руки! Это мое!",
				"Нет, я это не отдам.",
				"Мое сокровище! Моя прелесть!",
			};
			say(maprnd(text));
		}
		if(run)
			e.setidentify(true);
		return false;
	} else if(e.is(Natural)) {
		if(talk) {
			static const char* text[] = {
				"Это часть меня!",
				"Как я это сниму? Отрежу?",
				"Ты в своем уме?",
				"Это приросло ко мне намертво.",
			};
			say(maprnd(text));
		}
		return false;
	}
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
	restore_energy -= v;
}

void creature::attack(creature& enemy, slot_s id, int bonus) {
	auto ai = getattack(id);
	bonus += ai.attack;
	bonus -= enemy.get(Deflect);
	if(bonus < 5)
		bonus = 5;
	if(!rollv(bonus, 0)) {
		act("%герой промазал%а.");
		return;
	}
	act("%герой попал%а.");
	auto dv = ai.dice.roll();
	enemy.damage(dv, ai.type);
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

void creature::aimove() {
	// Need active location
	auto loc = location::getactive();
	if(!loc)
		return;
	auto d = (direction_s)xrand(Left, RightDown);
	auto i = loc->to(getposition(), d);
	if(loc->isfreenw(i))
		move(i);
	else
		wait();
}

void creature::aiturn() {
	// Need active location
	auto loc = location::getactive();
	if(!loc)
		return;
	// If horror are near run away
	auto horror = gethorror();
	if(horror && loc->getrange(horror->getposition(), getposition()) <= getlos() + 1) {
		moveaway(horror->getposition());
		return;
	}
	// Get nearest creatures
	creaturea source;
	source.select(getposition(), getlos());
	creaturea enemies = source;
	enemies.matchenemy(this);
	if(enemies) {
		enemies.sort(getposition());
		auto enemy = enemies[0];
		moveto(enemy->getposition());
	} else {
		// If creature guard some square move to guard position
		if(guard != Blocked) {
			moveto(guard);
			return;
		}
		// If creature have a leader don't move far away from him
		auto leader = getleader();
		if(leader) {
			if(loc->getrange(leader->getposition(), getposition()) > 2) {
				moveto(leader->getposition());
				return;
			}
		}
		if(d100() < chance_act) {
			// Do nothing
			wait();
			return;
		}
		// When we try to stand and think
		if(d100() < chance_act) {
		//	if(aiboost())
		//		return;
		//	if(use_skills(*this, sc))
		//		return;
			if(d100() < chance_act) {
		//		if(use_spells(*this, sc, false))
		//			return;
			}
		}
		//// When we move and traps or close door just before our step
		//if(move_skills(*this, sc))
		//	return;
		aimove();
	}
}

void creature::makemove() {
	const auto pc = StandartEnergyCost * 20;
	// RULE: paralized creature don't move and don't restore
	if(is(Paralized))
		return;
	if(restore_hits > pc) {
		if(hp < get(LifePoints))
			hp++;
		restore_hits -= pc;
	} else
		restore_hits += get(LifeRate);
	if(restore_mana > pc) {
		if(mp < get(ManaPoints))
			mp++;
		restore_mana -= pc;
	} else
		restore_mana += get(LifeRate);
	if(restore_energy < StandartEnergyCost) {
		restore_energy += get(Speed);
		return;
	}
	// RULE: sleeped creature don't move
	if(is(Sleeped))
		return;
	if(isactive()) {
		auto start = restore_energy;
		while(start == restore_energy) {
			location::setcamera(getposition());
			playui();
		}
	} else
		aiturn();
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

void creature::applyaward() const {
	auto index = getposition();
	if(index == Blocked)
		return;
	auto award = getaward();
	for(auto& e : bsmeta<creature>()) {
		if(!e)
			continue;
		if(!e.isenemy(this))
			continue;
		e.addexp(award);
	}
}

void creature::kill() {
	applyaward();
	unlink();
	auto loc = location::getactive();
	if(loc) {
		if(d100() < chance_blood_when_dead)
			loc->set(getposition(), Blooded);
	}
	clear();
}

void creature::damage(int value, attack_s type) {
	if(value < 0) {
		value = -value;
		auto mhp = get(LifePoints);
		if(hp + value > mhp)
			value = mhp - hp;
		if(value <= 0)
			return;
		act("%герой восстановил%а [+%1i] повреждений.", value);
	} else {
		if(hp <= value) {
			act("%герой получил%а [%1i] повреждений и упал%а.", value);
			kill();
		} else {
			act("%герой получил%а [%1i] повреждений.", value);
			hp -= value;
		}
	}
}

void creature::addexp(int v) {
	experience += v;
}

void creature::enslave() {
	creaturea source;
	source.select();
	source.match(Hostile);
	auto p = source.choose(true, "В кого хотите вселиться?");
	p->activate();
	wait();
}

void creature::move(indext index, bool runaway) {
	auto pos = getposition();
	if(index == pos)
		return;
	if(location::getrange(pos, index) > 1) {
		auto loc = location::getactive();
		if(!loc)
			return;
		loc->clearblock();
		loc->blockwalls();
		loc->blockcreatures();
		loc->makewave(index);
		if(runaway)
			index = loc->stepfrom(pos);
		else
			index = loc->stepto(pos);
		if(index == Blocked)
			return;
	}
	move(index);
}

void creature::set(ability_s id, int v) {
	dressoff();
	abilities[id] = v;
	dresson();
}

void creature::set(skill_s id, int v) {
	skills[id] = v;
}

int creature::get(skill_s id) const {
	const auto& ei = bsmeta<skilli>::elements[id];
	return skills[id] + get(ei.abilities[0]) + get(ei.abilities[1]);
}

bool creature::isallow(item_s v) const {
	auto& ci = bsmeta<classi>::elements[kind];
	if(ci.restricted.is(v))
		return false;
	return true;
}

creature* creature::getleader() const {
	if(charmer != Blocked)
		return getobject(charmer);
	if(is(Friendly)) {
		auto pa = getactive();
		if(pa != this)
			return pa;
	}
	return 0;
}

bool creature::isvisible() const {
	auto loc = location::getactive();
	if(!loc)
		return false;
	return loc->is(getposition(), Visible);
}
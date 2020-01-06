#include "main.h"

DECLDATA(creature, 256);

static int			skill_level[] = {20, 50, 75, 90};
static const char*	skill_names[] = {"Начальный", "Продвинутый", "Экспертный", "Мастерский"};
static creature*	current_player;
const int			restore_points_when_percent = 60;
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

void creature::add(variant id, int v, bool interactive) {
	switch(id.type) {
	case Ability:
		dressoff();
		abilities[id.value] += v;
		dresson();
		break;
	case State:
		if(v > 0)
			set((state_s)id.value);
		else
			remove((state_s)id.value);
		break;
	case Item:
		equip(item_s(id.value));
		break;
	}
}

void creature::addboost(variant id, int modifier, unsigned rounds) {
	auto p = bsmeta<boosti>::add();
	p->id = id;
	p->modifier = modifier;
	p->time = rounds;
	p->owner = getid();
}

creature* creature::getobject(short unsigned v) {
	if(v == Blocked)
		return 0;
	return bsmeta<creature>::elements + v;
}

void creature::clearboost() const {
	auto id = getid();
	auto ps = bsmeta<boosti>::elements;
	for(auto& e : bsmeta<boosti>()) {
		if(e.owner == id)
			continue;
		*ps++ = e;
	}
	bsmeta<boosti>::source.setcount(ps - bsmeta<boosti>::elements);
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
	clearboost();
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
			abilities[AttackRanged] += m * mi * 4;
			abilities[DamageRanged] += m * mi / 2;
			break;
		default:
			abilities[AttackMelee] += wa;
			abilities[AttackRanged] += wa;
			break;
		}
		abilities[Protection] += m * ei.armor.protection;
		abilities[Deflect] += m * ei.armor.deflect;
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
	for(auto i = Backpack; i <= LastBackpack; i = (slot_s)(i + 1)) {
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
			for(auto ef : ei.features) {
				e.add(ef, 1, false);
				switch(ef.type) {
				case Item:
					if(bsmeta<itemi>::elements[ef.value].weapon.ammunition)
						e.add(bsmeta<itemi>::elements[ef.value].weapon.ammunition, 1, false);
					break;
				}
			}
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
		abilities[i] = ri.abilities[i] + (rand() % 5) - 2;
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
	if(abilities[Level] > 0) {
		abilities[LifePoints] += getclass().hp;
		abilities[ManaPoints] += getclass().mp;
	}
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
	if(id == Melee || weapon) {
		result = weapon.getattack();
		result.dice = bsmeta<dicei>::elements[result.damage];
	}
	if(!result.dice.max)
		return result;
	auto& ci = getclass();
	result.attack += ci.weapon.base;
	result.attack += get(Level) * ci.weapon.multiplier;
	result.attack += get(attack_ability);
	result.dice.min += get(damage_ability);
	result.dice.max += get(damage_ability);
	if(skill && skills[skill]) {
		// Weapon focus modify damage, attack and speed
		auto& ei = bsmeta<skilli>::elements[skill];
		if(ei.weapon.attack)
			result.attack += get(skill) / ei.weapon.attack;
		if(ei.weapon.damage) {
			result.dice.min += get(skill) / ei.weapon.damage;
			result.dice.max += get(skill) / ei.weapon.damage;
		}
		if(ei.weapon.speed)
			result.speed += get(skill) / ei.weapon.speed;
	}
	switch(id) {
	case Melee:
		if(weapon.is(Versatile) && !wears[OffHand]) {
			result.dice.min += 1;
			result.dice.max += 1;
		}
		if(wears[OffHand] && wears[OffHand].is(Light)) {
			result.attack -= 20;
			result.speed -= 8;
			result.speed += wears[OffHand].getitem().weapon.speed;
			auto& ei = bsmeta<skilli>::elements[TwoWeaponFighting];
			if(ei.weapon.attack)
				result.attack += get(skill) / ei.weapon.attack;
			if(ei.weapon.damage) {
				result.dice.min += get(skill) / ei.weapon.damage;
				result.dice.max += get(skill) / ei.weapon.damage;
			}
			if(ei.weapon.speed)
				result.speed += get(TwoWeaponFighting) / ei.weapon.speed;
		}
		break;
	case OffHand:
		if(wears[Melee] && wears[OffHand].is(Light)) {
			result.attack -= 30;
			auto& ei = bsmeta<skilli>::elements[TwoWeaponFighting];
			if(ei.weapon.attack)
				result.attack += get(skill) / ei.weapon.attack;
		}
		break;
	}
	result.dice.normalize();
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
	return Backpack;
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
					if(!equip(*pi, *p2, true))
						pause();
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

void creature::look(indext index) {
	auto d1 = location::getdirection(getposition(), index);
	if(d1 != Down && d1 != Up)
		direction = d1;
}

void creature::move(indext index) {
	auto loc = location::getactive();
	if(!loc)
		return;
	if(index == Blocked) {
		cantmovehere();
		return;
	}
	look(index);
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

void creature::attack(creature& enemy, const attacki& ai, int bonus) {
	bonus += ai.attack;
	bonus -= enemy.get(Protection);
	if(bonus < 5)
		bonus = 5;
	look(enemy.getposition());
	if(!rollv(bonus, 0)) {
		act("%герой промазал%а.");
		return;
	}
	auto dv = ai.dice.roll();
	auto pierce = 0;
	auto deflect = enemy.get(Deflect);
	if(roll(FindWeakness, -deflect)) {
		auto danger = 30;
		switch(ai.type) {
		case Piercing:
			danger += 10;
			pierce = 100;
			act("%герой попал%а в уязвимое место.");
			break;
		case Slashing:
			act("%герой нанес%ла кровоточащую рану.");
			enemy.set(Wounded);
			enemy.bloodstain();
			break;
		case Bludgeon:
			act("%герой нанес%ла ошеломляющий удар.");
			enemy.set(Dazzled);
			break;
		}
		auto damage_bonus = dv * danger / 100;
		if(damage_bonus < 1)
			damage_bonus = 1;
		dv += damage_bonus;
	} else
		act("%герой попал%а.");
	enemy.damage(dv, ai.type, pierce);
}

void creature::meleeattack(creature& enemy, int bonus) {
	auto am = getattack(Melee);
	attack(enemy, am, bonus);
	if(wears[OffHand] && wears[OffHand].is(Light))
		attack(enemy, getattack(OffHand), bonus);
	consume(am.getenergy());
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
		if(loc->getrange(enemy->getposition(), getposition()) > 1
			&& canshoot(false)) {
			rangeattack(*enemy);
			return;
		}
		moveto(enemy->getposition());
	} else {
		// When we try to stand and think
		if(d100() < chance_act) {
			auto mhp = get(LifePoints), mmp = get(ManaPoints);
			auto mhz = restore_points_when_percent * mhp / 100, mmz = restore_points_when_percent * mmp / 100;
			if((mhz && hp <= mhz) || (mmz && mp <= mmz)) {
				if(aieat(false))
					return;
			}
			//	if(aiboost())
			//		return;
			//	if(use_skills(*this, sc))
			//		return;
			if(d100() < chance_act) {
				//		if(use_spells(*this, sc, false))
				//			return;
			}
		}
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
		//// When we move and traps or close door just before our step
		//if(move_skills(*this, sc))
		//	return;
		aimove();
	}
}

void creature::makemove() {
	const auto pc = StandartEnergyCost * 20;
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
	if(is(Wounded)) {
		// Wounded creature loose 1 hit point each turn
		if(rollv(get(Constitution) * 2, 0)) {
			remove(Wounded);
			act("Кровотечение у %героя прекратилось.");
		} else {
			act("%герой истекает кровью.");
			damage(1, Slashing, 100, false);
			if(!(*this))
				return;
		}
	}
	// Dazzled creature don't make turn
	if(is(Dazzled)) {
		if(rollv(get(Constitution) + 8, 0))
			act("%герой очухался от головокружения.");
		else {
			wait();
			return;
		}
	}
	// Sleeped creature don't move
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
	auto r = get(v);
	switch(divider) {
	case 0: break;
	case 1: r = r * 2 / 3; break;
	default: r /= divider; break;
	}
	r += bonus;
	return rollv(r, 0);
}

bool creature::rollv(int v, int* r) {
	if(r)
		*r = 100;
	if(v <= 0)
		return false;
	if(v < 5)
		v = 5;
	auto result = d100();
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

void creature::bloodstain() const {
	auto loc = location::getactive();
	if(!loc)
		return;
	loc->set(getposition(), Blooded);
}

void creature::kill() {
	applyaward();
	if(d100() < chance_blood_when_dead)
		bloodstain();
	unlink();
	clear();
}

void creature::damage(int value, attack_s type, int pierce, bool interactive) {
	if(value < 0) {
		value = -value;
		auto mhp = get(LifePoints);
		if(hp + value > mhp)
			value = mhp - hp;
		if(value <= 0)
			return;
		if(interactive)
			act("%герой восстановил%а [+%1i] повреждений.", value);
		hp += value;
	} else {
		auto armor = get(Armor);
		if(armor > 0 && pierce > 0) {
			if(pierce > armor)
				armor = 0;
			else
				armor -= pierce;
		}
		value -= armor;
		if(value <= 0) {
			if(interactive)
				act("Броня поглатила весь урон.");
		} else if(hp <= value) {
			if(interactive)
				act("%герой получил%а [%1i] повреждений и упал%а.", value);
			kill();
		} else {
			if(interactive)
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

bool creature::cansee(indext i) const {
	auto loc = location::getactive();
	if(!loc)
		return false;
	return loc->cansee(getposition(), i);
}

bool creature::canshoot(bool talk) const {
	if(!wears[Ranged]) {
		if(talk) {
			static const char* text[] = {
				"Мне надо дистанционное оружие.",
				"Чем стрелять?",
			};
			say(maprnd(text));
		}
		return false;
	}
	auto am = wears[Ranged].getammo();
	if(am) {
		if(!wears[Amunitions]) {
			if(talk) {
				static const char* text[] = {
					"Нет боеприпасов.",
					"Закончились %-1.",
				};
				say(maprnd(text), getstr(am));
			}
			return false;
		}
		if(wears[Amunitions].getkind() != am) {
			if(talk) {
				static const char* text[] = {
					"Не подходят боеприпасы. Надо %-1.",
					"Для стрельбы необходимы %-1, а я пытаюсь использовать %-2.",
				};
				say(maprnd(text), getstr(am), wears[Amunitions].getname());
			}
			return false;
		}
	}
	return true;
}

void creature::shoot() {
	if(!canshoot(true))
		return;
	creaturea enemies;
	enemies.select(getposition(), getlos());
	enemies.matchenemy(this);
	enemies.sort(getposition());
	if(!enemies) {
		static const char* text[] = {
			"Я не вижу вокруг ни одного врага. Куда целится?",
			"Не в кого стрелять.",
			"Стрелять куда?",
		};
		say(maprnd(text));
		return;
	}
	if(location::getrange(enemies[0]->getposition(), getposition()) <= 1) {
		static const char* text[] = {
			"Враг слишком близко!",
			"Надо драться в рукопашную а не стрелять!",
			"Надо отойти от врага!",
		};
		say(maprnd(text));
		return;
	}
	rangeattack(*enemies[0], 0);
}

void creature::rangeattack(creature& enemy, int bonus) {
	auto ai = getattack(Ranged);
	attack(enemy, ai, 0);
	if(wears[Ranged].getammo())
		wears[Amunitions].use();
	consume(ai.getenergy());
}

void creature::testweapons() {
	auto ai = getattack(Melee);
	act("%герой опробывал%а свое оружие.");
	act("%1 с шансом [%2i%%] наносит [%3i-%4i] повреждений.", wears[Melee].getname(), ai.attack, ai.dice.min, ai.dice.max);
	if(wears[OffHand].is(Light)) {
		auto ai = getattack(OffHand);
		act("%1 с шансом [%2i%%] наносит [%3i-%4i] повреждений.", wears[OffHand].getname(), ai.attack, ai.dice.min, ai.dice.max);
	}
	act("Рукопашная атака стоит [%1i] энергии.", ai.getenergy());
	if(wears[Ranged]) {
		auto ai = getattack(Ranged);
		act("%1 с шансом [%2i%%] наносит [%3i-%4i] повреждений.", wears[Ranged].getname(), ai.attack, ai.dice.min, ai.dice.max);
		act("Дистанционная атака стоит [%1i] энергии.", ai.getenergy());
	}
}

boosti*	creature::find(variant id) const {
	auto owner_id = getid();
	for(auto& e : bsmeta<boosti>()) {
		if(e.owner == owner_id && e.id == id)
			return &e;
	}
	return 0;
}

bool creature::match(variant id) const {
	switch(id.type) {
	case Race:
		if(getrace() != id.value)
			return false;
		break;
	case Skill:
		if(!skills[id.value])
			return false;
		break;
	case State:
		if(!is(state_s(id.value)))
			return false;
		break;
	case Spell:
		if(!spells[id.value])
			return false;
		break;
	}
	return true;
}

void creature::add(variant id, int v, unsigned minutes, bool interactive) {
	add(id, v, interactive);
	addboost(id, -v, game.getrouns() + minutes);
}

static int getchance(int chance, bool hostile, int quality, int damage) {
	if(!chance)
		return 100;
	auto m = 1;
	if(chance > 5)
		m = chance / 4;
	if(hostile)
		chance += m*damage - quality;
	else
		chance += m*quality - damage;
	if(chance < 0)
		chance = 0;
	if(chance > 100)
		chance = 100;
	return chance;
}

bool creature::apply(variant id, int chance, bool interactive, item_type_s magic, int quality, int damaged, int minutes) {
	bool need_test = true;
	bool need_message = true;
	if(id.type == Ability) {
		switch(id.value) {
		case LifePoints:
		case ManaPoints:
			need_test = false;
			break;
		}
	}
	if(need_test) {
		if(chance >= 0) {
			auto n = getchance(chance, false, quality, damaged);
			if(d100() >= n)
				return false;
		} else {
			auto n = getchance(-chance, true, quality, damaged);
			if(d100() >= n)
				return false;
		}
	}
	switch(id.type) {
	case Ability:
		switch(id.value) {
		case LifePoints: case ManaPoints:
			if(chance >= 0) {
				switch(magic) {
				case Artifact: add(id, (1 + quality) * 3, interactive); break;
				case Cursed: add(id, -(1 + quality), interactive); break;
				case Blessed: damage(-5 * (chance + quality * chance), Magic, 0, interactive); break;
				default: damage(-(chance + quality * chance), Magic, 0, interactive); break;
				}
				return true;
			} else {
				chance = -chance;
				switch(magic) {
				case Blessed: case Artifact: return false;
				case Cursed: damage(5 * (chance + quality * chance), Magic, 0, interactive); break;
				default: damage(chance + quality * chance, Magic, 0, interactive); break;
				}
				return true;
			}
			break;
		case AttackMelee: case AttackRanged: case Protection: case Deflect:
			if(chance >= 0) {
				switch(magic) {
				case Artifact: add(id, (1 + quality) * 3, interactive); break;
				case Cursed: add(id, -(20 + quality * 5), minutes * 10, interactive); break;
				case Blessed: add(id, 20 + quality * 5, minutes * 10, interactive); break;
				default: add(id, 5 + quality * 3, minutes, interactive); break;
				}
				break;
			} else {
				switch(magic) {
				case Blessed: case Artifact: return false;
				case Cursed: add(id, -(20 + quality * 5), minutes * 10, interactive); break;
				default: add(id, -(5 + quality * 3), minutes, interactive); break;
				}
				break;
			}
			break;
		default:
			if(chance >= 0) {
				switch(magic) {
				case Artifact: add(id, 1 + quality, interactive); break;
				case Cursed: add(id, -(1 + quality), interactive); break;
				case Blessed: add(id, xrand(1, 3), 10 * minutes * (1 + quality), interactive); break;
				default: add(id, xrand(1, 3), minutes * (1 + quality), interactive); break;
				}
			} else {
				switch(magic) {
				case Artifact: return false;
				case Cursed: add(id, -(2 + quality), interactive); break;
				case Blessed: add(id, 1, minutes - (1 + quality)*minutes / 5, interactive); break;
				default: add(id, -xrand(1, 3), minutes * (1 + quality), interactive); break;
				}
			}
			break;
		}
		break;
	case State:
		break;
	}
	return true;
}

bool creature::aieat(bool interactive) {
	itema source; source.selectb(*this);
	source.match(Edible);
	auto pi = source.choose(interactive, "Что хотите съесть?", 0, NoSlotName);
	if(pi)
		return eat(*pi, interactive);
	return false;
}

void creature::eat() {
	aieat(isactive());
}

void creature::backpack() {
	itema source; source.selectb(*this);
	source.choose(true, "Рюкзак", 0, NoSlotName);
}
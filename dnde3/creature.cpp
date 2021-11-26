#include "main.h"

BSDATAC(creature, 256)

static creature*	current_player;
static int			skill_level[] = {30, 60, 90};
static const char*	skill_names[] = {"Новичек", "Специалист", "Эксперт", "Мастер"};
static slot_s		armor_slots[] = {Head, TorsoBack, Torso, OffHand, Elbows, Legs};
static int			experience_count[] = {
	0, 100, 1000, 2500, 4500, 7000, 10000, 13500, 17500, 22000,
	27000, 35000, 50000,
};

static int getrang(int r) {
	auto n = 0;
	for(auto a : skill_level) {
		if(r < a)
			break;
		n++;
	}
	return n;
}

void creature::clear() {
	memset(this, 0, sizeof(*this));
	guard = Blocked;
	site_id = location_id = Blocked;
}

creature* creature::getactive() {
	return current_player;
}

short unsigned creature::getid() const {
	if(!this)
		return Blocked;
	return this - bsdata<creature>::elements;
}

void creature::feel(ability_s id, bool raise) {
	auto& ei = bsdata<abilityi>::elements[id];
	act("%герой почувстовал%а себя %1.", raise ? ei.name_how : ei.curse_how);
}

void creature::add(ability_s id, int v, bool interactive) {
	if(!v)
		return;
	abilities[id] += v;
	if(interactive)
		feel(id, v > 0);
}

void creature::add(state_s id, int v, bool interactive) {
	if(!v)
		return;
	if(v > 0) {
		if(basic.states.is(id))
			return;
		basic.states.set(id);
		if(interactive)
			act(bsdata<statei>::elements[id].text_set);
		prepare();
	} else {
		if(!basic.states.is(id))
			return;
		basic.states.remove(id);
		if(interactive)
			act(bsdata<statei>::elements[id].text_remove);
		prepare();
	}
}

void creature::add(variant id, int v, bool interactive) {
	switch(id.type) {
	case Ability: add((ability_s)id.value, v, interactive); break;
	case Spell: add((spell_s)id.value, v, interactive); break;
	case State: add((state_s)id.value, v, interactive); break;
	case Skill: add((skill_s)id.value, v, interactive); break;
	case Item:
		item it(item_s(id.value), v);
		equip(it);
		if(bsdata<itemi>::elements[id.value].getammo()) {
			item it(bsdata<itemi>::elements[id.value].getammo(), v);
			equip(it);
		}
		break;
	case Harm:
		switch(id.value) {
		case Piercing:
		case Slashing:
		case Bludgeon:
			if(roll(Acrobatics)) {
				if(interactive)
					act("%герой отскачил%а в сторону.");
				return;
			}
			break;
		}
		damage(v, damage_s(id.value), 0, interactive);
		break;
	}
}

void creature::dispell(variant source, bool interactive) {
	auto id = getid();
	auto ps = bsdata<boosti>::elements;
	for(auto& e : bsdata<boosti>()) {
		if(e.owner_id == id || e.source == source) {
			if(e.id)
				add(e.id, e.modifier, interactive);
			else if(source.type == Spell) {
				if(interactive) {
					switch(source.value) {
					case Sleep:
						act("%герой внезапно проснул%ась.");
						break;
					default:
						feel(LifePoints, true);
						break;
					}
				}
			}
			continue;
		}
		*ps++ = e;
	}
	bsdata<boosti>::source.setcount(ps - bsdata<boosti>::elements);
}

void creature::recall(variant id, variant source, int modifier, unsigned rounds) {
	auto p = bsdata<boosti>::add();
	p->id = id;
	p->source = source;
	p->modifier = modifier;
	p->time = rounds;
	p->owner_id = getid();
}

creature* creature::getobject(short unsigned v) {
	if(v == Blocked)
		return 0;
	return bsdata<creature>::elements + v;
}

void creature::dispell(bool interactive) {
	auto id = getid();
	auto ps = bsdata<boosti>::elements;
	for(auto& e : bsdata<boosti>()) {
		if(e.owner_id == id) {
			add(e.id, e.modifier, interactive);
			continue;
		}
		*ps++ = e;
	}
	bsdata<boosti>::source.setcount(ps - bsdata<boosti>::elements);
}

void creature::unlink() {
	if(current_player == this)
		current_player = 0;
	site::unlink(*this);
	setguard(Blocked);
	setposition(Blocked);
}

void creature::applyab() {
	for(auto i = Attack; i <= ManaRate; i = (ability_s)(i + 1)) {
		auto& ei = bsdata<abilityi>::elements[i];
		abilities[i] += calculate(ei.formula);
	}
}

void creature::applywr() {
	for(auto i = Head; i <= Ranged; i = slot_s(i + 1)) {
		if(!wears[i])
			continue;
		auto ei = wears[i].getarmor();
		abilities[Protection] += ei.protection;
		abilities[Deflect] += ei.deflect;
		abilities[Armor] += ei.armor;
		abilities[Attack] += ei.attack;
		auto v = wears[i].geteffect();
		auto b = wears[i].getquality();
		auto isweapon = (i == Melee) || (i == Ranged);
		if(i == OffHand && wears[i].geti().slot == Melee)
			isweapon = true;
		auto isring = (i == RightFinger);
		switch(v.type) {
		case Ability:
			if(isweapon)
				break;
			if(v.type >= Strenght && v.type <= Charisma && !isring) {
				if(b < 0) {
					if(abilities[v.value] > 4)
						abilities[v.value] = 4;
				} else {
					b += 17;
					if(abilities[v.value] < b)
						abilities[v.value] = b;
				}
			} else {
				b = bsdata<abilityi>::elements[v.value].getbonus(1 + b) + abilities[v.value];
				if(b < 0)
					b = 0;
				abilities[v.value] = b;
			}
			break;
		case Skill:
			if(skills[v.value]) {
				b += (1 + b) * 20 + skills[v.value];
				if(b < 0)
					b = 0;
				skills[v.value] = b;
			}
			break;
		case Spell:
			if(isweapon)
				break;
			b += 1 + spells[v.value];
			if(b < 0)
				b = 0;
			spells[v.value] = b;
			break;
		case State:
			if(b < 0)
				states.remove(v.value);
			else
				states.set(v.value);
			break;
		case Harm:
			if(isweapon)
				break;
			if(b < 0)
				vulnerability.set(v.value);
			else if(b > 0)
				immunity.set(v.value);
			else
				resistance.set(v.value);
			break;
		}
	}
}

void creature::applyen() {
	auto aw = getallowedweight();
	auto mw = getweight();
	auto penalty_attack = 0;
	auto penalty_speed = 0;
	auto penalty_deflect = 0;
	encumbrance = NoEncumbered;
	if(mw > aw) {
		penalty_speed += 25;
		penalty_deflect += 5;
		encumbrance = Encumbered;
	}
	if(mw > aw * 2) {
		penalty_speed += 25;
		penalty_attack += 15;
		penalty_deflect += 10;
		encumbrance = HeavilyEncumbered;
	}
	abilities[Speed] -= penalty_speed;
	abilities[Attack] -= penalty_attack;
	abilities[Protection] -= penalty_deflect;
}

void creature::applybs() {
	variant pid = this;
	for(auto& e : bsdata<boosti>()) {
		if(e.owner_id == pid.value)
			add(e.id, e.modifier, false);
	}
}

void creature::prepare() {
	if(!this)
		return;
	copy(&basic);
	applywr();
	applyab();
	applybs();
	applyen();
}

const char* creature::getrangname(int v) {
	auto n = getrang(v);
	return maptbl(skill_names, n);
}

bool creature::ismaster(skill_s v) const {
	return skills[v] >= skill_level[2];
}

void creature::equip(item it, slot_s id) {
	if(id >= Head && id <= LastWear)
		it.set(KnownStats);
	wears[id] = it;
}

bool creature::add(item v, bool run, bool talk) {
	if(v.is(Coinable)) {
		money += v.getcost() * v.getcount();
		return true;
	}
	// Second place item to backpack
	for(auto i = Backpack; i <= LastBackpack; i = (slot_s)(i + 1)) {
		if(wears[i]) {
			if(wears[i].stack(v))
				return true;
			continue;
		}
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
	for(auto i = Head; i <= LastWear; i = (slot_s)(i + 1)) {
		if(!v.is(i))
			continue;
		if(wears[i]) {
			if(wears[i].stack(v))
				return true;
			continue;
		}
		equip(v, i);
		return true;
	}
	return add(v, true, false);
}

bool creature::canuse(const item& e, bool talk) const {
	if(!isallow(e.getkind())) {
		if(talk) {
			static const char* text[] = {
				"Да ну, это не мой размер.",
				"Я не знаю как этим пользоваться.",
				"Я такое носить не буду.",
			};
			say(maprnd(text));
		}
		return false;
	}
	return true;
}

bool creature::cantakeoff(slot_s id, bool talk) {
	if(id >= Head && id <= LastWear) {
		auto& e = wears[id];
		if(e.is(Cursed)) {
			if(talk) {
				static const char* text[] = {
					"Уберите руки! Это мое!",
					"Нет, я это не отдам.",
					"Мое сокровище! Моя прелесть!",
				};
				say(maprnd(text));
				e.set(KnownPower);
			}
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
	}
	return true;
}

bool creature::equip(item& v1, item& v2, bool run) {
	if(!canuse(v2, true))
		return false;
	auto v = v1;
	v1 = v2;
	v2 = v;
	v1.set(KnownStats);
	prepare();
	return true;
}

void creature::raiseathletics() {
	auto n = get(Athletics);
	if(!basic.skills[Athletics])
		return;
	static ability_s source[] = {Strenght, Dexterity};
	adat<ability_s, 4> raised;
	for(auto s : source) {
		auto v = get(s);
		auto chance = n - (v - 10) * 10;
		if(rollv(chance)) {
			basic.abilities[s]++;
			raised.add(s);
		}
	}
	if(raised && is(Friendly)) {
		sb.add("Благодаря атлетическому сложению повысилась ");
		auto count = 0;
		for(auto s : raised) {
			count++;
			if(count > 1) {
				if(count == raised.getcount())
					sb.add(" и ");
				else
					sb.add(", ");
			}
			sb.add("%-1", getstr(s));
		}
		sb.add(".");
	}
}

void creature::raiseskills(int number, bool interactive) {
	if(number <= 0)
		return;
	skillu source;
	source.select(*this);
	for(auto e : source) {
		auto n = basic.get(e);
		n += get(bsdata<skilli>::elements[e].ability);
		if(n > 100)
			n = 100;
		source.setcap(e, n);
	}
	if(interactive) {
		source.sort();
		while(number > 0) {
			char temp[260]; stringbuilder sb(temp);
			sb.add("Какой навык повысить (осталось еще %1i)?", number);
			auto s = source.choose(&basic, true, temp, 0);
			basic.raise(s);
			auto v = source.getcap(s);
			if(basic.skills[s] > v)
				basic.skills[s] = v;
			number--;
		}
	} else {
		source.shuffle();
		unsigned index = 0;
		unsigned count = source.getcount();
		while(number > 0) {
			if(index >= count)
				index = 0;
			auto s = source[index++];
			basic.raise(s);
			auto v = source.getcap(s);
			if(basic.skills[s] > v)
				basic.skills[s] = v;
			number--;
		}
	}
}

void creature::randomequip() {
	for(auto& ei : bsdata<equipmenti>()) {
		if((!ei.race || ei.race == getrace()) && is(ei.type)) {
			for(auto v : ei.features)
				add(v, 3, false);
			break;
		}
	}
	equip(item(Ration, 2));
	money += xrand(3, 18) * GP;
}

void creature::finish() {
	prepare();
	hp = get(LifePoints);
	mp = get(ManaPoints);
	faith = get(FaithPoints);
}

void creature::create(race_s race, gender_s gender, class_s type) {
	clear();
	this->type = Role;
	this->value = Character;
	this->kind = type;
	setname(race, gender);
	basic.create(type, race);
	basic.raise(Climbing);
	basic.raise(Climbing);
	if(basic.abilities[Intellegence] >= 9)
		basic.raise(Literacy);
	raiselevel(false);
	randomequip();
	finish();
}

void creature::getfullname(stringbuilder& sb) const {
	sb.add(getname());
	sb.adds("%-3-%-1 %2i уровня", getstr(kind), get(Level), getstr(getrace()));
}

attacki creature::getattack(slot_s id, const item& weapon) const {
	attacki result = {};
	auto skill = weapon.geti().skill;
	if(id == Melee || weapon)
		result = weapon.getattack();
	if(!result.dice.max)
		return result;
	auto& ci = getclass();
	result.attack += get(Attack);
	result.dice.min += get(Damage);
	result.dice.max += get(Damage);
	if(skill.type == Skill && skills[skill.value]) {
		// Weapon focus modify damage, attack and speed
		auto& ei = bsdata<skilli>::elements[skill.value];
		auto value = get(skill_s(skill.value));
		if(ei.weapon.attack)
			result.attack += value / ei.weapon.attack;
		if(ei.weapon.damage)
			result.dice.max += value / ei.weapon.damage;
		if(ei.weapon.speed)
			result.speed += value / ei.weapon.speed;
	}
	switch(id) {
	case Melee:
		if(weapon.is(Versatile) && !wears[OffHand]) {
			result.dice.min += 1;
			result.dice.max += 1;
		}
		if(wears[OffHand].is(Light)) {
			result.attack -= 20;
			result.speed -= 8;
			result.speed += wears[OffHand].geti().weapon.speed;
			auto& ei = bsdata<skilli>::elements[TwoWeaponFighting];
			auto value = get(TwoWeaponFighting);
			if(ei.weapon.attack)
				result.attack += value / ei.weapon.attack;
			if(ei.weapon.damage)
				result.dice.max += value / ei.weapon.damage;
			if(ei.weapon.speed)
				result.speed += value / ei.weapon.speed;
		}
		break;
	case OffHand:
		if(weapon.is(Light) && wears[Melee]) {
			result.attack -= 30;
			auto& ei = bsdata<skilli>::elements[TwoWeaponFighting];
			if(ei.weapon.attack)
				result.attack += get(TwoWeaponFighting) / ei.weapon.attack;
		}
		break;
	}
	result.dice.normalize();
	return result;
}

void creature::activate() {
	current_player = this;
}

creature* creature::getactive(int n) {
	creaturea creatures;
	creatures.select(Friendly);
	creatures.match(Busy, true);
	creatures.matchact(Sleep, true);
	if(!creatures || n >= creatures.getcount())
		return 0;
	return creatures[n];
}

int creature::calculate(const varianta& source) const {
	int result = 0, r = 0;
	for(auto v : source) {
		switch(v.type) {
		case Ability:
			result += r;
			r = get((ability_s)v.value);
			break;
		case Skill:
			result += r;
			r = get((skill_s)v.value);
			break;
		case Number:
			result += r;
			r = (char)v.value;
			break;
		case Formula:
			switch(v.value) {
			case Negative: r = -r; break;
			case Divide2: r = r / 2; break;
			case Divide3: r = r / 3; break;
			case Divide4: r = r / 4; break;
			case Divide10: r = r / 10; break;
			case Multiply2: r = r * 2; break;
			case Multiply3: r = r * 3; break;
			case Multiply4: r = r * 4; break;
			default: return 0;
			}
			break;
		default:
			return 0;
		}
	}
	return result + r;
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
	if(!remove(item, false, true, false))
		return;
	loc.drop(getposition(), item);
	remove(item, true, true, false);
	consume(StandartEnergyCost / 4);
}

void creature::dropdown() {
	itema items; items.selectb(*this);
	auto pi = items.choose("Выбросить предмет", 0, NoSlotName);
	if(pi)
		dropdown(*pi);
}

void creature::pickup() {
	itema items; items.select(getposition());
	auto pi = items.choose("Поднять предмет", 0, NoSlotName);
	if(pi) {
		auto site = getsite();
		if(site && !pi->is(NotForSale)) {
			auto seller = site->getowner();
			if(seller && seller != this) {
				int cost = pi->getcost();
				if(!seller->ask(*this, "Хотите купить за [%1i] монет?", pi->getcost()))
					return;
				if(getmoney() < cost) {
					static const char* text[] = {
						"Возвращайся когда будет достаточно денег.",
						"У тебя нету нужного количества монет.",
						"Нет нужного количества монет - нет товара.",
					};
					seller->say(maprnd(text));
					return;
				}
				if(rollv(get(Bargaining), seller->get(Bargaining)) > 0) {
					cost = cost * 70 / 100;
					static const char* ask_discount[] = {
						"Сколько? Я не собираюсь платить по таким расценкам!",
						"Твой товар бывший в упортеблении. Сделаешь скидку?",
						"Я не буду покупать старье за цену нового. Или будет скидка, или сделки не будет!",
					};
					static const char* accept_discount[] = {
						"Уговорил. Плати только [%1i] монет.",
						"Эх, я сегодня добрый. Давай за [%1i] монет.",
					};
					say(maprnd(ask_discount), cost);
					seller->say(maprnd(accept_discount), cost);
				}
				money -= cost;
				pi->set(NotForSale);
			}
		}
		if(add(*pi, true, true)) {
			pi->clear();
			consume(StandartEnergyCost / 4);
		}
	}
}

void creature::inventory() {
	while(true) {
		itema items; items.select(*this);
		auto pi = items.choose("Инвенторий", 0, SlotName);
		if(!pi)
			break;
		auto slot = pi->getwearerslot();
		if(*pi) {
			if(!remove(*pi, false, true, true)) {
				pause();
				continue;
			}
			if(!add(*pi, false, true)) {
				pause();
				continue;
			}
			add(*pi, true, false);
			remove(*pi, true, false, true);
		} else {
			if(slot >= Head && slot <= LastWear) {
				items.clear();
				items.selectb(*this);
				items.match(slot, false);
				auto p2 = items.choose("Рюкзак", 0, NoSlotName, true);
				if(p2) {
					if(!equip(*pi, *p2, true))
						pause();
				}
			}
		}
	}
}

bool creature::remove(item& e, bool run, bool talk, bool same_owner) {
	if(!e)
		return false;
	auto slot = e.getwearerslot();
	if(e.is(Cursed) && slot >= Head && slot <= LastWear) {
		if(talk) {
			static const char* text[] = {
				"Уберите руки! Это мое!",
				"Нет, я это не отдам.",
				"Мое сокровище! Моя прелесть!",
			};
			say(maprnd(text));
			e.set(KnownPower);
		}
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
	} else if(!same_owner && e.ispersonal()) {
		if(talk) {
			static const char* text[] = {
				"Я не отбам свое.",
				"Это только для моего личного использования.",
				"Не могу. Это мое личное. Оно дорого мне, как память."
			};
			say(maprnd(text));
		}
		return false;
	}
	if(run) {
		e.clear();
		prepare();
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
	skillu source;
	source.select(*this);
	source.sort();
	auto s = source.choose(this, true, "Какой навык использовать?", &cancel);
	if(cancel)
		return;
	creaturea creatures(*this);
	use(creatures, s);
}

void creature::usespells() {
	bool cancel = false;
	spella source;
	source.select(*this);
	//source.sort();
	auto s = source.choose("У вас нет ни одного заклинания.", "Какое заклинание использовать?", &cancel, this);
	if(cancel)
		return;
	if(!use(s, get(s), 0, true))
		return;
	wait();
}

void creature::cantmovehere() const {
	info("Сюда не пройти.");
}

void creature::look(indext index) {
	auto d1 = location::getdirection(getposition(), index);
	if(d1 != Down && d1 != Up)
		direction = d1;
}

void creature::lookobjects() {
	loc.choose(getposition(), true, true, false);
}

void creature::lookaround() {
	if(isactive()) {
		loc.addobject(getposition(), sb);
		loc.additems(getposition(), sb);
	}
}

bool creature::use(indext index, bool moving) {
	switch(loc.getobject(index)) {
	case Door:
		if(getposition() == index)
			info("Вы стоите в дверном проходе. Двери закрыть не получится.");
		else if(!loc.is(index, Opened)) {
			if(loc.is(index, Sealed))
				say("Здесь заперто.");
			else
				loc.set(index, Opened);
			wait();
			return true;
		} else if(!moving) {
			wait();
			loc.remove(index, Opened);
			return true;
		}
		break;
	case StairsDown:
		if(isactive()) {
			if(askyn("Хотите спуститься вниз?")) {
				wait();
				game.use(StairsDown);
				return true;
			}
		}
		break;
	case StairsUp:
		if(isactive()) {
			if(askyn("Хотите подняться наверх?")) {
				wait();
				game.use(StairsUp);
				return true;
			}
		}
		break;
	case Altar:
		if(getsite()) {
			auto site = getsite();
			auto param = site->getparam();
			if(param.type != God)
				return false;
			itema items;
			items.selectb(*this);
			auto p = items.choose(isactive() ? "Что хотите принести в жертву на алтаре?" : 0, 0, NoSlotName);
			if(p)
				sacrifice((diety_s)param.value, *p);
			return true;
		}
		break;
	case Tree:
		if(moving)
			cantmovehere();
		else {
			act("%герой потряс%ла дерево.");
		}
		return true;
	}
	return false;
}

void creature::move(indext index) {
	if(is(Drunken) && d100() < 30) {
		index = loc.movernd(getposition(), index);
		static const char* talk[] = {"Да чтоб тебя!", "Ик!", "Нетуда..", "Извиняюсь...", "Я щас...", "Зачем я столько пил%а?"};
		say(maprnd(talk));
	}
	if(index == Blocked) {
		cantmovehere();
		return;
	}
	look(index);
	if(loc.gettile(index) == Wall) {
		cantmovehere();
		return;
	}
	auto current_index = getposition();
	if(loc.is(current_index, Webbed)) {
		if(roll(Climbing))
			loc.remove(current_index, Webbed);
		else {
			act("%герой запутал%ась в паутине.");
			wait();
			return;
		}
	}
	if(loc.gettrap(current_index) == TrapPit
		&& !loc.is(current_index, Hidden)) {
		if(!roll(Climbing)) {
			act("%герой не смог%ла вылезти из ямы.");
			wait();
			return;
		}
	}
	if(use(index, true))
		return;
	auto p = find(index);
	if(p) {
		if(isenemy(p)) {
			meleeattack(*p);
			return;
		} else if(!isactive()) {
			// Монстры и другие персонажи не меняются
			wait();
			return;
		} else if(p->is(Guardian)) {
			static const char* talk[] = {
				"Я охраняю это место.",
				"Здесь не пройти.",
				"Не толкайся, я не отойду.",
			};
			appear();
			p->say(maprnd(talk));
			wait();
			return;
		} else if(p->is(Owner)) {
			static const char* talk[] = {
				"Не толкай меня в моем заведении.",
				"Держи руки на виду.",
				"Я за тобой слежу",
			};
			appear();
			p->say(maprnd(talk));
			wait();
			return;
		} else {
			p->setposition(getposition());
			p->wait();
			if(d100() < 50) {
				if(p->saybusy()) {
					// Все уже сказали
				} else if(is(Invisible)) {
					static const char* talk[] = {
						"Кто здесь?",
						"Не понял.",
						"Что происходит?",
					};
					p->say(maprnd(talk));
				} else {
					static const char* talk[] = {
						"Эй! Не толкайся.",
						"Давай, проходи.",
						"Куда ты так спешишь?",
					};
					p->say(maprnd(talk));
				}
			}
		}
	}
	auto site = getsite();
	auto pnewsite = site::find(index);
	if(site && site->getowner() == this) {
		// Владелец заведения не покидает его
		if(site != pnewsite) {
			static const char* talk[] = {
				"Пожалуй, покидать мое место обитания я не буду.",
				"Там нечего делать. Вернусь к делам.",
				"Зачем мне туда идти?",
			};
			say(maprnd(talk));
			wait();
			return;
		}
	}
	// Выведем сообщение об окруающей среде
	if(site != pnewsite && pnewsite) {
		if(isactive()) {
			auto p = sb.get();
			sb.add("Перед вами ");
			auto p1 = sb.get();
			pnewsite->getname(sb);
			if(p1 == sb.get())
				sb.set(p);
			else
				sb.add(". ");
			pnewsite->addlook(sb);
		}
	}
	movecost(index);
	if(getposition() != index) {
		set(pnewsite);
		setposition(index);
		usestealth();
		usetrap();
		if((*this))
			lookaround();
	}
}

void creature::movecost(indext index) {
	auto v = StandartEnergyCost;
	v -= (StandartEnergyCost / 20) * abilities[Movement];
	if(v < StandartEnergyCost / 20)
		v = StandartEnergyCost / 20;
	if(is(RoomOfSticking))
		v *= 3;
	consume(v);
}

void creature::usestealth() {
	if(!is(Invisible))
		return;
	if(roll(MoveSilently))
		return;
	creaturea creatures;
	creatures.select(getposition(), 3); // Can be heard on this distance
	creatures.match(*this, Hostile, false, true);
	for(auto p : creatures) {
		if(p->roll(HearNoises)) {
			static const char* text[] = {"Вот он!", "Держи его!", "На нас напали!"};
			appear();
			p->say(maprnd(text));
			p->addexp(10);
			break;
		}
	}
}

creature* creature::find(indext i) {
	for(auto& e : bsdata<creature>()) {
		if(!e)
			continue;
		if(e.getposition() == i)
			return &e;
	}
	return 0;
}

void creature::consume(int v) {
	if(*this)
		restore_energy -= v;
}

void creature::attack(creature& enemy, const attacki& ai, int bonus, int danger) {
	if(is(Invisible) || enemy.is(Sleep) || enemy.is(Unaware)) {
		appear();
		enemy.dispell(Sleep, true);
		// Attack from invisible state
		// increase danger and chance to hit
		bonus += get(Backstabbing);
		danger += get(Backstabbing) * 3;
	}
	bonus += ai.attack;
	bonus -= enemy.get(Protection);
	if(enemy.is(Unaware)) {
		bonus += 20;
		danger += 20;
	}
	if(enemy.is(Fear)) {
		bonus += 20;
		danger += 20;
	}
	if(bonus < 5)
		bonus = 5;
	look(enemy.getposition());
	if(!rollv(bonus)) {
		act("%герой промазал%а.");
		return;
	}
	auto pierce = 0;
	auto deflect = enemy.get(Deflect);
	if(roll(FindWeakness, -deflect)) {
		danger += 30;
		switch(ai.type) {
		case Piercing:
			danger += 10;
			pierce = 100;
			act("%герой попал%а в уязвимое место.");
			break;
		case Slashing:
			act("%герой нанес%ла кровоточащую рану.");
			enemy.add(Wounded, 1, false);
			enemy.bloodstain();
			break;
		case Bludgeon:
			act("%герой нанес%ла ошеломляющий удар.");
			enemy.add(Dazzled, 1, false);
			break;
		}
	} else
		act("%герой попал%а.");
	// Calculate damage from danger
	auto dv = ai.dice.roll();
	dv = dv * danger / 100;
	enemy.damage(dv, ai.type, pierce);
}

void creature::meleeattack(creature& enemy, int bonus) {
	auto am = getattack(Melee, wears[Melee]);
	auto mp = 100;
	if(is(RoomOfDamage))
		mp = 300;
	attack(enemy, am, bonus, mp);
	wears[Melee].breaktest();
	if(wears[OffHand] && wears[OffHand].is(Light)) {
		attack(enemy, getattack(OffHand), bonus, mp);
		wears[OffHand].breaktest();
	}
	consume(am.getenergy());
}

bool creature::isenemy(const creature* target) const {
	return is(Hostile) != target->is(Hostile);
}

void creature::say(const char* format, ...) const {
	if(!(*this))
		return;
	sayv(sb, format, xva_start(format));
}

void creature::aimove() {
	// Need active location
	auto d = (direction_s)xrand(Left, RightDown);
	auto i = loc.to(getposition(), d);
	if(loc.isfreenw(i))
		move(i);
	else
		wait();
}

bool creature::needrestore(ability_s id) const {
	auto max = restore_points_percent * get(id) / 100;
	switch(id) {
	case LifePoints: return hp <= max;
	case ManaPoints: return mp <= max;
	}
	return false;
}

void creature::add(spell_s id, int v, bool interactive) {
	if(v > 0) {
		basic.spells[id] += v;
		if(interactive) {
			if(basic.spells[id] == 1)
				act("%герой изучил%а заклинание [%+1].", getstr(id));
			else if(basic.spells[id] > 1)
				act("%герой улучшил%а владение заклинанием [%+1] до [+%2i] ранга.", getstr(id), basic.spells[id]);
		}
	} else if(v < 0) {
		v = -v;
		if(v > basic.spells[id])
			v = basic.spells[id];
		basic.spells[id] -= v;
		if(interactive) {
			if(basic.spells[id] == 0)
				act("%герой забыл%а заклинание %+1.", getstr(id));
			else
				act("%герой ухудшил%а уровень владения заклинанием [%+1] до [-%2i] ранга.", getstr(id), basic.spells[id]);
		}
	}
}

void creature::add(skill_s id, int v, bool interactive) {
	if(v > 0) {
		skills[id] += v;
		if(interactive)
			act("%герой повысил%а навык [%+1] до [+%2i%%].", getstr(id), get(id));
	} else if(v < 0) {
	}
}

bool creature::aispells(creaturea& creatures) {
	spella source;
	source.select(*this);
	source.shuffle();
	for(auto s : source) {
		if(use(creatures, s, get(s), 0, false)) {
			wait();
			return true;
		}
	}
	return false;
}

bool creature::aiskills(creaturea& creatures, bool long_action, bool run) {
	skilla source;
	source.select(*this);
	source.removent();
	source.match(LongAction, !long_action);
	source.shuffle();
	if(!run)
		return source.getcount() != 0;
	else {
		auto room = getsite();
		for(auto s : source) {
			if(use(creatures, s))
				return true;
			if(!(*this))
				return true;
		}
	}
	return false;
}

void creature::aiturn(creaturea& creatures, creaturea& enemies, creature* enemy) {
	if(enemies) {
		// Испуганное существо бежит прочь при виде врага
		if(is(Fear) && loc.getrange(enemy->getposition(), getposition()) <= getlos() + 1) {
			if(d100() < chance_scarry_cry) {
				static const char* text[] = {"Спасите!", "Помогите!", "Аааа!!!", "Не убивайте!!!"};
				say(maprnd(text));
			}
			moveaway(enemy->getposition());
			return;
		}
		// Пытаемся стрелять если есть такая возможность
		if(loc.getrange(enemy->getposition(), getposition()) > 1 && canshoot()) {
			rangeattack(*enemy);
			return;
		}
		if(aispells(creatures))
			return;
		moveto(enemy->getposition());
	} else {
		if(is(Fear))
			add(Fear, -1, true);
		// When we try to stand and think
		if(d100() < chance_act) {
			if(is(LowInt)) {
				if(needrestore(LifePoints)) {
					if(aiuse(creatures, 0, Drinkable, LifePoints))
						return;
					if(aiuse(creatures, 0, Edible, LifePoints))
						return;
				}
			}
			if(is(AveInt)) {
				if(needrestore(ManaPoints)) {
					if(aiuse(creatures, 0, Drinkable, ManaPoints))
						return;
					if(aiuse(creatures, 0, Edible, ManaPoints))
						return;
				}
			}
			if(aiskills(creatures, false, true))
				return;
		}
		// If creature guard some square move to guard position
		if(guard != Blocked) {
			moveto(guard);
			return;
		}
		// If creature have a leader don't move far away from him
		auto leader = getleader();
		if(leader) {
			if(loc.getrange(leader->getposition(), getposition()) > 2) {
				moveto(leader->getposition());
				return;
			}
		}
		// Chance to do nothing
		if(d100() < chance_act) {
			wait();
			return;
		}
		// Investigate items
		aimove();
	}
}

void creature::checksick() {
	if(!is(Sick))
		return;
	if(hp >= get(LifePoints) / 3) {
		damage(1, Magic, 100, false);
		info("Болезнь прогрессирует.");
	}
}

void creature::checkpoison() {
	if(is(Poisoned)) {
		if(resist(Poison, 0, false)) {
			if(poison > 0)
				poison--;
		} else {
			act("%герой страдает от яда.");
			damage(1, Magic, 100, false);
		}
		if(poison <= 0)
			add(Poisoned, -1, true);
	} else if(is(Drunken)) {
		if(resist(Poison, 15, false)) {
			if(poison > 0)
				poison--;
		}
		if(poison <= 0)
			add(Drunken, -1, true);
	}
}

void creature::checkmood() {
	if(mood < 0)
		mood++;
	else if(mood > 0)
		mood--;
}

void creature::restoration() {
	const auto pc = StandartEnergyCost;
	while(restore_hits > pc) {
		if(!is(Sick)) {
			if(hp < get(LifePoints))
				hp++;
		}
		restore_hits -= pc;
	}
	restore_hits += get(LifeRate);
	while(restore_mana > pc / 4) {
		if(mp < get(ManaPoints))
			mp++;
		restore_mana -= pc / 4;
	}
	restore_mana += get(ManaRate);
	if(is(RoomOfMana))
		restore_mana += get(ManaRate) * 2;
	if(is(Wounded)) {
		// Wounded creature loose 1 hit point each turn
		if(roll(Constitution, -10))
			add(Wounded, -1, true);
		else {
			act("%герой истекает кровью.");
			if(is(RoomOfBlood))
				damage(xrand(2, 4), Slashing, 100, false);
			else
				damage(1, Slashing, 100, false);
		}
	}
}

void creature::makemove() {
	if(restore_energy < StandartEnergyCost) {
		restore_energy += get(Speed);
		return;
	}
	// Dazzled creature don't make turn
	if(is(Dazzled)) {
		if(roll(Constitution))
			add(Dazzled, -1, true);
		else {
			wait();
			return;
		}
	}
	if(!is(Invisible)) {
		// Персонаж под действием заклинения невидимости
		// должен быть становиться невидимым в начале своего хода
		// и если он оглушен, то он видимый, но если спит, то невидимый!!!
		if(is(Invisibility))
			add(Invisible, 1, true);
	}
	// Sleeped creature don't move
	if(is(Sleep))
		return;
	if(is(Unaware))
		add(Unaware, -1, true);
	// Get nearest creatures
	creaturea creatures;
	creatures.select(getposition(), getlos());
	creaturea enemies = creatures;
	enemies.match(*this, Hostile, false);
	creature* enemy = 0;
	if(enemies) {
		// Combat situation - need eliminate enemy
		enemies.sort(getposition());
		enemy = enemies[0];
	}
	if(isactive()) {
		auto start = restore_energy;
		while(start == restore_energy) {
			location::setcamera(getposition());
			playui();
		}
	} else
		aiturn(creatures, enemies, enemy);
}

bool creature::roll(skill_s v, int bonus, int divider) const {
	auto r = get(v);
	switch(divider) {
	case 0: break;
	case 1: r = r * 2 / 3; break;
	default: r /= divider; break;
	}
	r += bonus;
	return rollv(r);
}

bool creature::rollv(int v) {
	if(v <= 0)
		return false;
	if(v < 5)
		v = 5;
	else if(v > 95)
		v = 95;
	return d100() < v;
}

int	creature::rollv(int v1, int v2) {
	auto r1 = d100();
	auto r2 = d100();
	auto d1 = imax(0, v1 - r1);
	auto d2 = imax(0, v2 - r2);
	return d1 - d2;
}

void creature::applyaward() const {
	auto index = getposition();
	if(index == Blocked)
		return;
	auto award = getaward();
	for(auto& e : bsdata<creature>()) {
		if(!e)
			continue;
		if(!e.isenemy(this))
			continue;
		e.addexp(award);
	}
}

void creature::bloodstain() const {
	loc.set(getposition(), Blooded);
}

void creature::dropitems() {
	auto index = getposition();
	for(auto i = Backpack; i <= LastWear; i = (slot_s)(i + 1)) {
		auto& e = wears[i];
		if(!e)
			continue;
		if(e.is(Natural))
			continue;
		if(e.getmagic() != Artifact) {
			if(d100() >= chance_drop_item)
				continue;
		}
		e.loot();
		loc.drop(index, e);
		e.destroy(Magic, false);
	}
}

void creature::kill() {
	if(d100() < chance_blood_when_dead)
		bloodstain();
	auto corpse = item::findcorpse((role_s)value);
	if(corpse && d100() < 30) {
		item it(corpse);
		it.seteffect((role_s)value);
		it.set(KnownPower);
		loc.drop(getposition(), it);
	}
	hp = mp = 0;
	applyaward();
	dropitems();
	dispell(false);
	unlink();
}

void creature::damage(int value, damage_s type, int pierce, bool interactive) {
	auto& di = bsdata<damagei>::elements[type];
	// Innate resist skills
	if(isresist(type) && value > 0)
		value /= 2;
	if(isvulnerable(type) && value > 0)
		value *= 2;
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
		if(is(RoomOfNature))
			value /= 3;
		if(di.damage_wears != 0 && di.damage_chance != 0) {
			auto count = di.damage_wears;
			if(count < 0)
				count = value / -count;
			if(d100() < di.damage_chance)
				damagewears(value, type, count);
		}
		auto armor = get(Armor);
		switch(type) {
		case WaterAttack: value /= 3; break;
		case Electricity: pierce += 2; break;
		}
		if(value < 0)
			return;
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

unsigned creature::getlevelup() const {
	auto n = abilities[Level];
	if(n + 2 >= sizeof(experience_count) / sizeof(experience_count[0]))
		return 0;
	return experience_count[n + 1];
}

void creature::raiselevel(bool interactive) {
	basic.add(Level, 1);
	auto start_log = sb.get();
	basic.raise((role_s)value, kind);
	raiseathletics();
	if(sb.get() > start_log)
		pause(interactive);
	raiseskills(interactive);
	prepare();
}

void creature::addexp(int v, bool interactive) {
	if(v > 0) {
		v = v * (90 + get(Intellegence)) / 100;
		experience += v;
		if(interactive)
			act("%герой получил%а [+%1i] опыта.", v);
		while(experience >= getlevelup()) {
			if(!getlevelup())
				break;
			if(is(Friendly)) {
				activate();
				act("Мои поздравления, %герой получил%а новый уровень опыта.");
				act("Теперь вы %-1 [%2i] уровня.", getstr(kind), abilities[Level] + 1);
				pause(interactive);
			}
			raiselevel(interactive);
		}
	} else {
		v = -v;
		if(experience > (unsigned)v)
			experience -= v;
		else
			experience = 0;
		if(interactive)
			act("%герой потерял%а [-%1i] опыта.", v);
	}
}

void creature::enslave() {
	creaturea source;
	source.select(getposition(), 48, false);
	source.match(*this, Hostile, false);
	auto p = source.choose("В кого хотите вселиться?");
	p->activate();
	wait();
}

void creature::moveto(indext index) {
	auto pos = getposition();
	if(index == pos)
		return;
	if(location::getrange(pos, index) > 1) {
		loc.clearblock();
		loc.blockwalls();
		loc.blockcreatures();
		loc.makewave(index);
		index = loc.stepto(pos);
		if(index == Blocked)
			return;
	}
	move(index);
}

void creature::moveaway(indext index) {
	loc.clearblock();
	loc.blockwalls();
	loc.makewave(index);
	index = loc.stepfrom(getposition());
	if(index == Blocked)
		return;
	move(index);
}

bool creature::isallow(item_s v) const {
	auto& ci = bsdata<classi>::elements[kind];
	auto& ei = bsdata<itemi>::elements[v];
	if(ei.skill.type == Skill && skills[ei.skill.value] > 0)
		return true;
	if(ci.restricted.is(v))
		return false;
	return true;
}

creature* creature::getleader() const {
	if(is(Friendly)) {
		auto pa = getactive();
		if(pa != this)
			return pa;
	}
	return 0;
}

bool creature::cansee(indext i) const {
	return loc.cansee(getposition(), i);
}

bool creature::canshoot() const {
	auto interactive = isactive();
	if(!wears[Ranged]) {
		if(interactive) {
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
			if(interactive) {
				static const char* text[] = {
					"Нет боеприпасов.",
					"Закончились %-1.",
				};
				say(maprnd(text), getstr(am));
			}
			return false;
		}
		if(wears[Amunitions].getkind() != am) {
			if(interactive) {
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
	if(!canshoot())
		return;
	creaturea enemies;
	enemies.select(getposition(), getlos());
	enemies.match(*this, Hostile, false);
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
	if(is(RoomOfWind) || enemy.is(RoomOfWind))
		ai.attack -= 40;
	attack(enemy, ai, 0, 100);
	if(wears[Ranged].getammo())
		wears[Amunitions].use();
	else if(wears[Ranged].iscountable())
		wears[Ranged].use();
	wears[Melee].breaktest();
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
	for(auto& e : bsdata<boosti>()) {
		if(e.owner_id == owner_id && e.id == id)
			return &e;
	}
	return 0;
}

boosti*	creature::finds(variant source) const {
	auto owner_id = getid();
	for(auto& e : bsdata<boosti>()) {
		if(e.owner_id == owner_id && e.source == source)
			return &e;
	}
	return 0;
}

void creature::add(spell_s id, unsigned minutes) {
	recall({}, id, 0, game.getrounds() + minutes);
}

void creature::add(ability_s id, variant source, int v, bool interactive, unsigned minutes) {
	if(!v)
		return;
	add(id, v, interactive);
	recall(id, source, -v, game.getrounds() + minutes);
}

void creature::paymana(int value, bool interactive) {
	if(value < 0) {
		auto max = get(ManaPoints);
		value = -value;
		if(mp + value > max)
			value = max - mp;
		if(value <= 0)
			return;
		mp += value;
		if(interactive)
			act("%герой восстановил%а %1i маны.", value);
	} else {
		if(value > mp)
			value = mp;
		if(value == 0)
			return;
		mp -= value;
		if(interactive)
			act("%герой потерял%а %1i маны.", value);
	}
}

bool creature::aiuse(const creaturea& creatures, const char* title, slot_s slot, variant effect) {
	itema source;
	source.selectb(*this);
	source.select(getposition(), true);
	source.match(slot, false);
	if(effect)
		source.matchboost(effect);
	auto pi = source.choose(title, 0, NoSlotName);
	if(pi)
		return use(creatures, *pi);
	return false;
}

void creature::drink() {
	creaturea creatures(*this);
	aiuse(creatures, "Чего хотите выпить?", Drinkable, {});
}

void creature::eat() {
	creaturea creatures(*this);
	aiuse(creatures, "Что хотите съесть?", Edible, {});
}

void creature::usewands() {
	creaturea creatures(*this);
	aiuse(creatures, "Использовать какой предмет?", Zapable, {});
}

void creature::backpack() {
	itema source; source.selectb(*this);
	source.choose("Рюкзак", 0, NoSlotName);
}

int	creature::getboost(variant id) const {
	auto owner_id = getid();
	auto result = 0;
	for(auto& e : bsdata<boosti>()) {
		if(e.owner_id == owner_id && e.id == id)
			result += e.modifier;
	}
	return result;
}

void creature::minimap() {
	loc.minimap(getposition(), true);
}

bool creature::match(variant v) const {
	switch(v.type) {
	case Alignment: return true;
	case Class: return kind == v.value;
	case Condition: return is((condition_s)v.value);
	case Gender: return getgender() == v.value;
	case Race: return getrace() == v.value;
	case Role: return getrole() == v.value;
	case Skill: return skills[v.value] != 0;
	case State: return is(state_s(v.value));
	case Spell: return spells[v.value] != 0;
	case Room: return is(Owner) && getsite()->getkind() == v.value;
	}
	return true;
}

bool creature::ismatch(const creature& opponent, skill_s id, int value) const {
	auto r1 = get(id);
	auto r2 = value + opponent.get(Intellegence) * 2 - 20;
	return  r1 >= r2;
}

void creature::fail(skill_s id) {
	const int chance_fail = 30;
	auto& ei = bsdata<skilli>::elements[id];
	auto isbad = d100() < chance_fail;
	if(ei.is(Strenght) && isbad) {
		act("%герой растянул%а мышцу.");
		damage(1, Bludgeon, 100, false);
	} else if((ei.is(Intellegence) || ei.is(Wisdow)) && isbad) {
		act("%герой почуствовал%а моральное переутомление.");
		paymana(1, false);
	} else if(ei.is(Dexterity) && isbad) {
		act("%герой испытал%а мышечный спазм.");
		damage(1, Bludgeon, 100, false);
	} else if(isbad) {
		info("Вы убили кучу времени, но все было тщетно.");
		wait(xrand(2, 4));
	}
}

void creature::appear() {
	add(Invisible, -1, true);
}

void creature::readsomething() {
	if(!skills[Literacy]) {
		info("Вы не грамотны. Для начала научитесь читать.");
		return;
	}
	creaturea creatures(*this);
	use(creatures, Literacy);
}

bool creature::charmresist(int bonus) const {
	return resist(Charm, bonus, true);
}

int creature::getlos() const {
	int v = abilities[Visibility];
	int b = loc.getlight();
	if(b < 0 && !is(Darkvision))
		v += b;
	return imax(1, imin(7, v));
}

void creature::damagewears(int count, damage_s type, int item_count) {
	itema items;
	select(items, Head, LastWear, true);
	items.shuffle();
	int maximum = items.getcount();
	if(item_count > maximum)
		item_count = maximum;
	for(auto i = 0; i < maximum; i++)
		items[i]->damage(count, type, true);
}

bool creature::ask(const nameable& opponent, const char* format, ...) const {
	return askv(sb, opponent, format, xva_start(format));
}

bool creature::askyn(const char* format, ...) const {
	if(!isactive())
		return true;
	act(format);
	return askyn();
}

bool creature::saybusy() {
	if(is(Sleep)) {
		static const char* text[] = {"%герой мирно спит.",
			"%герой смачно похрапывает.",
			"%герой во сне перевернул%ась на другой бок."
		};
		act(maprnd(text));
		return true;
	}
	if(!is(Busy))
		return false;
	static const char* text[] = {"Я занят",
		"Мне надо закончить дело.",
		"Дайте мне еще около [%1i] минут и я освобожусь."
	};
	say(maprnd(text), -(restore_energy / StandartEnergyCost));
	return true;
}

void creature::usetools() {
	creaturea creatures(*this);
	use(creatures, wears[Tool]);
}

void creature::quitandsave() {
	if(askyn("Вы действительно хотите [сохранить] игру и выйти?")) {
		game.write();
		exit(0);
	}
}

bool creature::knownreceipt(variant id) const {
	auto i = item::getreceipts().indexof(id);
	if(i == -1)
		return false;
	return recipes.is(i);
}

void creature::learnreceipt(variant id) {
	int receipt_count = recipes.getcount();
	int receipt_maximum = get(Alchemy) / 10;
	act("%герой внимательно изучил%а рецепт.");
	if(knownreceipt(id) || receipt_count >= receipt_maximum) {
		info("В нем были важные сведения по алхимии, которых вам не доставало.");
		addexp(5 * get(Alchemy));
	} else {
		auto i = item::getreceipts().indexof(id);
		if(i == -1)
			return;
		act("В нем подробно излагалась алхимическая формула [%1]. Теперь, имея набор алхимика вы сможете создавать зелья такого типа.", id.getnameof());
		recipes.set(i);
	}
	wait(30);
}

variant creature::choosereceipt(const char* interactive) const {
	auto source = item::getreceipts();
	answeri an;
	for(unsigned i = 0; i < source.count; i++) {
		if(!knownreceipt(source[i]))
			continue;
		an.add(i, "Рецепт %1", source[i].getnameof());
	}
	return source[an.choosev(interactive, false, false, interactive)];
}

site* creature::getsite() const {
	return (site_id != Blocked) ? &bsdata<site>::elements[site_id] : 0;
}

void creature::set(const site* v) {
	if(!v)
		site_id = Blocked;
	else
		site_id = v - bsdata<site>::elements;
}

item* creature::finditem(item_s v) {
	for(auto& it : wears) {
		if(it.getkind() == v)
			return &it;
	}
	return 0;
}

void creature::zoomon() {
	game.use(StairsDown);
}

bool creature::canleave(direction_s v) const {
	auto i = getposition();
	if(i == Blocked)
		return false;
	auto x = loc.getx(i);
	auto y = loc.gety(i);
	switch(v) {
	case Left: return x == 0;
	case Right: return x == mmx - 1;
	case Down: return y == mmy - 1;
	case Up: return y == 0;
	default: return false;
	}
}

bool creature::leaving(direction_s v) {
	if(!isactive())
		return false;
	if(!canleave(v))
		return false;
	sb.add("Вы действительно хотите покинуть это место?");
	if(!askyn())
		return false;
	game.use(StairsUp);
	wait();
	return true;
}

static const char* getmaterials(const void* object, stringbuilder& sb) {
	auto p = (crafti*)object;
	for(auto e : p->materials) {
		if(sb)
			sb.add(", ");
		sb.add(bsdata<itemi>::elements[e].name);
	}
	return sb;
}

void creature::testpotion() {
	//loc.growplants();
	//game.decoyfood();
	//additem(Wand1, Domination);
	//additem(Wand1, SummonAlly);
	static crafti crafts[] = {
		{10, SwordLong, {IronIgnot}, {}},
		{10, SwordTwoHanded, {IronIgnot, IronIgnot}, {}},
	};
	static answeri::column columns[] = {
		{"Ингридиенты", 400, getmaterials},
		{},
	};
	answeri an;
	for(auto& e : crafts)
		an.add((int)&e, bsdata<itemi>::elements[e.type].name);
	an.choosev("Создание зелий", "Укажите зелье, которое необходимо создать. При этом будут потрачены указанные ингридиенты.", true, columns);
}

int	creature::getallowedweight() const {
	return get(Strenght) * 500;
}

bool creature::execute(action_s v, bool run) {
	site* pst;
	switch(v) {
	case GuardPosition:
		if(guard != Blocked)
			return false;
		if(run)
			guard = getposition();
		break;
	case StopGuardPosition:
		if(guard == Blocked)
			return false;
		if(run)
			guard = Blocked;
		break;
	case UseLongActionSkill:
		if(true) {
			creaturea creatures;
			creatures.select(getposition(), getlos());
			return aiskills(creatures, true, run);
		}
		break;
	case MakeDiscount:
		pst = getsite();
		if(!pst || pst->getowner() != this)
			return false;
		if(true) {
			itema items;
			items.select(*pst);
			items.selectg(Sale100);
			if(!items)
				return false;
			if(run) {
				static const char* talk[] = {"Ладно, толко сегодня распродажа. %+1 отпущу немного дешевле.",
					"Надо делать скидочку. Сегодня я добрый. %+1 отпущу дешевле.",
					"%+1 не мой профильный товар. Даю скидку.",
				};
				auto pi = items.random();
				char temp[260]; stringbuilder  sb(temp);
				pi->getname(sb, true);
				say(maprnd(talk), temp);
				auto sale = pi->getsale();
				pi->set(sale_s(sale - 1));
			}
		}
		break;
	case MakeAnger:
		if(mood < -100)
			return false;
		if(run)
			mood -= xrand(1, 4);
		break;
	case MakeHappy:
		if(mood > 100)
			return false;
		if(run)
			mood += xrand(2, 4);
		break;
	case MinorWound:
		if(hp <= 0)
			return false;
		if(run)
			damage(1, Bludgeon);
		break;
	case MinorDisaster:
		if(mp <= 0)
			return false;
		if(run)
			mp -= 1;
		break;
	}
	return true;
}

void creature::decoyfood() {
	for(auto& e : wears) {
		if(!e)
			continue;
		e.decoy();
	}
}

void creature::additem(item_s type, variant effect, bool identified) {
	item it(type);
	it.seteffect(effect);
	if(it.ischargeable())
		it.setcharge(xrand(2, 12));
	if(identified)
		it.set(KnownPower);
	add(it, true, false);
}

item creature::craft(item_s type, variant effect, skill_s skill, int bonus) {
	item result(type);
	result.seteffect(effect);
	if(!roll(skill, bonus))
		result.set(Cursed);
	return result;
}

const summona& creature::getsummon() const {
	static summona monsters = {GoblinWarrior, GoblinRockthrower, GoblinRockthrowerWarriorF, OrcWarrior};
	if(value != Character) {

	}
	return monsters;
}

void creature::setfriendlyto(const creature& player) {
	if(player.is(Hostile))
		add(Hostile, 1, false);
	else
		add(Friendly, 1, false);
}

void creature::wait(duration_s v) {
	consume(bsdata<durationi>::elements[v].roll());
}

bool creature::is(condition_s v) const {
	switch(v) {
	case Anger: return mood < 30;
	case Berserking: return mood < 60;
	case Busy: return restore_energy <= -(StandartEnergyCost * 4) && is(Unaware);
	case Guardian: return guard != Blocked;
	case Happy: return mood > 40;
	case MissHits: return gethits() < get(LifePoints);
	case MissHalfHits: return gethits() < get(LifePoints) / 2;
	case MissAlmostAllHits: return gethits() < get(LifePoints) / 5;
	case MissMana: return getmana() < get(ManaPoints);
	case MissHalfMana: return getmana() < get(ManaPoints) / 2;
	case MissAlmostAllMana: return getmana() < get(ManaPoints) / 5;
	case Owner: return getsite() && getsite()->getowner() == this;
	default: return false;
	}
}

bool creature::resist(damage_s v, int bonus, bool interactive) const {
	if(isimmune(v))
		return true;
	auto& ei = bsdata<damagei>::elements[v];
	if(isresist(v))
		bonus += 30;
	bonus += get(ei.ability) * 2;
	bonus += get(Luck) * 5;
	if(!rollv(bonus))
		return false;
	if(interactive)
		act(ei.resist_text);
	return true;
}

bool creature::roll(ability_s v, int bonus) const {
	bonus += get(v) * 3;
	bonus += get(Luck) * 5;
	return rollv(bonus);
}

bool creature::pray(bool run) {
	if(is(Sick)) {
		if(run)
			add(Sick, -1, true);
	} else if(is(Poisoned)) {
		if(run) {
			poison = 0;
			add(Poison, -1, true);
		}
	} else if(is(MissAlmostAllMana)) {
		if(run) {
			mp = get(ManaPoints);
			act("%герой полностью ментально восстановил%ась.");
		}
	} else if(is(MissAlmostAllHits)) {
		if(run) {
			hp = get(LifePoints);
			act("%герой полностью исцелил%ась.");
		}
	} else
		return false;
	return true;
}

void creature::pray() {
	static const char* speech_male[] = {"%1, дай мне силы!", "Да святится %1 и будет его воля!", "%1 всемогущий, помоги!", "Да снизойдет благодать %2 на меня и моих товарищей!"};
	static const char* speech_female[] = {"%1, дай мне силы!", "Да святится %1 и будет ее воля!", "%1 всемогущая, помоги!", "Да снизойдет благодать %2 на меня и моих товарищей!"};
	auto god = getgod();
	auto& ei = bsdata<dietyi>::elements[god];
	if(ei.gender == Male)
		say(maprnd(speech_male), ei.name, ei.nameof);
	else
		say(maprnd(speech_female), ei.name, ei.nameof);
	if(!faith)
		return;
	if(!pray(false))
		return;
	pray(true);
	faith--;
}

void creature::qsearch() {
	auto site = getsite();
	if(site) {
		if(isactive())
			site->addlook(sb);
	}
	auto x0 = loc.getx(getposition());
	auto y0 = loc.gety(getposition());
	for(auto x = x0 - 1; x <= x0 + 1; x++) {
		for(auto y = y0 - 1; y <= y0 + 1; y++) {
			auto i = loc.get(x, y);
			if(i == Blocked)
				continue;
			if(!loc.ishidden(i))
				continue;
			if(roll(Wisdow, get(Alertness) / 3)) {
				static const char* speech[] = {"Смотрите! Это %1.", "Я наш%ла %1.", "Тут %1.", "Я кое что наш%ел."};
				say(maprnd(speech), getstr(loc.getobject(i)));
				loc.reveal(i);
				addexp(10, false);
			}
		}
	}
}

void creature::search() {
	qsearch();
	wait(CoupleMinutes);
}

void creature::sacrifice(diety_s god, item& it) {
	auto& ei = bsdata<dietyi>::elements[god];
	if(!remove(it, false, isactive(), false))
		return;
	if(ei.sacred.is(it.getkind())) {
		switch(ei.gender) {
		case Female: info("%1 приняла дар и осталась довольна.", ei.name); break;
		default: info("%1 принял дар и остался доволен.", ei.name); break;
		}
		auto magic = it.getmagic();
		it.clear();
		faith = get(FaithPoints);
		if(magic >= Cursed)
			faith += 2;
		if(magic >= Artifact)
			faith += 10;
	} else {
		switch(ei.gender) {
		case Female: info("%1 осталась равнодушной.", ei.name); break;
		default: info("%1 оставил тебя без внимания.", ei.name); break;
		}
		dropdown(it);
	}
}

bool creature::is(room_s v) const {
	auto s = getsite();
	if(!s || s->type != Room)
		return false;
	return s->value == v;
}

diety_s creature::getgod() const {
	switch(getrace()) {
	case Orc: return GodGruumsh;
	case Dwarf: return GodMoradin;
	default: return GodMistra;
	}
}

item creature::craft(item_s type, variant effect, skill_s skill) {
	auto value = get(skill);
	if(!value)
		return item();
	item result(type);
	result.seteffect(effect);
	auto number = rand() % value;
	if(number < 13)
		result.set(Cursed);
	else if(number >= 70)
		result.set(Blessed);
	else if(number >= 98)
		result.set(Artifact);
	return result;
}
#include "main.h"

const int basic_enchantment_cost = 100;

static const char* power_text[][3] = {{"обычное", "обычный", "обычная"},
{"улучшенное", "улучшенный", "улучшенная"},
{"сильное", "сильный", "сильная"},
{"могущественное", "могущественный", "могущественная"},
};
static const char* damage_text[] = {0, "Треснуло", "Повреждено", "Сломано"};

static variant common_potions[] = {Dexterity, Wisdow, Charisma,
LifePoints, ManaPoints,
LifePoints, ManaPoints};
static variant uncommon_potions[] = {Strenght, Dexterity, Wisdow, Charisma,
LifeRate, ManaRate,
LifePoints, ManaPoints, Speed};
static variant rare_potions[] = {Attack, Damage,
Strenght, Dexterity, Constitution, Intellegence, Wisdow, Charisma,
Armor, Protection,
LifeRate, ManaRate,
LifePoints, ManaPoints, Speed};
static variant swords_enchanments[] = {{},
Attack, Damage,
Strenght, Dexterity, Constitution, Intellegence, Wisdow, Charisma,
Protection, LifePoints, ManaPoints, Speed};
static variant ring_enchanments[] = {ResistAcid, ResistCharm, ResistCold, ResistElectricity,
ResistFire, ResistParalize, ResistPoison, ResistWater,
Strenght, Dexterity, Constitution, Intellegence, Wisdow, Charisma,
Armor, Protection,
LifePoints, ManaPoints, Speed};
static variant boots_enchanments[] = {{}, ResistElectricity, ResistParalize, ResistWater,
Dexterity, Constitution,
Armor, Speed};
static variant wand_enchanments[] = {MagicMissile, ShokingGrasp, HealingSpell, ArmorSpell,
CharmPerson, FearSpell, Invisibility, Repair, SickSpell, Sleep};
static variant common_scroll[] = {BlessItem, DetectEvil, DetectMagic, Identify};

itemi bsmeta<itemi>::elements[] = {{"Рука", 0, 0, 0, NoGender, Organic, {0, 3, {1, 3}, Bludgeon, 4, 2}, {}, {}, {}, Melee},
{"Боевой топор", 850, 5 * GP, 0, Male, Iron, {-4, 3, {1, 8}, Slashing, 0, 2}, {}, {}, {Versatile}, Melee, FocusAxes},
{"Дубина", 1000, 5 * CP, 0, Female, Wood, {-6, 3, {1, 6}, Bludgeon, 0, 2}, {}, {}, {}, Melee, Bargaining},
{"Кинжал", 50, 2 * GP, 0, Male, Iron, {-2, 3, {1, 4}, Piercing, 2, 3}, {}, {}, {Light}, Melee},
{"Молот", 800, 2 * GP, 0, Male, Wood, {-3, 3, {2, 5}, Bludgeon, 0, 1}, {}, {}, {}, Melee},
{"Булава", 700, 8 * GP, 0, Female, Iron, {-3, 3, {2, 7}, Bludgeon, 0, 1}, {}, {}, {}, Melee},
{"Копье", 700, 8 * GP, 0, NoGender, Iron, {-5, 3, {1, 8}, Piercing, 0, 2}, {}, {}, {}, Melee},
{"Посох", 700, 8 * GP, 0, Male, Iron, {-2, 3, {1, 6}, Bludgeon, 1, 2}, {}, {}, {TwoHanded}, Melee},
{"Длинный меч", 700, 8 * GP, 0, Male, Iron, {-4, 3, {1, 8}, Slashing, 0, 2}, {}, swords_enchanments, {Versatile}, Melee, FocusSwords},
{"Короткий меч", 700, 8 * GP, 0, Male, Iron, {-3, 3, {1, 6}, Slashing, 0, 2}, {}, swords_enchanments, {Light}, Melee, FocusSwords},
{"Двуручный меч", 700, 8 * GP, 0, Male, Iron, {-9, 3, {2, 12}, Slashing, -2, 2}, {}, swords_enchanments, {TwoHanded}, Melee, FocusTwohanded},
//
{"Арбалет", 700, 40 * GP, 0, Male, Wood, {-3, 3, {1, 8}, Piercing, -2, 0, Bolt}, {}, {}, {}, Ranged},
{"Тяжелый арбалет", 1200, 80 * GP, 0, Male, Wood, {-10, 3, {1, 12}, Piercing, -6, 0, Bolt}, {}, {}, {}, Ranged},
{"Длинный лук", 500, 60 * GP, 0, Male, Wood, {-10, 3, {1, 8}, Piercing, 1, 0, Arrow}, {}, {}, {}, Ranged, FocusBows},
{"Короткий лук", 300, 30 * GP, 0, Male, Wood, {-6, 3, {1, 6}, Piercing, 1, 0, Arrow}, {}, {}, {}, Ranged, FocusBows},
{"Дарт", 30, 1 * SP, 0, Male, Wood, {-4, 3, {1, 3}, Piercing, 3}, {}, {}, {}, Ranged},
{"Праща", 50, 1 * SP, 0, Female, Leather, {-6, 3, {1, 4}, Bludgeon, 0, 0, Rock}, {}, {}, {}, Ranged},
//
{"Камни", 20, 0, 0, NoGender, Stone, {}, {}, {}, {Countable}, Amunitions},
{"Стрелы", 3, 2 * CP, 0, NoGender, Wood, {}, {}, {}, {Countable}, Amunitions},
{"Болты", 2, 1 * CP, 0, NoGender, Iron, {}, {}, {}, {Countable}, Amunitions},
//
{"Кожанная броня", 1000, 5 * GP, 0, Female, Leather, {-5}, {10, 1, 15}, {}, {}, Torso},
{"Клепанная броня", 1500, 15 * GP, 0, Female, Leather, {-7}, {15, 1, 15}, {}, {}, Torso},
{"Чешуйчатый доспех", 2500, 30 * GP, 0, Male, Iron, {-12}, {25, 2, 30}, {}, {}, Torso},
{"Кольчуга", 2600, 50 * GP, 0, Female, Iron, {-10}, {25, 3, 25}, {}, {}, Torso},
{"Бахрец", 3000, 200 * GP, 0, Male, Iron, {-15}, {30, 4, 35}, {}, {}, Torso},
{"Латы", 3500, 800 * GP, 0, Female, Iron, {-20}, {40, 5, 35}, {}, {}, Torso},
//
{"Щит", 800, 0 * GP, 0, Male, Iron, {-5, 1}, {10, 0, 25, 5}, {}, {}, OffHand},
{"Шлем", 100, 0 * GP, 0, Male, Iron, {-1}, {5, 0, 20, 3}, {}, {}, Head},
{"Наручи", 70, 0 * GP, 0, NoGender, Iron, {-1}, {5, 0, 15, 2}, {}, {}, Elbows},
{"Железные наручи", 110, 0 * GP, 0, NoGender, Iron, {-3}, {5, 1, 20, 2}, {}, {}, Elbows},
//
{"Плащ", 30, 10 * SP, -2, Male, Leather, {}, {3, 0, 2}, {}, {}, TorsoBack},
{"Плащ", 30, 15 * GP, -1, Male, Leather, {}, {3, 0, 2}, {}, {}, TorsoBack},
{"Плащ", 35, 20 * GP, 0, Female, Leather, {}, {3, 0, 2}, {}, {}, TorsoBack},
{"Плащ", 35, 5 * GP, 1, Female, Leather, {}, {5, 0, 2}, {}, {}, TorsoBack},
{"Плащ", 40, 5 * GP, 2, Female, Leather, {}, {5, 0, 2}, {}, {}, TorsoBack},
//
{"Туфли", 0, 0 * GP, -1, NoGender, Leather, {}, {3, 0, 10}, boots_enchanments, {}, Legs},
{"Сапоги", 0, 0 * GP, 0, NoGender, Leather, {}, {3, 0, 10}, boots_enchanments, {}, Legs},
{"Сапоги", 0, 0 * GP, -1, NoGender, Iron, {}, {5, 0, 15}, boots_enchanments, {}, Legs},
{"Сапоги", 0, 0 * GP, 0, NoGender, Iron, {}, {3, 1, 10}, boots_enchanments, {}, Legs},
{"Сапоги", 0, 0 * GP, 1, NoGender, Iron, {}, {3, 1, 10}, boots_enchanments, {}, Legs},

{"Еда", 100, 5 * SP, 0, NoGender, Organic, {}, {}, {}, {}, Edible},
{"Яблоко", 10, 5 * CP, 0, NoGender, Organic, {}, {}, {}, {}, Edible},
{"Хлеб хоббитов", 50, 1 * SP, 0, NoGender, Organic, {}, {}, {}, {}, Edible},
{"Хлеб эльфов", 50, 2 * SP, 1, NoGender, Organic, {}, {}, {}, {}, Edible},
{"Хлеб гномов", 80, 8 * CP, -1, NoGender, Organic, {}, {}, {}, {}, Edible},
{"Пирожное", 20, 1 * GP, 0, NoGender, Organic, {}, {}, {}, {}, Edible},
{"Колбаса", 60, 8 * SP, 0, NoGender, Organic, {}, {}, {}, {}, Edible},
{"Мясо", 80, 5 * CP, -1, NoGender, Organic, {}, {}, {}, {}, Edible},
//
{"Свиток", 1, 10 * GP, -1, Male, Paper, {}, {}, common_scroll, {SingleUse}, Readable},
{"Свиток", 1, 12 * GP, 0, Male, Paper, {}, {}, common_scroll, {SingleUse}, Readable},
{"Свиток", 1, 15 * GP, 1, Male, Paper, {}, {}, common_scroll, {SingleUse}, Readable},
//
{"Жезл", 10, 100 * GP, -2, NoGender, Wood, {}, {}, wand_enchanments, {Chargeable}, Zapable},
{"Жезл", 10, 120 * GP, -1, NoGender, Wood, {}, {}, wand_enchanments, {Chargeable}, Zapable},
{"Жезл", 20, 150 * GP, 0, NoGender, Iron, {}, {}, wand_enchanments, {Chargeable}, Zapable},
{"Жезл", 20, 160 * GP, 1, NoGender, Iron, {}, {}, wand_enchanments, {Chargeable}, Zapable},
{"Жезл", 30, 180 * GP, 2, NoGender, Iron, {}, {}, wand_enchanments, {Chargeable}, Zapable},
//
{"Книга", 0, 0 * GP, -1, NoGender, Paper, {}, {}, {}, {}},
{"Книга", 0, 0 * GP, 0, NoGender, Paper, {}, {}, {}, {}},
{"Книга", 0, 0 * GP, 0, NoGender, Paper, {}, {}, {}, {}},
{"Мануал", 0, 0 * GP, 1, NoGender, Paper, {}, {}, {}, {}},
{"Том", 0, 0 * GP, 2, NoGender, Paper, {}, {}, {}, {}},
//
{"Зелье", 15, 20 * GP, -1, NoGender, Glass, {}, {}, common_potions, {}, Drinkable},
{"Зелье", 20, 30 * GP, 0, NoGender, Glass, {}, {}, common_potions, {}, Drinkable},
{"Зелье", 10, 40 * GP, 0, NoGender, Glass, {}, {}, uncommon_potions, {}, Drinkable},
{"Экстракт", 10, 45 * GP, 1, Male, Glass, {}, {}, rare_potions, {}, Drinkable},
{"Элексир", 5, 50 * GP, 2, Male, Glass, {}, {}, rare_potions, {}, Drinkable},
//
{"Кольцо", 1, 35 * GP, 0, NoGender, Iron, {}, {}, ring_enchanments, {}, RightFinger},
{"Кольцо", 1, 40 * GP, 0, NoGender, Iron, {}, {}, ring_enchanments, {}, RightFinger},
{"Кольцо", 2, 45 * GP, 1, NoGender, Iron, {}, {}, ring_enchanments, {}, RightFinger},
//
{"Амулет", 5, 0 * GP, -1, NoGender, Wood, {}, {}, {}, {}, Neck},
{"Амулет", 6, 0 * GP, 0, NoGender, Wood, {}, {}, {}, {}, Neck},
{"Амулет", 6, 0 * GP, 0, NoGender, Iron, {}, {}, {}, {}, Neck},
{"Амулет", 7, 0 * GP, 1, NoGender, Iron, {}, {}, {}, {}, Neck},
{"Амулет", 7, 0 * GP, 2, NoGender, Iron, {}, {}, {}, {}, Neck},
//
{"Ключ", 0, 0 * GP, 0, NoGender, Iron, {}, {}, {}, {}},
//
{"Монеты", 0, 1 * CP, -1, NoGender, Iron, {}, {}, {}, {Coinable, Countable}},
{"Серебрянные монеты", 0, 1 * SP, 0, NoGender, Iron, {}, {}, {}, {Coinable, Countable}},
{"Золотые монеты", 0, 1 * GP, 1, NoGender, Iron, {}, {}, {}, {Coinable, Countable}},
//
{"Когти", 0, 0 * GP, 0, NoGender, Organic, {}, {}, {}, {Natural}, Melee},
{"Кулаки", 0, 0 * GP, 0, NoGender, Organic, {}, {}, {}, {Natural}, Melee},
{"Укус", 0, 0 * GP, 0, NoGender, Organic, {}, {}, {}, {Natural}, Melee},
{"Хитин", 0, 0 * GP, 0, NoGender, Leather, {-10}, {30, 2, 40}, {}, {Natural}, Torso},
{"Мех", 0, 0 * GP, 0, NoGender, Leather, {-3}, {20, 0, 20}, {}, {Natural}, Torso},
//
{"Много предметов"},
};
assert_enum(item, ManyItems);
static_assert(sizeof(item) == sizeof(int), "Struct 'item' must be sizeof(int)");

item::item(item_s type, int level) {
	auto chance_artifact = level / 3;
	auto chance_magic = level;
	auto chance_cursed = 5;
	auto chance_quality = 15 + level;
	if(chance_quality > 70)
		chance_quality = 70;
	create(type, chance_artifact, chance_magic, chance_cursed, chance_quality);
}

void item::create(item_s item_type, int chance_artifact, int chance_magic, int chance_cursed, int chance_quality) {
	clear();
	type = item_type;
	auto& ei = getitem();
	magic = Mundane;
	if(chance_magic) {
		if(d100() < chance_cursed)
			magic = Cursed;
		else if(d100() < chance_magic)
			magic = Blessed;
		else if(d100() < chance_artifact)
			magic = Artifact;
	}
	quality = 0;
	if(chance_quality && (d100() < chance_quality)) {
		static char quality_chances[] = {1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3};
		quality = maprnd(quality_chances);
	}
	if(!iscountable() && ei.effects) {
		if(ei.effects[0])
			effect = rand() % ei.effects.getcount();
		else if(ei.effects[1]) {
			if(d100() < chance_magic)
				effect = 1 + (rand() % (ei.effects.getcount() - 1));
			else
				effect = 0;
		}
	}
	if(ischargeable())
		charge = xrand(3, 18) + quality * 3;
	if(iscountable())
		setcount(xrand(10, 20));
}

creature* item::getwearer() const {
	auto i = bsmeta<creature>::source.indexof((creature*)(this));
	if(i == -1)
		return 0;
	return (creature*)bsmeta<creature>::source.ptr(i);
}

slot_s item::getwearerslot() const {
	return getwearer()->getwearerslot(this);
}

bool item::is(slot_s v) const {
	auto& ei = getitem();
	switch(v) {
	case LeftFinger: return ei.slot == RightFinger;
	case OffHand: return ei.slot == OffHand || (ei.slot == Melee && ei.flags.is(Light));
	default: return ei.slot == v;
	}
}

const char* item::getdamagetext() const {
	return damage_text[damaged];
}

void item::getstatistic(stringbuilder& sb) const {
	auto& ei = getitem();
	if(ei.slot >= Head && ei.slot <= Ranged) {
		auto ai = getattack();
		if(ai.attack)
			sb.adds("%1:%2i%%", bsmeta<abilityi>::elements[Attack].name_short, ai.attack);
		if(ai.dice.max)
			sb.adds("%1:%2i-%3i", bsmeta<abilityi>::elements[Damage].name_short, ai.dice.min, ai.dice.max);
		auto am = getarmor();
		if(am.protection || am.armor) {
			sb.adds("%1:%2i%%", bsmeta<abilityi>::elements[Armor].name_short, am.protection);
			if(am.armor)
				sb.adds("и %1i", am.armor);
			if(am.deflect)
				sb.adds("%1:%2i%%", bsmeta<abilityi>::elements[Deflect].name_short, am.deflect);
		}
	}
}

void item::getname(stringbuilder& sb, bool show_cab) const {
	auto& ei = getitem();
	auto q = getbonus();
	if(show_cab) {
		if(is(KnownMagic)) {
			if(getmagic())
				sb.adds(bsmeta<item_typei>::elements[getmagic()].name[ei.gender]);
		}
	}
	sb.adds("%-1", getname());
	if(is(KnownPower)) {
		const char* format = "%+1i";
		auto effect = geteffect();
		if(effect) {
			if(q < 0)
				sb.adds(effect.getnameofc());
			else
				sb.adds(effect.getnameof());
			if(effect.type == Ability) {
				format = bsmeta<abilityi>::elements[effect.value].format;
				q = bsmeta<abilityi>::elements[effect.value].getbonus(q);
			}
		}
		if(ei.slot >= Head && ei.slot <= Amunitions && q != 0)
			sb.adds(format, q);
	}
	if(iscountable()) {
		auto n = getcount();
		if(n > 1)
			sb.adds("%1i шт", getcount());
	}
}

void item::seteffect(variant v) {
	auto p = getwearer();
	if(p)
		p->dressoff();
	auto source = getitem().effects;
	if(source) {
		auto n = source.indexof(v);
		if(n != -1)
			effect = n;
	}
	if(p)
		p->dresson();
}

int	item::getcount() const {
	if(!iscountable())
		return 1;
	return us[1] + 1;
}

void item::setcount(int count) {
	if(!iscountable() && count)
		return;
	auto p = getwearer();
	if(p)
		p->dressoff();
	if(!count)
		clear();
	else {
		if(count > 256 * 256)
			count = 256 * 256;
		us[1] = count - 1;
	}
	if(p)
		p->dresson();
}

bool item::use() {
	auto c = getcount();
	setcount(getcount() - 1);
	return c > 1;
}

void item::usecharge() {
	if(!charge || !ischargeable())
		return;
	if(charge == 1) {
		if(d100() < chance_turn_chargeable_to_dust) {
			auto p = getwearer();
			if(p) {
				p->act("%1 рассыпался в прах.", getname());
				p->dressoff();
			}
			clear();
			if(p)
				p->dresson();
		}
	} else
		charge--;
}

bool item::ismatch(variant v) const {
	switch(v.type) {
	case NoVariant:
		return true;
	case Item:
		if(type == v.value)
			return true;
		break;
	case ItemType:
		if(magic == v.value)
			return true;
		break;
	case Slot:
		if(is((slot_s)v.value))
			return true;
		break;
	}
	return false;
}

int	item::getbonus() const {
	auto m = 0;
	switch(magic) {
	case Artifact:
		m = 2 + quality;
		break;
	case Blessed:
		m = 1 + quality;
		break;
	case Cursed:
		return -1 - quality;
	default:
		m = quality;
		break;
	}
	auto& ei = getitem();
	if(ei.effects.count && (effect || ei.effects.data[0]))
		m++;
	return m;
}

variant item::geteffect() const {
	if(is(Countable))
		return variant();
	auto& ei = getitem();
	if(ei.effects.getcount() > 0)
		return ei.effects[effect];
	return variant();
}

armori item::getarmor() const {
	auto result = getitem().armor;
	auto d = getdamage();
	auto b = getbonus();
	result.protection += b*result.protection_bonus - d;
	result.armor += b*result.armor_bonus - d;
	if(result.armor < 0)
		result.armor = 0;
	return result;
}

attacki item::getattack() const {
	auto& ei = getitem();
	auto result = ei.weapon;
	auto d = getdamage();
	auto b = getbonus();
	result.attack += b * ei.weapon.attack_bonus - d;
	if(ei.weapon.damage_bonus) {
		result.dice.min += b / ei.weapon.damage_bonus;
		result.dice.max += b / ei.weapon.damage_bonus - d;
		if(result.dice.min < 0)
			result.dice.min = 0;
		if(result.dice.max < 0)
			result.dice.max = 0;
	}
	return result;
}

void item::set(item_type_s v) {
	auto p = getwearer();
	if(p)
		p->dressoff();
	magic = v;
	if(p)
		p->dresson();
}

void item::set(identify_s v) {
	if(!v || v > identify) {
		auto p = getwearer();
		if(p)
			p->dressoff();
		identify = v;
		if(p)
			p->dresson();
	}
}

void item::setquality(int v) {
	auto p = getwearer();
	if(p)
		p->dressoff();
	quality = v;
	if(p)
		p->dresson();
}

void item::loot() {
	set(Unknown);
}

static unsigned getenchantcost(variant id, item_type_s magic, int quality) {
	static int power_cost[] = {0, 0, 1, 10};
	auto m = 2;
	switch(id.type) {
	case Ability: m += bsmeta<abilityi>::elements[id.value].cost; break;
	case Skill: m += 1; break;
	}
	m += power_cost[magic];
	m += quality;
	return m * basic_enchantment_cost;
}

unsigned item::getcost() const {
	if(iscountable())
		return bsmeta<itemi>::elements[type].cost*getcount();
	return bsmeta<itemi>::elements[type].cost;
}

int	item::getdamage() const {
	if(is(Countable))
		return 0;
	return damaged;
}

indext item::getposition() const {
	auto p = getwearer();
	if(p)
		return p->getposition();
	return Blocked;
}

void item::act(const char* format, ...) const {
	actv(sb, format, xva_start(format));
}

void item::actv(stringbuilder& st, const char* format, const char* format_param) const {
	auto p = creature::getactive();
	if(p) {
		auto i1 = getposition();
		auto i2 = p->getposition();
		if(i1 == Blocked || i2 == Blocked)
			return;
	}
	string sb = st;
	sb.name = getname();
	sb.gender = getgender();
	sb.addsep(' ');
	sb.addv(format, format_param);
	st = sb;
}

bool item::stack(item& v) {
	if(!iscountable() || us[0] != v.us[0])
		return false;
	int c1 = us[1];
	int c2 = v.us[1];
	auto result = false;
	auto p1 = getwearer();
	auto p2 = v.getwearer();
	if(p1 == p2)
		p2 = 0;
	if(p1)
		p1->dressoff();
	if(p2)
		p2->dressoff();
	c1 += c2 + 1;
	if(c1 <= 0xFFFF) {
		us[1] = c1;
		v.clear();
		result = true;
	} else {
		us[1] = 0xFFFF;
		v.us[1] = c1 - 0xFFFF - 1;
	}
	if(p2)
		p2->dresson();
	if(p1)
		p1->dresson();
	return result;
}

void item::destroy() {
	auto p = getwearer();
	if(p)
		p->dressoff();
	clear();
	if(p)
		p->dresson();
}

void item::decoy(bool interactive) {
	if(magic == Artifact || is(Natural))
		return;
	if(iscountable())
		return;
	if(magic == Blessed && damaged == 3)
		return;
	auto& ei = bsmeta<itemi>::elements[getkind()];
	if(damaged < 3)
		damaged++;
	else {
		if(interactive) {
			static descriptioni damage_text[] = {
				{Glass, Fire, "%герой расплавил%ась и взорвалась."},
				{Glass, {}, "%герой разбил%ась вдребезги."},
				{Wood, Fire, "%герой сгорел%а до тла."},
				{Paper, Fire, "%герой моментально превратил%ась в пепел."},
				{{}, Fire, "%герой расплавил%ась."},
				{{}, Cold, "%герой замерз%ла и разлетел%ась на куски."},
				{{}, {}, "%герой уничтожен%а."}
			};
			act(damage_text->get(ei.material, type));
		}
		destroy();
	}
}

void item::damage(int count, damage_s type, bool interactive) {
	if(count < 0)
		return;
	auto& ei = bsmeta<itemi>::elements[getkind()];
	auto chance_resist = 70;
	chance_resist -= count;
	chance_resist += bsmeta<materiali>::elements[ei.material].resist.data[type];
	chance_resist += getbonus() * 4;
	if(chance_resist < 5)
		chance_resist = 5;
	else if(chance_resist > 95)
		chance_resist = 95;
	auto roll_result = d100();
	if(roll_result < chance_resist)
		return;
	decoy(interactive);
}
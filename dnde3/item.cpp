#include "main.h"

const int basic_enchantment_cost = 100;

static const char* power_text[][3] = {{"обычное", "обычный", "обычная"},
{"улучшенное", "улучшенный", "улучшенная"},
{"сильное", "сильный", "сильная"},
{"могущественное", "могущественный", "могущественная"},
};

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

itemi bsmeta<itemi>::elements[] = {{"Рука", 0, 0, NoGender, Organic, {0, 3, {1, 3}, Bludgeon, 4, 2}, {}, {}, {}, Melee},
{"Боевой топор", 850, 5 * GP, Male, Iron, {-4, 3, {1, 8}, Slashing, 0, 2}, {}, {}, {Versatile}, Melee, FocusAxes},
{"Дубина", 1000, 5 * CP, Female, Wood, {-6, 3, {1, 6}, Bludgeon, 0, 2}, {}, {}, {}, Melee},
{"Кинжал", 50, 2 * GP, Male, Iron, {-2, 3, {1, 4}, Piercing, 2, 3}, {}, {}, {Light}, Melee},
{"Молот", 800, 2 * GP, Male, Wood, {-3, 3, {2, 5}, Bludgeon, 0, 1}, {}, {}, {}, Melee},
{"Булава", 700, 8 * GP, Female, Iron, {-3, 3, {2, 7}, Bludgeon, 0, 1}, {}, {}, {}, Melee},
{"Копье", 700, 8 * GP, NoGender, Iron, {-5, 3, {1, 8}, Piercing, 0, 2}, {}, {}, {}, Melee},
{"Посох", 700, 8 * GP, Male, Iron, {-2, 3, {1, 6}, Bludgeon, 1, 2}, {}, {}, {TwoHanded}, Melee},
{"Длинный меч", 700, 8 * GP, Male, Iron, {-4, 3, {1, 8}, Slashing, 0, 2}, {}, swords_enchanments, {Versatile}, Melee, FocusSwords},
{"Короткий меч", 700, 8 * GP, Male, Iron, {-3, 3, {1, 6}, Slashing, 0, 2}, {}, swords_enchanments, {Light}, Melee, FocusSwords},
{"Двуручный меч", 700, 8 * GP, Male, Iron, {-9, 3, {2, 12}, Slashing, -2, 2}, {}, swords_enchanments, {TwoHanded}, Melee, FocusTwohanded},
//
{"Арбалет", 700, 40 * GP, Male, Wood, {-3, 3, {1, 8}, Piercing, -2, Bolt}, {}, {}, {}, Ranged},
{"Тяжелый арбалет", 1200, 80 * GP, Male, Wood, {-10, 3, {1, 12}, Piercing, -6, Bolt}, {}, {}, {}, Ranged},
{"Длинный лук", 500, 60 * GP, Male, Wood, {-10, 3, {1, 8}, Piercing, 1, Arrow}, {}, {}, {}, Ranged, FocusBows},
{"Короткий лук", 300, 30 * GP, Male, Wood, {-6, 3, {1, 6}, Piercing, 1, Arrow}, {}, {}, {}, Ranged, FocusBows},
{"Дарт", 30, 1 * SP, Male, Wood, {-4, 3, {1, 3}, Piercing, 3}, {}, {}, {}, Ranged},
{"Праща", 50, 1 * SP, Female, Leather, {-6, 3, {1, 4}, Bludgeon, 0, Rock}, {}, {}, {}, Ranged},
//
{"Камни", 20, 0, Male, Stone, {}, {}, {}, {}, Amunitions, Bargaining, 30},
{"Стрелы", 3, 2 * CP, Female, Wood, {}, {}, {}, {}, Amunitions, Bargaining, 20},
{"Болты", 2, 1 * CP, Male, Iron, {}, {}, {}, {}, Amunitions, Bargaining, 20},
//
{"Кожанная броня", 1000, 5 * GP, Female, Leather, {-5}, {10, 1, 15}, {}, {}, Torso},
{"Клепанная броня", 1500, 15 * GP, Female, Leather, {-7}, {15, 1, 15}, {}, {}, Torso},
{"Чешуйчатый доспех", 2500, 30 * GP, Male, Iron, {-12}, {25, 2, 30}, {}, {}, Torso},
{"Кольчуга", 2600, 50 * GP, Female, Iron, {-10}, {25, 3, 25}, {}, {}, Torso},
{"Бахрец", 3000, 200 * GP, Male, Iron, {-15}, {30, 4, 35}, {}, {}, Torso},
{"Латы", 3500, 800 * GP, Female, Iron, {-20}, {40, 5, 35}, {}, {}, Torso},
//
{"Щит", 800, 0 * GP, Female, Iron, {-5, 1}, {10, 0, 25}, {}, {}, OffHand},
{"Шлем", 100, 0 * GP, Female, Iron, {-1}, {5, 0, 20}, {}, {}, Head},
{"Наручи", 70, 0 * GP, Female, Iron, {-1}, {5, 0, 15}, {}, {}, Elbows},
{"Железные наручи", 110, 0 * GP, Female, Iron, {-3}, {5, 1, 20}, {}, {}, Elbows},
//
{"Плащ", 30, 10 * SP, Male, Leather, {}, {3, 0, 2}, {}, {}, TorsoBack},
{"Плащ", 30, 15 * GP, Male, Leather, {}, {3, 0, 2}, {}, {}, TorsoBack},
{"Плащ", 35, 20 * GP, Female, Leather, {}, {3, 0, 2}, {}, {}, TorsoBack},
{"Плащ", 35, 5 * GP, Female, Leather, {}, {5, 0, 2}, {}, {}, TorsoBack},
{"Плащ", 40, 5 * GP, Female, Leather, {}, {5, 0, 2}, {}, {}, TorsoBack},
//
{"Туфли", 0, 0 * GP, NoGender, Leather, {}, {3, 0, 10}, boots_enchanments, {}, Legs},
{"Сапоги", 0, 0 * GP, NoGender, Leather, {}, {3, 0, 10}, boots_enchanments, {}, Legs},
{"Сапоги", 0, 0 * GP, NoGender, Iron, {}, {5, 0, 15}, boots_enchanments, {}, Legs},
{"Сапоги", 0, 0 * GP, NoGender, Iron, {}, {3, 1, 10}, boots_enchanments, {}, Legs},
{"Сапоги", 0, 0 * GP, NoGender, Iron, {}, {3, 1, 10}, boots_enchanments, {}, Legs},

{"Еда", 100, 5 * SP, NoGender, Organic, {}, {}, {}, {}, Edible},
{"Яблоко", 10, 5 * CP, NoGender, Organic, {}, {}, {}, {}, Edible},
{"Хлеб хоббитов", 50, 1 * SP, NoGender, Organic, {}, {}, {}, {}, Edible},
{"Хлеб эльфов", 50, 2 * SP, NoGender, Organic, {}, {}, {}, {}, Edible},
{"Хлеб гномов", 80, 8 * CP, NoGender, Organic, {}, {}, {}, {}, Edible},
{"Пирожное", 20, 1 * GP, NoGender, Organic, {}, {}, {}, {}, Edible},
{"Колбаса", 60, 8 * SP, NoGender, Organic, {}, {}, {}, {}, Edible},
{"Мясо", 80, 5 * CP, NoGender, Organic, {}, {}, {}, {}, Edible},
//
{"Свиток", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}, Readable},
{"Свиток", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}, Readable},
{"Свиток", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}, Readable},
//
{"Жезл", 0, 0 * GP, NoGender, Wood, {}, {}, {}, {}},
{"Жезл", 0, 0 * GP, NoGender, Wood, {}, {}, {}, {}},
{"Жезл", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
{"Жезл", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
{"Жезл", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
//
{"Книга", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Книга", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Книга", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Мануал", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"Том", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
//
{"Зелье", 15, 20 * GP, NoGender, Glass, {}, {}, common_potions, {}, Drinkable},
{"Зелье", 20, 30 * GP, NoGender, Glass, {}, {}, common_potions, {}, Drinkable},
{"Зелье", 10, 40 * GP, NoGender, Glass, {}, {}, uncommon_potions, {}, Drinkable},
{"Экстракт", 10, 45 * GP, Male, Glass, {}, {}, rare_potions, {}, Drinkable},
{"Элексир", 5, 50 * GP, Male, Glass, {}, {}, rare_potions, {}, Drinkable},
//
{"Кольцо", 1, 35 * GP, NoGender, Iron, {}, {}, ring_enchanments, {}, RightFinger},
{"Кольцо", 1, 40 * GP, NoGender, Iron, {}, {}, ring_enchanments, {}, RightFinger},
{"Кольцо", 2, 45 * GP, NoGender, Iron, {}, {}, ring_enchanments, {}, RightFinger},
//
{"Амулет", 5, 0 * GP, NoGender, Wood, {}, {}, {}, {}, Neck},
{"Амулет", 6, 0 * GP, NoGender, Wood, {}, {}, {}, {}, Neck},
{"Амулет", 6, 0 * GP, NoGender, Iron, {}, {}, {}, {}, Neck},
{"Амулет", 7, 0 * GP, NoGender, Iron, {}, {}, {}, {}, Neck},
{"Амулет", 7, 0 * GP, NoGender, Iron, {}, {}, {}, {}, Neck},
//
{"Ключ", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
//
{"Монета", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
{"Серебрянная монета", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
{"Золотая монета", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
//
{"Когти", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {Natural}, Melee},
{"Кулаки", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {Natural}, Melee},
{"Укус", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {Natural}, Melee},
{"Хитин", 0, 0 * GP, NoGender, Leather, {-10}, {30, 2, 40}, {}, {Natural}, Torso},
{"Мех", 0, 0 * GP, NoGender, Leather, {-3}, {20, 0, 20}, {}, {Natural}, Torso},
//
{"Много предметов"},
};
assert_enum(item, ManyItems);
static_assert(sizeof(item) == sizeof(int), "Struct 'item' must be sizeof(int)");

item::item(item_s type) {
	clear();
	this->type = type;
	auto& ei = getitem();
	if(ei.count > 0)
		setcount(ei.count);
}

item::item(item_s item_type, int chance_artifact, int chance_magic, int chance_cursed, int chance_quality) {
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
	if(ei.effects) {
		if(ei.effects[0])
			effect = rand() % ei.effects.getcount();
		else if(ei.effects[1]) {
			if(d100() < chance_magic)
				effect = 1 + (rand() % (ei.effects.getcount() - 1));
			else
				effect = 0;
		}
	}
	if(ei.count > 0)
		setcount(ei.count);
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
	int q = quality;
	if(ei.slot >= Head && ei.slot <= Amunitions)
		q = getbonus();
	else if(is(Cursed))

	if(show_cab) {
		if(is(KnownMagic))
			sb.adds(bsmeta<item_typei>::elements[getmagic()].name[ei.gender]);
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
		if(q != 0)
			sb.adds(format, q);
	}
	auto n = getcount();
	if(n > 1)
		sb.adds("%1iшт.", n);
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

void item::setcount(int count) {
	auto p = getwearer();
	if(p)
		p->dressoff();
	if(!count)
		clear();
	else if(iscountable()) {
		auto mc = getitem().count;
		if(count > mc)
			count = mc;
		this->count = count - 1;
	}
	if(p)
		p->dresson();
}

bool item::use() {
	auto c = getcount();
	setcount(getcount() - 1);
	return c > 1;
}

bool item::match(variant v) const {
	switch(v.type) {
	case Item:
		if(type != v.value)
			return false;
		break;
	case ItemType:
		if(magic != v.value)
			return false;
		break;
	}
	return true;
}

int	item::getbonus() const {
	int m = 0;
	switch(magic) {
	case Artifact:
		m = 2 + quality;
		break;
	case Blessed:
		m = 1 + quality;
		break;
	case Cursed:
		return -1 - (int)quality;
	default:
		m = quality;
		break;
	}
	auto& ei = getitem();
	if(!ei.effects.count)
		return m;
	if(effect || ei.effects.data[0])
		m++;
	return m;
}

variant item::geteffect() const {
	auto& ei = getitem();
	if(ei.effects.getcount() > 0)
		return ei.effects[effect];
	return variant();
}

armori item::getarmor() const {
	auto result = getitem().armor;
	auto b = getbonus() - damaged;
	result.protection += b*result.protection_bonus;
	result.armor += b*result.armor_bonus;
	if(result.armor < 0)
		result.armor = 0;
	return result;
}

attacki item::getattack() const {
	auto& ei = getitem();
	auto result = ei.weapon;
	auto b = getbonus() - damaged;
	result.attack += b * ei.weapon.attack_bonus;
	if(ei.weapon.damage_bonus) {
		result.dice.min += b / ei.weapon.damage_bonus;
		result.dice.max += b / ei.weapon.damage_bonus;
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

unsigned getenchantcost(variant id, item_type_s magic, int quality) {
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
	return bsmeta<itemi>::elements[type].cost
		+ getenchantcost(geteffect(), getmagic(), quality);
}
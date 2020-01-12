#include "main.h"

const int basic_enchantment_cost = 100;

static const char* power_text[][3] = {{"обычное", "обычный", "обычная"},
{"улучшенное", "улучшенный", "улучшенная"},
{"сильное", "сильный", "сильная"},
{"могущественное", "могущественный", "могущественная"},
};
static const char* damage_text[] = {0, "Треснуло", "Повреждено", "Сломано"};

static variant common_potions[] = {LifePoints, ManaPoints};
static variant uncommon_potions[] = {Strenght, Dexterity, Wisdow, Charisma,
LifeRate, ManaRate, Level,
LifePoints, ManaPoints, Speed};
static variant rare_potions[] = {Attack, Damage,
Strenght, Dexterity, Constitution, Intellegence, Wisdow, Charisma,
Armor, Protection, Level,
LifeRate, ManaRate,
LifePoints, ManaPoints, Speed};
static variant weapon_enchanments[] = {{},
Attack, Damage, Speed,
Strenght, Constitution};
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
static variant wand_common_spells[] = {MagicMissile, ShokingGrasp, HealingSpell, ArmorSpell,
CharmPerson, FearSpell, Invisibility, Repair, SickSpell, Sleep};
static variant mage_common_spells[] = {MagicMissile, ShokingGrasp,
ArmorSpell, CharmPerson, FearSpell, Invisibility, Repair, Sleep};
static variant common_scroll[] = {BlessItem, DetectEvil, DetectMagic, Identify};
static variant common_amulet[] = {Charisma, Strenght, Lockpicking};

itemi bsmeta<itemi>::elements[] = {{"Рука", Unique, 0, 0, 0, NoGender, Organic, {0, 3, {1, 3}, Bludgeon, 4, 2}, {}, {}, {}, Melee},
{"Боевой топор", Common, 850, 5 * GP, 0, Male, Iron, {-4, 3, {1, 8}, Slashing, 0, 2}, {}, weapon_enchanments, {Versatile}, Melee, FocusAxes},
{"Дубина", Common, 1000, 5 * CP, 0, Female, Wood, {-6, 3, {1, 6}, Bludgeon, 0, 2}, {}, weapon_enchanments, {}, Melee, Bargaining},
{"Кинжал", Common, 50, 2 * GP, 0, Male, Iron, {-2, 3, {1, 4}, Piercing, 2, 3}, {}, weapon_enchanments, {Light}, Melee},
{"Молот", Common, 800, 2 * GP, 0, Male, Wood, {-3, 3, {2, 5}, Bludgeon, 0, 1}, {}, weapon_enchanments, {}, Melee},
{"Булава", Common, 700, 8 * GP, 0, Female, Iron, {-3, 3, {2, 7}, Bludgeon, 0, 1}, {}, weapon_enchanments, {}, Melee},
{"Копье", Common, 700, 8 * GP, 0, NoGender, Iron, {-5, 3, {1, 8}, Piercing, 0, 2}, {}, weapon_enchanments, {}, Melee},
{"Посох", Common, 700, 8 * GP, 0, Male, Iron, {-2, 3, {1, 6}, Bludgeon, 1, 2}, {}, weapon_enchanments, {TwoHanded}, Melee},
{"Длинный меч", Common, 700, 8 * GP, 0, Male, Iron, {-4, 3, {1, 8}, Slashing, 0, 2}, {}, swords_enchanments, {Versatile}, Melee, FocusSwords},
{"Короткий меч", Common, 700, 8 * GP, 0, Male, Iron, {-3, 3, {1, 6}, Slashing, 0, 2}, {}, swords_enchanments, {Light}, Melee, FocusSwords},
{"Двуручный меч", Uncommon, 700, 8 * GP, 0, Male, Iron, {-9, 3, {2, 12}, Slashing, -2, 2}, {}, swords_enchanments, {TwoHanded}, Melee, FocusTwohanded},
//
{"Арбалет", Uncommon, 700, 40 * GP, 0, Male, Wood, {-3, 3, {1, 8}, Piercing, -2, 0, Bolt}, {}, weapon_enchanments, {}, Ranged},
{"Тяжелый арбалет", Rare, 1200, 80 * GP, 0, Male, Wood, {-10, 3, {1, 12}, Piercing, -6, 0, Bolt}, {}, weapon_enchanments, {}, Ranged},
{"Длинный лук", Uncommon, 500, 60 * GP, 0, Male, Wood, {-10, 3, {1, 8}, Piercing, 1, 0, Arrow}, {}, weapon_enchanments, {}, Ranged, FocusBows},
{"Короткий лук", Common, 300, 30 * GP, 0, Male, Wood, {-6, 3, {1, 6}, Piercing, 1, 0, Arrow}, {}, weapon_enchanments, {}, Ranged, FocusBows},
{"Дарт", Uncommon, 30, 1 * SP, 0, Male, Wood, {-4, 3, {1, 3}, Piercing, 3}, {}, weapon_enchanments, {}, Ranged},
{"Праща", Uncommon, 50, 1 * SP, 0, Female, Leather, {-6, 3, {2, 4}, Bludgeon, 0, 0, Rock}, {}, weapon_enchanments, {}, Ranged},
//
{"Камень", Common, 15, 0, 0, Male, Stone, {0, 0, {1, 3}, Bludgeon, 0, 0, NoItem, Rock}, {}, {}, {Countable}, Ranged},
{"Стрела", Common, 3, 2 * CP, 0, Female, Wood, {0, 0, {}, Piercing, 0, 0, NoItem, Arrow}, {}, {}, {Countable}, Amunitions},
{"Болт", Common, 2, 1 * CP, 0, Male, Iron, {0, 0, {}, Piercing, 0, 0, NoItem, Bolt}, {}, {}, {Countable}, Amunitions},
//
{"Кожанная броня", Common, 1000, 5 * GP, 0, Female, Leather, {-5}, {10, 1, 15}, {}, {}, Torso},
{"Клепанная броня", Common, 1500, 15 * GP, 0, Female, Leather, {-7}, {15, 1, 15}, {}, {}, Torso},
{"Чешуйчатый доспех", Uncommon, 2500, 30 * GP, 0, Male, Iron, {-12}, {25, 2, 30}, {}, {}, Torso},
{"Кольчуга", Common, 2600, 50 * GP, 0, Female, Iron, {-10}, {25, 3, 25}, {}, {}, Torso},
{"Бахрец", Uncommon, 3000, 200 * GP, 0, Male, Iron, {-15}, {30, 4, 35}, {}, {}, Torso},
{"Латы", Rare, 3500, 800 * GP, 0, Female, Iron, {-20}, {40, 5, 35}, {}, {}, Torso},
//
{"Щит", Common, 800, 0 * GP, 0, Male, Iron, {-5, 1}, {10, 0, 25, 5}, {}, {}, OffHand},
{"Шлем", Common, 100, 0 * GP, 0, Male, Iron, {-1}, {5, 0, 20, 3}, {}, {}, Head},
{"Наручи", Common, 70, 0 * GP, 0, NoGender, Iron, {-1}, {5, 0, 15, 2}, {}, {}, Elbows},
{"Железные наручи", Uncommon, 110, 0 * GP, 0, NoGender, Iron, {-3}, {5, 1, 20, 2}, {}, {}, Elbows},
//
{"Плащ", Common, 30, 10 * SP, -2, Male, Leather, {}, {3, 0, 2}, {}, {}, TorsoBack},
{"Плащ", Common, 30, 15 * GP, -1, Male, Leather, {}, {3, 0, 2}, {}, {}, TorsoBack},
{"Плащ", Uncommon, 35, 20 * GP, 0, Female, Leather, {}, {3, 0, 2}, {}, {}, TorsoBack},
{"Плащ", Uncommon, 35, 5 * GP, 1, Female, Leather, {}, {5, 0, 2}, {}, {}, TorsoBack},
{"Плащ", Rare, 40, 5 * GP, 2, Female, Leather, {}, {5, 0, 2}, {}, {}, TorsoBack},
//
{"Туфли", Common, 100, 5 * GP, -1, NoGender, Leather, {}, {3, 0, 10}, boots_enchanments, {}, Legs},
{"Сапоги", Common, 200, 10 * GP, 0, NoGender, Leather, {}, {3, 0, 10}, boots_enchanments, {}, Legs},
{"Сапоги", Common, 400, 12 * GP, -1, NoGender, Iron, {}, {5, 0, 15}, boots_enchanments, {}, Legs},
{"Сапоги", Common, 500, 15 * GP, 0, NoGender, Iron, {}, {3, 1, 10}, boots_enchanments, {}, Legs},
{"Сапоги", Uncommon, 600, 17 * GP, 1, NoGender, Iron, {}, {3, 1, 10}, boots_enchanments, {}, Legs},
//
{"Еда", Common, 100, 5 * SP, 0, NoGender, Organic, {}, {}, {}, {}, Edible},
{"Яблоко", Common, 10, 5 * CP, 0, NoGender, Organic, {}, {}, {}, {}, Edible},
{"Хлеб хоббитов", Uncommon, 50, 1 * SP, 0, NoGender, Organic, {}, {}, {}, {}, Edible},
{"Хлеб эльфов", Uncommon, 50, 2 * SP, 1, NoGender, Organic, {}, {}, {}, {}, Edible},
{"Хлеб гномов", Uncommon, 80, 8 * CP, -1, NoGender, Organic, {}, {}, {}, {}, Edible},
{"Пирожное", Uncommon, 20, 1 * GP, 0, NoGender, Organic, {}, {}, {}, {}, Edible},
{"Колбаса", Common, 60, 8 * SP, 0, NoGender, Organic, {}, {}, {}, {}, Edible},
{"Мясо", Common, 80, 5 * CP, -1, NoGender, Organic, {}, {}, {}, {}, Edible},
//
{"Свиток", Common, 1, 10 * GP, -1, Male, Paper, {}, {}, common_scroll, {SingleUse}, Readable},
{"Свиток", Common, 1, 12 * GP, 0, Male, Paper, {}, {}, common_scroll, {SingleUse}, Readable},
{"Свиток", Uncommon, 1, 15 * GP, 1, Male, Paper, {}, {}, common_scroll, {SingleUse}, Readable},
//
{"Жезл", Uncommon, 10, 100 * GP, -2, NoGender, Wood, {}, {}, wand_common_spells, {}, Zapable},
{"Жезл", Uncommon, 10, 120 * GP, -1, NoGender, Wood, {}, {}, wand_common_spells, {}, Zapable},
{"Жезл", Uncommon, 20, 150 * GP, 0, NoGender, Iron, {}, {}, wand_common_spells, {}, Zapable},
{"Жезл", Uncommon, 20, 160 * GP, 1, NoGender, Iron, {}, {}, wand_common_spells, {}, Zapable},
{"Жезл", Uncommon, 30, 180 * GP, 2, NoGender, Iron, {}, {}, wand_common_spells, {}, Zapable},
//
{"Книга", Rare, 500, 100 * GP, -1, Female, Paper, {}, {}, mage_common_spells, {}, Readable},
{"Книга", Rare, 600, 150 * GP, 0, Female, Paper, {}, {}, mage_common_spells, {}, Readable},
{"Книга", Rare, 600, 200 * GP, 0, Female, Paper, {}, {}, mage_common_spells, {}, Readable},
{"Мануал", Rare, 800, 250 * GP, 1, Male, Paper, {}, {}, mage_common_spells, {}, Readable},
{"Том", VeryRare, 1000, 300 * GP, 2, Male, Paper, {}, {}, mage_common_spells, {}, Readable},
//
{"Зелье", Common, 10, 10 * GP, -1, NoGender, Glass, {}, {}, common_potions, {}, Drinkable},
{"Зелье", Common, 20, 35 * GP, 0, NoGender, Glass, {}, {}, uncommon_potions, {}, Drinkable},
{"Зелье", Uncommon, 10, 40 * GP, 0, NoGender, Glass, {}, {}, uncommon_potions, {}, Drinkable},
{"Экстракт", Rare, 10, 45 * GP, 1, Male, Glass, {}, {}, rare_potions, {}, Drinkable},
{"Элексир", Rare, 5, 80 * GP, 2, Male, Glass, {}, {}, rare_potions, {}, Drinkable},
//
{"Кольцо", Uncommon, 1, 35 * GP, 0, NoGender, Iron, {}, {}, ring_enchanments, {}, RightFinger},
{"Кольцо", Uncommon, 1, 40 * GP, 0, NoGender, Iron, {}, {}, ring_enchanments, {}, RightFinger},
{"Кольцо", Rare, 2, 60 * GP, 1, NoGender, Iron, {}, {}, ring_enchanments, {}, RightFinger},
//
{"Амулет", Uncommon, 20, 250 * GP, -1, Male, Wood, {}, {}, common_amulet, {}, Neck},
{"Медальон", Uncommon, 20, 300 * GP, 0, Male, Wood, {}, {}, common_amulet, {}, Neck},
{"Амулет", Uncommon, 20, 350 * GP, 0, Male, Iron, {}, {}, common_amulet, {}, Neck},
{"Амулет", Uncommon, 30, 400 * GP, 1, Male, Iron, {}, {}, common_amulet, {}, Neck},
{"Ожерелье", Rare, 40, 450 * GP, 2, NoGender, Iron, {}, {}, common_amulet, {}, Neck},
//
{"Ключ", Common, 0, 0 * GP, 0, Male, Iron, {}, {}, {}, {}},
//
{"Монета", Common, 0, 1 * CP, -1, Female, Iron, {}, {}, {}, {Coinable, Countable}},
{"Серебрянная монета", Common, 0, 1 * SP, 0, Female, Iron, {}, {}, {}, {Coinable, Countable}},
{"Золотая монета", Common, 0, 1 * GP, 1, Female, Iron, {}, {}, {}, {Coinable, Countable}},
//
{"Когти", Common, 0, 0 * GP, 0, NoGender, Organic, {-6, 3, {1, 6}, Slashing, 3, 1}, {}, {}, {Natural}, Melee},
{"Кулаки", Common, 0, 0 * GP, 0, NoGender, Organic, {0, 4, {1, 6}, Bludgeon, 0, 2}, {}, {}, {Natural}, Melee},
{"Укус", Common, 0, 0 * GP, 0, NoGender, Organic, {0, 2, {2, 5}, Slashing, 0, 1}, {}, {}, {Natural}, Melee},
{"Хитин", Uncommon, 0, 0 * GP, 0, NoGender, Leather, {-10}, {30, 2, 40}, {}, {Natural}, Torso},
{"Мех", Common, 0, 0 * GP, 0, NoGender, Leather, {-3}, {20, 0, 20}, {}, {Natural}, Torso},
//
{"Много предметов"},
};
assert_enum(item, ManyItems);
static_assert(sizeof(item) == sizeof(int), "Struct 'item' must be sizeof(int)");

bool itemi::is(slot_s v) const {
	switch(v) {
	case Amunitions: return weapon.ammunition_compatible != 0;
	case LeftFinger: return slot == RightFinger;
	case OffHand: return slot == OffHand || (slot == Melee && flags.is(Light));
	default: return slot == v;
	}
}

bool itemi::is(const aref<slot_s>& source) const {
	for(auto v : source) {
		if(is(v))
			return true;
	}
	return false;
}

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
	if(chance_magic > 0 && (d100() < chance_magic)) {
		if(d100() < chance_cursed)
			magic = Cursed;
		else if(d100() < chance_artifact)
			magic = Artifact;
		else
			magic = Blessed;
	}
	quality = 0;
	if(chance_quality > 0 && (d100() < chance_quality)) {
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
		charge = xrand(2, 12) + quality * 3;
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
			switch(effect.type) {
			case Ability:
				format = bsmeta<abilityi>::elements[effect.value].format;
				q = bsmeta<abilityi>::elements[effect.value].getbonus(q);
				break;
			case Skill:
				if(!is(Readable)) {
					q = q * 10;
					format = "%+1i%%";
				}
				break;
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
	if(sale) {
		auto n = getcost();
		if(n > 0)
			sb.adds("(цена %1i)", n);
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

void item::use() {
	if(iscountable()) {
		if(us[1])
			us[1]--;
		else
			destroy(Magic, false);
	} else if(ischargeable()) {
		if(charge)
			charge--;
		else
			destroy(Magic, true);
	} else
		destroy(Magic, false);
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
	if(ei.effects.count > 0)
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
	auto& ei = bsmeta<itemi>::elements[type];
	auto w = ei.cost;
	auto m = 100;
	if(sale == Sale75)
		m = 75;
	else if(sale == Sale150)
		m = 150;
	if(identify >= KnownMagic) {
		switch(magic) {
		case Blessed: m += 100; break;
		case Artifact: m += 1000; break;
		default: break;
		}
		if(identify >= KnownPower) {
			if(ei.slot >= Head && ei.slot <= Ranged) {
				w += 5 * GP * quality;
				auto effect = geteffect();
				if(effect) {
					if(ei.effects.count && !ei.effects[0])
						w += 10 * GP;
					switch(effect.type) {
					case Ability: w += 10 * GP * bsmeta<abilityi>::elements[effect.value].cost; break;
					}
				}
			}
		}
	}
	if(w < 0)
		w = 0;
	return w * m / 100;
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

void item::destroy(damage_s type, bool interactive) {
	auto p = getwearer();
	if(p)
		p->dressoff();
	if(interactive) {
		auto& ei = bsmeta<itemi>::elements[getkind()];
		static descriptioni text[] = {
			{Glass, Fire, "%герой расплавил%ась и взорвалась."},
			{Glass, {}, "%герой разбил%ась вдребезги."},
			{Wood, Fire, "%герой сгорел%а до тла."},
			{Paper, Fire, "%герой моментально превратил%ась в пепел."},
			{Paper, Magic, "%герой превратил%ась в пыль и рассыпал%ась."},
			{{}, Fire, "%герой расплавил%ась."},
			{{}, Cold, "%герой замерз%ла и разлетел%ась на куски."},
			{{}, {}, "%герой уничтожен%а."}
		};
		act(text->get(ei.material, type));
	}
	clear();
	if(p)
		p->dresson();
}

void item::decoy(damage_s type, bool interactive, bool include_artifact) {
	if(is(Natural))
		return;
	if(iscountable())
		return;
	if(!include_artifact) {
		if((magic == Blessed && damaged == 3) || magic == Artifact)
			return;
	}
	if(damaged < 3)
		damaged++;
	else
		destroy(type, interactive);
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
	decoy(type, interactive);
}

void item::breaktest() {
	if(!(*this))
		return;
	auto r = d100();
	if(r >= 3)
		return;
	damage(xrand(0, 5), Bludgeon, true);
}

bool item::ischargeable() const {
	auto v = geteffect();
	if(v.type == Spell)
		return true;
	return false;
}
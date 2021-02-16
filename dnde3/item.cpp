#include "main.h"

static unsigned short sale_percent[8] = {0, 65, 80, 100, 150, 200, 250, 300};
static const char* damage_text[] = {0, "Треснуло", "Повреждено", "Сломано"};
static variant common_potions[] = {LifePoints, ManaPoints, DrunkenSpell};
static variant uncommon_potions[] = {Strenght, Dexterity, Wisdow, Charisma, LifeRate, ManaRate, Level, Speed, Movement, PoisonSpell};
static variant rare_potions[] = {Attack, Damage, Strenght, Dexterity, Constitution, Intellegence, Wisdow, Charisma, Armor, Protection, Speed, Movement, Level};
static variant weapon_enchanments[] = {{}, Attack, Damage, Speed, Strenght, Constitution};
static variant swords_enchanments[] = {{}, Attack, Strenght, Dexterity, Constitution, Intellegence, Wisdow, Charisma, Protection, LifePoints, ManaPoints, Speed};
static variant common_rings[] = {Strenght, Dexterity, Constitution, Intellegence, Wisdow, Charisma,
Acid, Charm, Cold, Electricity, Fire, Poison, WaterAttack,
Armor, Protection, LifePoints, ManaPoints, Speed};
static variant common_boots[] = {{}, Constitution, Speed};
static variant common_armor[] = {{}, Charisma, Armor, Cold, Fire, Poison, Speed};
static variant common_helm[] = {{}, Intellegence, Charisma};
static variant common_bracers[] = {{}, FocusBows, Lockpicking, PickPockets, Dexterity, Protection};
static variant wand_common_spells[] = {Domination, MagicMissile, ShokingGrasp, HealingSpell, ArmorSpell, CharmPerson, FearSpell, Invisibility, Repair, SickSpell, Sleep, Web};
static variant common_mage_spells[] = {MagicMissile, ShokingGrasp, ArmorSpell, CharmPerson, FearSpell, Invisibility, Repair, Sleep, Web};
static variant common_priest_spells[] = {BlessSpell, HealingSpell, DetectEvil};
static variant common_scroll[] = {BlessItem, DetectEvil, DetectMagic, Identify, KnockDoor, SummonAlly};
static variant common_amulet[] = {Charisma, Strenght, Lockpicking};
static variant common_shield[] = {{}, Protection, Deflect};
static variant common_cloack[] = {Protection, Survival, HideInShadow, MoveSilently, Dancing};
static variant alchemy_receipts[] = {LifePoints, ManaPoints, Strenght, Dexterity, Constitution, Wisdow, Charisma, Armor, Protection, RemoveSickSpell, RemovePoisonSpell};
static variant lute_songs[] = {{}, Attack, Damage, BlessSpell, LifePoints, ManaPoints};
static variant flute_songs[] = {{}, Wisdow, Intellegence, Charisma};
static variant booben_songs[] = {{}, Strenght, Dexterity, Constitution, Armor};
static variant corpse[] = {GoblinWarrior, GoblinRockthrower, OrcWarrior, LargeBat, GiantRat,
HumanMale, HumanGuard, HumanChild, HumanFemale,
Shopkeeper, DwarvenSmith, Bartender,
KobolWarrior, KoboldShaman,
LargeDog, Lynx, GiantFrog,
GnollWarrior,
GoblinRockthrowerWarrior, GoblinRockthrowerWarriorF,
OrcWarrio2r, OrcWarrior2F,
LargeBat2, LargeBat3,
OrgeCommoner, OrgeCommonerF,
Bugbear, BugbearF};
static variant bones[] = {Skeleton};
static variant bugshell[] = {AntWorker, AntWarrior, AntQueen, Bee, Bee2, Bee3};

BSDATA(itemi) = {
	{"Кулак", "item-1", 0, 0, 0, 2, Male, Organic, {Bludgeon, D1d3, 0, 4}, {}, {}, {}, Melee},
	{"Боевой топор", "item5", 3, 850, 5 * GP, -1, Male, Iron, {Slashing, D1d8, 0, -1}, {}, weapon_enchanments, {Versatile}, Melee, FocusAxes},
	{"Дубина", "item1", 1, 1000, 5 * CP, -2, Female, Wood, {Bludgeon, D1d6}, {}, weapon_enchanments, {}, Melee},
	{"Кинжал", "item0", 2, 50, 2 * GP, 0, Male, Iron, {Piercing, D1d4, 0, 1}, {}, weapon_enchanments, {Light}, Melee},
	{"Молот", "item42", 3, 800, 2 * GP, -1, Male, Wood, {Bludgeon, D1d4, 1, -1}, {}, weapon_enchanments, {}, Melee},
	{"Булава", "item7", 3, 700, 8 * GP, -1, Female, Iron, {Bludgeon, D1d6, 1, -2}, {}, weapon_enchanments, {}, Melee},
	{"Копье", "item60", 4, 700, 8 * GP, 0, NoGender, Iron, {Piercing, D1d8, 0, -2}, {}, weapon_enchanments, {}, Melee},
	{"Посох", "item6", 1, 700, 8 * GP, 0, Male, Iron, {Bludgeon, D1d6}, {}, weapon_enchanments, {TwoHanded}, Melee},
	{"Длинный меч", "item4", 6, 700, 8 * GP, -1, Male, Iron, {Slashing, D1d8, 0, -1}, {}, swords_enchanments, {Versatile}, Melee, FocusSwords},
	{"Короткий меч", "item2", 5, 700, 8 * GP, 0, Male, Iron, {Slashing, D1d6}, {}, swords_enchanments, {Light}, Melee, FocusSwords},
	{"Двуручный меч", "item190", 7, 700, 8 * GP, -3, Male, Iron, {Slashing, D2d6, 0, -4}, {}, swords_enchanments, {TwoHanded}, Melee, FocusTwohanded},
	{"Арбалет", "item33", 4, 700, 40 * GP, 0, Male, Wood, {Piercing, D1d8, 0, -2, Bolt}, {}, weapon_enchanments, {}, Ranged},
	{"Тяжелый арбалет", "item67", 8, 1200, 80 * GP, -2, Male, Wood, {Piercing, D1d12, 0, -5, Bolt}, {}, weapon_enchanments, {}, Ranged},
	{"Длинный лук", "item76", 6, 500, 60 * GP, -2, Male, Wood, {Piercing, D1d8, 0, -1, Arrow}, {}, weapon_enchanments, {}, Ranged, FocusBows},
	{"Короткий лук", "item50", 4, 300, 20 * GP, -1, Male, Wood, {Piercing, D1d6, 0, 0, Arrow}, {}, weapon_enchanments, {}, Ranged, FocusBows},
	{"Дарт", "item34", 1, 30, 1 * SP, 1, Male, Wood, {Piercing, D1d3, 0, 4}, {}, weapon_enchanments, {}, Ranged},
	{"Праща", "item52", 2, 50, 1 * SP, 0, Female, Leather, {Bludgeon, D1d4, 0, -2, Rock}, {}, weapon_enchanments, {}, Ranged},
	{"Камень", "items53", 1, 15, 0, -1, Male, Stone, {Bludgeon, D1d3, 0, -2, NoItem, Rock}, {}, {}, {}, Ranged},
	{"Стрела", "item51", 2, 3, 2 * CP, 0, Female, Wood, {Piercing, NoDice, 0, 0, NoItem, Arrow}, {}, {}, {}, Amunitions},
	{"Болт", "item68", 1, 2, 1 * CP, 0, Male, Iron, {Piercing, NoDice, 0, 0, NoItem, Bolt}, {}, {}, {}, Amunitions},
	{"Булыжник", "item580", 5, 3000, 4 * CP, 0, Male, Stone, {}},
	{"Кожанная броня", "item10", 2, 1000, 5 * GP, 2, Female, Leather, {}, {0, 15}, common_armor, {}, Torso},
	{"Клепанная броня", "item43", 3, 1500, 15 * GP, 3, Female, Leather, {}, {0, 15}, common_armor, {}, Torso},
	{"Чешуйчатый доспех", "item11", 4, 2500, 30 * GP, 5, Male, Iron, {}, {1, 25}, common_armor, {}, Torso},
	{"Кольчуга", "item12", 5, 2600, 50 * GP, 6, Female, Iron, {}, {1, 20}, common_armor, {}, Torso},
	{"Бахрец", "item456", 6, 3000, 200 * GP, 8, Male, Iron, {}, {2, 15}, common_armor, {}, Torso},
	{"Латы", "item13", 10, 3500, 400 * GP, 10, Female, Iron, {}, {3, 30}, common_armor, {}, Torso},
	{"Щит", "item17", 3, 800, 0 * GP, 2, Male, Iron, {}, {0, 25}, common_shield, {}, OffHand},
	{"Шлем", "item28", 2, 100, 0 * GP, 1, Male, Iron, {}, {0, 20}, common_helm, {}, Head},
	{"Наручи", "item15", 2, 70, 0 * GP, 1, NoGender, Iron, {}, {0, 15}, common_bracers, {}, Elbows},
	{"Железные наручи", "item14", 6, 110, 0 * GP, 2, NoGender, Iron, {}, {1, 20}, common_bracers, {}, Elbows},
	{"Плащ", "item38", 1, 30, 10 * SP, 0, Male, Leather, {}, {0, 1}, common_cloack, {}, TorsoBack},
	{"Плащ", "item39", 2, 30, 15 * GP, 0, Male, Leather, {}, {0, 1}, common_cloack, {}, TorsoBack},
	{"Плащ", "item75", 3, 35, 20 * GP, 1, Female, Leather, {}, {0, 2}, common_cloack, {}, TorsoBack},
	{"Плащ", "item314", 4, 35, 5 * GP, 1, Female, Leather, {}, {0, 2}, common_cloack, {}, TorsoBack},
	{"Плащ", "item331", 8, 40, 5 * GP, 2, Female, Leather, {}, {1, 3}, common_cloack, {}, TorsoBack},
	{"Туфли", "item30", 1, 100, 5 * GP, 0, NoGender, Leather, {}, {0, 1}, common_boots, {}, Legs},
	{"Сапоги", "item27", 2, 200, 10 * GP, 1, NoGender, Leather, {}, {0, 5}, common_boots, {}, Legs},
	{"Сапоги", "item58", 3, 400, 12 * GP, 1, NoGender, Iron, {}, {0, 10}, common_boots, {}, Legs},
	{"Сапоги", "item128", 4, 500, 15 * GP, 2, NoGender, Iron, {}, {0, 10}, common_boots, {}, Legs},
	{"Сапоги", "item149", 5, 600, 17 * GP, 2, NoGender, Iron, {}, {1, 10}, common_boots, {}, Legs},
	{"Еда", "item21", 3, 100, 5 * SP, 5, Female, Organic, {}, {}, {}, {}, Edible},
	{"Яблоко", "item55", 1, 10, 5 * CP, 0, NoGender, Organic, {}, {}, {}, {}, Edible},
	{"Хлеб хоббитов", "item240", 2, 50, 1 * SP, 1, Male, Organic, {}, {}, {}, {}, Edible},
	{"Хлеб эльфов", "item56", 4, 50, 2 * SP, 2, Male, Organic, {}, {}, {}, {}, Edible},
	{"Хлеб гномов", "item57", 3, 80, 8 * CP, 2, Male, Organic, {}, {}, {}, {}, Edible},
	{"Пирожное", "item239", 3, 20, 1 * GP, -3, NoGender, Organic, {}, {}, {}, {}, Edible},
	{"Колбаса", "item238", 4, 60, 8 * SP, 0, Female, Organic, {}, {}, {}, {}, Edible},
	{"Мясо", "item22", 2, 80, 5 * CP, -4, NoGender, Organic, {}, {}, {}, {}, Edible},
	{"Шиповник", "item177", 7, 10, 2 * CP, -1, Male, Organic, {}, {}, {}, {}, Edible},
	{"Физалис", "item178", 8, 10, CP, -1, Male, Organic, {}, {}, {}, {}, Edible},
	{"Черемуха", "item179", 9, 10, CP, -1, Male, Organic, {}, {}, {}, {}, Edible},
	{"Калина", "item180", 5, 10, CP, -1, Male, Organic, {}, {}, {}, {}, Edible},
	{"Клевер", "item181", 3, 10, CP, -1, Male, Organic, {}, {}, {}, {}, Edible},
	{"Мята", "item182", 4, 10, CP, -1, Male, Organic, {}, {}, {}, {}, Edible},
	{"Ягода", "item183", 2, 10, CP, -1, Male, Organic, {}, {}, {}, {}, Edible},
	{"Ромашка", "item184", 1, 10, CP, -1, Male, Organic, {}, {}, {}, {}, Edible},
	{"Топинамбур", "item191", 12, 10, 2 * CP, -1, Male, Organic, {}, {}, {}, {}, Edible},
	{"Шпинат", "item192", 13, 10, 3 * CP, -1, Male, Organic, {}, {}, {}, {}, Edible},
	{"Петрушка", "item193", 14, 10, 2 * CP, -1, Female, Organic, {}, {}, {}, {}, Edible},
	{"Морковь", "item194", 15, 10, 3 * CP, -1, Female, Organic, {}, {}, {}, {}, Edible},
	{"Свиток", "item188", 1, 1, 10 * GP, -1, Male, Paper, {}, {}, common_scroll, {SingleUse}, Readable},
	{"Свиток", "item185", 3, 1, 12 * GP, 0, Male, Paper, {}, {}, common_scroll, {SingleUse}, Readable},
	{"Свиток", "item186", 5, 1, 15 * GP, 1, Male, Paper, {}, {}, common_scroll, {SingleUse}, Readable},
	{"Рецепт", "item200", 7, 1, 5 * GP, 1, Male, Paper, {}, {}, alchemy_receipts, {SingleUse}, Readable, Alchemy},
	{"Жезл", "item231", 4, 10, 100 * GP, -2, NoGender, Wood, {}, {}, wand_common_spells, {}, Zapable},
	{"Жезл", "item232", 5, 10, 120 * GP, -1, NoGender, Wood, {}, {}, wand_common_spells, {}, Zapable},
	{"Жезл", "item100", 6, 20, 150 * GP, 0, NoGender, Iron, {}, {}, wand_common_spells, {}, Zapable},
	{"Жезл", "item229", 8, 20, 160 * GP, 1, NoGender, Iron, {}, {}, wand_common_spells, {}, Zapable},
	{"Жезл", "item230", 10, 30, 180 * GP, 2, NoGender, Iron, {}, {}, wand_common_spells, {}, Zapable},
	{"Книга", "item88", 2, 500, 100 * GP, -1, Female, Paper, {}, {}, common_mage_spells, {}, Readable},
	{"Книга", "item681", 3, 600, 150 * GP, 0, Female, Paper, {}, {}, common_mage_spells, {}, Readable},
	{"Книга", "item514", 5, 600, 200 * GP, 0, Female, Paper, {}, {}, common_priest_spells, {}, Readable},
	{"Мануал", "item680", 6, 800, 250 * GP, 1, Male, Paper, {}, {}, common_mage_spells, {}, Readable},
	{"Том", "item649", 10, 1000, 300 * GP, 2, Male, Paper, {}, {}, common_mage_spells, {}, Readable},
	{"Зелье", "item259", 1, 20, 10 * GP, 0, NoGender, Glass, {}, {}, alchemy_receipts, {}, Drinkable},
	{"Зелье", "item49", 2, 10, 10 * GP, -1, NoGender, Glass, {}, {}, common_potions, {}, Drinkable},
	{"Зелье", "item63", 3, 20, 35 * GP, 0, NoGender, Glass, {}, {}, uncommon_potions, {}, Drinkable},
	{"Зелье", "item64", 4, 10, 40 * GP, 0, NoGender, Glass, {}, {}, uncommon_potions, {}, Drinkable},
	{"Экстракт", "item244", 5, 10, 45 * GP, 1, Male, Glass, {}, {}, rare_potions, {}, Drinkable},
	{"Элексир", "item246", 6, 5, 80 * GP, 2, Male, Glass, {}, {}, rare_potions, {}, Drinkable},
	{"Кольцо", "item204", 2, 1, 35 * GP, 0, NoGender, Iron, {}, {}, common_rings, {}, RightFinger},
	{"Кольцо", "item44", 3, 1, 40 * GP, 0, NoGender, Iron, {}, {}, common_rings, {}, RightFinger},
	{"Кольцо", "item48", 4, 2, 60 * GP, 1, NoGender, Iron, {}, {}, common_rings, {}, RightFinger},
	{"Амулет", "item166", 2, 20, 250 * GP, -1, Male, Wood, {}, {}, common_amulet, {}, Neck},
	{"Медальон", "item167", 3, 20, 300 * GP, 0, Male, Wood, {}, {}, common_amulet, {}, Neck},
	{"Амулет", "item168", 4, 20, 350 * GP, 0, Male, Iron, {}, {}, common_amulet, {}, Neck},
	{"Амулет", "item170", 5, 30, 400 * GP, 1, Male, Iron, {}, {}, common_amulet, {}, Neck},
	{"Ожерелье", "item174", 6, 40, 450 * GP, 2, NoGender, Iron, {}, {}, common_amulet, {}, Neck},
	{"Набор скалолаза", "item522", 1, 100, 10 * GP, 0, Male, Iron, {}, {}, {}, {}, Tool},
	{"Набор стрелодела", "item468", 1, 100, 8 * GP, 0, Male, Wood, {}, {}, {}, {}, Tool},
	{"Наковальня", "item72", 5, 2000, 9 * GP, 0, Male, Iron, {}, {}, {}, {}, Tool},
	{"Бинты", "item561", 1, 10, 1 * SP, 0, NoGender, Paper, {}, {}, {}, {}, Tool},
	{"Набор писца", "item679", 3, 50, 20 * GP, 0, Male, Paper, {}, {}, {}, {}, Tool},
	{"Кристальный шар", "item612", 6, 300, 25 * GP, 0, Male, Glass, {}, {}, {}, {}, Tool},
	{"Набор алхимика", "item739", 7, 400, 30 * GP, 0, Male, Wood, {}, {}, alchemy_receipts, {}, Tool, Alchemy},
	{"Инструменты вора", "item23", 1, 300, 25 * GP, 0, Male, Iron, {}, {}, {}, {}, Tool},
	{"Котелок", "item463", 3, 1000, 2 * GP, 0, Male, Iron, {}, {}, {}, {}, Tool},
	{"Лютня", "item81", 3, 300, 30 * GP, 0, Female, Wood, {}, {}, lute_songs, {}, Tool, Charisma},
	{"Арфа", "item82", 4, 300, 20 * GP, 0, Female, Wood, {}, {}, lute_songs, {}, Tool, Charisma},
	{"Бубен", "item83", 4, 300, 35 * GP, 0, Male, Leather, {}, {}, booben_songs, {}, Tool, Charisma},
	{"Гитара", "item84", 4, 300, 35 * GP, 0, Female, Wood, {}, {}, lute_songs, {}, Tool, Charisma},
	{"Скрипка", "item85", 3, 300, 35 * GP, 0, Female, Wood, {}, {}, flute_songs, {}, Tool, Charisma},
	{"Флейта", "item86", 10, 300, 35 * GP, 0, Female, Wood, {}, {}, flute_songs, {}, Tool, Charisma},
	{"Тело", "item103", 0, 1500, 0 * GP, 0, NoGender, Organic, {}, {}, corpse, {}, Edible},
	{"Кости", "items383", 0, 500, 0 * GP, 0, NoGender, Organic, {}, {}, bones, {}},
	{"Панцирь", "item258", 0, 400, 0 * GP, 0, NoGender, Organic, {}, {}, bugshell, {}},
	{"Ключ", "item354", 1, 0, 0 * GP, 0, Male, Iron, {}, {}, {}, {}},
	{"Монета", "items37", 1, 0, 1 * CP, -1, Female, Iron, {}, {}, {}, {}, Coinable},
	{"Серебрянная монета", "items37", 1, 0, 1 * SP, 0, Female, Iron, {}, {}, {}, {}, Coinable},
	{"Золотая монета", "items37", 1, 0, 1 * GP, 1, Female, Iron, {}, {}, {}, {}, Coinable},
	{"Когти", 0, 0, 0, 0 * GP, 2, NoGender, Organic, {Slashing, D1d6, 0, 6}, {}, {}, {Natural}, Melee},
	{"Кулаки", 0, 0, 0, 0 * GP, 0, NoGender, Organic, {Bludgeon, D1d6, 0, 4}, {}, {}, {Natural}, Melee},
	{"Укус", 0, 0, 0, 0 * GP, 0, Male, Organic, {Piercing, D1d4, 1, 3}, {}, {}, {Natural}, Melee},
	{"Хитин", 0, 0, 0, 0 * GP, 5, Male, Leather, {}, {1, 4, 40}, {}, {Natural}, Torso},
	{"Мех", 0, 0, 0, 0 * GP, 3, Male, Leather, {}, {0, 3, 20}, {}, {Natural}, Torso},
	{"Много предметов"},
};
assert_enum(itemi, ManyItems)
static_assert(sizeof(item) == sizeof(int), "Struct 'item' must be sizeof(int)");

bool itemi::is(slot_s v) const {
	switch(v) {
	case Amunitions: return weapon.ammunition_compatible != 0;
	case LeftFinger: return slot == RightFinger;
	case OffHand: return slot == OffHand || (slot == Melee && flags.is(Light));
	default: return slot == v;
	}
}

bool itemi::is(slota source) const {
	for(auto v : source) {
		if(is(v))
			return true;
	}
	return false;
}

item::item(item_s type, int level) {
	auto chance_artifact = level / 3;
	auto chance_magic = level * 2;
	auto chance_cursed = 5;
	create(type, chance_artifact, chance_magic, chance_cursed);
}

variant itemi::randeffect() const {
	if(effects.data) {
		if(effects[0])
			return effects[rand() % effects.getcount()];
		else if(effects[1])
			return effects[1 + (rand() % (effects.getcount() - 1))];
	}
	return {};
}

inline bool roll2c(int cv) {
	if(cv <= 0)
		return false;
	else if(cv > 70)
		cv = 70;
	return d100() < cv;
}

void item::create(item_s item_type, int chance_artifact, int chance_magic, int chance_cursed) {
	clear();
	type = item_type;
	auto& ei = geti();
	magic = Mundane;
	if(roll2c(chance_magic)) {
		if(roll2c(chance_cursed))
			magic = Cursed;
		else if(roll2c(chance_artifact))
			magic = Artifact;
		else
			magic = Blessed;
	}
	if(ei.effects.data) {
		if(ei.effects[0])
			effect = rand() % ei.effects.getcount();
		else if(ei.effects[1]) {
			if(roll2c(chance_magic))
				effect = 1 + (rand() % (ei.effects.getcount() - 1));
			else
				effect = 0;
		}
	}
	if(ischargeable())
		charge = xrand(2, 4) + ei.quality * 2;
	if(iscountable()) {
		if(is(Edible))
			setcount(xrand(1, 3));
		else
			setcount(xrand(10, 20));
	}
}

creature* item::getwearer() const {
	auto i = bsdata<creature>::source.indexof((creature*)(this));
	if(i == -1)
		return 0;
	return (creature*)bsdata<creature>::source.ptr(i);
}

slot_s item::getwearerslot() const {
	return getwearer()->getwearerslot(this);
}

const char* item::getdamagetext() const {
	if(iscountable())
		return 0;
	return damage_text[damaged];
}

void item::getstatistic(stringbuilder& sb) const {
	auto& ei = geti();
	if(ei.slot >= Head && ei.slot <= Ranged) {
		auto ai = getattack();
		if(ai.dice.max) {
			if(ai.attack)
				sb.adds("%1:%2i%%", bsdata<abilityi>::elements[Attack].name_short, ai.attack);
			if(ai.dice.max)
				sb.adds("%1:%2i-%3i", bsdata<abilityi>::elements[Damage].name_short, ai.dice.min, ai.dice.max);
		}
		auto am = getarmor();
		if(am.armor || am.deflect || am.protection) {
			if(am.attack)
				sb.adds("%1:%2i%%", bsdata<abilityi>::elements[Attack].name_short, am.attack);
			sb.adds("%1:%2i%%", bsdata<abilityi>::elements[Armor].name_short, am.protection);
			if(am.armor)
				sb.adds("и %1i", am.armor);
			if(am.deflect)
				sb.adds("%1:%2i%%", bsdata<abilityi>::elements[Deflect].name_short, am.deflect);
		}
	}
}

void item::getname(stringbuilder& sb, bool show_cab) const {
	auto& ei = geti();
	auto q = getbonus();
	if(show_cab) {
		if(is(KnownMagic)) {
			if(getmagic())
				sb.adds(bsdata<item_typei>::elements[getmagic()].name[ei.gender]);
		}
	}
	sb.adds("%-1", getname());
	const char* format = "%+1i";
	if(is(KnownPower)) {
		auto effect = geteffect();
		if(effect) {
			if(q < 0)
				sb.adds(effect.getnameofc());
			else
				sb.adds(effect.getnameof());
			switch(effect.type) {
			case Ability:
				format = bsdata<abilityi>::elements[effect.value].format;
				q = bsdata<abilityi>::elements[effect.value].getbonus(q);
				break;
			case Skill:
				if(!is(Readable)) {
					q = q * 10;
					format = "%+1i%%";
				}
				break;
			}
		}
		if(is(KnownStats)) {
			if(ei.slot >= Head && ei.slot <= LastWear && q != 0)
				sb.adds(format, q);
		}
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
	if(iscountable())
		return;
	auto source = geti().effects;
	if(source) {
		auto n = source.indexof(v);
		if(n != -1)
			effect = n;
	}
	getwearer()->prepare();
}

int	item::getcount() const {
	if(!iscountable())
		return 1;
	return count + 1;
}

void item::setcount(int count) {
	if(!iscountable() && count)
		return;
	if(!count)
		clear();
	else {
		if(count > 256 * 256)
			count = 256 * 256;
		this->count = count - 1;
	}
	getwearer()->prepare();
}

void item::use() {
	if(iscountable()) {
		if(count)
			count--;
		else
			destroy(Magic, false);
	} else if(ischargeable()) {
		if(charge)
			charge--;
		else
			destroy(Magic, true);
	} else
		destroy(Magic, false);
	getwearer()->prepare();
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

int item::getquality() const {
	switch(magic) {
	case Artifact: return 3;
	case Blessed: return 1;
	case Cursed: return -2;
	default: return 0;
	}
}

int	item::getbonus() const {
	int m = geti().quality;
	if(identifyc) {
		switch(magic) {
		case Artifact: m += 2; break;
		case Blessed: m += 1; break;
		case Cursed: return -1 - m;
		}
	}
	return m;
}

variant item::geteffect() const {
	auto& ei = geti();
	if(ei.effects.count)
		return ei.effects[effect];
	else
		return variant();
}

itemi::armori item::getarmor() const {
	auto& ei = geti();
	auto result = ei.armor;
	if(!result.armor && !result.deflect)
		return result;
	auto d = getdamage();
	auto b = getbonus();
	result.protection += b * 4 - d * 2;
	result.armor -= (d / 2);
	if(result.armor < 0)
		result.armor = 0;
	result.attack = -(ei.quality * 2 - b);
	if(result.attack > 0)
		result.attack = 0;
	return result;
}

attacki item::getattack() const {
	auto& ei = geti().weapon;
	auto d = getdamage();
	auto b = getbonus();
	attacki result;
	result.type = ei.type;
	result.attack = b * 4 - d * 2;
	result.dice = bsdata<dicei>::elements[ei.dice];
	result.dice.min += ei.damage;
	result.dice.max += ei.damage;
	result.speed = ei.speed;
	result.ammunition = ei.amunition;
	return result;
}

void item::setpersonal(int v) {
	personal = v;
}

void item::set(item_type_s v) {
	magic = v;
	getwearer()->prepare();
}

void item::set(identify_s v) {
	if(is(v))
		return;
	if(v == KnownPower && iscountable())
		return;
	switch(v) {
	case KnownMagic: identifyc = 1; break;
	case KnownPower: identifye = 1; break;
	case KnownStats: identifys = 1; break;
	default: identifyc = identifys = identifye = 0; break;
	}
}

void item::loot() {
	set(Unknown);
}

unsigned item::getcost() const {
	auto& ei = bsdata<itemi>::elements[type];
	auto w = ei.cost;
	auto m = sale_percent[sale];
	if(is(KnownMagic)) {
		switch(magic) {
		case Blessed: m += 100; break;
		case Artifact: m += 1000; break;
		case Cursed: return 0; break;
		default: break;
		}
	}
	if(is(KnownPower)) {
		if(ei.slot >= Head && ei.slot <= Ranged) {
			if(geteffect())
				w += 10 * GP;
		}
	}
	if(w < 0)
		w = 0;
	return w * m / 100;
}

int	item::getdamage() const {
	if(iscountable())
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

bool item::islike(const item& v) const {
	return iscountable()
		&& type == v.type
		&& identifyc == v.identifyc
		&& identifys == v.identifys
		&& magic == v.magic
		&& sale == v.sale;
}

bool item::stack(item& v) {
	if(!islike(v))
		return false;
	int c1 = count;
	int c2 = v.count;
	auto result = false;
	c1 += c2 + 1;
	if(c1 <= 0xFFFF) {
		count = c1;
		v.clear();
		result = true;
	} else {
		count = 0xFFFF;
		v.count = c1 - 0xFFFF - 1;
	}
	auto p1 = getwearer();
	auto p2 = v.getwearer();
	p2->prepare();
	if(p2 != p1)
		p1->prepare();
	return result;
}

void item::destroy(damage_s type, bool interactive) {
	if(interactive) {
		auto& ei = bsdata<itemi>::elements[getkind()];
		static descriptioni text[] = {
			{Glass, Fire, "%герой расплавил%ась и взорвалась."},
			{Glass, {}, "%герой разбил%ась вдребезги."},
			{Wood, Fire, "%герой сгорел%а до тла."},
			{Paper, Fire, "%герой моментально превратил%ась в пепел."},
			{Paper, Magic, "%герой превратил%ась в пыль и рассыпал%ась."},
			{Organic, Magic, "%герой полностью сгнил%а."},
			{{}, Fire, "%герой расплавил%ась."},
			{{}, Cold, "%герой замерз%ла и разлетел%ась на куски."},
			{{}, {}, "%герой уничтожен%а."}
		};
		act(text->get(ei.material, type));
	}
	setcount(getcount() - 1);
}

void item::decoy() {
	auto& ei = geti();
	if(ei.slot != Edible)
		return;
	auto chance = 20 + ei.quality * 20;
	auto owner = getwearer();
	if(owner)
		chance += owner->get(Cooking) / 3;
	if(creature::rollv(40 + ei.quality * 20))
		return;
	destroy(Magic, true);
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
	if(!(*this))
		return;
	auto& ei = bsdata<itemi>::elements[getkind()];
	auto chance_resist = 70;
	chance_resist -= count;
	chance_resist += bsdata<materiali>::elements[ei.material].resist.data[type];
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

bool item::iscountable() const {
	return bsdata<itemi>::elements[type].effects.data == 0;
}

bool item::ischargeable() const {
	if(is(Drinkable))
		return false;
	auto v = geteffect();
	if(v.type == Spell)
		return true;
	return false;
}

bool item::is(identify_s v) const {
	switch(v) {
	case KnownMagic: return identifyc != 0;
	case KnownStats: return identifys != 0;
	case KnownPower: return !iscountable() && identifye != 0;
	default: return !identifys && !identifyc && !identifye;
	}
}

const aref<variant> item::getreceipts() {
	return alchemy_receipts;
}

item_s item::findcorpse(role_s v) {
	for(auto& e : bsdata<itemi>()) {
		if(!e.effects)
			continue;
		if(e.effects[0].type != Role)
			continue;
		if(e.effects.indexof(v) != -1)
			return e.getid();
	}
	return NoItem;
}

item_s itemi::getid() const {
	return item_s(this - bsdata<itemi>::elements);
}

void item::repair() {
	if(iscountable())
		return;
	if(!damaged)
		return;
	damaged--;
}
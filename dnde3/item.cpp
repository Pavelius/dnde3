#include "main.h"

const int basic_enchantment_cost = 100;

static const char* power_text[][3] = {{"�������", "�������", "�������"},
{"����������", "����������", "����������"},
{"�������", "�������", "�������"},
{"��������������", "��������������", "��������������"},
};
static const char* damage_text[] = {0, "��������", "����������", "�������"};

static variant common_potions[] = {LifePoints, ManaPoints, DrunkenSpell};
static variant uncommon_potions[] = {Strenght, Dexterity, Wisdow, Charisma,
LifeRate, ManaRate, Level, Speed, PoisonSpell};
static variant rare_potions[] = {Attack, Damage,
Strenght, Dexterity, Constitution, Intellegence, Wisdow, Charisma,
Armor, Protection, Level};
static variant weapon_enchanments[] = {{}, Attack, Damage, Speed, Strenght, Constitution};
static variant swords_enchanments[] = {{}, Attack, Damage,
Strenght, Dexterity, Constitution, Intellegence, Wisdow, Charisma,
Protection, LifePoints, ManaPoints, Speed};
static variant common_rings[] = {ResistAcid, ResistCharm, ResistCold, ResistElectricity,
ResistFire, ResistParalize, ResistPoison, ResistWater,
Strenght, Dexterity, Constitution, Intellegence, Wisdow, Charisma,
Armor, Protection,
LifePoints, ManaPoints, Speed};
static variant common_boots[] = {{},
ResistElectricity, ResistParalize, ResistWater,
Constitution, Speed};
static variant common_armor[] = {{},
ResistElectricity, ResistParalize, ResistWater,
Charisma, Wisdow,
Armor, Speed};
static variant common_helm[] = {{}, Intellegence, Charisma};
static variant common_bracers[] = {{}, FocusBows, Lockpicking, PickPockets, Dexterity, Protection};
static variant wand_common_spells[] = {Domination, MagicMissile, ShokingGrasp, HealingSpell, ArmorSpell,
CharmPerson, FearSpell, Invisibility, Repair, SickSpell, Sleep, Web};
static variant common_mage_spells[] = {MagicMissile, ShokingGrasp,
ArmorSpell, CharmPerson, FearSpell, Invisibility, Repair, Sleep, Web};
static variant common_priest_spells[] = {BlessSpell, HealingSpell, DetectEvil};
static variant common_scroll[] = {BlessItem, DetectEvil, DetectMagic, Identify, KnockDoor, SummonAlly};
static variant common_amulet[] = {Charisma, Strenght, Lockpicking};
static variant common_shield[] = {{}, Protection, Deflect,
ResistFire, ResistAcid, ResistElectricity, ResistCharm};
static variant common_cloack[] = {Protection, Survival, HideInShadow, MoveSilently, Dancing};
static variant alchemy_receipts[] = {LifePoints, ManaPoints,
Strenght, Dexterity, Constitution, Wisdow, Charisma,
Armor, Protection,
ResistAcid, ResistCharm, ResistCold, ResistElectricity, ResistFire, ResistParalize, ResistPoison, ResistWater,
RemoveSickSpell, RemovePoisonSpell};
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

BSDATA(itemi) = {{"����", "item-1", 0, 0, 0, 0, NoGender, Organic, {0, 3, {1, 3}, Bludgeon, 4, 2}, {}, {}, {}, Melee},
{"������ �����", "item5", 3, 850, 5 * GP, 0, Male, Iron, {-3, 3, {1, 8}, Slashing, 0, 2}, {}, weapon_enchanments, {Versatile}, Melee, FocusAxes},
{"������", "item1", 1, 1000, 5 * CP, 0, Female, Wood, {-4, 3, {1, 6}, Bludgeon, 0, 2}, {}, weapon_enchanments, {}, Melee},
{"������", "item0", 2, 50, 2 * GP, 0, Male, Iron, {0, 3, {1, 4}, Piercing, 2, 3}, {}, weapon_enchanments, {Light}, Melee},
{"�����", "item42", 3, 800, 2 * GP, 0, Male, Wood, {-1, 3, {2, 5}, Bludgeon, 0, 1}, {}, weapon_enchanments, {}, Melee},
{"������", "item7", 3, 700, 8 * GP, 0, Female, Iron, {-2, 3, {2, 7}, Bludgeon, 0, 1}, {}, weapon_enchanments, {}, Melee},
{"�����", "item60", 4, 700, 8 * GP, 0, NoGender, Iron, {-1, 3, {1, 8}, Piercing, 0, 2}, {}, weapon_enchanments, {}, Melee},
{"�����", "item6", 1, 700, 8 * GP, 0, Male, Iron, {0, 3, {1, 6}, Bludgeon, 1, 2}, {}, weapon_enchanments, {TwoHanded}, Melee},
{"������� ���", "item4", 6, 700, 8 * GP, 0, Male, Iron, {0, 3, {1, 8}, Slashing, 0, 2}, {}, swords_enchanments, {Versatile}, Melee, FocusSwords},
{"�������� ���", "item2", 5, 700, 8 * GP, 0, Male, Iron, {0, 3, {1, 6}, Slashing, 0, 2}, {}, swords_enchanments, {Light}, Melee, FocusSwords},
{"��������� ���", "item190", 7, 700, 8 * GP, 0, Male, Iron, {-6, 3, {2, 12}, Slashing, -2, 2}, {}, swords_enchanments, {TwoHanded}, Melee, FocusTwohanded},
{"�������", "item33", 4, 700, 40 * GP, 0, Male, Wood, {0, 3, {1, 8}, Piercing, -2, 0, Bolt}, {}, weapon_enchanments, {}, Ranged},
{"������� �������", "item67", 8, 1200, 80 * GP, 0, Male, Wood, {-4, 3, {1, 12}, Piercing, -6, 0, Bolt}, {}, weapon_enchanments, {}, Ranged},
{"������� ���", "item76", 6, 500, 60 * GP, 0, Male, Wood, {-2, 3, {1, 8}, Piercing, 1, 0, Arrow}, {}, weapon_enchanments, {}, Ranged, FocusBows},
{"�������� ���", "item50", 4, 300, 20 * GP, 0, Male, Wood, {-1, 3, {1, 6}, Piercing, 1, 0, Arrow}, {}, weapon_enchanments, {}, Ranged, FocusBows},
{"����", "item34", 1, 30, 1 * SP, 0, Male, Wood, {-4, 3, {1, 3}, Piercing, 3}, {}, weapon_enchanments, {}, Ranged},
{"�����", "item52", 2, 50, 1 * SP, 0, Female, Leather, {-5, 3, {2, 4}, Bludgeon, 0, 0, Rock}, {}, weapon_enchanments, {}, Ranged},
{"������", "items53", 1, 15, 0, 0, Male, Stone, {0, 0, {1, 3}, Bludgeon, 0, 0, NoItem, Rock}, {}, {}, {}, Ranged},
{"������", "item51", 2, 3, 2 * CP, 0, Female, Wood, {0, 0, {}, Piercing, 0, 0, NoItem, Arrow}, {}, {}, {}, Amunitions},
{"����", "item68", 1, 2, 1 * CP, 0, Male, Iron, {0, 0, {}, Piercing, 0, 0, NoItem, Bolt}, {}, {}, {}, Amunitions},
{"��������", "item580", 5, 3000, 4 * CP, 0, Male, Stone, {}},
{"�������� �����", "item10", 2, 1000, 5 * GP, 0, Female, Leather, {-5}, {10, 0, 15}, common_armor, {}, Torso},
{"��������� �����", "item43", 3, 1500, 15 * GP, 0, Female, Leather, {-7}, {15, 0, 15}, common_armor, {}, Torso},
{"���������� ������", "item11", 4, 2500, 30 * GP, 0, Male, Iron, {-15}, {25, 1, 30}, common_armor, {}, Torso},
{"��������", "item12", 5, 2600, 50 * GP, 0, Female, Iron, {-10}, {25, 1, 25}, common_armor, {}, Torso},
{"������", "item456", 6, 3000, 200 * GP, 0, Male, Iron, {-15}, {30, 2, 35}, common_armor, {}, Torso},
{"����", "item13", 10, 3500, 400 * GP, 0, Female, Iron, {-20}, {40, 3, 35}, common_armor, {}, Torso},
{"���", "item17", 3, 800, 0 * GP, 0, Male, Iron, {-5, 1}, {10, 0, 25, 5}, common_shield, {}, OffHand},
{"����", "item28", 2, 100, 0 * GP, 0, Male, Iron, {-1}, {5, 0, 20, 3}, common_helm, {}, Head},
{"������", "item15", 2, 70, 0 * GP, 0, NoGender, Iron, {-1}, {5, 0, 15, 2}, common_bracers, {}, Elbows},
{"�������� ������", "item14", 6, 110, 0 * GP, 0, NoGender, Iron, {-3}, {5, 1, 20, 2}, common_bracers, {}, Elbows},
{"����", "item38", 1, 30, 10 * SP, -2, Male, Leather, {}, {3, 0, 2}, common_cloack, {}, TorsoBack},
{"����", "item39", 2, 30, 15 * GP, -1, Male, Leather, {}, {3, 0, 2}, common_cloack, {}, TorsoBack},
{"����", "item75", 3, 35, 20 * GP, 0, Female, Leather, {}, {3, 0, 2}, common_cloack, {}, TorsoBack},
{"����", "item314", 4, 35, 5 * GP, 1, Female, Leather, {}, {5, 0, 2}, common_cloack, {}, TorsoBack},
{"����", "item331", 8, 40, 5 * GP, 2, Female, Leather, {}, {5, 0, 2}, common_cloack, {}, TorsoBack},
{"�����", "item30", 1, 100, 5 * GP, -1, NoGender, Leather, {}, {3, 0, 10}, common_boots, {}, Legs},
{"������", "item27", 2, 200, 10 * GP, 0, NoGender, Leather, {}, {3, 0, 10}, common_boots, {}, Legs},
{"������", "item58", 3, 400, 12 * GP, -1, NoGender, Iron, {}, {5, 0, 15}, common_boots, {}, Legs},
{"������", "item128", 4, 500, 15 * GP, 0, NoGender, Iron, {}, {3, 1, 10}, common_boots, {}, Legs},
{"������", "item149", 5, 600, 17 * GP, 1, NoGender, Iron, {}, {3, 1, 10}, common_boots, {}, Legs},
{"���", "item21", 3, 100, 5 * SP, 5, Female, Organic, {}, {}, {}, {}, Edible},
{"������", "item55", 1, 10, 5 * CP, 0, NoGender, Organic, {}, {}, {}, {}, Edible},
{"���� ��������", "item240", 2, 50, 1 * SP, 1, Male, Organic, {}, {}, {}, {}, Edible},
{"���� ������", "item56", 4, 50, 2 * SP, 2, Male, Organic, {}, {}, {}, {}, Edible},
{"���� ������", "item57", 3, 80, 8 * CP, 2, Male, Organic, {}, {}, {}, {}, Edible},
{"��������", "item239", 3, 20, 1 * GP, -3, NoGender, Organic, {}, {}, {}, {}, Edible},
{"�������", "item238", 4, 60, 8 * SP, 0, Female, Organic, {}, {}, {}, {}, Edible},
{"����", "item22", 2, 80, 5 * CP, -4, NoGender, Organic, {}, {}, {}, {}, Edible},
{"��������", "item177", 7, 10, 2 * CP, -1, Male, Organic, {}, {}, {}, {}, Edible},
{"�������", "item178", 8, 10, CP, -1, Male, Organic, {}, {}, {}, {}, Edible},
{"��������", "item179", 9, 10, CP, -1, Male, Organic, {}, {}, {}, {}, Edible},
{"������", "item180", 5, 10, CP, -1, Male, Organic, {}, {}, {}, {}, Edible},
{"������", "item181", 3, 10, CP, -1, Male, Organic, {}, {}, {}, {}, Edible},
{"����", "item182", 4, 10, CP, -1, Male, Organic, {}, {}, {}, {}, Edible},
{"�����", "item183", 2, 10, CP, -1, Male, Organic, {}, {}, {}, {}, Edible},
{"�������", "item184", 1, 10, CP, -1, Male, Organic, {}, {}, {}, {}, Edible},
{"����������", "item191", 12, 10, 2 * CP, -1, Male, Organic, {}, {}, {}, {}, Edible},
{"������", "item192", 13, 10, 3 * CP, -1, Male, Organic, {}, {}, {}, {}, Edible},
{"��������", "item193", 14, 10, 2 * CP, -1, Female, Organic, {}, {}, {}, {}, Edible},
{"�������", "item194", 15, 10, 3 * CP, -1, Female, Organic, {}, {}, {}, {}, Edible},
{"������", "item188", 1, 1, 10 * GP, -1, Male, Paper, {}, {}, common_scroll, {SingleUse}, Readable},
{"������", "item185", 3, 1, 12 * GP, 0, Male, Paper, {}, {}, common_scroll, {SingleUse}, Readable},
{"������", "item186", 5, 1, 15 * GP, 1, Male, Paper, {}, {}, common_scroll, {SingleUse}, Readable},
{"������", "item200", 7, 1, 5 * GP, 1, Male, Paper, {}, {}, alchemy_receipts, {SingleUse}, Readable, Alchemy},
{"����", "item231", 4, 10, 100 * GP, -2, NoGender, Wood, {}, {}, wand_common_spells, {}, Zapable},
{"����", "item232", 5, 10, 120 * GP, -1, NoGender, Wood, {}, {}, wand_common_spells, {}, Zapable},
{"����", "item100", 6, 20, 150 * GP, 0, NoGender, Iron, {}, {}, wand_common_spells, {}, Zapable},
{"����", "item229", 8, 20, 160 * GP, 1, NoGender, Iron, {}, {}, wand_common_spells, {}, Zapable},
{"����", "item230", 10, 30, 180 * GP, 2, NoGender, Iron, {}, {}, wand_common_spells, {}, Zapable},
{"�����", "item88", 2, 500, 100 * GP, -1, Female, Paper, {}, {}, common_mage_spells, {}, Readable},
{"�����", "item681", 3, 600, 150 * GP, 0, Female, Paper, {}, {}, common_mage_spells, {}, Readable},
{"�����", "item514", 5, 600, 200 * GP, 0, Female, Paper, {}, {}, common_priest_spells, {}, Readable},
{"������", "item680", 6, 800, 250 * GP, 1, Male, Paper, {}, {}, common_mage_spells, {}, Readable},
{"���", "item649", 10, 1000, 300 * GP, 2, Male, Paper, {}, {}, common_mage_spells, {}, Readable},
{"�����", "item259", 1, 20, 10 * GP, 0, NoGender, Glass, {}, {}, alchemy_receipts, {}, Drinkable},
{"�����", "item49", 2, 10, 10 * GP, -1, NoGender, Glass, {}, {}, common_potions, {}, Drinkable},
{"�����", "item63", 3, 20, 35 * GP, 0, NoGender, Glass, {}, {}, uncommon_potions, {}, Drinkable},
{"�����", "item64", 4, 10, 40 * GP, 0, NoGender, Glass, {}, {}, uncommon_potions, {}, Drinkable},
{"��������", "item244", 5, 10, 45 * GP, 1, Male, Glass, {}, {}, rare_potions, {}, Drinkable},
{"�������", "item246", 6, 5, 80 * GP, 2, Male, Glass, {}, {}, rare_potions, {}, Drinkable},
{"������", "item204", 2, 1, 35 * GP, 0, NoGender, Iron, {}, {}, common_rings, {}, RightFinger},
{"������", "item44", 3, 1, 40 * GP, 0, NoGender, Iron, {}, {}, common_rings, {}, RightFinger},
{"������", "item48", 4, 2, 60 * GP, 1, NoGender, Iron, {}, {}, common_rings, {}, RightFinger},
{"������", "item166", 2, 20, 250 * GP, -1, Male, Wood, {}, {}, common_amulet, {}, Neck},
{"��������", "item167", 3, 20, 300 * GP, 0, Male, Wood, {}, {}, common_amulet, {}, Neck},
{"������", "item168", 4, 20, 350 * GP, 0, Male, Iron, {}, {}, common_amulet, {}, Neck},
{"������", "item170", 5, 30, 400 * GP, 1, Male, Iron, {}, {}, common_amulet, {}, Neck},
{"��������", "item174", 6, 40, 450 * GP, 2, NoGender, Iron, {}, {}, common_amulet, {}, Neck},
{"����� ���������", "item522", 1, 100, 10 * GP, 0, Male, Iron, {}, {}, {}, {}, Tool},
{"����� ����������", "item468", 1, 100, 8 * GP, 0, Male, Wood, {}, {}, {}, {}, Tool},
{"����������", "item72", 5, 2000, 9 * GP, 0, Male, Iron, {}, {}, {}, {}, Tool},
{"�����", "item561", 1, 10, 1 * SP, 0, NoGender, Paper, {}, {}, {}, {}, Tool},
{"����� �����", "item679", 3, 50, 20 * GP, 0, Male, Paper, {}, {}, {}, {}, Tool},
{"����������� ���", "item612", 6, 300, 25 * GP, 0, Male, Glass, {}, {}, {}, {}, Tool},
{"����� ��������", "item739", 7, 400, 30 * GP, 0, Male, Wood, {}, {}, alchemy_receipts, {}, Tool, Alchemy},
{"����������� ����", "item23", 1, 300, 25 * GP, 0, Male, Iron, {}, {}, {}, {}, Tool},
{"�������", "item463", 3, 1000, 2 * GP, 0, Male, Iron, {}, {}, {}, {}, Tool},
{"�����", "item81", 3, 300, 30 * GP, 0, Female, Wood, {}, {}, lute_songs, {}, Tool, Charisma},
{"����", "item82", 4, 300, 20 * GP, 0, Female, Wood, {}, {}, lute_songs, {}, Tool, Charisma},
{"�����", "item83", 4, 300, 35 * GP, 0, Male, Leather, {}, {}, booben_songs, {}, Tool, Charisma},
{"������", "item84", 4, 300, 35 * GP, 0, Female, Wood, {}, {}, lute_songs, {}, Tool, Charisma},
{"�������", "item85", 3, 300, 35 * GP, 0, Female, Wood, {}, {}, flute_songs, {}, Tool, Charisma},
{"������", "item86", 10, 300, 35 * GP, 0, Female, Wood, {}, {}, flute_songs, {}, Tool, Charisma},
{"����", "item103", 0, 1500, 0 * GP, 0, NoGender, Organic, {}, {}, corpse, {}, Edible},
{"�����", "items383", 0, 500, 0 * GP, 0, NoGender, Organic, {}, {}, bones, {}},
{"�������", "item258", 0, 400, 0 * GP, 0, NoGender, Organic, {}, {}, bugshell, {}},
{"����", "item354", 1, 0, 0 * GP, 0, Male, Iron, {}, {}, {}, {}},
{"������", "items37", 1, 0, 1 * CP, -1, Female, Iron, {}, {}, {}, {}, Coinable},
{"����������� ������", "items37", 1, 0, 1 * SP, 0, Female, Iron, {}, {}, {}, {}, Coinable},
{"������� ������", "items37", 1, 0, 1 * GP, 1, Female, Iron, {}, {}, {}, {}, Coinable},
{"�����", 0, 0, 0, 0 * GP, 0, NoGender, Organic, {-6, 3, {1, 6}, Slashing, 3, 1}, {}, {}, {Natural}, Melee},
{"������", 0, 0, 0, 0 * GP, 0, NoGender, Organic, {0, 4, {1, 6}, Bludgeon, 0, 2}, {}, {}, {Natural}, Melee},
{"����", 0, 0, 0, 0 * GP, 0, NoGender, Organic, {0, 2, {2, 5}, Slashing, 0, 1}, {}, {}, {Natural}, Melee},
{"�����", 0, 0, 0, 0 * GP, 0, NoGender, Leather, {-10}, {30, 2, 40}, {}, {Natural}, Torso},
{"���", 0, 0, 0, 0 * GP, 0, NoGender, Leather, {-3}, {20, 0, 20}, {}, {Natural}, Torso},
{"����� ���������"},
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
	auto chance_magic = level;
	auto chance_cursed = 5;
	auto chance_quality = 15 + level;
	if(chance_quality > 70)
		chance_quality = 70;
	create(type, chance_artifact, chance_magic, chance_cursed, chance_quality);
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

void item::create(item_s item_type, int chance_artifact, int chance_magic, int chance_cursed, int chance_quality) {
	clear();
	type = item_type;
	auto& ei = geti();
	magic = Mundane;
	if(roll2c(chance_magic)) {
		if(d100() < chance_cursed)
			magic = Cursed;
		else if(roll2c(chance_artifact))
			magic = Artifact;
		else
			magic = Blessed;
	}
	quality = 0;
	if(roll2c(chance_quality)) {
		quality++;
		if(roll2c(chance_quality / 2)) {
			quality++;
			if(roll2c(chance_quality / 4))
				quality++;
		}
	}
	if(!iscountable() && ei.effects.data) {
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
		charge = xrand(2, 4) + quality * 2;
	if(iscountable()) {
		if(is(Edible))
			setcount(xrand(1, 3));
		else
			setcount(xrand(10, 20));
	}
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
	if(iscountable())
		return 0;
	return damage_text[damaged];
}

void item::getstatistic(stringbuilder& sb) const {
	auto& ei = geti();
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
				sb.adds("� %1i", am.armor);
			if(am.deflect)
				sb.adds("%1:%2i%%", bsmeta<abilityi>::elements[Deflect].name_short, am.deflect);
		}
	}
}

void item::getname(stringbuilder& sb, bool show_cab) const {
	auto& ei = geti();
	auto q = getbonus();
	if(show_cab) {
		if(is(KnownMagic)) {
			if(getmagic())
				sb.adds(bsmeta<item_typei>::elements[getmagic()].name[ei.gender]);
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
		if(is(KnownStats)) {
			if(ei.slot >= Head && ei.slot <= LastWear && q != 0)
				sb.adds(format, q);
		}
	}
	if(iscountable()) {
		auto n = getcount();
		if(n > 1)
			sb.adds("%1i ��", getcount());
	}
	if(sale) {
		auto n = getcost();
		if(n > 0)
			sb.adds("(���� %1i)", n);
	}
}

void item::seteffect(variant v) {
	if(iscountable())
		return;
	auto p = getwearer();
	if(p)
		p->dressoff();
	auto source = geti().effects;
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
	auto m = quality;
	if(identifyc) {
		switch(magic) {
		case Artifact: m += 2; break;
		case Blessed: m += 1; break;
		case Cursed: return -1 - quality;
		}
	}
	if(identifye) {
		auto& ei = geti();
		if(ei.effects.count && (effect || ei.effects.data[0]))
			m++;
	}
	return m;
}

variant item::geteffect() const {
	if(iscountable())
		return variant();
	auto& ei = geti();
	if(ei.effects.count > 0)
		return ei.effects[effect];
	return variant();
}

armori item::getarmor() const {
	auto result = geti().armor;
	auto d = getdamage();
	auto b = getbonus();
	result.protection += b * result.protection_bonus - d;
	result.armor += b * result.armor_bonus - (d / 2);
	if(result.armor < 0)
		result.armor = 0;
	return result;
}

attacki item::getattack() const {
	auto& ei = geti();
	auto result = ei.weapon;
	auto d = getdamage();
	auto b = getbonus();
	result.attack += b * ei.weapon.attack_bonus - d;
	if(ei.weapon.damage_bonus) {
		result.dice.min += b / ei.weapon.damage_bonus;
		result.dice.max += b / ei.weapon.damage_bonus - (d / 2);
		if(result.dice.min < 0)
			result.dice.min = 0;
		if(result.dice.max < 0)
			result.dice.max = 0;
	}
	return result;
}

void item::setpersonal(int v) {
	personal = v;
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
	if(is(v))
		return;
	if(v == KnownPower && iscountable())
		return;
	auto p = getwearer();
	if(p)
		p->dressoff();
	switch(v) {
	case KnownMagic: identifyc = 1; break;
	case KnownPower: identifye = 1; break;
	case KnownStats: identifys = 1; break;
	default: identifyc = identifys = identifye = 0; break;
	}
	if(p)
		p->dresson();
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

unsigned item::getcost() const {
	auto& ei = bsmeta<itemi>::elements[type];
	auto w = ei.cost;
	auto m = 100;
	if(sale == Sale75)
		m = 75;
	else if(sale == Sale150)
		m = 150;
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
			w += 5 * GP * quality;
			if(ei.effects.count && !ei.effects[0] && geteffect())
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
	if(interactive) {
		auto& ei = bsmeta<itemi>::elements[getkind()];
		static descriptioni text[] = {
			{Glass, Fire, "%����� ���������%��� � ����������."},
			{Glass, {}, "%����� ������%��� ���������."},
			{Wood, Fire, "%����� ������%� �� ���."},
			{Paper, Fire, "%����� ����������� ���������%��� � �����."},
			{Paper, Magic, "%����� ���������%��� � ���� � ��������%���."},
			{Organic, Magic, "%����� ��������� �����%�."},
			{{}, Fire, "%����� ���������%���."},
			{{}, Cold, "%����� ������%�� � ��������%��� �� �����."},
			{{}, {}, "%����� ���������%�."}
		};
		act(text->get(ei.material, type));
	}
	setcount(getcount() - 1);
}

void item::decoy() {
	auto& ei = geti();
	if(ei.slot != Edible)
		return;
	if(creature::rollv(40 + ei.quality * 10))
		return;
	if(quality)
		quality--;
	else
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

bool item::iscountable() const {
	return bsmeta<itemi>::elements[type].effects.data == 0;
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
	for(auto& e : bsmeta<itemi>()) {
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
	return item_s(this - bsmeta<itemi>::elements);
}

void item::repair() {
	if(iscountable())
		return;
	if(!damaged)
		return;
	damaged--;
}
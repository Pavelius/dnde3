#include "main.h"

const int basic_enchantment_cost = 100;

static const char* power_text[][3] = {{"�������", "�������", "�������"},
{"����������", "����������", "����������"},
{"�������", "�������", "�������"},
{"��������������", "��������������", "��������������"},
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
static variant wand_enchanments[] = {MagicMissile, ShokingGrasp, HealingSpell, ArmorSpell};
static variant common_scroll[] = {DetectEvil, DetectMagic, Identify};

itemi bsmeta<itemi>::elements[] = {{"����", 0, 0, NoGender, Organic, {0, 3, {1, 3}, Bludgeon, 4, 2}, {}, {}, {}, Melee},
{"������ �����", 850, 5 * GP, Male, Iron, {-4, 3, {1, 8}, Slashing, 0, 2}, {}, {}, {Versatile}, Melee, FocusAxes},
{"������", 1000, 5 * CP, Female, Wood, {-6, 3, {1, 6}, Bludgeon, 0, 2}, {}, {}, {}, Melee},
{"������", 50, 2 * GP, Male, Iron, {-2, 3, {1, 4}, Piercing, 2, 3}, {}, {}, {Light}, Melee},
{"�����", 800, 2 * GP, Male, Wood, {-3, 3, {2, 5}, Bludgeon, 0, 1}, {}, {}, {}, Melee},
{"������", 700, 8 * GP, Female, Iron, {-3, 3, {2, 7}, Bludgeon, 0, 1}, {}, {}, {}, Melee},
{"�����", 700, 8 * GP, NoGender, Iron, {-5, 3, {1, 8}, Piercing, 0, 2}, {}, {}, {}, Melee},
{"�����", 700, 8 * GP, Male, Iron, {-2, 3, {1, 6}, Bludgeon, 1, 2}, {}, {}, {TwoHanded}, Melee},
{"������� ���", 700, 8 * GP, Male, Iron, {-4, 3, {1, 8}, Slashing, 0, 2}, {}, swords_enchanments, {Versatile}, Melee, FocusSwords},
{"�������� ���", 700, 8 * GP, Male, Iron, {-3, 3, {1, 6}, Slashing, 0, 2}, {}, swords_enchanments, {Light}, Melee, FocusSwords},
{"��������� ���", 700, 8 * GP, Male, Iron, {-9, 3, {2, 12}, Slashing, -2, 2}, {}, swords_enchanments, {TwoHanded}, Melee, FocusTwohanded},
//
{"�������", 700, 40 * GP, Male, Wood, {-3, 3, {1, 8}, Piercing, -2, 0, Bolt}, {}, {}, {}, Ranged},
{"������� �������", 1200, 80 * GP, Male, Wood, {-10, 3, {1, 12}, Piercing, -6, 0, Bolt}, {}, {}, {}, Ranged},
{"������� ���", 500, 60 * GP, Male, Wood, {-10, 3, {1, 8}, Piercing, 1, 0, Arrow}, {}, {}, {}, Ranged, FocusBows},
{"�������� ���", 300, 30 * GP, Male, Wood, {-6, 3, {1, 6}, Piercing, 1, 0, Arrow}, {}, {}, {}, Ranged, FocusBows},
{"����", 30, 1 * SP, Male, Wood, {-4, 3, {1, 3}, Piercing, 3}, {}, {}, {}, Ranged},
{"�����", 50, 1 * SP, Female, Leather, {-6, 3, {1, 4}, Bludgeon, 0, Rock}, {}, {}, {}, Ranged},
//
{"�����", 20, 0, NoGender, Stone, {}, {}, {}, {Countable}, Amunitions},
{"������", 3, 2 * CP, NoGender, Wood, {}, {}, {}, {Countable}, Amunitions},
{"�����", 2, 1 * CP, NoGender, Iron, {}, {}, {}, {Countable}, Amunitions},
//
{"�������� �����", 1000, 5 * GP, Female, Leather, {-5}, {10, 1, 15}, {}, {}, Torso},
{"��������� �����", 1500, 15 * GP, Female, Leather, {-7}, {15, 1, 15}, {}, {}, Torso},
{"���������� ������", 2500, 30 * GP, Male, Iron, {-12}, {25, 2, 30}, {}, {}, Torso},
{"��������", 2600, 50 * GP, Female, Iron, {-10}, {25, 3, 25}, {}, {}, Torso},
{"������", 3000, 200 * GP, Male, Iron, {-15}, {30, 4, 35}, {}, {}, Torso},
{"����", 3500, 800 * GP, Female, Iron, {-20}, {40, 5, 35}, {}, {}, Torso},
//
{"���", 800, 0 * GP, Male, Iron, {-5, 1}, {10, 0, 25, 5}, {}, {}, OffHand},
{"����", 100, 0 * GP, Male, Iron, {-1}, {5, 0, 20, 3}, {}, {}, Head},
{"������", 70, 0 * GP, NoGender, Iron, {-1}, {5, 0, 15, 2}, {}, {}, Elbows},
{"�������� ������", 110, 0 * GP, NoGender, Iron, {-3}, {5, 1, 20, 2}, {}, {}, Elbows},
//
{"����", 30, 10 * SP, Male, Leather, {}, {3, 0, 2}, {}, {}, TorsoBack},
{"����", 30, 15 * GP, Male, Leather, {}, {3, 0, 2}, {}, {}, TorsoBack},
{"����", 35, 20 * GP, Female, Leather, {}, {3, 0, 2}, {}, {}, TorsoBack},
{"����", 35, 5 * GP, Female, Leather, {}, {5, 0, 2}, {}, {}, TorsoBack},
{"����", 40, 5 * GP, Female, Leather, {}, {5, 0, 2}, {}, {}, TorsoBack},
//
{"�����", 0, 0 * GP, NoGender, Leather, {}, {3, 0, 10}, boots_enchanments, {}, Legs},
{"������", 0, 0 * GP, NoGender, Leather, {}, {3, 0, 10}, boots_enchanments, {}, Legs},
{"������", 0, 0 * GP, NoGender, Iron, {}, {5, 0, 15}, boots_enchanments, {}, Legs},
{"������", 0, 0 * GP, NoGender, Iron, {}, {3, 1, 10}, boots_enchanments, {}, Legs},
{"������", 0, 0 * GP, NoGender, Iron, {}, {3, 1, 10}, boots_enchanments, {}, Legs},

{"���", 100, 5 * SP, NoGender, Organic, {}, {}, {}, {}, Edible},
{"������", 10, 5 * CP, NoGender, Organic, {}, {}, {}, {}, Edible},
{"���� ��������", 50, 1 * SP, NoGender, Organic, {}, {}, {}, {}, Edible},
{"���� ������", 50, 2 * SP, NoGender, Organic, {}, {}, {}, {}, Edible},
{"���� ������", 80, 8 * CP, NoGender, Organic, {}, {}, {}, {}, Edible},
{"��������", 20, 1 * GP, NoGender, Organic, {}, {}, {}, {}, Edible},
{"�������", 60, 8 * SP, NoGender, Organic, {}, {}, {}, {}, Edible},
{"����", 80, 5 * CP, NoGender, Organic, {}, {}, {}, {}, Edible},
//
{"������", 1, 10 * GP, Male, Paper, {}, {}, common_scroll, {SingleUse}, Readable},
{"������", 1, 12 * GP, Male, Paper, {}, {}, common_scroll, {SingleUse}, Readable},
{"������", 1, 15 * GP, Male, Paper, {}, {}, common_scroll, {SingleUse}, Readable},
//
{"����", 10, 100 * GP, NoGender, Wood, {}, {}, wand_enchanments, {Chargeable}, Zapable},
{"����", 10, 120 * GP, NoGender, Wood, {}, {}, wand_enchanments, {Chargeable}, Zapable},
{"����", 20, 150 * GP, NoGender, Iron, {}, {}, wand_enchanments, {Chargeable}, Zapable},
{"����", 20, 160 * GP, NoGender, Iron, {}, {}, wand_enchanments, {Chargeable}, Zapable},
{"����", 30, 180 * GP, NoGender, Iron, {}, {}, wand_enchanments, {Chargeable}, Zapable},
//
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"���", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
//
{"�����", 15, 20 * GP, NoGender, Glass, {}, {}, common_potions, {}, Drinkable},
{"�����", 20, 30 * GP, NoGender, Glass, {}, {}, common_potions, {}, Drinkable},
{"�����", 10, 40 * GP, NoGender, Glass, {}, {}, uncommon_potions, {}, Drinkable},
{"��������", 10, 45 * GP, Male, Glass, {}, {}, rare_potions, {}, Drinkable},
{"�������", 5, 50 * GP, Male, Glass, {}, {}, rare_potions, {}, Drinkable},
//
{"������", 1, 35 * GP, NoGender, Iron, {}, {}, ring_enchanments, {}, RightFinger},
{"������", 1, 40 * GP, NoGender, Iron, {}, {}, ring_enchanments, {}, RightFinger},
{"������", 2, 45 * GP, NoGender, Iron, {}, {}, ring_enchanments, {}, RightFinger},
//
{"������", 5, 0 * GP, NoGender, Wood, {}, {}, {}, {}, Neck},
{"������", 6, 0 * GP, NoGender, Wood, {}, {}, {}, {}, Neck},
{"������", 6, 0 * GP, NoGender, Iron, {}, {}, {}, {}, Neck},
{"������", 7, 0 * GP, NoGender, Iron, {}, {}, {}, {}, Neck},
{"������", 7, 0 * GP, NoGender, Iron, {}, {}, {}, {}, Neck},
//
{"����", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
//
{"������", 0, 1 * CP, NoGender, Iron, {}, {}, {}, {Coinable, Countable}},
{"����������� ������", 0, 1 * SP, NoGender, Iron, {}, {}, {}, {Coinable, Countable}},
{"������� ������", 0, 1 * GP, NoGender, Iron, {}, {}, {}, {Coinable, Countable}},
//
{"�����", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {Natural}, Melee},
{"������", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {Natural}, Melee},
{"����", 0, 0 * GP, NoGender, Organic, {}, {}, {}, {Natural}, Melee},
{"�����", 0, 0 * GP, NoGender, Leather, {-10}, {30, 2, 40}, {}, {Natural}, Torso},
{"���", 0, 0 * GP, NoGender, Leather, {-3}, {20, 0, 20}, {}, {Natural}, Torso},
//
{"����� ���������"},
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
				sb.adds("� %1i", am.armor);
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
			sb.adds("%1i ��", getcount());
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
				p->act("%1 ���������� � ����.", getname());
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
	auto b = getbonus() - getdamage();
	result.protection += b*result.protection_bonus;
	result.armor += b*result.armor_bonus;
	if(result.armor < 0)
		result.armor = 0;
	return result;
}

attacki item::getattack() const {
	auto& ei = getitem();
	auto result = ei.weapon;
	auto b = getbonus() - getdamage();
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

bool item::apply(creature& player, variant id, int v, int order, bool run) {
	switch(id.type) {
	case ItemType:
		if(getmagic() == id.value)
			return false;
		if(run)
			set((item_type_s)id.value);
		break;
	case ItemIdentify:
		if(is((identify_s)id.value))
			return false;
		if(run)
			set((identify_s)id.value);
		break;
	case Spell:
		switch(id.value) {
		case BlessItem:
			if(!is(KnownMagic) && !is(Mundane))
				return false;
			if(run) {
				act("%����� ��������%��� ������� ������� ���������.");
				set(Blessed);
			}
			break;
		case DetectMagic:
			if(is(KnownMagic) || !is(Blessed))
				return false;
			if(run) {
				act("%����� ��������%��� ����� ������.");
				set(KnownMagic);
			}
			break;
		case DetectEvil:
			if(is(KnownMagic) || !is(Cursed))
				return false;
			if(run) {
				act("%����� ��������%��� [-�������] ������.");
				set(KnownPower);
			}
			break;
		case Identify:
			if(is(KnownPower))
				return false;
			if(run) {
				act("%����� ��������%��� ������ ���������.");
				set(KnownPower);
				if(player.isactive()) {
					char temp[260]; stringbuilder st(temp); getname(st, true);
					sb.adds("��� [%-1].", temp);
				}
			}
			break;
		}
		break;
	case Skill:
		switch(id.value) {
		case Literacy:
			if(!is(Readable))
				return false;
			if(run) {
				auto v = geteffect();
				auto level = getbonus();
				auto consume = is(SingleUse);
				auto b = -level*5;
				if(is(Unknown))
					b += 10;
				if(is(SingleUse))
					b += 35;
				auto result = player.roll(Literacy, b);
				if(is(Unknown)) {
					player.act("%����� ����������� ������%� %-1.", getname());
					if(result) {
						player.addexp(10);
						set(KnownPower);
						if(player.isactive()) {
							char temp[260]; stringbuilder st(temp); getname(st, true);
							sb.adds("��� ������� ������, ��� ��� [%-1].", temp);
						}
					} else {
						if(player.isactive())
							player.act("������, ��� ��� � �� ������� ������ ������.");
					}
					consume = false;
				} else {
					switch(v.type) {
					case Spell:
						if(is(SingleUse)) {
							if(is(Blessed) || is(Artifact))
								result = true;
						}
						if(!result || !player.cast((spell_s)v.value, level, this))
							player.act("%����� �������%� %-1 � ������ ��������%�.", getname());
						break;
					}
				}
				if(consume) {
					act("%����� ���������%��� � ���� � ��������%���.");
					clear();
				}
			}
			break;
		}
		break;
	}
	return true;
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
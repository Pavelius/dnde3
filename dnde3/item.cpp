#include "main.h"

static variant common_potions[] = {Dexterity, Wisdow, Charisma, LifePoints, ManaPoints};

itemi bsmeta<itemi>::elements[] = {{"����", 0, 0, NoGender, Organic, {0, D1n3, Bludgeon, 4}, {}, {}, {}, Melee},
{"������ �����", 850, 5 * GP, Male, Iron, {-4, D1n8, Slashing}, {}, {}, {Versatile}, Melee, FocusAxes},
{"������", 1000, 5 * CP, Female, Wood, {-6, D1n4, Bludgeon}, {}, {}, {}, Melee},
{"������", 50, 2 * GP, Male, Iron, {-2, D1n3, Piercing, 2}, {}, {}, {Light}, Melee},
{"�����", 800, 2 * GP, Male, Wood, {-3, D1n4, Bludgeon}, {}, {}, {}, Melee},
{"������", 700, 8 * GP, Female, Iron, {-3, D2n7, Bludgeon}, {}, {}, {}, Melee},
{"�����", 700, 8 * GP, NoGender, Iron, {-5, D1n8, Piercing}, {}, {}, {}, Melee},
{"�����", 700, 8 * GP, Male, Iron, {-2, D1n6, Bludgeon, 1}, {}, {}, {TwoHanded}, Melee},
{"������� ���", 700, 8 * GP, Male, Iron, {-4, D1n8, Slashing}, {}, {}, {Versatile}, Melee, FocusSwords},
{"�������� ���", 700, 8 * GP, Male, Iron, {-3, D1n6, Slashing}, {}, {}, {Light}, Melee, FocusSwords},
{"��������� ���", 700, 8 * GP, Male, Iron, {-9, D2n12, Slashing, -2}, {}, {}, {TwoHanded}, Melee, FocusTwohanded},
//
{"�������", 700, 40 * GP, Male, Wood, {-3, D1n8, Piercing, -2, Bolt}, {}, {}, {}, Ranged},
{"������� �������", 1200, 80 * GP, Male, Wood, {-10, D1n12, Piercing, -6, Bolt}, {}, {}, {}, Ranged},
{"������� ���", 500, 60 * GP, Male, Wood, {-10, D1n8, Piercing, 1, Arrow}, {}, {}, {}, Ranged, FocusBows},
{"�������� ���", 300, 30 * GP, Male, Wood, {-6, D1n6, Piercing, 1, Arrow}, {}, {}, {}, Ranged, FocusBows},
{"����", 30, 1 * SP, Male, Wood, {-4, D1n3, Piercing, 3}, {}, {}, {}, Ranged},
{"�����", 50, 1 * SP, Female, Leather, {-6, D1n4, Bludgeon, 0, Rock}, {}, {}, {}, Ranged},
//
{"�����", 20, 0, Male, Stone, {}, {}, {}, {}, Amunitions, Bargaining, 30},
{"������", 3, 2 * CP, Female, Wood, {}, {}, {}, {}, Amunitions, Bargaining, 20},
{"�����", 2, 1 * CP, Male, Iron, {}, {}, {}, {}, Amunitions, Bargaining, 20},
//
{"�������� �����", 1000, 5 * GP, Female, Leather, {-5}, {10, 1, 15}, {}, {}, Torso},
{"��������� �����", 1500, 15 * GP, Female, Leather, {-7}, {15, 1, 15}, {}, {}, Torso},
{"���������� ������", 2500, 30 * GP, Male, Iron, {-12}, {25, 2, 30}, {}, {}, Torso},
{"��������", 2600, 50 * GP, Female, Iron, {-10}, {25, 3, 25}, {}, {}, Torso},
{"������", 3000, 200 * GP, Male, Iron, {-15}, {30, 4, 35}, {}, {}, Torso},
{"����", 3500, 800 * GP, Female, Iron, {-20}, {40, 5, 35}, {}, {}, Torso},
//
{"���", 0, 0 * GP, Female, Iron, {-5}, {10, 0, 25}, {}, {}, OffHand},
{"����", 0, 0 * GP, Female, Iron, {-1}, {5, 0, 20}, {}, {}, Head},
{"������", 0, 0 * GP, Female, Iron, {-1}, {5, 0, 15}, {}, {}, Elbows},
{"�������� ������", 0, 0 * GP, Female, Iron, {-3}, {5, 1, 20}, {}, {}, Elbows},
//
{"����", 0, 0 * GP, Male, Leather, {}, {3, 0, 2}, {}, {}, TorsoBack},
{"����", 0, 0 * GP, Male, Leather, {}, {3, 0, 2}, {}, {}, TorsoBack},
{"����", 0, 0 * GP, Female, Leather, {}, {3, 0, 2}, {}, {}, TorsoBack},
{"����", 0, 0 * GP, Female, Leather, {}, {5, 0, 2}, {}, {}, TorsoBack},
{"����", 0, 0 * GP, Female, Leather, {}, {5, 0, 2}, {}, {}, TorsoBack},
//
{"������", 0, 0 * GP, NoGender, Leather, {}, {3, 0, 10}, {}, {}, Legs},
{"������", 0, 0 * GP, NoGender, Leather, {}, {3, 0, 10}, {}, {}, Legs},
{"������", 0, 0 * GP, NoGender, Iron, {}, {5, 0, 15}, {}, {}, Legs},
{"������", 0, 0 * GP, NoGender, Iron, {}, {3, 1, 10}, {}, {}, Legs},
{"������", 0, 0 * GP, NoGender, Iron, {}, {3, 1, 10}, {}, {}, Legs},

{"���", 100, 5 * SP, NoGender, Organic, {}, {}, {}, {}, Edible},
{"������", 10, 5 * CP, NoGender, Organic, {}, {}, {}, {}, Edible},
{"���� ��������", 50, 1 * SP, NoGender, Organic, {}, {}, {}, {}, Edible},
{"���� ������", 50, 2 * SP, NoGender, Organic, {}, {}, {}, {}, Edible},
{"���� ������", 80, 8 * CP, NoGender, Organic, {}, {}, {}, {}, Edible},
{"��������", 20, 1 * GP, NoGender, Organic, {}, {}, {}, {}, Edible},
{"�������", 60, 8 * SP, NoGender, Organic, {}, {}, {}, {}, Edible},
{"����", 80, 5 * CP, NoGender, Organic, {}, {}, {}, {}, Edible},
//
{"������", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}, Readable},
{"������", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}, Readable},
{"������", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}, Readable},
//
{"����", 0, 0 * GP, NoGender, Wood, {}, {}, {}, {}},
{"����", 0, 0 * GP, NoGender, Wood, {}, {}, {}, {}},
{"����", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
{"����", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
{"����", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
//
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
{"�����", 0, 0 * GP, NoGender, Paper, {}, {}, {}, {}},
//
{"�����", 15, 20 * GP, NoGender, Glass, {}, {}, common_potions, {}, Drinkable},
{"�����", 20, 30 * GP, NoGender, Glass, {}, {}, common_potions, {}, Drinkable},
{"�����", 10, 40 * GP, NoGender, Glass, {}, {}, common_potions, {}, Drinkable},
{"�����", 10, 45 * GP, NoGender, Glass, {}, {}, common_potions, {}, Drinkable},
{"�����", 5, 50 * GP, NoGender, Glass, {}, {}, common_potions, {}, Drinkable},
//
{"������", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
//
{"������", 0, 0 * GP, NoGender, Wood, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Wood, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
{"������", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
//
{"����", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
//
{"������", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
{"����������� ������", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
{"������� ������", 0, 0 * GP, NoGender, Iron, {}, {}, {}, {}},
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
	auto slot = ei.slot;
	auto ai = ei.weapon;
	auto player = getwearer();
	ai.dice = ai.getdice();
	if(player) {
		auto s = getwearerslot();
		if(s && (slot == Melee || slot == Ranged || slot == OffHand))
			ai = player->getattack(s, *this);
	}
	if(ai.attack)
		sb.adds("%1:%2i%%", bsmeta<abilityi>::elements[AttackMelee].name_short, ai.attack);
	auto dc = ai.dice;
	if(dc.max)
		sb.adds("%1:%2i-%3i", bsmeta<abilityi>::elements[DamageMelee].name_short, dc.min, dc.max);
	auto& am = ei.armor;
	if(am.protection || am.armor) {
		sb.adds("%1:%2i%%", bsmeta<abilityi>::elements[Armor].name_short, am.protection);
		if(am.armor)
			sb.adds("� %1i", am.armor);
		if(am.deflect)
			sb.adds("%1:%2i%%", bsmeta<abilityi>::elements[Deflect].name_short, am.deflect);
	}
}

void item::getname(stringbuilder& sb, bool show_cab) const {
	auto& ei = getitem();
	if(show_cab) {
		if(isidentified())
			sb.adds(bsmeta<item_typei>::elements[getmagic()].name[ei.gender]);
	}
	sb.adds("%-1", getname());
	if(isidentified()) {
		auto effect = geteffect();
		if(effect)
			sb.adds(effect.getnameof());
		auto q = quality;
		if(ei.slot == Melee || ei.slot == OffHand || ei.slot == Head)
			q = getbonus();
		if(q != 0)
			sb.adds("%+1i", q);
	}
	auto n = getcount();
	if(n > 1)
		sb.adds("%1i��.", n);
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
	if(!isidentified())
		return 0;
	switch(magic) {
	case Artifact: return 2 + quality;
	case Blessed: return 1 + quality;
	case Cursed: return -1 - quality;
	default: return quality;
	}
}

variant item::geteffect() const {
	auto& ei = getitem();
	if(ei.effects.getcount() > 0)
		return ei.effects[effect];
	return variant();
}
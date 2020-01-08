#include "answer.h"
#include "point.h"
#include "rect.h"

#pragma once

const int GP = 100;
const int SP = 10;
const int CP = 1;

const short unsigned mmx = 96;
const short unsigned mmy = 96;
const unsigned short Blocked = 0xFFFF;

const int StandartEnergyCost = 1000;

const int chance_corridor_content = 10;
const int chance_door_closed = 30;
const int chance_generate_room = 40;
const int chance_special_area = 5;
const int dense_forest = 5;
const int max_building_size = 15;

const int chance_act = 40;
const int chance_blood_when_dead = 70;
const int chance_drop_item = 40;
const int restore_points_percent = 75;

enum item_s : unsigned char {
	NoItem,
	AxeBattle, Club, Dagger, HammerWar, Mace,
	Spear, Staff,
	SwordLong, SwordShort, SwordTwoHanded,
	CrossbowLight, CrossbowHeavy, BowLong, BowShort, Dart, Sling,
	Rock, Arrow, Bolt,
	LeatherArmor, StuddedLeatherArmor, ScaleMail, ChainMail, SplintMail, PlateMail,
	Shield, Helmet, BracersLeather, BracersIron,
	Cloack1, Cloack2, Cloack3, Cloack4, Cloack5,
	Boot1, Boot2, IronBoot1, IronBoot2, IronBoot3,
	Ration, Apple, BreadHalflings, BreadEvlen, BreadDwarven, Cake, Sausage, Meat,
	Scroll1, Scroll2, Scroll3,
	Wand1, Wand2, Wand3, Wand4, Wand5,
	Book1, Book2, Book3, Book4, Book5,
	Potion1, Potion2, Potion3, Potion4, Potion5,
	RingRed, RingBlue, RingGreen,
	Amulet1, Amulet2, Amulet3, Amulet4, Amulet5,
	DoorKey, Coin, CoinSP, CoinGP,
	Claws, Slam, Bite, Hitin, Fur,
	ManyItems
};
enum diety_s : unsigned char {
	NoGod,
	GodBane, GodBhaal, GodGruumsh, GodHelm, GodMistra, GodTempus, GodTyr
};
enum slot_s : unsigned char {
	Backpack, Edible, Readable, Drinkable, LastBackpack = Backpack + 31,
	Head, Neck, Melee, OffHand, TorsoBack, Torso, RightFinger, LeftFinger, Elbows, Legs, Ranged, Amunitions,
};
enum race_s : unsigned char {
	Human, Dwarf, Elf, Halfling,
	Goblin, Kobold, Orc, Gnoll,
	Animal, Insect, Undead,
	LastRace = Undead
};
enum class_s : unsigned char {
	Commoner,
	Cleric, Fighter, Mage, Paladin, Ranger, Theif,
	Monster,
};
enum gender_s : unsigned char {
	NoGender, Male, Female,
};
enum role_s : unsigned char {
	GoblinWarrior, GoblinRockthrower, OrcWarrior, LargeBat, GiantRat,
	HumanMale, HumanGuard, HumanChild, HumanFemale,
	Shopkeeper, DwarvenSmith, Bartender, Skeleton, Zombie,
	KobolWarrior, KoboldShaman,
	LargeDog, Lynx, GiantFrog,
	AntWorker, AntWarrior, AntQueen,
	GnollWarrior,
	Character
};
enum alignment_s : unsigned char {
	Lawfull, Neutral, Chaotic
};
enum ability_s : unsigned char {
	Strenght, Dexterity, Constitution, Intellegence, Wisdow, Charisma,
	Attack, Damage,
	Pierce, Protection, Armor, Deflect, Speed, Visibility,
	ResistAcid, ResistCharm, ResistCold, ResistElectricity,
	ResistFire, ResistParalize, ResistPoison, ResistWater,
	FirstResist = ResistAcid, LastResist = ResistWater,
	Level, LifePoints, LifeRate, ManaPoints, ManaRate,
};
enum skill_s : unsigned char {
	Bargaining, Bluff, Diplomacy,
	Acrobatics, Alertness, Athletics, Backstabbing, Climbing, Concetration,
	DisarmTraps, FindWeakness, HearNoises, HideInShadow, Lockpicking, PickPockets,
	Alchemy, Dancing, Engineering, Gambling, History, Healing, Herbalism,
	Literacy, Mining, Riding, Smithing, Survival, Swimming,
	FocusBows, FocusSwords, FocusAxes, FocusTwohanded,
	TwoWeaponFighting,
	FirstSkill = Bargaining, LastSkill = TwoWeaponFighting,
};
enum state_s : unsigned char {
	Anger, Dazzled, Drunken, Friendly, Hostile, Hunger,
	Invisible, Poisoned, Sick, Sleeped,
	Wounded,
	LastState = Wounded,
};
enum tile_s : unsigned char {
	Plain, Water, Floor, Wall, Road,
	Swamp, Hill,
	Sea, Foothills, Mountains, CloudPeaks, Forest,
	City
};
enum site_s : unsigned char {
	EmpthyRoom, TreasureRoom,
	StairsDownRoom, StairsUpRoom, House, Lair,
	Temple, Tavern, Barracs, CityHall,
	ShopWeaponAndArmor, ShopPotionAndScrolls, ShopFood,
};
enum map_object_s : unsigned char {
	NoTileObject,
	Door, Tree, Altar, Statue, Trap, Plants, StairsUp, StairsDown
};
enum trap_s : unsigned char {
	NoTrap,
	TrapAnimal, TrapAcid, TrapArrow, TrapCorrupt, TrapElectricity, TrapFire,
	TrapLight, TrapPit, TrapSpikedPit, TrapSpear, TrapBleed, TrapCorrosion, TrapWater
};
enum direction_s : unsigned char {
	Center,
	Left, Right, Up, Down, LeftUp, LeftDown, RightUp, RightDown
};
enum img_s : unsigned char {
	ResNone,
	ResBlood,
	ResGrass, ResGrassW,
	ResDungeon, ResDungeonW,
	ResShadow,
	ResRoad,
	ResWater,
	ResMonsters,
	ResItems,
	ResDoors,
	ResFog,
	ResFeature,
	ResSea, ResPlains, ResFoothills, ResMountains, ResCloudPeaks, ResDecals,
	ResUI,
	ResPCmar, ResPCmbd, ResPCmac
};
enum spell_s : unsigned char {
	NoSpell,
	ArmorSpell, BlessSpell, BlessItem, CharmPerson, DetectEvil, DetectMagic, Fear, HealingSpell,
	Identify, Invisibility, LightSpell, MagicMissile,
	Repair, RemovePoisonSpell, RemoveSickSpell,
	ShieldSpell, ShokingGrasp, Sleep, SlowMonster,
	FirstSpell = Armor, LastSpell = SlowMonster
};
enum map_flag_s : unsigned char {
	Visible, Hidden, Opened, Sealed, Explored, Webbed, Blooded,
};
enum item_type_s : unsigned char {
	Mundane, Cursed, Blessed, Artifact,
};
enum attack_s : unsigned char {
	Bludgeon, Slashing, Piercing,
	Acid, Cold, Electricity, Fire, Magic, Poison, WaterAttack
};
enum material_s : unsigned char {
	Glass, Iron, Leather, Organic, Paper, Stone, Wood,
};
enum encumbrance_s : unsigned char {
	NoEncumbered,
	Encumbered, HeavilyEncumbered,
};
enum range_s : unsigned char {
	You, Close, Reach, Near, Far
};
enum target_s : unsigned char {
	SingleTarget, NearestTarget, RandomTarget, AllTargets,
};
enum item_flag_s : unsigned char {
	Coinable, Countable, TwoHanded, Versatile, Light, Natural,
};
enum variant_s : unsigned char {
	NoVariant,
	Ability, Alignment, Creature, Formula, God, Harm,
	Item, ItemType,
	Number, Object, Race, Range, Role,
	Skill, Slot, Spell, State, Target, Tile,
	Variant,
};
enum formula_s : unsigned char {
	Negative,
	Divide2, Divide3, Divide4, Divide10,
	Multiply2, Multiply3, Multiply4
};
enum slot_mode_s : unsigned char {
	NoSlotName, SlotName, SlotWhere
};
enum identify_s : unsigned char {
	Unknown, KnownStats, KnownMagic, KnownPower,
};
typedef short unsigned indext;
typedef adat<rect, 64> rooma;
typedef flagable<1 + Chaotic / 8> alignmenta;
typedef flagable<1 + LastState / 8> statea;
typedef flagable<1 + LastRace / 8> racea;
typedef casev<ability_s> abilityv;
class creature;
struct variant {
	variant_s			type;
	unsigned char		value;
	constexpr variant() : type(NoVariant), value(0) {}
	constexpr variant(ability_s v) : type(Ability), value(v) {}
	constexpr variant(alignment_s v) : type(Alignment), value(v) {}
	constexpr variant(attack_s v) : type(Harm), value(v) {}
	constexpr variant(diety_s v) : type(God), value(v) {}
	constexpr variant(formula_s v) : type(Formula), value(v) {}
	constexpr variant(item_s v) : type(Item), value(v) {}
	constexpr variant(item_type_s v) : type(ItemType), value(v) {}
	constexpr variant(map_object_s v) : type(Object), value(v) {}
	constexpr variant(range_s v) : type(Range), value(v) {}
	constexpr variant(race_s v) : type(Race), value(v) {}
	constexpr variant(role_s v) : type(Role), value(v) {}
	constexpr variant(skill_s v) : type(Skill), value(v) {}
	constexpr variant(slot_s v) : type(Slot), value(v) {}
	constexpr variant(spell_s v) : type(Spell), value(v) {}
	constexpr variant(state_s v) : type(State), value(v) {}
	constexpr variant(target_s v) : type(Target), value(v) {}
	constexpr variant(variant_s v) : type(Variant), value(v) {}
	constexpr variant(int v) : type(Number), value(v) {}
	variant(const creature* v);
	explicit operator bool() const { return type != NoVariant; }
	bool operator==(const variant& e) const { return type == e.type && value == e.value; }
	const char*			getname() const;
	const char*			getnameof() const;
	const char*			getnameofc() const;
};
typedef variant			varianta[16];
typedef adat<casev<variant>, 8> chancev;
struct string : stringbuilder {
	const char			*name, *opponent_name;
	gender_s			gender, opponent_gender;
	constexpr string(const stringbuilder& source) : stringbuilder(source),
		name(0), opponent_name(0),
		gender(Male), opponent_gender(Male) {
	}
	template<unsigned N> constexpr string(char(&result)[N]) : stringbuilder(result, result + N - 1), name(0), gender(Female) {}
	void				addformula(const variant* p);
	void				addidentifier(const char* identifier) override;
};
struct sloti {
	const char*			id;
	const char*			name;
	const char*			name_where;
};
struct chancei {
	state_s				state;
};
struct boosti {
	short unsigned		owner;
	variant				id;
	char				modifier;
	unsigned			time;
};
struct classi {
	struct weaponi {
		char			base;
		char			multiplier;
	};
	const char*			name;
	unsigned char		hp, mp;
	unsigned char		naked_avatar;
	char				ability[6];
	weaponi				weapon;
	adat<skill_s, 8>	skills;
	adat<spell_s, 6>	spells;
	flagable<ManyItems>	restricted;
};
struct abilityi {
	const char*			id;
	const char*			name;
	const char*			name_short;
	const char*			nameof;
	const char*			cursedof;
	const char*			name_how;
	const char*			curse_how;
	varianta			formula;
	char				bonus_base, bonus_multiplier;
	const char*			format;
	char				cost;
	//
	ability_s			getid() const;
	int					getbonus(int v) const;
};
struct skilli {
	struct weaponi {
		char			attack;
		char			damage;
		char			speed;
	};
	const char*			name;
	const char*			name_tome;
	ability_s			abilities[2];
	weaponi				weapon;
	varianta			usable;
	//
	skill_s				getid() const;
	const char*			getusetext() const;
	bool				isresist() const { return getid() >= FirstResist; }
	constexpr bool		isweapon() const { return weapon.attack != 0; }
};
struct equipmenti {
	race_s				race;
	class_s				type;
	varianta			features;;
};
struct tilei {
	const char*			id;
	const char*			name;
	gender_s			gender;
};
struct map_objecti {
	const char*			id;
	const char*			name;
	short unsigned		start, count;
};
struct picture : point {
	img_s				img;
	unsigned short		frame;
	unsigned short		flags;
	unsigned char		alpha;
	unsigned char		level;
	constexpr explicit operator bool() const { return img != ResNone; }
	void				clear() { memset(this, 0, sizeof(*this)); alpha = 0xFF; }
	void				render(int x, int y) const;
	void				set(indext i);
	void				set(short x, short y);
	void				setcursor(indext i, int size);
};
struct statei {
	const char*			id;
	const char*			name;
	const char*			nameof;
	bool				hostile;
	const char*			remove;
};
struct racei {
	const char*			name;
	char				abilities[6];
	skill_s				skills[3];
	adat<abilityv, 8>	skillvs;
};
struct dicei {
	char				min;
	char				max;
	int					roll() const;
	void				normalize();
};
struct armori {
	char				protection;
	char				armor;
	char				deflect;
	char				protection_bonus;
	char				armor_bonus;
};
struct attacki {
	char				attack, attack_bonus; // Percent bonus to hit
	dicei				dice;
	attack_s			type;
	char				speed;
	char				damage_bonus;
	item_s				ammunition;
	int					getenergy() const { return StandartEnergyCost - speed * 50; }
};
struct trapi {
	const char*			name;
	char				modifier;
	char				multiplier;
	dicei				damage;
	variant				effect;
	const char*			text_use;
};
struct item_typei {
	const char*			id;
	const char*			name[3];
	char				bonus;
};
struct itemi {
	const char*			name;
	int					weight;
	int					cost;
	gender_s			gender;
	material_s			material;
	attacki				weapon;
	armori				armor;
	aref<variant>		effects;
	cflags<item_flag_s>	flags;
	slot_s				slot;
	skill_s				skill;
};
class item {
	union {
		struct {
			item_s		type;
			//
			item_type_s	magic : 2;
			unsigned char quality : 2;
			identify_s	identify : 2;
			unsigned char forsale : 1;
			//
			unsigned char charge : 6;
			unsigned char damaged : 2;
			//
			unsigned char effect;
		};
		short unsigned	us[2];
		int				i;
	};
public:
	item() = default;
	item(item_s type, int level);
	explicit operator bool() const { return type != NoItem; }
	void				act(const char* format, ...) const;
	void				clear() { memset(this, 0, sizeof(*this)); }
	void				create(item_s type, int chance_artifact, int chance_magic, int chance_cursed, int chance_quality);
	bool				damageb();
	void				damage();
	item_s				getammo() const { return getitem().weapon.ammunition; }
	armori				getarmor() const;
	attacki				getattack() const;
	int					getbonus() const;
	unsigned			getcost() const;
	int					getcount() const;
	int					getdamage() const;
	variant				geteffect() const;
	const itemi&		getitem() const { return bsmeta<itemi>::elements[type]; }
	gender_s			getgender() const { return getitem().gender; }
	item_s				getkind() const { return type; }
	item_type_s			getmagic() const { return magic; }
	material_s			getmaterial() const { return getitem().material; }
	const char*			getname() const { return getitem().name; }
	void				getname(stringbuilder& sb, bool show_cab) const;
	int					getquality() const { return quality; }
	void				getstatistic(stringbuilder& sb) const;
	creature*			getwearer() const;
	slot_s				getwearerslot() const;
	int					getweightsingle() const { return getitem().weight; }
	int					getweight() const { return getweightsingle()*getcount(); }
	bool				is(slot_s v) const;
	bool				is(identify_s v) const { return v ? identify >= v : (v == identify); }
	bool				is(item_flag_s v) const { return getitem().flags.is(v); }
	bool				is(item_type_s v) const { return magic == v; }
	bool				isboost(variant id) const;
	bool				ischargeable() const { return false; }
	bool				iscountable() const { return is(Countable); }
	bool				isdamaged() const { return getdamage() > 0; }
	bool				isunbreakable() const { return magic != Mundane; }
	void				loot();
	bool				ismatch(variant v) const;
	void				repair(int level);
	void				set(item_type_s v);
	void				set(identify_s v);
	void				setcharges(int v);
	void				setcount(int v);
	void				seteffect(variant v);
	void				setquality(int v);
	void				setsale(int v) { forsale = v; }
	bool				use();
};
class itema : public adat<item*> {
public:
	item*				chooses(bool interactive, const char* title, const char* format, slot_mode_s mode);
	item*				choose(bool interactive, const char* title, const char* format, slot_mode_s mode);
	void				footer(stringbuilder& sb) const;
	void				match(variant v);
	void				matchboost(variant v);
	void				remove(variant v);
	void				select(creature& e);
	void				select(indext index);
	void				selecta(creature& e);
	void				selectb(creature& e);
};
class skilla :public adat<skill_s, 64> {
public:
	bool				choose(bool interactive, const char* title, const char* format, skill_s& result) const;
	void				select(const creature& e);
	void				sort();
};
class skillu : public skilla {
	creature*			player;
	unsigned char		cap[LastSkill + 1];
public:
	skillu(creature* player);
	skill_s				choose(bool interactive, const char* title, bool* cancel_result = 0) const;
	int					getcap(skill_s i) const { return cap[i]; }
	void				setcap(skill_s i, int v) { cap[i] = v; }
	void				setcaps();
};
class site : public rect {
	site_s				type;
	unsigned char		name[2];
	diety_s				diety;
	short unsigned		owner_id;
	unsigned char		found;
	unsigned			recoil;
public:
	constexpr site() : rect({0, 0, 0, 0}), type(EmpthyRoom), diety(NoGod),
		name(), owner_id(Blocked), found(0), recoil(0) {}
	operator bool() const { return x1 != x2; }
	creature*			add(race_s race, gender_s gender, class_s type);
	creature*			add(role_s type);
	void				create(int x, int y, int w, int h, site_s type);
	static site*		find(indext index);
	void				getname(stringbuilder& sb) const;
	creature*			getowner() const;
	indext				getposition() const;
	creature*			priest();
	void				set(site_s v) { type = v; }
	void				set(diety_s v) { diety = v; }
	void				setowner(const creature* v);
	creature*			shopkeeper();
};
class posable {
	indext				index;
public:
	constexpr indext	getposition() const { return index; }
	constexpr void		setposition(indext v) { index = v; }
};
class nameable : public variant {
	short unsigned		name;
public:
	void				act(const char* format, ...) const;
	void				actv(stringbuilder& sb, const char* format, const char* param) const;
	void				actv(stringbuilder& sb, nameable& e, const char* format, const char* param) const;
	gender_s			getgender() const;
	const char*			getname() const;
	void				sayv(stringbuilder& st, const char* format, const char* param) const;
	void				setname(race_s race, gender_s gender);
	race_s				getrace() const;
	bool				ischaracter() const { return value == Character; }
};
struct rolei {
	const char*			name;
	race_s				race;
	gender_s			gender;
	alignment_s			alignment;
	class_s				type;
	char				level;
	varianta			features;
	role_s				minions[4];
};
class creature : public nameable, public posable {
	char				abilities[ManaRate + 1];
	unsigned char		skills[LastResist + 1];
	unsigned char		spells[LastSpell + 1];
	item				wears[Amunitions + 1];
	int					restore_energy, restore_hits, restore_mana;
	char				hp, mp;
	statea				states;
	short unsigned		charmer, horror;
	short unsigned		location_id, site_id;
	encumbrance_s		encumbrance;
	class_s				kind;
	unsigned short		guard;
	direction_s			direction;
	unsigned			experience;
	unsigned			money;
	//
	void				addx(variant id, int modifier, unsigned rounds);
	bool				aiuse(bool interactive, const char* title, slot_s slot, variant effect);
	void				aimove();
	void				aiturn();
	void				applyabilities();
	void				applyaward() const;
	void				attack(creature& enemy, const attacki& ai, int bonus);
	const variant*		calculate(const variant* formula, int& result) const;
	void				cantmovehere() const;
	bool				cantakeoff(slot_s id, bool interactive);
	bool				canuse(const item& e, bool talk) const;
	void				clearboost() const;
	void				dress(int m);
	void				dresssk(int m);
	void				dresssa(int m);
	void				dropdown(item& item);
	void				dropitems();
	void				equip(item it, slot_s id);
	void				finish();
	void				move(indext index, bool runaway);
	bool				remove(item& it, bool run, bool talk);
	void				usetrap();
public:
	explicit operator bool() const { return hp > 0; }
	//
	void				activate();
	void				add(variant id, int v);
	void				add(variant id, int v, bool interactive);
	void				add(variant id, int v, bool interactive, unsigned minutes);
	void				add(variant id, int v, bool interactive, item_type_s magic, int quality, int damage, int minutes);
	bool				add(item v, bool run, bool interactive);
	void				addexp(int count);
	bool				alertness();
	bool				askyn(creature* opponent, const char* format, ...);
	void				athletics(bool interactive);
	void				backpack();
	void				bloodstain() const;
	int					calculate(const variant* formule) const;
	bool				canhear(short unsigned index) const;
	bool				cansee(indext i) const;
	bool				canshoot(bool talk) const;
	void				chat(creature* opponent);
	void				create(race_s race, gender_s gender, class_s type);
	void				create(role_s type);
	void				clear();
	void				consume(int energy_value);
	void				damage(int count, attack_s type, int pierce = 0, bool interactive = true);
	void				damagewears(int count, attack_s type);
	void				dressoff();
	void				dresson();
	void				drink();
	void				dropdown();
	void				eat();
	bool				equip(item value);
	bool				equip(item& v1, item& v2, bool run);
	void				enslave();
	static creature*	find(indext i);
	boosti*				find(variant id) const;
	int					get(ability_s v) const { return abilities[v]; }
	int					get(spell_s v) const { return spells[v]; }
	int					get(skill_s v) const;
	const item&			get(slot_s v) const { return wears[v]; }
	static creature*	getactive();
	static creature*	getactive(int index);
	attacki				getattack(slot_s slot, const item& weapon) const;
	attacki				getattack(slot_s slot) const { return getattack(slot, wears[slot]); }
	int					getaward() const { return 10 + 15 * get(Level); }
	int					getbasic(ability_s value) const;
	int					getbasic(skill_s v) const { return skills[v]; }
	int					getboost(variant id) const;
	const classi&		getclass() const { return bsmeta<classi>::elements[kind]; }
	direction_s			getdirection() const { return direction; }
	encumbrance_s		getencumbrance() const { return encumbrance; }
	int					getexperience() const { return experience; }
	void				getfullname(stringbuilder& sb) const;
	short unsigned		getguard() const { return guard; }
	int					gethits() const { return hp; }
	creature*			gethorror() const { return getobject(horror); }
	short unsigned		getid() const;
	creature*			getleader() const;
	int					getlevel(skill_s v) const;
	const char*			getlevelname(skill_s v) const;
	int					getlos() const { return abilities[Visibility]; }
	int					getmana() const { return mp; }
	int					getmoney() const { return money; }
	static creature*	getobject(short unsigned v);
	dicei				getraise(skill_s id) const;
	role_s				getrole() const { return (role_s)value; }
	site*				getsite() const { return 0; }
	slot_s				getwearerslot(const item* p) const;
	int					getweight() const;
	void				heal(int value) { damage(-value, Magic); }
	void				inventory();
	bool				is(class_s v) const { return kind == v; }
	bool				is(state_s v) const { return states.is(v); }
	bool				is(encumbrance_s value) const { return encumbrance == value; }
	bool				isactive() const { return getactive() == this; }
	bool				isallow(item_s v) const;
	bool				isenemy(const creature* target) const;
	bool				isguard() const { return guard != Blocked; }
	bool				ismatch(variant v) const;
	bool				isvisible() const;
	void				kill();
	void				look(indext index);
	void				lookaround();
	void				makemove();
	bool				match(variant id) const;
	void				meleeattack(creature& enemy, int bonus = 0);
	void				minimap();
	void				move(indext index);
	void				moveaway(indext index) { move(index, true); }
	void				moveto(indext index) { move(index, false); }
	bool				needrestore(ability_s id) const;
	static void			pause();
	void				paymana(int value, bool interactive);
	void				playui();
	void				pickup();
	void				raise(skill_s value);
	void				raiseskills(int number);
	void				raiseskills() { raiseskills(get(Intellegence) / 2); }
	void				rangeattack(creature& enemy, int bonus = 0);
	bool				roll(ability_s v) const { return rollv(get(v), 0); }
	bool				roll(ability_s v, int bonus) const { return rollv(get(v) + bonus, 0); }
	bool				roll(skill_s v) const { return rollv(get(v), 0); }
	bool				roll(skill_s v, int bonus) const { return rollv(get(v) + bonus, 0); }
	bool				roll(skill_s v, int bonus, int divider) const;
	static bool			rollv(int v, int* r);
	void				say(const char* format, ...) const;
	void				select(itema& a, slot_s i1, slot_s i2, bool filled_only);
	void				select(skilla& e) const;
	void				set(ability_s id, int v);
	void				set(spell_s id, int v) { spells[id] = v; }
	void				set(skill_s id, int v);
	void				setcharmer(const creature* p) { charmer = p->getid(); }
	void				setguard(short unsigned value) { guard = value; }
	void				sethorror(const creature* p) { horror = p->getid(); }
	void				setmoney(int value) { money = value; }
	void				setposition(indext v);
	void				shoot();
	void				testweapons();
	void				trapeffect();
	void				unlink();
	bool				use(item& it, bool interactive);
	static bool			usechance(int chance, bool hostile, item_type_s magic, int quality, int damaged);
	void				useskills();
	void				wait() { consume(StandartEnergyCost); }
};
class creaturea : public adat<creature*> {
public:
	creaturea() = default;
	creaturea(const creature& v) { select(v.getposition(), v.getlos()); }
	creature*			choose(bool interactive, const char* title);
	void				match(variant v);
	void				match(indext index, int range);
	void				matchenemy(const creature* v);
	void				remove(variant v);
	void				select();
	void				select(indext start, int distance);
	void				select(state_s v);
	void				sort(indext start);
};
class indexa : public adat<indext> {
public:
	int					choose(bool interactive, const char* title);
	void				match(variant v);
	void				match(indext index, int range);
	void				remove(variant v);
	void				select(indext index, int distance);
	void				sort(indext start);
};
struct spelli {
	const char*			name;
	varianta			effect;
};
struct itemground : item {
	short unsigned		index;
};
struct coordinate {
	constexpr coordinate() : index(Blocked), level(1) {}
	short unsigned		index; // Позиция на карте мира
	unsigned char		level; // Уровень поздземелья
};
struct areainfo : coordinate {
	unsigned char		rooms; // Количество комнат
	unsigned short		artifacts;
	bool				isdungeon; // Underground dungeons has 'true'
	race_s				habbitants[4];
	short unsigned		positions[8]; // Several positions
	constexpr areainfo() : rooms(0), isdungeon(false),
		artifacts(0), habbitants(),
		positions{Blocked, Blocked, Blocked, Blocked, Blocked, Blocked, Blocked, Blocked} {
	}
};
struct vproc {
	void(*pinp)();
	void(creature::*pcre)();
	constexpr vproc() : pinp(0), pcre(0) {}
	constexpr vproc(void(*v)()) : pinp(v), pcre(0) {}
	constexpr vproc(void(creature::*v)()) : pinp(0), pcre(v) {}
};
struct dungeon {
	struct layer {
		tile_s			type;
		unsigned char	count;
	};
	const char*			name; // name of area location
	short unsigned		world_index; // overland index
	unsigned char		icon; // overland icon overlay
	unsigned char		level; // Start level: 0 form city, 1 for undeground dungeon
	adat<layer, 8>		layers;
};
struct manual {
	typedef void(*proc)(stringbuilder& sb, manual& e, answeri& an);
	variant				parent;
	variant				value;
	const char*			name;
	const char*			text;
	aref<proc>			procs;
	explicit operator bool() const { return (bool)value; }
	const char*			getname() const;
};
struct statistici {
	short				artifacts;
	short				level;
	indext				positions[4];
};
class analize {
	target_s			target;
	variant_s			type;
	creature&			player;
	itema				items;
	indexa				indecies;
	creaturea			creatures;
	//
	void				set(variant_s v);
public:
	analize(creature& player);
	analize(creature& player, creaturea& opponents);
	void				apply(const varianta& source, int value);
	void				match(variant v);
	void				remove(variant v);
	variant				select(const varianta& source);
};
class location : public statistici {
	typedef bool(location::*procis)(indext i) const;
	tile_s				tiles[mmx*mmy];
	map_object_s		objects[mmx*mmy];
	unsigned char		random[mmx*mmy];
	flagable<1>			flags[mmx*mmy];
	role_s				monsters[6];
	bool				is_dungeon;
	//
	indext				bpoint(indext index, int w, int h, direction_s dir) const;
	bool				isdungeon() const { return is_dungeon; }
	indext				getfree(indext i, procis proc, int radius_maximum) const;
	void				room(const rect& rc);
	bool				linelos(int x0, int y0, int x1, int y1) const;
	bool				wget(short unsigned i, direction_s direction, tile_s value) const;
	bool				wget(short unsigned i, direction_s direction, tile_s value, bool default_result) const;
	bool				xget(short unsigned i, direction_s direction) const;
public:
	creature*			add(indext index, role_s role);
	creature*			add(indext index, race_s race, gender_s gender, class_s type);
	void				addinfo(indext i, stringbuilder& sb) const;
	void				additems(indext i, stringbuilder& sb) const;
	void				addobject(indext i, stringbuilder& sb) const;
	creature*			adventurer(indext index);
	void				blockcreatures();
	void				blockwalls(bool water = true);
	indext				building(indext i, int width, int height, direction_s dir = Center);
	bool				cansee(indext i1, indext i2) const;
	static indext		center(int x, int y, int w, int h);
	static indext		center(const rect& rc);
	indext				choose(bool allow_cancel);
	void				clear();
	static void			clearblock();
	void				create(bool explored, bool visualize);
	void				create(const rect& rc, int count, map_object_s object);
	void				create(const rect& rc, int count, tile_s v);
	void				drop(indext i, item v);
	void				editor();
	void				ellipse(rect rc, tile_s object);
	static indext		get(short x, short y) { return y * mmx + x; }
	static direction_s	getdirection(indext from, indext to);
	static direction_s	getdirection(point from, point to);
	indext				getfree(indext i) const { return getfree(i, &location::isfreenc, 5); }
	static short		getx(indext i) { return i % mmx; }
	static short		gety(indext i) { return i / mmx; }
	int					getindex(indext i, tile_s e) const;
	int					getitemscount(indext i) const;
	map_object_s		getobject(indext i) const { return objects[i]; }
	tile_s				gettile(indext i) const;
	trap_s				gettrap(indext i) const;
	static int			getrange(indext i1, indext i2);
	int					getrand(indext i) const { return random[i]; }
	void				indoor(point camera, bool show_fow = true, const picture* effects = 0);
	bool				is(indext i, map_flag_s v) const { return flags[i].is(v); }
	bool				isfree(indext i) const;
	bool				isfreenc(indext i) const;
	bool				isfreenw(indext i) const;
	bool				ismatch(indext index, const rect& rectanle) const;
	bool				ismatch(indext index, const aref<variant>& v) const;
	bool				ismatch(indext index, variant v) const;
	void				lake(int x, int y, int w, int h);
	void				makewave(indext index);
	void				minimap(int x, int y, point camera) const;
	void				minimap(indext index) const;
	creature*			monster(indext index);
	bool				read(const char* url);
	void				remove(indext i, map_flag_s v) { flags[i].remove(v); }
	void				set(indext i, map_flag_s v) { flags[i].set(v); }
	void				set(indext i, tile_s v) { tiles[i] = v; }
	void				set(indext i, tile_s v, int width, int height);
	void				set(indext i, trap_s v);
	void				set(indext i, map_object_s v) { objects[i] = v; }
	static void			setcamera(short x, short y);
	static void			setcamera(indext i);
	void				setdungeon(bool v) { is_dungeon = v; }
	indext				setiwh(int x, int y, int s, tile_s o, map_object_s r, bool locked_doors);
	indext				setiwv(int x, int y, int s, tile_s o, map_object_s r, bool locked_doors);
	void				setlos(indext index, int r);
	static void			show(rooma& rooms);
	indext				stepto(indext index);
	indext				stepfrom(indext index);
	static indext		to(indext index, direction_s v);
	static direction_s	to(direction_s d, direction_s v);
	void				worldmap(point camera, bool show_fow = true) const;
	bool				write(const char* url) const;
};
struct outdoor : public posable {
	struct leveli {
		char			count;
		race_s			habbitants[5];
		constexpr operator bool() const { return count != 0; }
	};
	char				name[32];
	leveli				levels[8];
};
class gamei {
	unsigned			rounds;
	bool				checkalive();
	void				playactive();
public:
	void				applyboost();
	int					getrounds() const { return rounds; }
	void				intialize();
	static void			help();
	void				play();
};
extern gamei			game;
extern location			loc;
extern stringbuilder	sb;
DECLENUM(class);
DECLENUM(item);
DECLENUM(map_object);
DECLENUM(skill);
DECLENUM(tile);
DECLENUM(race);
DECLENUM(slot);
DECLENUM(state);
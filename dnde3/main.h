#include "answer.h"
#include "point.h"
#include "rect.h"

#pragma once

const int GP = 100;
const int SP = 10;
const int CP = 1;
const int chance_to_hit = 40; // Dexterity add to this value. Mediaval dexterity is 10, so medium chance to hit is 50%.
const short unsigned mmx = 96;
const short unsigned mmy = 96;
const unsigned short Blocked = 0xFFFF;
const unsigned short BlockedCreature = Blocked - 1;

enum dice_s : unsigned char {
	NoDice,
	D1n3, D1n6, D2n7, D3n8, D4n9, D5n10,
	D2n12, D3n13, D4n14, D5n15,
	D3n18, D4n19, D5n20,
};
enum item_s : unsigned char {
	NoItem,
	AxeBattle, Club, Dagger, HammerWar, Mace,
	Spear, Staff,
	SwordLong, SwordShort, SwordTwoHanded,
	CrossbowLight, CrossbowHeavy, BowLong, BowShort, Dart, Sling,
	Rock, Arrow, Bolt,
	LeatherArmour, StuddedLeatherArmour, ScaleMail, ChainMail, SplintMail, PlateMail,
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
	Head, Neck, Melee, OffHand, TorsoBack, Torso, RightFinger, LeftFinger, Elbows, Legs, Ranged, Amunitions,
	FirstBackpack, LastBackpack = FirstBackpack + 19,
};
enum enchantment_s : unsigned char {
	NoEffect,
	OfArmor,
	OfCharisma, OfCold, OfConstitution,
	OfDefence, OfDexterity,
	OfHoliness,
	OfFire,
	OfIntellegence,
	OfMana, OfMissileDeflection,
	OfOrcSlying,
	OfParalize, OfPoison, OfPrecision,
	OfRegeneration,
	OfSickness, OfSharping, OfSlowing, OfSmashing, OfSpeed, OfStrenght, OfSustenance,
	OfVampirism, OfWeakness, OfWisdow,
	// Resistances
	OfAcidResistance, OfCharmResistance, OfColdResistance, OfFireResistance, OfParalizeResistance,
	OfElectricityResistance, OfPoisonResistance, OfWaterproof,
	LastEnchantment = OfWaterproof,
};
enum race_s : unsigned char {
	Animal,
	Human, Dwarf, Elf, Halfling,
	Goblin, Kobold, Orc, Gnoll,
	Insect, Undead
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
	AttackMelee, AttackRanged, Pierce, Deflect, Armor, Damage, Speed, Visibility,
	LifePoints, LifeRate, ManaPoints, ManaRate,
};
enum skill_s : unsigned char {
	Bargaining, Bluff, Diplomacy,
	Acrobatics, Alertness, Athletics, Backstabbing, Concetration, DisarmTraps, HearNoises, HideInShadow, Lockpicking, PickPockets,
	Alchemy, Dancing, Engineering, Gambling, History, Healing, Literacy, Mining, Smithing, Survival, Swimming,
	WeaponFocusBows, WeaponFocusBlades, WeaponFocusAxes, TwoWeaponFighting,
	LastSkill = TwoWeaponFighting,
	ResistAcid, ResistCharm, ResistCold, ResistElectricity, ResistFire, ResistParalize, ResistPoison, ResistWater,
	LastResist = ResistWater,
};
enum state_s : unsigned char {
	Anger, Blessed, Charmed, Drunken, Invisible, Paralized, Poisoned, Sick, Sleeped,
	LastState = Sleeped,
};
enum tile_s : unsigned char {
	Plain, Water, Floor, Wall, Road,
	Swamp, Hill,
	Sea, Foothills, Mountains, CloudPeaks, Forest,
	City,
};
enum site_s : unsigned char {
	EmpthyRoom, TreasureRoom,
	StairsDownRoom, StairsUpRoom, House, Lair,
	Temple, Tavern, Barracs, CityHall,
	ShopWeaponAndArmor, ShopPotionAndScrolls, ShopFood,
};
enum map_object_s : unsigned char {
	NoTileObject,
	Door, Tree, Altar, Statue, Trap, StairsUp, StairsDown
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
	ArmorSpell, Bless, BlessItem, CharmPerson, DetectEvil, DetectMagic, Fear, HealingSpell,
	Identify, Invisibility, LightSpell, MagicMissile,
	Repair, RemovePoisonSpell, RemoveSickSpell,
	ShieldSpell, ShokingGrasp, Sleep, SlowMonster,
	FirstSpell = Armor, LastSpell = SlowMonster
};
enum map_flag_s : unsigned char {
	Visible, Hidden, Opened, Sealed, Explored,
};
enum duration_s : unsigned {
	Instant = 0,
	Minute = 12, Turn = 10 * Minute,
	Halfhour = 30 * Minute, Hour = 60 * Minute, HalfDay = 12 * Hour, Day = 24 * Hour,
	Week = 7 * Day, Month = 30 * Day, Season = 3 * Month, Year = 4 * Season,
	Permanent = 100 * Year
};
enum item_type_s : unsigned char {
	Mundane, Cursed, BlessedItem, Artifact,
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
enum speech_s : unsigned char {
	NoTalking,
	Answer, Action, Speech,
};
enum manual_s : unsigned char {
	Element, Header
};
enum range_s : unsigned char {
	You, Close, Reach, Near,
};
enum dungeon_area_s : unsigned char {
	AreaMaze, AreaDungeon,
	AreaPlain, AreaForest, AreaHills,
	AreaCity,
};
enum variant_s : unsigned char {
	NoVariant,
	Ability, Creature, Enchantment, God, Items, ItemObject, Skill, Spell, State,
};
class creature;
typedef short unsigned indext;
typedef void(*eventproc)(); 
struct variant {
	variant_s			type;
	unsigned char		value;
	constexpr variant() : type(NoVariant), value(0) {}
	constexpr variant(ability_s v) : type(Ability), value(v) {}
	constexpr variant(diety_s v) : type(God), value(v) {}
	constexpr variant(enchantment_s v) : type(Enchantment), value(v) {}
	constexpr variant(item_s v) : type(Items), value(v) {}
	constexpr variant(skill_s v) : type(Skill), value(v) {}
	constexpr variant(spell_s v) : type(Spell), value(v) {}
	constexpr variant(state_s v) : type(State), value(v) {}
	variant(const creature* v);
	explicit operator bool() const { return type != NoVariant; }
};
struct boosti {
	short unsigned		owner;
	variant				id;
	char				modifier;
	unsigned			time;
};
struct abilityi {
	const char*			id;
	const char*			name;
	const char*			name_short;
	const char*			nameof;
	const char*			cursedof;
};
struct tilei {
	const char*			id;
	const char*			name;
};
struct picture : point {
	img_s				img;
	unsigned short		frame;
	unsigned short		flags;
	unsigned char		alpha;
	unsigned char		level;
	void				clear() { memset(this, 0, sizeof(*this)); alpha = 0xFF; }
	void				render(int x, int y) const;
	void				set(indext i);
	void				set(short x, short y);
	void				setcursor(indext i, int size);
};
struct skillvalue {
	skill_s				id;
	char				value;
};
struct damagei {
	char				min;
	char				max;
	attack_s			type;
	explicit operator bool() const { return max != 0; }
	int					roll() const;
};
struct attacki {
	char				speed;
	damagei				damage;
	char				bonus; // Percent bonus to hit
	char				critical;
	char				multiplier;
	enchantment_s		effect;
	char				quality;
};
struct foodi {
	char				hits;
	char				mana;
	char				abilities[Charisma + 1];
	unsigned			duration;
	char				sickness;
	char				poision;
	explicit operator bool() const { return hits != 0; }
	int					get(int value) const { return value * 50; }
};
struct speciali {
	char				chance_broke;
	char				bonus;
	char				chance_side;
};
struct hotkey {
	char				key[8];
	const char*			name;
	void(*proc)();
	explicit operator bool() const { return proc != 0; }
};
class item {
	item_s				type;
	//
	enchantment_s		effect;
	//
	unsigned char		count : 6;
	unsigned char		damaged : 2;
	//
	item_type_s			magic : 2;
	unsigned char		quality : 2;
	bool				identify : 1;
	unsigned char		forsale : 1;
public:
	constexpr item() : type(NoItem), effect(NoEffect), count(0), magic(Mundane), quality(0), identify(0), forsale(0), damaged(0) {}
	constexpr item(spell_s spell) : type(Scroll1), effect((enchantment_s)spell), count(0), magic(), quality(0), identify(1), forsale(0), damaged(0) {}
	constexpr item(item_s type) : type(type), effect(NoEffect), count(0), magic(Mundane), quality(0), identify(0), forsale(0), damaged(0) {}
	constexpr item(item_s type, enchantment_s effect) : type(type), effect(effect), count(0), magic(Mundane), quality(0), identify(1), forsale(0), damaged(0) {}
	item(item_s type, int chance_artifact, int chance_magic, int chance_cursed, int chance_quality);
	explicit operator bool() const { return type != NoItem; }
	void				act(const char* format, ...) const;
	void				clear();
	bool				damageb();
	void				damage();
	void				get(attacki& e) const;
	item_s				getammo() const;
	int					getarmor() const;
	int					getbonus(enchantment_s type) const;
	int					getcharges() const;
	unsigned			getcost() const;
	int					getcount() const;
	int					getdefence() const;
	enchantment_s		geteffect() const;
	char				getenchantcost() const;
	skill_s				getfocus() const;
	const foodi&		getfood() const;
	static aref<item_s>	getitems(aref<item_s> result, aref<slot_s> source);
	gender_s			getgender() const;
	item_type_s			getmagic() const { return magic; }
	material_s			getmaterial() const;
	static const char*	getname(spell_s value);
	static const char*	getname(state_s value);
	char*				getname(char* result, const char* result_maximum, bool show_info = true) const;
	int					getquality() const;
	int					getqualityraw() const { return quality; }
	int					getsalecost() const;
	skill_s				getskill() const;
	spell_s				getspell() const;
	const speciali&		getspecial() const;
	state_s				getstate() const;
	item_s				gettype() const { return type; }
	int					getweight() const;
	int					getweightsingle() const;
	bool				is(slot_s value) const;
	bool				isarmor() const;
	bool				isartifact() const { return magic == Artifact; }
	bool				ischargeable() const;
	bool				iscountable() const;
	bool				iscursed() const { return magic == Cursed; }
	bool				isdamaged() const { return damaged != 0; }
	bool				isdrinkable() const;
	bool				isedible() const;
	bool				isforsale() const { return forsale != 0; }
	bool				isidentified() const { return identify; }
	bool				isnatural() const;
	bool				ismagical() const { return magic != Mundane; }
	bool				isreadable() const;
	bool				istome() const;
	bool				isthrown() const;
	bool				istwohanded() const;
	bool				isversatile() const;
	bool				isunbreakable() const;
	void				loot();
	void				repair(int level);
	item&				set(item_type_s value) { magic = value; return *this; }
	item&				set(enchantment_s value) { effect = value; return *this; }
	item&				setcharges(int count);
	item&				setcount(int count);
	item&				setforsale() { forsale = 1; return *this; }
	item&				setidentify(bool v) { identify = v; return *this; }
	item&				setsold() { forsale = 0;  return *this; }
	item&				setquality(unsigned char value) { quality = value; return *this; }
};
class posable {
	indext				index;
public:
	constexpr indext	getposition() const { return index; }
	constexpr void		setposition(indext v) { index = v; }
};
class nameable {
	short unsigned		name;
public:
	void				act(const char* format, ...) const;
	void				actv(stringbuilder& sb, const char* format, const char* param) const;
	void				actv(stringbuilder& sb, nameable& e, const char* format, const char* param) const;
	gender_s			getgender() const;
	const char*			getname() const;
};
class creature : public nameable, public posable {
	char				abilities[ManaRate + 1];
	short				abilities_raise[Charisma + 1];
	unsigned char		skills[LastResist + 1];
	unsigned char		spells[LastSpell + 1];
	item				wears[LastBackpack + 1];
	char				hp, mp;
	unsigned			restore_hits, restore_mana, restore_action;
	flagable<1 + LastState / 8> states;
	short unsigned		charmer, horror;
	short unsigned		location;
	encumbrance_s		encumbrance;
	race_s				race;
	class_s				type;
	role_s				role;
	unsigned short		name;
	unsigned char		level;
	unsigned short		guard;
	direction_s			direction;
	int					experience;
	unsigned			money;
	//
	bool				remove(item& it);
	void				updateitems();
public:
	creature() = default;
	creature(role_s value);
	creature(race_s race, gender_s gender, class_s type) { create(race, gender, type); }
	explicit operator bool() const { return hp > 0; }
	//
	static creature*	add(short unsigned index, role_s role);
	static creature*	add(short unsigned index, race_s race, gender_s gender, class_s type);
	void				add(variant id, int v, unsigned time);
	void				addexp(int count);
	static void			addexp(int value, short unsigned position, int range, const creature* exclude, const creature* enemies);
	static creature*	addplayer();
	bool				alertness();
	void				apply(aref<variant> features);
	void				apply(state_s state, item_type_s magic, int quality, unsigned duration, bool interactive);
	bool				askyn(creature* opponent, const char* format, ...);
	void				athletics(bool interactive);
	void				attack(creature* defender, slot_s slot, int bonus = 0, int multiplier = 0);
	bool				canhear(short unsigned index) const;
	void				chat(creature* opponent);
	item*				choose(aref<item*> source, bool interactive, const char* title = 0) const;
	creature*			choose(aref<creature*> source, bool interactive) const;
	short unsigned		choose(aref<short unsigned> source, bool interactive) const;
	void				create(race_s race, gender_s gender, class_s type);
	void				applyability();
	void				clear();
	void				consume(int value, bool interactive);
	void				damage(int count, attack_s type, bool interactive);
	void				damagewears(int count, attack_s type);
	void				drink(item& it, bool interactive);
	void				dropdown(item& value);
	bool				equip(item_s value);
	bool				equip(item value);
	void				equip(slot_s slot, item value);
	int					get(ability_s value) const;
	int					get(spell_s value) const;
	int					get(skill_s value) const;
	const item&			get(slot_s v) const { return wears[v]; }
	const creature&		getai() const;
	int					getarmor() const;
	attacki				getattack(slot_s slot) const;
	int					getattacktime(slot_s slot) const;
	int					getbasic(ability_s value) const;
	int					getbasic(skill_s value) const;
	int					getbonus(enchantment_s value) const;
	class_s				getclass() const { return type; }
	int					getcost(spell_s value) const;
	unsigned			getcostexp() const;
	int					getdefence() const;
	int					getdiscount(creature* customer) const;
	direction_s			getdirection() const { return direction; }
	creature*			getenemy(aref<creature*> source) const;
	encumbrance_s		getencumbrance() const { return encumbrance; }
	int					getexperience() const { return experience; }
	char*				getfullname(stringbuilder& sb, bool show_level, bool show_alignment) const;
	short unsigned		getguard() const { return guard; }
	int					gethits() const { return hp; }
	creature*			gethorror() const { return getobject(horror); }
	short unsigned		getid() const;
	creature*			getleader() const;
	int					getlos() const;
	int					getlos(unsigned flags) const;
	int					getmana() const { return mp; }
	int					getmaxhits() const;
	int					getmaxmana() const;
	int					getmoney() const { return money; }
	const char*			getmonstername() const;
	int					getmoverecoil() const;
	static creature*	getobject(short unsigned v);
	static creature*	getplayer();
	static creature*	getplayer(int index);
	race_s				getrace() const { return race; }
	damagei				getraise(skill_s id) const;
	role_s				getrole() const { return role; }
	int					getweight() const;
	int					getweight(encumbrance_s id) const;
	bool				give(creature& opponent, item& it, bool interactive);
	void				heal(int value, bool interactive) { damage(-value, Magic, interactive); }
	void				hint(const char* format, ...) const;
	static void			initialize();
	bool				interact(short unsigned index);
	bool				is(state_s value) const;
	bool				is(encumbrance_s value) const { return encumbrance == value; }
	bool				isagressive() const;
	bool				ischaracter() const { return role == Character; }
	bool				isenemy(const creature* target) const;
	bool				isfriend(const creature* target) const;
	bool				isguard() const { return guard != 0xFFFF; }
	bool				isinteractive() const;
	bool				isparty(const creature* target) const;
	bool				isplayer() const;
	bool				isranged(bool interactive) const;
	void				levelup(bool interactive);
	void				lookfloor();
	void				makemove();
	void				manipulate(short unsigned index);
	void				meleeattack(creature* target, int bonus = 0, int multiplier = 0);
	bool				move(short unsigned index);
	bool				moveto(short unsigned index);
	bool				moveaway(short unsigned index);
	static void			play();
	void				pickup(item& value, bool interactive = true);
	void				post(ability_s i, int value, unsigned rounds);
	void				raise(skill_s value);
	void				raiseskills(int number);
	void				rangeattack(creature* enemy);
	void				readbook(item& it);
	void				release() const;
	void				remove(state_s value);
	bool				roll(skill_s skill, int bonus = 0) const;
	int					roll(skill_s skill, int bonus, const creature& opponent, skill_s opponent_skill, int opponent_bonus) const;
	void				say(const char* format, ...);
	bool				sayv(const char* format, const char* param, creature* opponent);
	void				sayvs(creature& opponent, const char* format, ...);
	bool				saving(bool interactive, skill_s save, int bonus) const;
	static void			select(creature** result, rect rc);
	static aref<role_s>	select(aref<role_s> result, int min_level, int max_level, alignment_s alignment, const race_s races[4]);
	void				set(state_s value, unsigned segments);
	void				set(spell_s value, int level);
	void				setcharmer(const creature* p) { charmer = p->getid(); }
	static void			setblocks(short unsigned* movements, short unsigned value);
	void				setguard(short unsigned value) { guard = value; }
	void				sethorror(const creature* p) { horror = p->getid(); }
	void				setlos();
	void				setplayer();
	void				setmoney(int value) { money = value; }
	void				trapeffect();
	void				update();
	bool				use(short unsigned index);
	void				wait(int segments = 0);
};
class creaturea : adat<creature*> {
public:
	void				add(const creature* e);
	creature*			choose(bool interactive, const char* title);
};
class itema : adat<item*> {
public:
	item*				choose(bool interactive, const char* title);
};
class indexa : adat<short unsigned> {
public:
	short unsigned		choose(bool interactive, const char* title);
};
struct site : rect {
	site_s				type;
	unsigned char		name[2];
	diety_s				diety;
	short unsigned		owner;
	constexpr site() : rect({0, 0, 0, 0}), type(EmpthyRoom), diety(NoGod), name(), owner(),
		found(0), recoil(0) {
	}
	operator bool() const { return x1 != x2; }
	void				entering(creature& player);
	int					getfoundchance() const;
	char*				getname(char* result) const;
	short unsigned		getposition() const;
	void				update();
private:
	unsigned char		found;
	unsigned			recoil;
	void				wait(unsigned count);
};
struct grounditem : item {
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
struct dungeon {
	struct layer {
		dungeon_area_s	type;
		unsigned char	count;
	};
	const char*			name; // name of area location
	short unsigned		world_index; // overland index
	unsigned char		icon; // overland icon overlay
	unsigned char		level; // Start level: 0 form city, 1 for undeground dungeon
	adat<layer, 8>		layers;
};
struct manual {
	typedef void(*proc)(stringbuilder& sc, manual& e);
	manual_s			type;
	variant				value;
	const char*			text;
	manual*				child;
	aref<proc>			procs;
	explicit operator bool() const { return value.type != 0; }
};
class location {
	tile_s				tiles[mmx*mmy];
	unsigned char		random[mmx*mmy];
public:
	void				clear();
	void				editor();
	void				fill(rect rc, tile_s v);
	indext				get(short x, short y) const { return y*mmx + x; }
	static short		getx(indext i) { return i%mmx; }
	static short		gety(indext i) { return i / mmx; }
	int					getindex(indext i, tile_s e) const;
	tile_s				gettile(indext i) const { return tiles[i]; }
	int					getrand(indext i) const { return random[i]; }
	bool				read(const char* url);
	void				set(indext i, tile_s v) { tiles[i] = v; }
	static indext		to(indext index, direction_s id);
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
public:
	void				intialize();
	void				pass(unsigned seconds);
};
namespace draw {
const int				gui_border = 8;
const int				gui_padding = 4;
void					breakmodal(int param);
int						button(int x, int y, const char* format, const char* key, eventproc proc);
int						detail(int x, int y, int width, const char* format);
int						detail(int x, int y, int width, const char* format, int width_right, const char* text_value);
int						detail(int x, int y, int width, const char* format, int width_right, int v1);
int						detail(int x, int y, int width, const char* format, int width_right, int v1, int v2);
point					getcamera();
int						getheight();
int						getwidth();
int						header(int x, int y, int width, const char* format);
int						headof(int& x, int y, int& width, const char* format);
void					layer();
void					render(aref<picture> source);
void					setbackground(eventproc proc);
void					setnextlayer(eventproc proc);
bool					shortcuts(const hotkey* ph);
int						texth();
indext					translate(indext i);
int						widget(eventproc before, eventproc after);
void					window(rect rc, bool disabled = false, int border = 0);
}
extern gamei			game;
DECLENUM(tile);
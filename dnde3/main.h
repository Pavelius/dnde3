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
const int chance_door_closed = 10;
const int chance_generate_room = 40;
const int chance_special_area = 5;
const int dense_forest = 5;
const int max_building_size = 15;

const int chance_act = 40;
const int chance_blood_when_dead = 70;
const int chance_drop_item = 25;
const int chance_scarry_cry = 20;
const int chance_turn_chargeable_to_dust = 70;
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
	AlchemyPotion, Potion1, Potion2, Potion3, Potion4, Potion5,
	RingRed, RingBlue, RingGreen,
	Amulet1, Amulet2, Amulet3, Amulet4, Amulet5,
	ClimbingTool, FletcherySet, Forge, HealingKit, ScriblingKit, CrystalBall, AlchemySet, TheifTool, CookingSet,
	Lute, Arfa, Baboon, Guitar, Vilanchelle, Flute,
	Corpse, DoorKey, Coin, CoinSP, CoinGP,
	Claws, Slam, Bite, Hitin, Fur,
	ManyItems
};
enum diety_s : unsigned char {
	NoGod,
	GodBane, GodBhaal, GodGruumsh, GodHelm, GodMistra, GodTempus, GodTyr
};
enum slot_s : unsigned char {
	Backpack, Edible, Readable, Drinkable, Zapable, Coinable, LastBackpack = Backpack + 31,
	Head, Neck, Melee, OffHand, TorsoBack, Torso, RightFinger, LeftFinger, Elbows, Legs, Ranged, Amunitions, Tool,
	LastWear = Tool,
};
enum race_s : unsigned char {
	Human, Dwarf, Elf, Halfling,
	Goblin, Kobold, Ogre, Orc, Gnoll,
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
	GoblinRockthrowerWarrior, GoblinRockthrowerWarriorF,
	OrcWarrio2r, OrcWarrior2F,
	LargeBat2, LargeBat3,
	Bee, Bee2, Bee3,
	OrgeCommoner, OrgeCommonerF,
	Bugbear, BugbearF,
	Character
};
enum alignment_s : unsigned char {
	Lawfull, Neutral, Chaotic
};
enum ability_s : unsigned char {
	Strenght, Dexterity, Constitution, Intellegence, Wisdow, Charisma,
	Attack, Damage,
	Pierce, Protection, Armor, Deflect, Speed, Movement, Visibility,
	ResistAcid, ResistCharm, ResistCold, ResistElectricity,
	ResistFire, ResistParalize, ResistPoison, ResistWater,
	FirstResist = ResistAcid, LastResist = ResistWater,
	Level, LifePoints, LifeRate, ManaPoints, ManaRate,
};
enum skill_s : unsigned char {
	Bargaining, Bluff, Diplomacy,
	Acrobatics, Alertness, Athletics, Backstabbing, Climbing, Concetration,
	DisarmTraps, FindWeakness, HearNoises, HideInShadow, Lockpicking, MoveSilently, PickPockets,
	Alchemy, Dancing, Engineering, Gambling, History, Healing, Herbalism,
	Literacy, Mining, Riding, Smithing, Survival, Swimming,
	FocusBows, FocusSwords, FocusAxes, FocusTwohanded,
	TwoWeaponFighting,
	FirstSkill = Bargaining, LastSkill = TwoWeaponFighting,
};
enum state_s : unsigned char {
	Darkvision, Dazzled, Drunken, Fear, Friendly, Hostile,
	Invisible, Poisoned, Sick, Summoned,
	Unaware, Wounded,
	LastState = Wounded,
};
enum tile_s : unsigned char {
	Plain, Water, Floor, Wall, Road,
	Swamp, Hill,
	Sea, Foothills, Mountains, CloudPeaks, Forest, Lake,
	City
};
enum landscape_s : unsigned char {
	AreaPlain, AreaForest, AreaSwamp,
	AreaDungeon, AreaCity,
};
enum room_s : unsigned char {
	EmpthyRoom, TreasureRoom,
	StairsDownRoom, StairsUpRoom, House, Lair,
	Temple, Tavern, Barracs, CityHall,
	ShopWeaponAndArmor, ShopPotions, ShopScrolls,
	ShopFood, // Last magazine
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
	ResTraps,
	ResSea, ResPlains, ResFoothills, ResMountains, ResCloudPeaks, ResDecals,
	ResUI,
	ResPCmar, ResPCmbd, ResPCmac
};
enum spell_s : unsigned char {
	ArmorSpell, BlessSpell, BlessItem, ChatPerson, CharmPerson, DetectEvil, DetectMagic, FearSpell, HealingSpell,
	Identify, Invisibility, KnockDoor, LightSpell, MagicMissile, PoisonSpell,
	Repair, RemovePoisonSpell, RemoveSickSpell,
	SickSpell, ShieldSpell, ShokingGrasp, Sleep, SlowMonster,
	FirstSpell = ArmorSpell, LastSpell = SlowMonster
};
enum map_flag_s : unsigned char {
	Visible, Hidden, Opened, Sealed, Explored, Webbed, Blooded,
};
enum item_type_s : unsigned char {
	Mundane, Cursed, Blessed, Artifact,
};
enum damage_s : unsigned char {
	Bludgeon, Slashing, Piercing,
	Acid, Cold, Electricity, Fire, Magic, WaterAttack
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
enum item_flag_s : unsigned char {
	SingleUse, TwoHanded, Versatile, Light, Natural,
};
enum variant_s : unsigned char {
	NoVariant,
	Ability, Alignment, Command, Creature, Formula, Gender, God, Harm,
	Item, ItemIdentify, ItemType,
	Number, Object, ObjectFlags, Outdoor, Race, Range, Rarity, Role, Room,
	Skill, Slot, Spell, State, Target, Tile,
	Variant,
};
enum outdoor_s : unsigned char{
	VillageTaho, CityMeher, LostMine,
	LastOutdoor = LostMine,
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
enum sale_s : unsigned char {
	NotForSale, Sale75, Sale100, Sale150,
};
enum target_flag_s : unsigned char {
	NotYou, Friends, Enemies, AlwaysChoose,
	RandomTargets, TwoTargets, ThreeTargets,
	AllTargets,
};
enum rarity_s : unsigned char {
	Common, Uncommon, Rare, VeryRare, Unique,
};
enum dialog_s : unsigned char {
	Say, Ask
};
enum intellegence_s : unsigned char {
	NoInt, AnimalInt, SemiInt, LowInt, AveInt, VeryInt, HighInt, ExpInt, GenInt, SupGenInt, GodInt
};
enum map_object_flag_s : unsigned char {
	BlockMovement, BlockLight,
};
enum command_s : unsigned char {
	AddReputation, LoseReputation, BadReputation, GoodReputation,
	AddMoney10, AddMoney20, AddMoney50, LoseMoney10, LoseMoney20, LoseMoney50,
	LastCommand = LoseMoney50,
};
struct dungeoni;
struct targeti;
struct landscapei;
class creature;
class creaturea;
typedef short unsigned indext;
typedef adat<rect, 64> rooma;
typedef flagable<1 + Chaotic / 8> alignmenta;
typedef flagable<1 + LastState / 8> statea;
typedef flagable<1 + LastRace / 8> racea;
typedef flagable<1 + ManyItems / 8>	itemf;
typedef flagable<1 + Blooded / 8> mapflf;
typedef cflags<map_object_flag_s> mapobjf;
typedef casev<ability_s> abilityv;
typedef aset<damage_s, 1 + WaterAttack> damagea;
typedef void(*gentileproc)(indext index);
typedef indext(*getposproc)(direction_s i);
typedef void(*genareaproc)(const rect& rc, rooma& rooms, const landscapei& landscape, bool visualize);
struct variant {
	variant_s			type;
	unsigned char		value;
	constexpr variant() : type(NoVariant), value(0) {}
	constexpr variant(ability_s v) : type(Ability), value(v) {}
	constexpr variant(alignment_s v) : type(Alignment), value(v) {}
	constexpr variant(command_s v) : type(Command), value(v) {}
	constexpr variant(damage_s v) : type(Harm), value(v) {}
	constexpr variant(diety_s v) : type(God), value(v) {}
	constexpr variant(formula_s v) : type(Formula), value(v) {}
	constexpr variant(gender_s v) : type(Gender), value(v) {}
	constexpr variant(identify_s v) : type(ItemIdentify), value(v) {}
	constexpr variant(item_s v) : type(Item), value(v) {}
	constexpr variant(item_type_s v) : type(ItemType), value(v) {}
	constexpr variant(map_object_s v) : type(Object), value(v) {}
	constexpr variant(map_flag_s v) : type(ObjectFlags), value(v) {}
	constexpr variant(range_s v) : type(Range), value(v) {}
	constexpr variant(race_s v) : type(Race), value(v) {}
	constexpr variant(rarity_s v) : type(Rarity), value(v) {}
	constexpr variant(role_s v) : type(Role), value(v) {}
	constexpr variant(room_s v) : type(Room), value(v) {}
	constexpr variant(skill_s v) : type(Skill), value(v) {}
	constexpr variant(slot_s v) : type(Slot), value(v) {}
	constexpr variant(spell_s v) : type(Spell), value(v) {}
	constexpr variant(state_s v) : type(State), value(v) {}
	constexpr variant(tile_s v) : type(Tile), value(v) {}
	constexpr variant(variant_s v) : type(Variant), value(v) {}
	constexpr variant(int v) : type(Number), value(v) {}
	variant(const creature* v);
	explicit operator bool() const { return type != NoVariant; }
	constexpr bool operator==(const variant& e) const { return type == e.type && value == e.value; }
	constexpr bool operator!=(const variant& e) const { return type != e.type || value != e.value; }
	const char*			getname() const;
	const char*			getnameof() const;
	const char*			getnameofc() const;
};
typedef variant			varianta[12];
typedef adat<casev<variant>, 8> chancev;
struct deck : adat<unsigned short> {
	void				drop(short unsigned v);
	short unsigned		take();
};
struct string : stringbuilder {
	const char			*name, *opponent_name;
	gender_s			gender, opponent_gender;
	creature			*player, *opponent;
	constexpr string(const stringbuilder& source) : stringbuilder(source),
		name(0), opponent_name(0),
		player(0), opponent(0),
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
	short unsigned		owner_id;
	variant				source;
	variant				id;
	char				modifier;
	unsigned			time;
};
struct leveli {
	class_s				type;
	char				level;
	casev<variant>		features[16];
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
	itemf				restricted;
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
	int					cost;
	//
	ability_s			getid() const;
	int					getbonus(int v) const;
};
struct equipmenti {
	race_s				race;
	class_s				type;
	varianta			features;;
};
struct map_objecti {
	const char*			id;
	const char*			name;
	mapobjf				flags;
	short unsigned		start, count;
};
struct spritei {
	img_s				image;
	const char*			id;
	const char*			name;
	short				frame;
	static void			initialize();
};
struct picture : point {
	img_s				img;
	unsigned short		frame;
	unsigned short		flags;
	unsigned char		alpha;
	unsigned char		level;
	variant				object;
	explicit operator bool() const { return img != ResNone; }
	void				clear() { memset(this, 0, sizeof(*this)); alpha = 0xFF; }
	void				render(int x, int y) const;
	void				set(indext i);
	void				set(short x, short y);
	void				setcursor(indext i, int size);
};
struct rarityi {
	const char*			name;
	char				weight;
	char				chance;
};
struct statei {
	const char*			name;
	const char*			nameof;
	statea				flags;
	const char*			text_set;
	const char*			text_remove;
};
struct adjectivei {
	const char*			name;
	const char*			name_male;
	const char*			name_female;
	const char*			get(gender_s v) const;
};
struct objectivei {
	const char*			name;
	gender_s			gender;
};
struct racei {
	const char*			name;
	char				abilities[6];
	skill_s				skills[3];
	adat<abilityv, 8>	abilityvs;
	statea				states;
};
struct dicei {
	char				min;
	char				max;
	int					roll() const;
	void				normalize();
};
struct costi {
	char				mundane;
	char				artifact;
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
	damage_s			type;
	char				speed;
	char				damage_bonus;
	item_s				ammunition;
	item_s				ammunition_compatible;
	int					getenergy() const { return StandartEnergyCost - speed * 50; }
};
struct materiali {
	const char*			name;
	const char*			genders[3];
	damagea				resist;
};
struct damagei {
	const char*			name;
	ability_s			resist;
	char				damage_wears;
	char				damage_chance;
	const char*			resist_text;
};
struct pooli {
	const char*			name;
	const char*			avatar_id;
};
struct trapi {
	const char*			name;
	const char*			avatar_id;
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
struct descriptioni {
	variant				v1, v2;
	const char*			text;
	const char*			get(variant v1, variant v2) const;
};
struct itemi {
	const char*			name;
	const char*			avatar_id;
	rarity_s			rarity;
	int					weight;
	int					cost;
	int					quality;
	gender_s			gender;
	material_s			material;
	attacki				weapon;
	armori				armor;
	aref<variant>		effects;
	cflags<item_flag_s>	flags;
	slot_s				slot;
	variant				skill;
	//
	bool				is(slot_s v) const;
	bool				is(const aref<slot_s>& source) const;
	variant				randeffect() const;
};
class item {
	union {
		struct {
			item_s		type;
			//
			unsigned char identifyc : 1;
			unsigned char identifys : 1;
			item_type_s	magic : 2;
			unsigned char quality : 2;
			sale_s		sale : 2;
			//
			unsigned char personal : 1;
			unsigned char identifye : 1;
			unsigned char damaged : 2;
			unsigned char charge : 4;
			//
			unsigned char effect;
		};
		short unsigned	us[2];
		int				i;
	};
	bool				use(skill_s id, creature& player, int order, bool run);
	bool				use(spell_s id, creature& player, int level, int order, bool run);
public:
	item() = default;
	item(item_s type, int level);
	explicit operator bool() const { return type != NoItem; }
	void				act(const char* format, ...) const;
	void				actv(stringbuilder& st, const char* format, const char* format_param) const;
	bool				apply(creature& player, variant id, int v, int order, bool run);
	void				clear() { memset(this, 0, sizeof(*this)); }
	void				create(item_s type, int chance_artifact, int chance_magic, int chance_cursed, int chance_quality);
	void				breaktest();
	void				damage(int count, damage_s type, bool interactive);
	void				decoy(damage_s type, bool interactive, bool include_artifact = false);
	void				destroy(damage_s type, bool interactive);
	item_s				getammo() const { return getitem().weapon.ammunition; }
	armori				getarmor() const;
	attacki				getattack() const;
	int					getbonus() const;
	int					getcharges() const { return charge; }
	unsigned			getcost() const;
	int					getcount() const;
	int					getdamage() const;
	const char*			getdamagetext() const;
	variant				geteffect() const;
	const itemi&		getitem() const { return bsmeta<itemi>::elements[type]; }
	gender_s			getgender() const { return getitem().gender; }
	item_s				getkind() const { return type; }
	item_type_s			getmagic() const { return magic; }
	material_s			getmaterial() const { return getitem().material; }
	const char*			getname() const { return getitem().name; }
	void				getname(stringbuilder& sb, bool show_cab) const;
	indext				getposition() const;
	int					getquality() const { return quality; }
	static const aref<variant> getreceipts();
	void				getstatistic(stringbuilder& sb) const;
	creature*			getwearer() const;
	slot_s				getwearerslot() const;
	int					getweightsingle() const { return getitem().weight; }
	int					getweight() const { return getweightsingle()*getcount(); }
	bool				is(slot_s v) const { return getitem().is(v); }
	bool				is(identify_s v) const;
	bool				is(item_flag_s v) const { return getitem().flags.is(v); }
	bool				is(item_type_s v) const { return magic == v; }
	bool				is(sale_s v) const { return sale == v; }
	bool				isboost(variant id) const;
	bool				ischargeable() const;
	bool				iscountable() const;
	bool				isdamaged() const { return getdamage() > 0; }
	void				loot();
	bool				ismatch(variant v) const;
	void				set(item_type_s v);
	void				set(identify_s v);
	void				set(sale_s v) { sale = v; }
	void				setcount(int v);
	void				seteffect(variant v);
	void				setquality(int v);
	bool				stack(item& v);
	void				use();
};
class itema : public adat<item*> {
public:
	item*				choose(const char* interactive, const char* format, slot_mode_s mode, bool show_always = false, bool cancel = true);
	void				footer(stringbuilder& sb) const;
	void				match(variant v, bool remove);
	void				match(item& v, bool remove);
	void				matcha(creature& player, variant id, int v);
	void				matchboost(variant v);
	void				select(creature& e);
	void				select(indext index);
	void				selecta(creature& e);
	void				selectb(creature& e);
};
class variantc : public adat<casev<variant>> {
	void				add(variant v);
	void				add(variant v, rarity_s r);
public:
	void				additems(const aref<slot_s>& source);
	int					getweight() const;
	bool				is(variant v) const;
	void				match(slot_s v, bool remove);
	void				match(item_flag_s v, bool remove);
	void				matchp(int count, bool greater);
	variant				random() const;
	variant				randomw() const;
};
class skilla :public adat<skill_s, 64> {
public:
	bool				choose(bool interactive, const char* title, const char* format, skill_s& result) const;
	void				removent();
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
	int					getcap(skill_s i, creature& player) const;
	void				setcap(skill_s i, int v) { cap[i] = v; }
	void				setcaps();
};
struct dialogi {
	char				index;
	dialog_s			type;
	varianta			conditions;
	const char*			text;
	varianta			actions;
	int					next;
	explicit operator bool() const { return text != 0; }
};
struct roomi {
	const char*			name;
	const char*			title;
	map_object_s		heart;
};
class posable {
	indext				index;
public:
	constexpr explicit operator bool() const { return index != Blocked; }
	void				clear() { index = Blocked; }
	constexpr indext	getposition() const { return index; }
	constexpr void		setposition(indext v) { index = v; }
};
class geoposable : public posable {
	unsigned short		level;
public:
	constexpr int		getlevel() const { return level; }
	bool				isoverland() const { return level == 0; }
	constexpr void		setlevel(int v) { level = v; }
};
class nameable : public variant, public posable {
	short unsigned		name[2];
public:
	void				act(const char* format, ...) const;
	void				actv(stringbuilder& sb, const char* format, const char* param) const;
	void				actv(stringbuilder& sb, nameable& e, const char* format, const char* param) const;
	bool				askv(stringbuilder& st, const nameable& e, const char* format, const char* param) const;
	bool				cansee() const;
	gender_s			getgender() const;
	const char*			getname() const;
	void				getname(stringbuilder& sb) const;
	void				randomname();
	void				sayv(stringbuilder& st, const char* format, const char* param) const;
	void				setname(race_s race, gender_s gender);
	race_s				getrace() const;
	bool				isactive() const;
	bool				ischaracter() const { return value == Character; }
};
class site : public nameable, public rect {
	variant				param;
	short unsigned		owner_id;
	unsigned char		found;
	unsigned			recoil;
public:
	explicit operator bool() { return x2 > x1 && y2 > y1; }
	void				clear();
	static site*		find(indext index);
	room_s				getkind() const { return (room_s)value; }
	creature*			getowner() const;
	variant				getparam() const { return param; }
	creature*			priest();
	void				set(const rect& v) { *static_cast<rect*>(this) = v; }
	void				set(room_s v) { type = Room; value = v; }
	void				setowner(const creature* v);
	void				setparam(variant v) { param = v; }
	creature*			shopkeeper();
	static void			unlink(const creature& player);
};
struct rolei {
	const char*			name;
	const char*			avatar_id;
	race_s				race;
	gender_s			gender;
	alignment_s			alignment;
	class_s				type;
	char				level;
	varianta			features;
	role_s				minions[4];
	int					getcr() const;
};
struct foodi {
	item_s				type;
	variant				condition;
	char				hp, mp, chance;
	variant				effect;
	char				chance_poison;
	char				chance_sick;
	//
	bool				match(const creature* player, const item it) const;
};
struct effecti {
	variant				id;
	int					value;
	const char*			text;
	bool				permanent;
};
class paperdoll {
	unsigned char		hair, face_hair, eyes;
	unsigned char		skin;
public:
	constexpr paperdoll() : skin(0), hair(0), face_hair(0), eyes(0) {}
	int					getskin() const { return skin; }
	int					gethair() const { return hair; }
	int					gethairf() const { return face_hair; }
	void				setskin(int v) { skin = v; }
	void				sethair(int v) { hair = v; }
	void				sethairf(int v) { face_hair = v; }
};
class creature : public nameable, public paperdoll {
	char				abilities[ManaRate + 1];
	unsigned char		skills[LastSkill + 1];
	unsigned char		skills_potency[LastSkill + 1];
	unsigned char		spells[LastSpell + 1];
	item				wears[LastWear + 1];
	int					restore_energy, restore_hits, restore_mana;
	flagable<4>			recipes;
	char				hp, mp, poison;
	statea				states;
	short unsigned		location_id, site_id;
	class_s				kind;
	indext				guard;
	direction_s			direction;
	unsigned			experience;
	unsigned			money;
	//
	void				add(skill_s id, int v, bool interactive);
	void				add(spell_s id, unsigned minutes);
	void				add(ability_s id, variant source, int v, bool interactive, unsigned minutes);
	bool				aiuse(const creaturea& creatures, const char* interactive, slot_s slot, variant effect);
	void				aimove();
	void				aioverland() {}
	bool				aiskills(creaturea& creatures);
	bool				aispells(creaturea& creatures);
	void				aiturn(creaturea& creatures, creaturea& enemies, creature* enemy);
	void				applyabilities();
	void				applyaward() const;
	void				attack(creature& enemy, const attacki& ai, int bonus, int multiplier);
	const variant*		calculate(const variant* formula, int& result) const;
	void				cantmovehere() const;
	bool				cantakeoff(slot_s id, bool interactive);
	bool				canuse(const item& e, bool talk) const;
	void				dress(int m);
	void				dresssk(int m);
	void				dresssa(int m);
	void				dropdown(item& item);
	void				dropitems();
	void				equip(item it, slot_s id);
	void				finish();
	void				movecost(indext index);
	void				raiseability();
	void				raiselevel();
	void				randomequip();
	void				recall(variant id, variant source, int modifier, unsigned rounds);
	bool				remove(item& it, bool run, bool talk);
	bool				use(skill_s id, creature& player, int order, bool run);
	bool				use(spell_s id, creature& player, int level, int order, bool run);
	void				use(const foodi& ei, const item it, bool interactive);
	void				usestealth();
	void				usetrap();
public:
	explicit operator bool() const { return hp > 0; }
	//
	void				activate();
	void				add(ability_s id, int v, bool interactive);
	void				add(spell_s id, int v, bool interactive);
	void				add(state_s id, int v, bool interactive);
	void				add(variant id, int v, bool interactive);
	bool				add(item v, bool run, bool interactive);
	void				add(const effecti& id, item_s source);
	void				addexp(int count, bool interactive = false);
	void				appear();
	bool				apply(creature& target, variant id, int v, int order, bool run);
	bool				ask(const nameable& opponent, const char* format, ...) const;
	bool				askyn(const char* format, ...) const;
	static bool			askyn();
	void				backpack();
	void				bloodstain() const;
	int					calculate(const variant* formule) const;
	bool				canhear(short unsigned index) const;
	bool				canleave(direction_s v) const;
	bool				cansee(indext i) const;
	bool				canshoot() const;
	void				chat();
	void				chat(creature& opponent);
	void				chat(creature& opponent, const dialogi* source);
	bool				charmresist(int bonus = 0) const;
	void				checkpoison();
	void				checksick();
	variant				choosereceipt(const char* interactive) const;
	void				create(race_s race, gender_s gender, class_s type);
	void				create(role_s type);
	void				clear();
	void				consume(int energy_value);
	void				closedoor();
	void				damage(int count, damage_s type, int pierce = 0, bool interactive = true);
	void				damagewears(int count, damage_s type, int item_count = 1);
	void				dispell(bool interactive);
	void				dispell(variant source, bool interactive);
	void				dressoff();
	void				dresson();
	void				drink();
	void				dropdown();
	void				eat();
	bool				equip(item value);
	bool				equip(item& v1, item& v2, bool run);
	void				enslave();
	void				fail(skill_s id);
	void				feel(ability_s id, bool raise);
	static creature*	find(indext i);
	boosti*				find(variant id) const;
	boosti*				finds(variant id) const;
	item*				finditem(item_s v);
	int					get(ability_s v) const { return abilities[v]; }
	int					get(spell_s v) const { return spells[v]; }
	int					get(skill_s v) const;
	const item&			get(slot_s v) const { return wears[v]; }
	static creature*	getactive();
	static creature*	getactive(int index);
	attacki				getattack(slot_s slot, const item& weapon) const;
	attacki				getattack(slot_s slot) const { return getattack(slot, wears[slot]); }
	int					getaward() const { return 10 + 15 * get(Level); }
	int					getbasic(skill_s v) const { return skills[v]; }
	int					getboost(variant id) const;
	const classi&		getclass() const { return bsmeta<classi>::elements[kind]; }
	direction_s			getdirection() const { return direction; }
	int					getexperience() const { return experience; }
	void				getfullname(stringbuilder& sb) const;
	short unsigned		getguard() const { return guard; }
	int					gethits() const { return hp; }
	short unsigned		getid() const;
	intellegence_s		getint() const;
	creature*			getleader() const;
	int					getlevel(skill_s v) const;
	unsigned			getlevelup() const;
	const char*			getlevelname(skill_s v) const;
	int					getlos() const;
	int					getmana() const { return mp; }
	int					getmoney() const { return money; }
	static creature*	getobject(short unsigned v);
	int					getpoisondamage() const { return 1 + poison / 5; }
	dicei				getraise(skill_s id) const;
	role_s				getrole() const { return (role_s)value; }
	site*				getsite() const;
	slot_s				getwearerslot(const item* p) const;
	int					getweight() const;
	void				heal(int value) { damage(-value, Magic); }
	void				inventory();
	bool				is(class_s v) const { return kind == v; }
	bool				is(intellegence_s v) const;
	bool				is(gender_s v) const { return getgender() == v; }
	bool				is(race_s v) const { return getrace() == v; }
	bool				is(state_s v) const { return states.is(v); }
	bool				is(spell_s v) const { return finds(v) != 0; }
	bool				is(const creature* p) const { return this == p; }
	bool				isallow(item_s v) const;
	bool				isbusy() const { return restore_energy <= -(StandartEnergyCost * 4) && is(Unaware); }
	bool				isenemy(const creature* target) const;
	bool				isguard() const { return guard != Blocked; }
	bool				ismatch(variant v) const;
	bool				ismatch(const creature& opponent, variant id) const;
	bool				ismatch(const creature& opponent, skill_s id, int value) const;
	bool				ismatch(const creature& opponent, const varianta& source) const;
	const char*			isusedisable(skill_s id) const;
	void				kill();
	bool				knownreceipt(variant id) const;
	void				learnreceipt(variant id);
	bool				leaving(direction_s v);
	void				look(indext index);
	void				lookaround();
	void				makemove();
	bool				match(variant id) const;
	void				meleeattack(creature& enemy, int bonus = 0);
	void				minimap();
	void				move(indext index);
	void				moveto(indext index);
	void				moveaway(indext index);
	bool				needrestore(ability_s id) const;
	static void			pause();
	void				paymana(int value, bool interactive);
	void				playuioverland();
	void				playui();
	void				pickup();
	void				potion(ability_s id, variant source, bool interactive, item_type_s magic, int quality, int minutes);
	void				quitandsave();
	void				raise(skill_s value);
	void				raiseskills(int number);
	void				raiseskills() { raiseskills(get(Intellegence) / 2); }
	void				rangeattack(creature& enemy, int bonus = 0);
	static void			restore(const creature* sp, const creature* spe, const boosti* sb, const boosti* sbe);
	void				readsomething();
	void				restoration();
	bool				roll(ability_s v) const { return rollv(get(v)); }
	bool				roll(ability_s v, int bonus) const { return rollv(get(v) + bonus); }
	bool				roll(skill_s v) const { return rollv(get(v)); }
	bool				roll(skill_s v, int bonus) const { return rollv(get(v) + bonus); }
	bool				roll(skill_s v, int bonus, int divider) const;
	static bool			rollv(int v);
	static int			rollv(int v1, int v2);
	void				say(const char* format, ...) const;
	bool				saybusy();
	void				select(itema& a, slot_s i1, slot_s i2, bool filled_only);
	void				select(skilla& e) const;
	void				set(ability_s id, int v);
	void				set(skill_s id, int v);
	void				set(const site* v);
	void				setguard(short unsigned value) { guard = value; }
	void				setmoney(int value) { money = value; }
	void				shoot();
	static void			store(creature* sp, const creature* spe, boosti* sb, const boosti* sbe);
	void				testweapons();
	void				unlink();
	bool				use(const creaturea& source, skill_s id);
	bool				use(const creaturea& creatures, spell_s id, int level, item* magic_source, bool show_errors);
	bool				use(const creaturea& creatures, item& it);
	bool				use(spell_s id, int level, item* magic_source = 0, bool show_errors = false);
	void				useskills();
	void				usespells();
	void				usetools();
	void				usewands();
	void				wait() { consume(StandartEnergyCost); }
	void				wait(int n) { consume(StandartEnergyCost * n); }
	void				waitturn() { wait(10*3); }
	void				zoomon();
};
class creaturea : public adat<creature*> {
public:
	creaturea() = default;
	creaturea(const creature& v) { select(v.getposition(), v.getlos()); }
	creature*			choose(const char* interactive);
	void				match(variant v, bool remove = false);
	void				match(creature& player, variant v, bool remove, bool target_insivible = false);
	void				matcha(creature& player, variant id, int v, bool remove = false);
	void				matchr(indext index, int range);
	void				matchbs(bool remove);
	void				select();
	void				select(indext start, int distance);
	void				select(state_s v);
	void				sort(indext start);
};
struct landscapei {
	const char*			name;
	char				border;
	tile_s				tile;
	casev<variant>		tiles[8];
	genareaproc			genarea;
	genareaproc			genroom;
	getposproc			getstart;
};
struct dungeoni {
	struct itemc {
		char			quality;
		char			cursed;
		char			magical;
	};
	landscape_s			type;
	short				level;
	char				light_level;
	room_s				rooms[4];
	racea				denyrace;
	itemc				items;
	explicit constexpr operator bool() const { return level != 0; }
	const dungeoni*		find(int level) const;
	constexpr bool		isdungeon() const { return type == AreaDungeon; }
};
class indexa : public adat<indext> {
public:
	int					choose(const char* interactive);
	void				match(variant v, bool remove);
	void				matcha(creature& player, variant id, int v);
	void				matchr(indext index, int range);
	void				select(indext index, int distance);
	void				sort(indext start);
};
class spella : public adat<spell_s, LastSpell + 1> {
public:
	spell_s				choose(const char* interactive, const char* title, bool* cancel_result, const creature* player) const;
	void				match(target_flag_s v, bool remove);
	void				select(const creature& player);
};
struct targeti {
	variant_s			type;
	cflags<target_flag_s> flags;
	range_s				range;
	const char*			interactive;
	explicit constexpr operator bool() const { return type != NoVariant; }
	unsigned			getcount(creaturea& creatures, itema& items, indexa& indecies) const;
	constexpr bool		is(target_flag_s v) const { return flags.is(v); }
	bool				prepare(creature& player, creaturea& creatures, itema& items, indexa& indecies, variant id, int v, bool show_errors = false) const;
	bool				use(creature& player, const creaturea& source, variant id, int v, bool show_erros = false) const;
	void				use(creature& player, const creaturea& source, creaturea& creatures, itema& items, indexa& indecies, variant id, int v) const;
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
	targeti				target;
	//
	skill_s				getid() const;
	constexpr bool		is(ability_s v) const { return abilities[0] == v || abilities[1] == v; }
	constexpr bool		isweapon() const { return weapon.attack != 0; }
};
struct spelli {
	const char*			name;
	const char*			nameof;
	unsigned char		mp;
	targeti				target;
	dicei				dice;
	char				multiplier;
	variant				bonus;
	const char*			throw_text;
};
struct itemground : item {
	short unsigned		index;
};
struct vproc {
	void(*pinp)();
	void(creature::*pcre)();
	constexpr vproc() : pinp(0), pcre(0) {}
	constexpr vproc(void(*v)()) : pinp(v), pcre(0) {}
	constexpr vproc(void(creature::*v)()) : pinp(0), pcre(v) {}
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
struct statistici : public dungeoni {
	short				artifacts;
	short				magic_items;
};
class location : public statistici {
	typedef bool(location::*procis)(indext i) const;
	tile_s				tiles[mmx*mmy];
	map_object_s		objects[mmx*mmy];
	unsigned char		random[mmx*mmy];
	mapflf				flags[mmx*mmy];
	char				light_level;
	//
	indext				getfree(indext i, procis proc, int radius_maximum) const;
	bool				istile(indext i) const;
	bool				istile2(indext i) const;
	site&				room(const rect& rc);
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
	site*				addsite(room_s type, const rect& rc);
	creature*			adventurer(indext index);
	bool				apply(creature& player, indext index, variant id, int v, int order, bool run);
	void				blockcreatures();
	void				blockwalls(bool water = true);
	indext				building(const rect& rc, direction_s dir = Center);
	bool				cansee(indext i1, indext i2) const;
	static indext		center(int x, int y, int w, int h);
	static indext		center(const rect& rc);
	indext				choose(bool allow_cancel);
	void				clear();
	static void			clearblock();
	void				content(const rect& rc, room_s type, site* p);
	creature*			commoner(indext index);
	void				create(const dungeoni& type, int level, bool explored, bool visualize);
	void				drop(indext i, item v);
	void				editor();
	void				ellipse(rect rc, tile_s object);
	void				fill(const rect& rc, int count, map_object_s v);
	void				fill(const rect& rc, int count, tile_s v);
	void				fill(const rect& rc, int count, variant id);
	void				fill(const rect& rc, map_object_s v);
	void				fill(const rect& rc, tile_s v);
	indext				find(map_object_s v) const;
	indext				find(tile_s v, const rect& rc) const;
	void				forest(const rect& rc);
	constexpr static indext	get(short x, short y) { return y * mmx + x; }
	int					getlight() const { return light_level; }
	static direction_s	getdirection(indext from, indext to);
	static direction_s	getdirection(point from, point to);
	indext				getfree(indext i) const { return getfree(i, &location::isfreenc, 5); }
	static short		getx(indext i) { return i % mmx; }
	static short		gety(indext i) { return i / mmx; }
	int					getindex(indext i, tile_s e) const;
	int					getitemscount(indext i) const;
	map_object_s		getobject(indext i) const { return objects[i]; }
	indext				getpoint(const rect& rc, direction_s dir) const;
	static int			getrange(indext i1, indext i2);
	int					getrand(indext i) const { return random[i]; }
	indext				getrand(const rect& rc) const;
	static rect			getrect(indext i, int rx = 3, int ry = 2);
	tile_s				gettile(indext i) const;
	trap_s				gettrap(indext i) const;
	void				indoor(point camera, bool show_fow = true, const picture* effects = 0);
	bool				is(indext i, map_flag_s v) const { return flags[i].is(v); }
	bool				isfree(indext i) const;
	bool				isfreelt(indext i) const;
	bool				isfreenc(indext i) const;
	bool				isfreenw(indext i) const;
	bool				ismatch(indext index, const rect& rectanle) const;
	bool				ismatch(indext index, variant v) const;
	void				lake(const rect& rc);
	void				interior(const rect& rc, room_s type, indext index, int level, rect* result_rect, site* ps);
	void				loot(indext index, item_s type, int level, char chance_bigger_price = 0, identify_s identify = Unknown, char chance_curse = 10, char bonus_quality = 0);
	void				loot(indext index, const aref<slot_s>& slots, int level, char chance_bigger_price = 0, identify_s identify = Unknown, char chance_curse = 10, char bonus_quality = 0);
	void				loot(const rect& rc, const aref<slot_s>& slots, int chance, int level, char chance_bigger_price = 0, identify_s identify = Unknown, char chance_curse = 10, char bonus_quality = 0);
	void				makewave(indext index);
	void				minimap(int x, int y, point camera, bool fow) const;
	void				minimap(indext index, bool fow) const;
	creature*			monster(indext index);
	static rect			normalize(const rect& rc);
	bool				read(const char* url, bool overland);
	bool				read(indext index, int level);
	void				remove(indext i, map_flag_s v) { flags[i].remove(v); }
	void				set(indext i, map_flag_s v) { flags[i].set(v); }
	void				set(indext i, tile_s v);
	void				set(indext i, trap_s v);
	void				set(indext i, map_object_s v) { objects[i] = v; }
	void				setr(indext i, unsigned char v) { random[i] = v; }
	static void			setcamera(short x, short y);
	static void			setcamera(indext i);
	indext				setiwh(int x, int y, int s, tile_s o, map_object_s r, bool locked_doors);
	indext				setiwv(int x, int y, int s, tile_s o, map_object_s r, bool locked_doors);
	void				setlight(int v) { light_level = v; }
	void				setlos(indext index, int r);
	static void			show(rooma& rooms);
	creature*			shopkeeper(indext index);
	indext				stepto(indext index);
	indext				stepfrom(indext index);
	static indext		to(indext index, direction_s v);
	static direction_s	to(direction_s d, direction_s v);
	void				worldmap(point camera, bool show_fow = true) const;
	bool				write(const char* url, bool overland) const;
	bool				write(indext index, int level);
};
struct outdoori {
	struct picture {
		img_s			image;
		unsigned char	frame;
		point			pos;
	};
	indext				index;
	const char*			name;
	const char*			descriptor;
	picture				avatar;
	dungeoni			levels[4];
	constexpr explicit operator bool() const { return index != Blocked; }
	void				clear();
	static const outdoori* find(indext index);
	constexpr indext	getposition() const { return index; }
	constexpr void		setposition(indext v) { index = v; }
};
struct tilei {
	const char*			id;
	const char*			name;
	gender_s			gender;
	const dungeoni*		wilderness;
};
struct eventi {
	unsigned short		index;
	unsigned short		type;
	varianta			actions;
	const char*			text;
	const char*			answer1;
	const char*			answer2;
	explicit operator bool() const { return text != 0; }
};
class gamei : public geoposable {
	unsigned			rounds;
	tile_s				tile;
	unsigned short		outdoor_id;
	char				reputation;
	int					restore_energy;
	deck				events;
	//
	bool				checkalive();
	void				checkcommand();
	void				playactive();
	void				playoverland();
public:
	void				addmoney(int v) {}
	void				addreputation(int v) { reputation += v; }
	void				applyboost();
	bool				enter(int level, map_object_s stairs);
	item*				find(item_s v) const;
	int					get(skill_s v) const;
	const dungeoni*		getdungeon() const;
	int					getmoney() const { return 0; }
	int					getreputation() const { return reputation; }
	int					getrounds() const { return rounds; }
	void				intialize();
	void				move(indext index);
	static void			help();
	void				passminute();
	void				play();
	bool				read();
	void				setposition(indext v);
	void				updatepos();
	void				use(map_object_s v);
	bool				write();
	void				wait();
};
struct commandi {
	typedef void(gamei::*addproc)(int v);
	typedef int(gamei::*getproc)() const;
	int					value;
	getproc				get;
	addproc				add;
};
extern gamei			game;
extern location			loc;
extern stringbuilder	sb;
DECLENUM(ability);
DECLENUM(class);
DECLENUM(item);
DECLENUM(map_object);
DECLENUM(skill);
DECLENUM(tile);
DECLENUM(race);
DECLENUM(slot);
DECLENUM(spell);
DECLENUM(state);
#include "answer.h"
#include "flagable.h"
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

const int chance_corridor_content = 8;
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
	Rock, Arrow, Bolt, Boulder,
	LeatherArmor, StuddedLeatherArmor, ScaleMail, ChainMail, SplintMail, PlateMail,
	Shield, Helmet, BracersLeather, BracersIron,
	Cloack1, Cloack2, Cloack3, Cloack4, Cloack5,
	Boot1, Boot2, IronBoot1, IronBoot2, IronBoot3,
	Ration, Apple, BreadHalflings, BreadEvlen, BreadDwarven, Cake, Sausage, Meat,
	RoseHip, Physalis, BirdCherry, Viburnum, Clover, Mint, Cherry, Tarnelle,
	Topinambur, Spinach, Parsley, Carrot,
	Scroll1, Scroll2, Scroll3, AlchemyReceipt,
	Wand1, Wand2, Wand3, Wand4, Wand5,
	Book1, Book2, Book3, Book4, Book5,
	AlchemyPotion, Potion1, Potion2, Potion3, Potion4, Potion5,
	RingRed, RingBlue, RingGreen,
	Amulet1, Amulet2, Amulet3, Amulet4, Amulet5,
	IronIgnot, SilverIgnot, MythrilIgnot,
	ClimbingTool, FletcherySet, Forge, HealingKit, ScriblingKit, CrystalBall, AlchemySet, TheifTool, CookingSet,
	Lute, Arfa, Baboon, Guitar, Vilanchelle, Flute,
	Corpse, Bones, Shell, DoorKey, Coin, CoinSP, CoinGP,
	Claws, Slam, Bite, Hitin, Fur,
	ManyItems
};
enum diety_s : unsigned char {
	GodBane, GodBhaal, GodGruumsh, GodHelm, GodMistra, GodMoradin, GodTempus, GodTyr
};
enum slot_s : unsigned char {
	Backpack, Edible, Readable, Drinkable, Zapable, Coinable, LastBackpack = Backpack + 31,
	Head, Neck, Melee, OffHand, TorsoBack, Torso, RightFinger, LeftFinger, Elbows, Legs, Ranged, Amunitions, Tool,
	LastWear = Tool,
};
enum race_s : unsigned char {
	Human, Dwarf, Elf, Halfling, Ratling, Orc,
	Goblin, Kobold, Ogre, Gnoll,
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
	Attack, AttackMelee, AttackRanged,
	Damage, DamageMelee, DamageRanged,
	Pierce, Protection, Armor, Deflect, Luck, Speed, Movement, Visibility,
	Level, LifeRate, ManaRate, FaithPoints,
	LifePoints, ManaPoints,
};
enum skill_s : unsigned char {
	Bargaining, Bluff, Diplomacy,
	Acrobatics, Alertness, Athletics, Backstabbing, Climbing, Concetration,
	DisarmTraps, HearNoises, HideInShadow, Lockpicking, MoveSilently, PickPockets,
	Alchemy, Cooking, Dancing, Engineering, Gambling, History, Healing, Herbalism,
	Literacy, Mining, MusicalInstrument, Religion, Riding, Smithing, Survival, Swimming,
	ProficiencyAxes, ProficiencyBows, ProficiencyDaggers, ProficiencyMaces, ProficiencyPolearms, ProficiencyStaff, ProficiencySwords,
	FirstSkill = Bargaining, LastSkill = ProficiencySwords,
};
enum state_s : unsigned char {
	Darkvision, Dazzled, Drunken, Fear,
	Friendly, Hostile, Invisible, Poisoned, Sick, Summoned,
	Unaware,
	LastState = Unaware,
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
	ShopWeaponAndArmor, ShopPotions, ShopScrolls, ShopFood, // Last magazine
	RoomOfBlood, RoomOfDamage, RoomOfDarkness, RoomOfDull, RoomOfSpellLearning, RoomOfMana,
	RoomOfNature, RoomOfSticking, RoomOfWind,
};
enum map_object_s : unsigned char {
	NoTileObject,
	Door, Tree, Altar, Statue, Trap, Plants, Pool, StairsUp, StairsDown
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
	ResSea, ResPlains, ResFoothills, ResForest, ResMountains, ResCloudPeaks,
	ResOutdoor, ResSwamp, ResTrail,
	ResUI,
	ResPCmar, ResPCmbd, ResPCmac
};
enum spell_s : unsigned char {
	ArmorSpell, BlessSpell, BlessItem, ChatPerson, CharmPerson,
	DrunkenSpell, DetectEvil, DetectMagic, Domination,
	FearSpell, FireBall, HealingSpell,
	Identify, Invisibility, KnockDoor, LightSpell, MagicMissile, PoisonSpell,
	Repair, RemovePoisonSpell, RemoveSickSpell,
	SickSpell, ShieldSpell, ShokingGrasp, Sleep, SlowMonster, SummonAlly, Web,
	FirstSpell = ArmorSpell, LastSpell = Web
};
enum map_flag_s : unsigned char {
	Visible, Hidden, Opened, Sealed, Explored, Webbed, Trailed = Webbed, Blooded,
};
enum item_type_s : unsigned char {
	Mundane, Cursed, Blessed, Artifact,
};
enum damage_s : unsigned char {
	Bludgeon, Slashing, Piercing,
	Acid, Charm, Cold, Electricity, Fire, Magic, Poison, WaterAttack
};
enum material_s : unsigned char {
	Glass, Iron, Leather, Organic, Paper, Stone, Wood,
};
enum encumbrance_s : unsigned char {
	NoEncumbered,
	Encumbered, HeavilyEncumbered,
};
enum range_s : unsigned char {
	You, Close, Reach, Near, Far, Everywhere
};
enum itemflag_s : unsigned char {
	SingleUse, TwoHanded, Versatile, Light, Natural,
};
enum outdoor_s : unsigned char {
	VillageTaho, CityMeher, LostMine,
	LastOutdoor = LostMine,
};
enum formula_s : unsigned char {
	Negative,
	Divide2, Divide3, Divide4, Divide10,
	Multiply2, Multiply3, Multiply4,
};
enum slot_mode_s : unsigned char {
	NoSlotName, SlotName, SlotWhere
};
enum identify_s : unsigned char {
	Unknown, KnownStats, KnownMagic, KnownPower,
};
enum sale_s : unsigned char {
	NotForSale, Sale65, Sale80, Sale100, Sale150, Sale200, Sale250, Sale300
};
enum target_flag_s : unsigned char {
	NotYou, Friends, Enemies,
	AlwaysChoose, LongAction,
	RandomTargets, TargetArea, MultiplyTarget,
	AllTargets,
};
enum intellegence_s : unsigned char {
	NoInt, AnimalInt, SemiInt, LowInt, AveInt, VeryInt, HighInt, ExpInt, GenInt, SupGenInt, GodInt
};
enum map_object_flag_s : unsigned char {
	BlockMovement, BlockLight,
};
enum site_flag_s : unsigned char {
	DungeonSite, CitySite,
	UsedPower, UsedSearch, KnownSite,
};
enum modifier_s : unsigned char {
	NoModifier,
	Opponent, Easy, Hard, Random,
	Resist, Immune, Vulnerable,
};
enum action_s : unsigned char {
	GuardPosition, StopGuardPosition, UseLongActionSkill, MakeDiscount, MakeHappy, MakeAnger,
	MinorWound, MinorDisaster,
};
enum duration_s : unsigned char {
	Instant, Round, Minute, CoupleMinutes, HalfHour, Hour,
};
enum command_s : unsigned char {
	AddReputation, LoseReputation, BadReputation, GoodReputation,
	AddMoney10, AddMoney20, AddMoney50, LoseMoney10, LoseMoney20, LoseMoney50,
	LastCommand = LoseMoney50,
};
enum condition_s : unsigned char {
	Anger, Berserking, Busy, Guardian, Happy, Wounded,
	MissHits, MissMana, MissHalfHits, MissHalfMana, MissAlmostAllHits, MissAlmostAllMana,
	Owner,
};
enum variant_s : unsigned char {
	NoVariant,
	Ability, Action, Alignment, Class, Command, Condition, Creature,
	Formula, Gender, God, Harm, Item, ItemIdentify, ItemType, Modifier,
	Number, Object, ObjectFlags, Outdoor, Race, Range, Role, Room,
	Sale, Skill, Slot, Spell, State, Target, Tile,
	Variant,
};
enum fraction_s : unsigned char {
	NoFraction,
	AdventurerGuild, MiddleKindom, NorthKindom, MountainKindom, ForestKindom,
	SeaKings, DemonLords,
	SavageHorde, MightyDragon, RavenousUndead, SecretOrder, TheivesGuild,
};
enum dice_s : unsigned char {
	NoDice,
	D1d3, D1d4, D1d6, D1d8, D1d10, D1d12, D2d6,
};
enum tile_flag_s : unsigned char {
	RestrictMovement, RestrictVision,
};
struct dungeoni;
struct targeti;
struct landscapei;
class creature;
class creaturea;
class location;
class site;
typedef short unsigned indext;
typedef adat<rect, 64> rooma;
typedef flagable<1 + Chaotic / 8> alignmentf;
typedef flagable<1 + LastState / 8> statef;
typedef flagable<1 + LastRace / 8> racef;
typedef flagable<1 + ManyItems / 8>	itemf;
typedef flagable<1 + Blooded / 8> mapflf;
typedef flagable<1 + WaterAttack / 8> damagef;
typedef flagable<1 + KnownSite / 8> sitef;
typedef flagable<1 + RestrictVision / 8> tilef;
typedef flagable<1 + LastSpell / 8> spellf;
typedef cflags<map_object_flag_s> mapobjf;
typedef adat<role_s, 4> summona;
typedef void(*gentileproc)(indext index);
typedef void(*stageproc)();
typedef indext(*getposproc)(direction_s i);
typedef void(*genareaproc)(const rect& rc, rooma& rooms, const landscapei& landscape);
typedef std::initializer_list<slot_s> slota;
typedef std::initializer_list<item_s> iteme;
struct variant {
	variant_s			type;
	unsigned char		value;
	constexpr variant() : type(NoVariant), value(0) {}
	constexpr variant(ability_s v) : type(Ability), value(v) {}
	constexpr variant(action_s v) : type(Action), value(v) {}
	constexpr variant(alignment_s v) : type(Alignment), value(v) {}
	constexpr variant(class_s v) : type(Class), value(v) {}
	constexpr variant(command_s v) : type(Command), value(v) {}
	constexpr variant(condition_s v) : type(Condition), value(v) {}
	constexpr variant(damage_s v) : type(Harm), value(v) {}
	constexpr variant(diety_s v) : type(God), value(v) {}
	constexpr variant(formula_s v) : type(Formula), value(v) {}
	constexpr variant(gender_s v) : type(Gender), value(v) {}
	constexpr variant(identify_s v) : type(ItemIdentify), value(v) {}
	constexpr variant(item_s v) : type(Item), value(v) {}
	constexpr variant(item_type_s v) : type(ItemType), value(v) {}
	constexpr variant(modifier_s v) : type(Modifier), value(v) {}
	constexpr variant(map_object_s v) : type(Object), value(v) {}
	constexpr variant(map_flag_s v) : type(ObjectFlags), value(v) {}
	constexpr variant(range_s v) : type(Range), value(v) {}
	constexpr variant(race_s v) : type(Race), value(v) {}
	constexpr variant(role_s v) : type(Role), value(v) {}
	constexpr variant(room_s v) : type(Room), value(v) {}
	constexpr variant(sale_s v) : type(Sale), value(v) {}
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
typedef std::initializer_list<variant> varianta;
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
	void				addidentifier(const char* identifier) override;
};
struct durationi {
	const char*			id;
	const char*			name;
	unsigned			range[2];
	int					roll() const;
};
struct conditioni {
	const char*			id;
};
struct sloti {
	const char*			id;
	const char*			name;
	const char*			name_where;
};
struct dietyi {
	const char*			name;
	const char*			nameof;
	gender_s			gender;
	itemf				sacred;
	const char*			symbol;
	void				act(const char* format, ...) const;
	void				actv(stringbuilder& sb, const char* format, const char* format_param) const;
};
struct chancei {
	state_s				state;
};
struct boosti {
	variant				id;
	char				modifier;
	short unsigned		owner_id;
	unsigned			time;
};
struct leveli {
	class_s				type;
	char				level;
	varianta			features;
};
struct classi {
	const char*			name;
	unsigned char		hp, mp;
	unsigned char		naked_avatar;
	char				ability[6];
	varianta			bonuses;
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
	char				bonus_multiplier;
	const char*			format;
	int					cost;
	//
	ability_s			getid() const;
	int					getbonus(int v) const;
};
struct equipmenti {
	race_s				race;
	class_s				type;
	varianta			features;
};
struct map_objecti {
	const char*			id;
	const char*			name;
	mapobjf				flags;
	short unsigned		start, count;
};
struct picture {
	img_s				img;
	unsigned short		frame;
	unsigned short		flags;
	point				pos;
	unsigned char		alpha;
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
	statef				flags;
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
	const char*			avatar_id;
	varianta			bonuses;
	bool				is(variant v) const { for(auto e : bonuses) if(e == v) return true; return false; }
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
struct attacki {
	damage_s			type;
	char				attack, speed;
	dicei				dice;
	item_s				ammunition;
	int					getenergy() const { return StandartEnergyCost - speed * 50; }
};
struct materiali {
	const char*			name;
	const char*			genders[3];
	damagef				resist, vulnerable;
};
struct damagei {
	const char*			name;
	char				damage_wears;
	char				damage_chance;
	ability_s			ability;
	const char*			resist_text;
};
struct pooli {
	const char*			name;
	const char*			avatar_id;
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
	struct weaponi {
		damage_s		type;
		dice_s			dice;
		char			damage, speed;
		item_s			amunition, ammunition_compatible;
	};
	struct armori {
		char			armor;
		char			deflect;
		char			protection;
	};
	const char*			name;
	const char*			avatar_id;
	unsigned char		level;
	int					weight;
	int					cost;
	gender_s			gender;
	material_s			material;
	weaponi				weapon;
	armori				armor;
	aref<variant>		effects;
	cflags<itemflag_s>	flags;
	slot_s				slot;
	skill_s				skill;
	//
	constexpr item_s	getammo() const { return weapon.amunition; }
	item_s				getid() const;
	skill_s				getskill() const;
	int					getspelllevel() const;
	bool				is(slot_s v) const;
	bool				is(slota source) const;
	variant				randeffect() const;
};
class item {
	item_s				type;
	unsigned char		known_magic : 1;
	unsigned char		known_stats : 1;
	item_type_s			magic : 2;
	sale_s				sale : 3;
	union {
		struct {
			unsigned char	effect;
			unsigned char	personal : 1;
			unsigned char	known_power : 1;
			unsigned char	damaged : 2;
			unsigned char	charge : 4;
		};
		unsigned short	count;
	};
	bool				use(skill_s id, creature& player, int order, bool run);
	bool				use(spell_s id, creature& player, int level, int order, bool run);
public:
	item() = default;
	item(item_s type, int level);
	constexpr item(item_s type) : type(type), known_magic(0), known_stats(0), magic(Mundane), sale(Sale100), count(0) {}
	explicit operator bool() const { return type != NoItem; }
	void				act(const char* format, ...) const;
	void				actv(stringbuilder& st, const char* format, const char* format_param) const;
	bool				apply(creature& player, variant id, int v, int order, bool run);
	void				clear() { memset(this, 0, sizeof(*this)); }
	void				create(item_s type, int chance_artifact, int chance_magic, int chance_cursed);
	void				breaktest();
	void				damage(int count, damage_s type, bool interactive);
	void				decoy();
	void				decoy(damage_s type, bool interactive, bool include_artifact = false);
	void				destroy(damage_s type, bool interactive);
	static item_s		findcorpse(role_s v);
	item_s				getammo() const { return geti().weapon.amunition; }
	itemi::armori		getarmor() const;
	attacki				getattack() const;
	int					getbonus() const;
	int					getcharges() const { return charge; }
	unsigned			getcost() const;
	int					getcount() const;
	int					getdamage() const;
	const char*			getdamagetext() const;
	variant				geteffect() const;
	const itemi&		geti() const { return bsdata<itemi>::elements[type]; }
	gender_s			getgender() const { return geti().gender; }
	item_s				getkind() const { return type; }
	item_type_s			getmagic() const { return magic; }
	material_s			getmaterial() const { return geti().material; }
	const char*			getname() const { return geti().name; }
	void				getname(stringbuilder& sb, bool show_cab) const;
	indext				getposition() const;
	static const aref<variant> getreceipts();
	skill_s				getskill() const { return geti().getskill(); }
	int					getspelllevel() const;
	void				getstatistic(stringbuilder& sb) const;
	sale_s				getsale() const { return sale; }
	creature*			getwearer() const;
	slot_s				getwearerslot() const;
	int					getweightsingle() const { return geti().weight; }
	int					getweight() const { return getweightsingle() * getcount(); }
	bool				is(slot_s v) const { return geti().is(v); }
	bool				is(identify_s v) const;
	bool				is(itemflag_s v) const { return geti().flags.is(v); }
	bool				is(item_type_s v) const { return magic == v; }
	bool				is(material_s v) const { return geti().material == v; }
	bool				is(sale_s v) const { return sale == v; }
	bool				isboost(variant id) const;
	bool				ischargeable() const;
	bool				iscountable() const;
	bool				isdamaged() const { return getdamage() > 0; }
	bool				islike(const item& v) const;
	bool				ispersonal() const { return !iscountable() && personal != 0; }
	void				loot();
	bool				ismatch(variant v) const;
	void				repair();
	void				set(item_type_s v);
	void				set(identify_s v);
	void				set(sale_s v) { sale = v; }
	void				setcharge(int v) { if(ischargeable()) charge = v; }
	void				setcount(int v);
	void				seteffect(variant v);
	void				setpersonal(int v);
	bool				stack(item& v);
	void				use();
};
struct trapi {
	const char*			name;
	const char*			avatar_id;
	char				modifier;
	char				multiplier;
	dicei				damage;
	variant				effect;
	const char*			text_use;
	item				loot;
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
	void				select(indext index, bool extend = false);
	void				select(site& e);
	void				selecta(creature& e);
	void				selectb(creature& e);
	void				selectg(variant v);
};
class variantc : public adat<variant> {
public:
	void				additems(slot_s v, int level);
	void				additems(slota source, int level);
	void				match(slot_s v, bool remove);
	void				match(itemflag_s v, bool remove);
	void				matchp(int count, bool greater);
	variant				random() const;
};
class skilla :public adat<skill_s, 64> {
public:
	void				removent();
	void				select(const creature& e);
	void				sort();
	void				match(target_flag_s i, bool remove);
};
class skillu : public skilla {
	unsigned char		cap[LastSkill + 1];
public:
	skillu();
	skill_s				choose(struct statable* player, bool interactive, const char* title, bool* cancel_result = 0) const;
	int					getcap(skill_s i) const { return cap[i]; }
	void				setcap(skill_s i, int v) { cap[i] = v; }
};
struct shopi {
	char				chance;
	slot_s				slot;
	char				price;
	char				cursed;
	char				quality;
};
typedef std::initializer_list<shopi> shopa;
struct roomi {
	typedef aref<const char*> strarray;
	const char*			name;
	sitef				flags;
	const char*			text;
	strarray			name1, name2;
	map_object_s		heart;
	role_s				keeper;
	shopa				shops;
	fntext				getdescription;
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
	void				actev(stringbuilder& st, const char* format, const char* param, bool add_sep) const;
	void				actv(stringbuilder& sb, const char* format, const char* param) const;
	void				actv(stringbuilder& sb, nameable& e, const char* format, const char* param) const;
	bool				ask(const nameable& opponent, const char* format, ...) const;
	bool				askv(stringbuilder& st, const nameable& e, const char* format, const char* param) const;
	static bool			askyn();
	bool				askyn(const char* format, ...) const;
	bool				cansee() const;
	void				feel(ability_s id, bool raise);
	void				info(const char* format, ...) const;
	void				infov(stringbuilder& sb, const char* format, const char* param) const;
	gender_s			getgender() const;
	const char*			getname() const;
	void				getname(stringbuilder& sb) const;
	race_s				getrace() const;
	site*				getsite() const;
	void				randomname();
	void				sayv(stringbuilder& st, const char* format, const char* param) const;
	void				setname(race_s race, gender_s gender);
	bool				isactive() const;
	bool				ischaracter() const { return value == Character; }
};
class site : public nameable, public rect {
	variant				param;
	short unsigned		owner_id;
	sitef				flags;
public:
	explicit operator bool() { return x2 > x1 && y2 > y1; }
	void				addlook(stringbuilder& sb) const;
	void				clear();
	static site*		find(indext index);
	room_s				getkind() const { return (room_s)value; }
	creature*			getowner() const;
	variant				getparam() const { return param; }
	bool				haslook() const;
	bool				is(site_flag_s v) const { return flags.is(v); }
	creature*			priest();
	void				set(const rect& v) { *static_cast<rect*>(this) = v; }
	void				set(room_s v);
	void				set(site_flag_s v) { flags.set(v); }
	void				setowner(const creature* v);
	void				setparam(variant v) { param = v; }
	creature*			shopkeeper();
	static void			unlink(const creature& player);
};
struct rolei {
	const char*			name;
	const char*			nameof;
	const char*			avatar_id;
	race_s				race;
	gender_s			gender;
	alignment_s			alignment;
	class_s				type;
	char				level;
	iteme				equipment;
	varianta			features;
	summona				minions;
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
struct quest {
	struct contexti {
		virtual void	add(answeri& an, const quest* p) const;
		virtual void	add(const quest* p) const;
		virtual bool	apply(const quest* p, bool run) { return true; }
	};
	int					index;
	varianta			bonus;
	const char*			name;
	int					next;
	constexpr operator bool() const { return index != 0; }
	const quest*		find(int index) const;
	const quest*		choose(contexti& e) const;
	void				play(contexti& e) const;
};
struct statable {
	short				abilities[ManaPoints + 1];
	unsigned char		skills[LastSkill + 1];
	unsigned char		spells[LastSpell + 1];
	damagef				resistance, immunity, vulnerability;
	spellf				active_spells;
	void				add(variant i, int v) { set(i, get(i) + v); }
	void				apply(varianta source);
	void				create(class_s type, race_s race);
	bool				isimmune(damage_s v) const { return immunity.is(v); }
	bool				ismaster(skill_s v) const { return skills[v] >= 70; }
	bool				isgrandmaster(skill_s v) const { return skills[v] >= 100; }
	bool				isresist(damage_s v) const { return resistance.is(v); }
	bool				isvulnerable(damage_s v) const { return vulnerability.is(v); }
	int					get(variant i) const;
	int					getbonus(ability_s v) const { return abilities[v] / 2 - 5; }
	//int					getcap(skill_s v) const { return 10; }
	dicei				getraise(skill_s v) const;
	void				raise(skill_s i);
	void				raise(role_s role, class_s type);
	void				set(variant i, int v);
	void				update(const statable& source);
	void				update_finish();
};
struct crafti {
	char				level;
	item_s				type;
	iteme				materials;
	variant				effect;
};
class creature : public nameable, public statable {
	class_s				kind;
	statable			basic;
	statef				states;
	item				wears[LastWear + 1];
	int					restore_energy, restore_hits, restore_mana;
	flagable<4>			recipes;
	short				hp, mp, poison, faith, mood, wounds;
	short unsigned		location_id, site_id;
	indext				guard;
	direction_s			direction;
	unsigned			experience;
	unsigned			money;
	encumbrance_s		encumbrance;
	void				add(ability_s id, int v, bool interactive);
	void				add(ability_s id, int v, bool interactive, unsigned minutes);
	void				add(condition_s id, int v, bool interactive);
	void				add(spell_s id, int v, bool interactive);
	void				add(state_s id, int v, bool interactive);
	void				add(skill_s id, int v, bool interactive);
	void				add(spell_s id, unsigned minutes);
	void				addboost(variant id, int modifier, unsigned minutes);
	void				additem(item_s type, variant effect, bool identified = true);
	bool				aiuse(const creaturea& creatures, const char* interactive, slot_s slot, variant effect);
	void				aimove();
	void				aioverland() {}
	bool				aiskills(creaturea& creatures, bool long_action, bool run);
	bool				aispells(creaturea& creatures);
	void				aiturn(creaturea& creatures, creaturea& enemies, creature* enemy);
	void				applyab();
	void				applyaward() const;
	void				attack(creature& enemy, const attacki& ai, int bonus, int multiplier, skill_s skill);
	void				cantmovehere() const;
	bool				cantakeoff(slot_s id, bool interactive);
	bool				canuse(const item& e, bool talk) const;
	void				dropdown(item& item);
	void				dropitems();
	void				equip(item it, slot_s id);
	void				finish();
	void				movecost(indext index);
	bool				pray(bool run);
	void				qsearch();
	void				raiselevel(bool intearctive);
	void				randomequip();
	bool				remove(item& it, bool run, bool talk, bool same_owner);
	void				update_boost();
	void				update_encumbrance();
	void				update_start() { statable::update(basic); }
	void				update_wear();
	bool				use(skill_s id, creature& player, int order, bool run);
	bool				use(spell_s id, creature& player, int level, int order, bool run);
	void				use(const foodi& ei, const item it, bool interactive);
	bool				use(indext index, bool moving);
	void				usestealth();
	void				usetrap();
public:
	typedef void (creature::*papply)();
	explicit operator bool() const { return hp > 0; }
	void				activate();
	void				add(variant id, int v, bool interactive);
	bool				add(item v, bool run, bool interactive);
	void				add(const effecti& id);
	void				addexp(int count, bool interactive = false);
	void				appear();
	bool				apply(creature& target, variant id, int v, int order, bool run);
	void				backpack();
	void				bloodstain() const;
	int					calculate(const varianta& source) const;
	bool				canleave(direction_s v) const;
	bool				cansee(indext i) const;
	bool				canshoot() const;
	void				chat();
	void				chat(creature& opponent);
	void				chat(creature& opponent, const quest* source);
	bool				charmresist(int bonus = 0) const;
	void				checkmood();
	void				checkpoison();
	void				checksick();
	variant				choosereceipt(const char* interactive) const;
	item				craft(item_s type, variant effect, skill_s skill);
	item				craft(item_s type, variant effect, skill_s skill, int bonus);
	void				create(race_s race, gender_s gender, class_s type);
	void				create(role_s type);
	void				clear();
	void				consume(int energy_value);
	void				damage(int count, damage_s type, int pierce = 0, bool interactive = true);
	void				damagewears(int count, damage_s type, int item_count = 1);
	void				decoyfood();
	void				dispell(bool interactive);
	void				dispell(variant source, bool interactive);
	void				drink();
	void				dropdown();
	void				eat();
	bool				equip(item value);
	bool				equip(item& v1, item& v2, bool run);
	void				enslave();
	bool				execute(action_s v, bool run);
	void				fail(skill_s id);
	static creature*	find(indext i);
	boosti*				find(variant id) const;
	item*				finditem(item_s v);
	int					get(ability_s v) const { return abilities[v]; }
	int					get(spell_s v) const { return spells[v]; }
	int					get(skill_s v) const { return skills[v]; }
	const item&			get(slot_s v) const { return wears[v]; }
	item&				get(slot_s v) { return wears[v]; }
	static creature*	getactive();
	static creature*	getactive(int index);
	int					getallowedweight() const;
	attacki				getattack(slot_s slot, const item& weapon) const;
	attacki				getattack(slot_s slot) const { return getattack(slot, wears[slot]); }
	int					getaward() const { return 10 + 15 * get(Level); }
	int					getbase(variant id) const { return basic.get(id); }
	int					getboost(variant id) const;
	const classi&		getclass() const { return bsdata<classi>::elements[kind]; }
	direction_s			getdirection() const { return direction; }
	encumbrance_s		getencumbred() const { return encumbrance; }
	int					getexperience() const { return experience; }
	void				getfullname(stringbuilder& sb) const;
	diety_s				getgod() const;
	short unsigned		getguard() const { return guard; }
	int					gethits() const { return hp; }
	short unsigned		getid() const;
	intellegence_s		getint() const;
	creature*			getleader() const;
	unsigned			getlevelup() const;
	int					getlos() const;
	int					getmana() const { return mp; }
	int					getmoney() const { return money; }
	static creature*	getobject(short unsigned v);
	int					getpoisondamage() const { return 1 + poison / 5; }
	static const char*	getrangname(int v);
	role_s				getrole() const { return (role_s)value; }
	site*				getsite() const;
	const summona&		getsummon() const;
	slot_s				getwearerslot(const item* p) const;
	int					getweight() const;
	void				heal(int value) { damage(-value, Magic); }
	void				inventory();
	bool				is(class_s v) const { return kind == v; }
	bool				is(condition_s v) const;
	bool				is(intellegence_s v) const;
	bool				is(gender_s v) const { return getgender() == v; }
	bool				is(race_s v) const { return getrace() == v; }
	bool				is(room_s v) const;
	bool				is(state_s v) const { return states.is(v); }
	bool				is(spell_s v) const { return active_spells.is(v); }
	bool				is(const creature* p) const { return this == p; }
	bool				isallow(item_s v) const;
	bool				isenemy(const creature* target) const;
	bool				ismatch(const creature& opponent, skill_s id, int value) const;
	bool				ismaster(skill_s v) const;
	const char*			isusedisable(skill_s id) const;
	void				kill();
	bool				knownreceipt(variant id) const;
	void				learnreceipt(variant id);
	bool				leaving(direction_s v);
	void				look(indext index);
	void				lookaround();
	void				lookobjects();
	void				makemove();
	bool				match(variant id) const;
	void				meleeattack(creature& enemy, int bonus = 0);
	void				minimap();
	void				move(indext index);
	void				moveto(indext index);
	void				moveaway(indext index);
	bool				needrestore(ability_s id) const;
	static void			pause(bool interactive = true);
	void				paymana(int value, bool interactive);
	void				playuioverland();
	void				playui();
	void				pickup();
	void				drink(ability_s id, bool interactive, item_type_s magic, int minutes);
	void				pray();
	void				quitandsave();
	void				raiseathletics();
	void				raiseskills(int number, bool interactive);
	void				raiseskills(bool interactive) { raiseskills(basic.abilities[Intellegence] / 3, interactive); }
	void				rangeattack(creature& enemy, int bonus = 0);
	void				readsomething();
	bool				resist(damage_s v, int bonus, bool interactive) const;
	void				restoration();
	bool				roll(ability_s v, int bonus = 0) const { return rollv(get(v) * 3 + bonus); }
	bool				roll(skill_s v, int bonus = 0) const { return rollv(get(v) + bonus); }
	bool				rolld(ability_s v, skill_s vs, int bonus = 0) const { return roll(v, bonus) || roll(vs, bonus); }
	static bool			rollv(int v);
	static int			rollv(int v1, int v2);
	void				sacrifice(diety_s god, item& it);
	void				say(const char* format, ...) const;
	bool				saybusy();
	void				search();
	void				select(itema& a, slot_s i1, slot_s i2, bool filled_only);
	void				select(skilla& e) const;
	void				set(ability_s i, int v) { abilities[i] = v; }
	void				set(skill_s i, int v) { skills[i] = v; }
	void				set(const site* v);
	void				setguard(short unsigned value) { guard = value; }
	void				setmoney(int value) { money = value; }
	void				setfriendlyto(const creature& player);
	void				shoot();
	void				testpotion();
	void				testweapons();
	void				unlink();
	void				update();
	bool				use(const creaturea& source, skill_s id);
	bool				use(const creaturea& creatures, spell_s id, int level, item* magic_source, bool show_errors);
	bool				use(const creaturea& creatures, item& it);
	bool				use(spell_s id, int level, item* magic_source = 0, bool show_errors = false);
	void				useobject();
	void				useroom();
	void				useskills();
	void				usespells();
	void				usetools();
	void				usewands();
	void				wait() { consume(StandartEnergyCost); }
	void				wait(int n) { consume(StandartEnergyCost * n); }
	void				wait(duration_s v);
	void				waitturn() { wait(10 * 3); }
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
	void				matchact(spell_s id, bool remove);
	void				matchr(indext index, int range);
	void				select();
	void				select(indext start, int distance, bool visible = true);
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
	char				level;
	char				light_level;
	room_s				rooms[8];
	racef				denyrace;
	itemc				items;
	explicit constexpr operator bool() const { return level != 0; }
	const dungeoni*		find(int level) const;
	constexpr bool		isdungeon() const { return type == AreaDungeon; }
};
struct packi {
	variant				monster;
	dicei				count;
	char				chance;
};
typedef std::initializer_list<packi> packa;
struct encounter {
	state_s				state;
	varianta			conditions;
	const char*			text;
	packa				monsters;
	void				play();
	bool				match(variant v);
	static encounter*	getrandom(tile_s landscape);
};
class indexa : public adat<indext> {
public:
	int					choose(const char* interactive);
	void				match(variant v, bool remove);
	void				matcha(creature& player, variant id, int v);
	void				matchr(indext index, int range);
	void				matchobj(bool remove);
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
	void				match(creature& player, creaturea& creatures, int r, variant id, int v) const;
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
	const char*			nameof;
	weaponi				weapon;
	targeti				target;
	//
	skill_s				getid() const;
	constexpr bool		isweapon() const { return weapon.attack != 0; }
};
struct spelli {
	const char*			name;
	const char*			nameof;
	unsigned char		mp;
	targeti				target;
	dicei				dice;
	variant				bonus;
	const char*			throw_text;
};
struct itemground : item {
	short unsigned		index;
	void				clear();
};
struct vproc {
	void(*pinp)();
	void(creature::*pcre)();
	void(location::*ploc)(indext i);
	constexpr vproc() : pinp(0), pcre(0), ploc(0) {}
	constexpr vproc(void(*v)()) : pinp(v), pcre(0), ploc(0) {}
	constexpr vproc(void(creature::*v)()) : pinp(0), pcre(v), ploc(0) {}
	constexpr vproc(void(location::*v)(indext i)) : pinp(0), pcre(0), ploc(v) {}
};
struct manual {
	typedef void(*fntext)(stringbuilder& sb, manual& e, answeri& an);
	variant				parent;
	variant				value;
	const char*			name;
	const char*			text;
	fntext				description;
	explicit operator bool() const { return (bool)value; }
	const char*			getname() const;
};
struct statistici : public dungeoni {
	short				artifacts;
	short				magic_items;
};
class location : public statistici {
	typedef bool(location::*procis)(indext i) const;
	unsigned			saveround;
	tile_s				tiles[mmx * mmy];
	map_object_s		objects[mmx * mmy];
	unsigned char		random[mmx * mmy];
	mapflf				flags[mmx * mmy];
	//
	indext				getfree(indext i, procis proc, int radius_maximum) const;
	indext				getfreex(int x1, int x2, int y, procis proc) const;
	indext				getfreey(int x, int y1, int y2, procis proc) const;
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
	void				blockcreatures();
	void				blockwalls(bool water = true);
	indext				building(const rect& rc, direction_s dir = Center);
	bool				cansee(indext i1, indext i2) const;
	static indext		center(int x, int y, int w, int h);
	static indext		center(const rect& rc);
	indext				choose(indext start, bool fow, bool allow_cancel, bool outdoor) const;
	void				clear();
	static void			clearblock();
	void				content(const rect& rc, room_s type, site* p);
	creature*			commoner(indext index);
	void				create(const dungeoni& type, int level, bool explored);
	void				drop(indext i, item v);
	void				editor();
	void				ellipse(rect rc, tile_s object);
	void				fill(const rect& rc, int count, map_object_s v);
	void				fill(const rect& rc, int count, tile_s v);
	void				fill(const rect& rc, int count, map_flag_s v);
	void				fill(const rect& rc, int count, variant id);
	void				fill(const rect& rc, map_flag_s v);
	void				fill(const rect& rc, map_object_s v);
	void				fill(const rect& rc, tile_s v);
	void				fille(const rect& rc, tile_s v);
	indext				find(map_object_s v) const;
	indext				find(tile_s v, const rect& rc) const;
	void				forest(const rect& rc);
	void				generate(indext index, tile_s tile, int chance, int count);
	constexpr static indext	get(short x, short y) { return y * mmx + x; }
	int					getlight() const { return light_level; }
	static direction_s	getdirection(indext from, indext to);
	static direction_s	getdirection(point from, point to);
	indext				getfree(indext i) const { return getfree(i, &location::isfreenc, 5); }
	static short		getx(indext i) { return i % mmx; }
	static short		gety(indext i) { return i / mmx; }
	int					getindex(indext i, tile_s e) const;
	int					getindex(indext i, map_flag_s e) const;
	int					getindex2(indext i, tile_s e, int r) const;
	int					getindex3(indext i, tile_s e) const;
	int					getitemscount(indext i) const;
	map_object_s		getobject(indext i) const { return objects[i]; }
	int					getplantgrow(indext i) const;
	indext				getpoint(const rect& rc, direction_s dir) const;
	static int			getrange(indext i1, indext i2);
	int					getrand(indext i) const { return random[i]; }
	indext				getrand(const rect& rc) const;
	static rect			getrect(indext i, int rx = 3, int ry = 2);
	tile_s				gettile(indext i) const;
	trap_s				gettrap(indext i) const;
	void				growplants();
	void				hide(indext i);
	void				indoor(point camera, bool show_fow = true, const picture* effects = 0);
	bool				is(indext i, map_flag_s v) const { return flags[i].is(v); }
	bool				ishidden(indext i) const;
	bool				isfree(indext i) const;
	bool				isfreelt(indext i) const;
	bool				isfreenc(indext i) const;
	bool				isfreenw(indext i) const;
	bool				ismatch(indext index, const rect& rectanle) const;
	bool				ismatch(indext index, variant v) const;
	void				lake(const rect& rc);
	void				interior(const rect& rc, room_s type, indext index, int level, rect* result_rect, site* ps);
	void				loot(indext index, item_s type, int level, char chance_bigger_price, identify_s identify, char chance_curse);
	void				loot(indext index, slot_s slot, int level, char chance_bigger_price, identify_s identify, char chance_curse);
	void				loot(indext index, slota slot, int level, char chance_bigger_price, identify_s identify, char chance_curse);
	void				loot(const rect& rc, const variantc& source, int chance, int level, char chance_bigger_price, identify_s identify, char chance_curse);
	void				makewave(indext index);
	void				minimap(int x, int y, point camera, bool fow) const;
	void				minimap(indext index, bool fow) const;
	creature*			monster(indext index);
	indext				movernd(indext from, indext to);
	static rect			normalize(const rect& rc);
	bool				read(const char* url, bool overland);
	bool				read(indext index, int level);
	void				remove(indext i, map_flag_s v) { flags[i].remove(v); }
	void				remove(indext i);
	void				reveal(indext i);
	void				serialx(const char* url, bool write_mode);
	void				set(indext i, map_flag_s v) { flags[i].set(v); }
	void				set(indext i, tile_s v);
	void				set(indext i, trap_s v);
	void				set(indext i, map_object_s v);// { objects[i] = v; }
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
	void				trail(indext i);
	bool				use(indext index, spell_s id, creature& player, int level, int order, bool run);
	bool				use(indext index, skill_s id, creature& player, int level, int order, bool run);
	bool				use(indext index, variant id, creature& player, int level, int order, bool run);
	void				viewfow(int x0, int y0, const rect& rc) const;
	void				worldmap(point camera, bool show_fow = true) const;
	bool				write(const char* url, bool overland);
	bool				write(indext index, int level);
};
struct outdoori {
	indext				index;
	const char*			name;
	const char*			avatar_id;
	const char*			descriptor;
	dungeoni			levels[4];
	constexpr explicit operator bool() const { return index != Blocked; }
	static outdoori*	choose();
	void				clear();
	static outdoori*	find(indext index);
	int					getid() const;
	constexpr indext	getposition() const { return index; }
	constexpr void		setposition(indext v) { index = v; }
};
struct tilei {
	const char*			id;
	tilef				flags;
	const char*			name;
	char				symbol;
	gender_s			gender;
	const dungeoni*		wilderness;
};
class fractiona : public aset<fraction_s, TheivesGuild + 1> {
};
class gamei : public geoposable {
	unsigned			rounds;
	tile_s				tile;
	unsigned short		outdoor_id;
	char				reputation;
	int					restore_energy;
	unsigned			restore_half_turn, restore_turn, restore_hour, restore_day_part, restore_day;
	deck				events;
	fractiona			progress;
	//
	bool				checkalive();
	void				checkcommand();
	void				playactive();
	void				playoverland();
public:
	void				addmoney(int v) {}
	void				addreputation(int v) { reputation += v; }
	void				begin();
	void				decoyfood();
	bool				enter(int level, map_object_s stairs);
	item*				find(item_s v) const;
	int					get(skill_s v) const;
	const dungeoni*		getdungeon() const;
	int					getmoney() const { return 0; }
	int					getreputation() const { return reputation; }
	unsigned			getrounds() const { return rounds; }
	void				intialize();
	bool				is(variant v) const;
	bool				isoutdoor() const { return outdoor_id == Blocked; }
	void				move(indext index);
	static void			help();
	void				passminute();
	void				play();
	void				randomencounter();
	bool				read();
	static void			setnobackground();
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
DECLENUM(landscape);
DECLENUM(map_object);
DECLENUM(skill);
DECLENUM(tile);
DECLENUM(race);
DECLENUM(slot);
DECLENUM(spell);
DECLENUM(state);
#include "main.h"

static const char* talk_subjects[] = {"гномов", "хоббитов", "эльфов", "рыцарей", "троллей", "дракона", "колдуна", "трех друзей"};
static const char* talk_object[] = {"сокровище", "волшебное кольцо", "проклятый артефакт", "гору", "истинную любовь", "прекрасную куртизанку"};
static const char* talk_location[] = {"библиотеку", "ратушу", "магазин", "таверну", "храм"};
static const char* talk_games[] = {"кубики", "карты", "наперстки", "шарады"};
static item_s common_plants[] = {Tarnelle, Cherry, RoseHip, Physalis};

BSDATA(skilli) = {{"Торговля", "торговли", Charisma, {}, {Creature, {NotYou}, Close, "С кем хотите поторговаться?"}},
	{"Блеф", "обмана", Charisma, {}, {Creature, {NotYou}, Close, "Кого будете обманывать?"}},
	{"Дипломатия", "дипломатии", Charisma, {}, {Creature, {NotYou}, Close, "С кем будете вести переговоры?"}},
	{"Акробатика", "акробатики", Dexterity},
	{"Внимательность", "внимательности", Wisdow},
	{"Атлетика", "атлетики", Strenght, {}, {Object, {}, Close, "Какую дверь выбить?"}},
	{"Убийство", "убийства", Dexterity, {}, {Creature, {NotYou, Friends}, Close, "Кого убить?"}},
	{"Лазание", "лазания", Strenght},
	{"Концентрация", "концетрации", Wisdow},
	{"Обезвредить ловушки", "ловушек", Dexterity, {}, {Object, {}, Close, "Какую ловушку обезвредить?"}},
	{"Удар в уязвимое место", "боевых трюков", Dexterity},
	{"Слышать звуки", "слуха", Wisdow},
	{"Прятаться в тени", "скрытности", Dexterity, {}, {Creature}},
	{"Открыть замок", "взлома", Dexterity, {}, {Object, {}, Close, "Какой замок вскрыть?"}},
	{"Двигаться тихо", "тихого движения", Dexterity},
	{"Очистить карманы", "воровства", Dexterity, {}, {Creature, {NotYou}, Close, "Кого обворовать?"}},
	{"Алхимия", "алхимии", Intellegence, {}, {Item, {LongAction, AlwaysChoose}, You, "Какое зелье хотите попробывать?"}},
	{"Готовка еды", "куховарения", Charisma, {}, {Item, {AlwaysChoose}, You, "Что хотите приготовить?"}},
	{"Танцы", "танцев", Dexterity, {}, {Creature, {NotYou, AllTargets}, Near}},
	{"Инженерное дело", "инженерии", Intellegence},
	{"Азартные игры", "азартных игр", Charisma, {}, {Creature, {Friends, NotYou}, Close, "С кем поиграть?"}},
	{"История", "истории", Intellegence, {}, {Object, {}, Reach, "Какой объект изучить?"}},
	{"Заживание ран", "здоровья", Wisdow},
	{"Травознавство", "травознания", Wisdow, {}, {Object, {}, Close, "Где собрать урожай?"}},
	{"Грамотность", "письма и чтения", Intellegence, {}, {Item, {LongAction, AlwaysChoose}, You, "Что хотите прочитать?"}},
	{"Шахтерское дело", "шахтерского дела", Strenght, {}, {Object, {LongAction}, Close, "Где добывать руду?"}},
	{"Религия", "религии", Wisdow, {}, {Creature, {}, You}},
	{"Езда верхом", "езды верхом", Dexterity},
	{"Кузнечное дело", "кузнечного дела", Strenght, {}, {Item, {LongAction}, You, "Что хотите отремонтировать?"}},
	{"Выживание", "выживания", Wisdow},
	{"Плавание", "плавания", Strenght},
	{"Владение луком", "стрельбы из лука", Dexterity, {3, 30, 30}},
	{"Владение мечом", "сражения на мечах", Strenght, {4, 20, 25}},
	{"Владение топором", "сражения на топорах", Strenght, {5, 15, 30}},
	{"Владение двуручным оружием", "двуручного оружия", Strenght, {4, 10, 35}},
	{"Сражение двумя оружиями", "ужасного оружия", Dexterity, {5, 30, 15}},
};
assert_enum(skilli, LastSkill)

skill_s	skilli::getid() const {
	return skill_s(this - bsdata<skilli>::elements);
}

bool creature::use(skill_s id, creature& player, int order, bool run) {
	site* pst;
	auto& ei = bsdata<skilli>::elements[id];
	switch(id) {
	case HideInShadow:
		if(is(Invisible))
			return false;
		if(run) {
			if(!player.roll(id)) {
				if(isactive())
					sb.add("Попытка не удалась.");
				wait(CoupleMinutes);
				return false;
			}
			add(Invisible, 1, true);
		}
		break;
	case Diplomacy:
		pst = getsite();
		if(pst) {
			if(pst->getowner() != this)
				return false;
			if(!execute(MakeDiscount, false))
				return false;
			if(run) {
				if(!player.roll(id)) {
					static const char* talk[] = {"Ой, не смеши меня.", "Эй, хватит торговаться.", "Давай, давай, проходи."};
					say(maprnd(talk));
				} else
					execute(MakeDiscount, true);
			}
		} else {

		}
		break;
	case Dancing:
		if(is(Hostile) || is(Friendly) || mood < 0 || !is(LowInt))
			return false;
		if(run) {
			if(player.roll(id)) {
				static const char* talk[] = {"Красиво.", "Неплохо!", "Молодец, давай еще танцуй!", "Браво!"};
				say(maprnd(talk));
				mood += xrand(2, 4);
			} else if(d100() < 50) {
				static const char* talk[] = {"Фууу.", "Уберите, ЭТО!", "Как я могу это развидеть?", "Какая гадость."};
				say(maprnd(talk));
				mood -= xrand(1, 3);
			}
			wait(Minute);
		}
		break;
	case Gambling:
		if(!skills[Gambling] || loc.getlight() <= -1)
			return false;
		if(run) {
			auto stack = 100 * (1 + player.get(Gambling) / 20);
			if(player.getmoney() < stack) {
				player.say("Дай [%2i] монет, сыграть в %1.", maprnd(talk_games), stack);
				return false;
			}
			player.say("Давай сыграем в %1?", maprnd(talk_games));
			if(getmoney() < stack) {
				say("Нет. Я на мели. В другой раз.");
				return false;
			}
			say("Идея неплохая. Я только за.");
			auto result = rollv(player.get(Gambling), get(Gambling));
			if(result > 0) {
				player.setmoney(player.getmoney() + stack);
				setmoney(getmoney() - stack);
				player.act("%герой выиграл%а [%1i] монет.", stack);
				player.addexp(10);
			} else if(result < 0) {
				player.setmoney(player.getmoney() - stack);
				setmoney(getmoney() + stack);
				player.act("%герой с дребезгом проиграл%а [%1i] монет.", stack);
			} else
				player.act("Игра не зашла. Все остались при своем.");
		}
		break;
	case Religion:
		if(!faith)
			return false;
		if(!pray(false))
			return false;
		if(run) {
			if(roll(Religion)) {
				act("%герой помолил%ась богу и случилось чудо.");
				pray(true);
			} else
				act("%герой помолил%ась богу, но молитва не была услышана.");
			faith--;
		}
		break;
	case PickPockets:
		if(getmoney() <= 20 || loc.getlight() <= -1)
			return false;
		if(run) {
			static const char* talk[] = {
				"Слушай смешной анекдот. Так ... как же он начинается? ... Забыл. Ладно, давай в другой раз расскажу.",
				"О - смотри кто это?",
				"Вы не знаете как пройти в %3? О, спасибо, я сам вспомнил дорогу.",
				"Слышал эту историю про %1 и %2? Нет? Я тоже..."
			};
			player.say(maprnd(talk), maprnd(talk_subjects), maprnd(talk_object), maprnd(talk_location));
			if(player.roll(PickPockets)) {
				auto count = xrand(3, 18) * (1 + player.get(PickPockets) / 5);
				if(count > getmoney())
					count = getmoney();
				setmoney(getmoney() - count);
				player.setmoney(player.getmoney() + count);
				player.act("%герой украл%а [%1i] монет.", count);
				player.addexp(100);
			} else {
				static const char* talk[] = {
					"Ах ты наглый вор! На тебе по морде!",
					"Ах тыж гадина! Руки убери! На, получай!",
					"Ну нет. Вот тебе, козлина, будешь знать как воровать!"
				};
				say(maprnd(talk));
				player.damage(xrand(1, 3), Bludgeon, 100, true);
			}
		}
		break;
	default: return false;
	}
	return true;
}

bool item::use(skill_s id, creature& player, int order, bool run) {
	switch(id) {
	case Alchemy:
		if(!is(Drinkable) || is(KnownPower))
			return false;
		if(run) {
			if(!player.roll(Alchemy)) {
				player.act("%герой пригубил%а %1, но так и не смог%ла понять что это.", getname());
				player.fail(Alchemy);
				return false;
			}
			set(KnownPower);
			char temp[260]; stringbuilder sb(temp); getname(sb, true);
			player.say("Похоже по вкусу на [%1].", temp);
		}
		break;
	case Cooking:
		if(!is(Edible))
			return false;
		if(run) {
			player.act("%герой приготовил%а %1.", getname());
			if(!player.roll(id)) {
				player.fail(id);
				return false;
			}
		}
		break;
	case Smithing:
		if(!is(Iron) || !isdamaged())
			return false;
		if(run) {
			if(!player.roll(id)) {
				player.fail(id);
				return false;
			}
			player.act("%герой починил%а %1.", getname());
			repair();
		}
		break;
	case Literacy:
		if(!is(Readable))
			return false;
		if(run) {
			auto v = geteffect();
			auto level = getbonus();
			auto b = (geti().quality + level) * 3;
			if(is(Unknown))
				b += 10;
			else if(is(SingleUse))
				b += 80;
			if(is(Blessed))
				b += 10;
			else if(is(Artifact))
				b += 25;
			if(!is(SingleUse)) {
				switch(v.type) {
				case Spell:
					b -= player.get((spell_s)v.value) * 12;
					break;
				}
			}
			auto result = player.roll(Literacy, b);
			if(is(Unknown)) {
				player.act("%герой внимательно изучал%а %-1.", getname());
				if(result) {
					player.addexp(10);
					set(KnownPower);
					if(player.isactive()) {
						char temp[260]; stringbuilder st(temp); getname(st, true);
						sb.adds("Вам удалось понять, что это [%+1].", temp);
					}
				} else {
					if(player.isactive()) {
						player.act("Однако, вам так и не удалось ничего понять.");
						player.fail(Literacy);
					}
				}
			} else {
				switch(v.type) {
				case Skill:
					break;
				case Ability:
					if(is(SingleUse)) {
						if(bsdata<itemi>::elements[type].skill == Alchemy)
							player.learnreceipt(v);
						destroy(Magic, true);
					}
					break;
				case Spell:
					if(is(SingleUse)) {
						if(is(Blessed) || is(Artifact))
							result = true;
						if(!result || !player.use((spell_s)v.value, level, this, false))
							player.act("%герой вытащил%а %-1 и громко прочитал%а.", getname());
						destroy(Magic, true);
					} else {
						if(!player.askyn("Чтение займет продолжительное время. Действительно хотите продолжить?"))
							return false;
						player.act("%герой достал%а %-1 и занял%ась чтением.", getname());
						player.wait(Hour);
						if(result)
							player.add(v, 1, true);
						else {
							static effecti read_books[] = {
								{{}, 0, "Почти все что было написано вы не поняли. Может стоит попробывать перечитать еще раз?"},
								{{}, 0, "Вы прочитали несколько десятков страниц. Сколько из этого усвоили? Ровно ноль."},
								{ManaPoints, -3, "Книга оказалось неинтересной, а чтение оказалось слишком утомительным. Вы даже немного разозлились."},
								{ManaPoints, -4, "В книге было описано множество скучных вещей и вы заскучали."},
							};
							player.add(maprnd(read_books), getkind());
						}
						use();
					}
					break;
				}
			}
		}
		break;
	default: return false;
	}
	return true;
}

bool location::use(indext index, skill_s id, creature& player, int level, int order, bool run) {
	switch(id) {
	case Athletics:
		if(getobject(index) != Door || !is(index, Sealed) || is(index, Opened) || is(index, Hidden))
			return false;
		if(run) {
			// Random bonus for door to open
			if(!player.roll(id, (getrand(index) % 21) - 10)) {
				player.act("%герой ударил%а двери, но те не поддались.");
				player.fail(id);
				return false;
			}
			player.act("%герой разнес%ла двери в щепки.");
			set(index, NoTileObject);
			player.addexp(20);
		}
		break;
	case DisarmTraps:
		if(getobject(index) != Trap || is(index, Hidden))
			return false;
		if(run) {
			if(!player.roll(id)) {
				player.act("%герой попытал%ась обезвредить ловушку, но что-то пошло не так.");
				player.fail(id);
				return false;
			}
			player.act("%герой обезвредил%а ловушку.");
			set(index, NoTileObject);
			player.addexp(50);
		}
		break;
	case Herbalism:
		if(getobject(index) != Plants || !getplantgrow(index))
			return false;
		if(run) {
			auto power = getplantgrow(index);
			auto type = maprnd(common_plants);
			auto it = player.craft(type, {}, id, power * 10);
			it.setcount(xrand(1, 3));
			if(d100() < 15 * power)
				set(index, NoTileObject);
			else
				random[index] = 0;
			player.addexp(10);
			player.act("%герой собрал%а %1.", it.getname());
			player.add(it, true, true);
		}
		break;
	case Lockpicking:
		if(getobject(index) != Door || !is(index, Sealed) || is(index, Opened) || is(index, Hidden))
			return false;
		if(run) {
			if(!player.roll(id, (getrand(index) % 31) - 15)) {
				player.act("%герой не смог%ла вскрыть замок.");
				player.fail(id);
				return false;
			}
			player.act("%герой взломал%а замок.");
			remove(index, Sealed);
			player.addexp(100);
		}
		break;
	default: return false;
	}
	return true;
}

bool creature::use(const creaturea& source, skill_s id) {
	auto v = get(id);
	if(v <= 0)
		return false;
	auto pu = isusedisable(id);
	if(pu) {
		if(isactive())
			sb.add(pu, getstr(id));
		return false;
	}
	auto& ei = bsdata<skilli>::elements[id];
	creaturea creatures = source; itema items; indexa indecies;
	if(!ei.target.prepare(*this, creatures, items, indecies, id, get(id), true))
		return false;
	ei.target.use(*this, source, creatures, items, indecies, id, v);
	// Appear when do some activity
	if(is(Invisible)) {
		if(ei.target.range != You)
			appear();
	}
	wait();
	return true;
}

const char* creature::isusedisable(skill_s id) const {
	auto& ei = bsdata<skilli>::elements[id];
	switch(id) {
	case PickPockets:
		if(hp < get(LifePoints))
			return "Вы не можете воровать пока ваше здоровье ниже максимума. Восстановите здоровье и попробуйте снова.";
		return 0;
	case Healing: return "Используется автоматически восстанавливая 1 очко [жизни] за определенный промежуток времени.";
	case Concetration: return "Используется автоматически восстанавливая 1 очко [маны] за определенный промежуток времени.";
	default:
		if(ei.isweapon())
			return "Навык владения оружием влияет на шанс [попадания], наносимый [урон], [скорость] проведения удара и используется автоматически где это применимо.";
		if(!ei.target)
			return "Этот навык используется автоматически.";
		return 0;
	}
}
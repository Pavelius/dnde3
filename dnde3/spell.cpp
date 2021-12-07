#include "main.h"

BSDATA(spelli) = {
	{"Волшебная броня", "волшебной брони", 10, {Creature}},
	{"Благословение", "благословения", 6, {Creature, {NotYou, Friends, AllTargets}, Reach}, {1, 3}},
	{"Зачаровать предмет", "зачарования предмета", 30, {Item, {AlwaysChoose}, You, "Какой предмет сделать волшебным?"}},
	{"Поговорить с персоной", "разговора", 0, {Creature, {NotYou}, Close, "С кем вы хотите поговорить?"}},
	{"Очаровать персону", "шарма", 20, {Creature, {NotYou, Enemies, RandomTargets, MultiplyTarget}, Near, "Кто станет вашим другом?"}},
	{"Опьянение", "бренди", 5, {Creature, {NotYou, Enemies, RandomTargets}, Near, "Кто станет пьяным вдрызг?"}},
	{"Обнаружить зло", "обнаружения зла", 10, {Item, {RandomTargets, MultiplyTarget}}},
	{"Обнаружить волшебство", "обнаружения магии", 10, {Item, {RandomTargets, MultiplyTarget}}},
	{"Подчинение", "подчинения", 30, {Creature, {NotYou, Enemies, RandomTargets}, Everywhere, "Кого хотите поработить?"}},
	{"Страх", "страха", 10, {Creature, {Enemies, MultiplyTarget}, Near}},
	{"Шар огня", "шара огня", 6, {Creature, {Enemies, TargetArea}, Far}, {3, 18}, Fire, "вылетел большой шар огня, поразивший цель"},
	{"Исцеление", "исцеления", 4, {Creature, {Friends}, Reach, "Кому вылечить ранения?"}, {1, 6}},
	{"Опознание", "опознания", 20, {Item, {AlwaysChoose}, You, "Какой предмет хотите опознать?"}},
	{"Невидимость", "невидимости", 8, {Creature, {}, You}},
	{"Открытие дверей", "открытия дверей", 1, {Object, {AllTargets}, Near}},
	{"Свет", "света", 1, {Creature, {Friends}, Near, "Где появится свет?"}},
	{"Волшебный заряд", "волшебного заряда", 3, {Creature, {Enemies}, Near}, {1, 4}, Magic, "вылетел светящийся шарик, поразивший цель"},
	{"Отравление", "яда", 2, {Creature, {Enemies}, Close}},
	{"Починка", "починки", 2, {Item, {}, You, "Какой предмет починить?"}},
	{"Убрать яд", "исцеления яда", 10, {Creature, {Friends}, Close, "У кого убрать яд?"}},
	{"Убрать болезнь", "исцеления болезни", 20, {Creature, {Friends}, Close, "Кого излечить от болезни?"}},
	{"Болезнь", "болезни", 10, {Creature, {Enemies}, Close, "Кого заразить?"}},
	{"Волшебный щит", "щита", 6, {Creature, {Friends}, Close, "На кого наложить щит?"}},
	{"Шокирующая хватка", "электричества", 3, {Creature, {Enemies}, Close}, {1, 8}, Electricity, "сорвался элекрический заряд"},
	{"Сон", "сна", 10, {Creature, {Enemies, MultiplyTarget, NotYou}, Reach}},
	{"Замедлить монстра", "замедления", 12, {Creature, {Enemies, RandomTargets}, Near}},
	{"Призвать союзника", "призыва", 10, {Creature, {MultiplyTarget}, You}},
	{"Паутина", "паутины", 10, {Creature, {Enemies, MultiplyTarget}, Near}},
};
assert_enum(spelli, LastSpell)

bool creature::use(spell_s id, creature& player, int level, int order, bool run) {
	if(is(id))
		return false; // Not allow two spells be effected
	auto& ei = bsdata<spelli>::elements[id];
	switch(id) {
	case ArmorSpell:
		if(run) {
			add(Armor, 4, false, 60 * level);
			act("%герой озарил%ась белым сиянием.");
		}
		break;
	case BlessSpell:
		if(run) {
			add(Attack, level * 5, false, 30);
			add(Damage, 1 + level, false, 30);
			act("%герой испытал%а небывалый прилив сил.");
		}
		break;
	case ChatPerson:
		if(run)
			player.chat(*this);
		break;
	case CharmPerson:
		if(match(Animal) || isimmune(Charm))
			return false;
		if(run) {
			if(charmresist())
				return false;
			setfriendlyto(player);
			setguard(Blocked);
			say("%1, друг мой, я тебе помогу!", player.getname());
		}
		break;
	case Domination:
		if(match(Animal) || isimmune(Charm) || isactive())
			return false;
		if(run) {
			if(charmresist())
				return false;
			if(is(Friendly))
				damage(xrand(2, 7), Magic, 100);
			else {
				actev(sb, "%герой был%а подчинен%а враждебной силой.", 0, true);
				activate();
			}
		}
		break;
	case FearSpell:
		if(isimmune(Charm))
			return false;
		if(run) {
			if(charmresist())
				return false;
			add(Fear, 1, true);
		}
		break;
	case HealingSpell:
		if(hp >= get(LifePoints))
			return false;
		if(run)
			damage(-level, Magic, 0, true);
		break;
	case Invisibility:
		if(run) {
			add(Invisible, 1, true);
			add(id, level * 30);
		}
		break;
	case LightSpell:
		if(run) {
			add(Visibility, 1, true, level * 60);
			act("Около %героя появилась маленькая волшебная сфера света.");
		}
		break;
	case PoisonSpell:
		if(run) {
			if(resist(Poison, 0, true))
				return false;
			if(is(Poisoned)) {
				poison += xrand(1, 3);
				act("%герой получил%а дополнительную дозу яда!");
			} else {
				poison += xrand(0, 2);
				add(Poisoned, 1, true);
			}
		}
		break;
	case DrunkenSpell:
		if(run) {
			if(resist(Poison, 0, false))
				return false;
			if(is(Drunken)) {
				poison += xrand(2, 5);
				act("%герой получил%а дополнительную дозу алкоголя!");
			} else {
				poison += xrand(3, 6);
				add(Drunken, 1, true);
			}
		}
		break;
	case RemovePoisonSpell:
		if(!is(Poisoned) && !is(Drunken))
			return false;
		if(run) {
			add(Poisoned, -1, true);
			add(Drunken, -1, true);
			poison = 0;
		}
		break;
	case RemoveSickSpell:
		if(!is(Sick))
			return false;
		if(run)
			add(Sick, -1, true);
		break;
	case SickSpell:
		if(is(Sick))
			return false;
		if(run) {
			if(resist(Poison, 10 - level * 5, false)) {
				act("%герой противостоял%а болезни.");
				return false;
			}
			add(Sick, 1, true);
		}
		break;
	case ShieldSpell:
		if(run) {
			add(Protection, 20 * level, false, 20);
			act("Вокруг %героя появилось защитное поле.");
		}
		break;
	case Sleep:
		if(isimmune(Charm) || is(Sleep))
			return false;
		if(run) {
			if(charmresist(15 * order))
				return false;
			add(Sleep, xrand(2 * level, 8 * level));
			act("%герой внезапно заснул%а.");
		}
		break;
	case SlowMonster:
		if(run)
			add(Movement, -level, true, 60);
		break;
	case SummonAlly:
		if(run) {
			for(auto i = 0; i < level; i++) {
				auto p = bsdata<creature>::addz();
				if(!p)
					continue;
				auto index = loc.getfree(getposition());
				p->create(GoblinWarrior);
				p->setposition(index);
				p->setfriendlyto(player);
				p->add(Summoned, 1, false);
			}
		}
		break;
	case Web:
		if(run) {
			auto i = getposition();
			auto x = loc.getx(i);
			auto y = loc.gety(i);
			loc.fill(loc.normalize({x - 1, y - 1, x + 1, y + 1}), Webbed);
		}
		break;
	default:
		if(!ei.bonus)
			return false;
		return apply(player, ei.bonus, level, order, run);
	}
	return true;
}

bool item::use(spell_s id, creature& player, int level, int order, bool run) {
	switch(id) {
	case BlessItem:
		if(!is(Mundane) || is(Readable))
			return false;
		if(run) {
			set(Blessed);
			set(KnownMagic);
			act("%герой заискрил%ась многими желтыми огоньками.");
		}
		break;
	case DetectMagic:
		if(is(KnownMagic) || !is(Blessed))
			return false;
		if(run) {
			set(KnownMagic);
			act("%герой засветил%ась синим светом.");
		}
		break;
	case DetectEvil:
		if(is(KnownMagic) || !is(Cursed))
			return false;
		if(run) {
			set(KnownMagic);
			act("%герой засветил%ась красным светом.");
		}
		break;
	case Identify:
		if(is(KnownPower))
			return false;
		if(run) {
			act("%герой засветил%ась белыми огоньками.");
			set(KnownPower);
			if(player.isactive()) {
				char temp[260]; stringbuilder st(temp); getname(st, true);
				sb.adds("Это [%-1].", temp);
			}
		}
		break;
	case Repair:
		if(!isdamaged() || getdamage() > level)
			return false;
		if(run) {
			damaged--;
			act("%герой волшебным образом соединил%а поврежденные части.");
		}
		break;
	default:
		return false;
	}
	return true;
}

bool location::use(indext index, spell_s id, creature& player, int level, int order, bool run) {
	switch(id) {
	case KnockDoor:
		if(getobject(index) != Door || is(index, Opened))
			return false;
		if(run) {
			set(index, Opened);
			remove(index, Sealed);
			if(order == 0)
				player.act("Двери волшебным образом открылись.");
		}
		break;
	}
	return true;
}

bool creature::use(const creaturea& source, spell_s id, int level, item* magic_source, bool show_errors) {
	if(!(*this))
		return true;
	auto& ei = bsdata<spelli>::elements[id];
	if(magic_source) {
		if(magic_source->ischargeable() && !magic_source->getcharges()) {
			info("Не хватает зарядов.");
			return false;
		}
	} else if(mp < ei.mp) {
		info("Не хватает маны.");
		return false;
	}
	variant effect = id;
	auto v = ei.dice.roll();
	if(!ei.target.is(MultiplyTarget))
		v += (level - 1) * 2;
	creaturea creatures = source;
	itema items;
	indexa indecies;
	if(!ei.target.prepare(*this, creatures, items, indecies, id, v, show_errors))
		return false;
	if(magic_source) {
		if(ei.target.type == Creature && ei.target.range != You)
			act("%герой выставил%а %-1 перед собой.", magic_source->getname());
		else if(magic_source->is(Readable))
			act("%герой достал%а %-1 и громко прочитал%а.", magic_source->getname());
		else
			act("%герой достал%а %-1 и взмахнула несколько раз.", magic_source->getname());
	} else
		act("%герой крикнул%а волшебную формулу.");
	if(ei.throw_text) {
		if(magic_source)
			act("С кончика %1.", ei.throw_text);
		else
			act("С кончика пальцев %1.", ei.throw_text);
	}
	ei.target.use(*this, source, creatures, items, indecies, id, v);
	if(magic_source) {
		if(magic_source->ischargeable())
			magic_source->use();
	} else
		paymana(ei.mp, false);
	return true;
}

bool creature::use(spell_s id, int level, item* magic_source, bool show_errors) {
	creaturea creatures(*this);
	return use(creatures, id, level, magic_source, show_errors);
}
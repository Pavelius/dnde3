#include "main.h"

bool item::isboost(variant id) const {
	// Simple case, when item have effect
	if(is(KnownPower)) {
		auto e = geteffect();
		if(e == id)
			return true;
	}
	// Check for food
	for(auto& e : bsmeta<foodi>()) {
		if(e.type != type)
			continue;
		for(auto& c : e.chances) {
			if(c.id == id)
				return true;
		}
	}
	return false;
}

bool creature::use(item& it, bool interactive) {
	bool consume = true;
	variant effect;
	switch(it.getitem().slot) {
	case Edible:
		act("%герой съел%а %-1.", it.getname());
		for(auto& e : bsmeta<foodi>()) {
			if(!e.match(this, it))
				continue;
			e.apply(this, it, interactive);
			break;
		}
		break;
	case Drinkable:
		act("%герой выпил%а %-1.", it.getname());
		effect = it.geteffect();
		if(effect) {
			auto v = 1;
			if(effect.type == Ability) {
				switch(effect.value) {
				case ManaPoints:
				case LifePoints:
					v = xrand(8, 16);
					break;
				case Attack: case Deflect: case Protection:
					v = xrand(5, 10);
					break;
				}
			}
			add(effect, it.getkind(), v, true, it.getmagic(), it.getquality(), it.getdamage(), 120);
		} else {
			if(interactive)
				act("Ќичего не произошло.");
		}
		break;
	case Zapable:
		consume = false;
		if(true) {
			auto v = it.geteffect();
			if(v.type == Spell) {
				auto level = it.getbonus();
				if(level > 0) {
					if(cast((spell_s)v.value, level, &it))
						it.set(KnownPower);
					else
						act("%герой вытащил%а %-1 и махнул%а несколько раз. Ќичего не произошло.", it.getname());
				} else {
					it.act("%герой превратил%ась в пыль и рассыпал%ась.");
					consume = true;
				}
			}
		}
		break;
	}
	if(consume) {
		dressoff();
		it.clear();
		dresson();
	}
	if(!(*this))
		return true;
	wait();
	return true;
}

bool creature::use(creaturea& source, skill_s id) {
	auto v = get(id);
	if(v <= 0)
		return false;
	auto& ei = bsmeta<skilli>::elements[id];
	creaturea creatures = source; itema items; indexa indecies;
	if(!ei.target.prepare(*this, creatures, items, indecies, id, get(id))) {
		if(isactive()) {
			switch(id) {
			case Literacy:
				sb.add("¬ рюкзаке нету ничего, что можно почитать.");
				break;
			default:
				sb.add("Ќе могу найти подход€щие цели.");
				break;
			}
		}
		return false;
	}
	ei.target.use(*this, source, creatures, items, indecies, id, v);
	// Appear when do some activity
	if(is(Invisible)) {
		if(ei.target.range != You)
			appear();
	}
	wait();
	return true;
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
				set(KnownPower);
				act("%герой засветил%ась [-красным] светом.");
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
					sb.adds("Ёто [%-1].", temp);
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
				auto player_rang = 0;
				auto level = getbonus();
				auto consume = is(SingleUse);
				auto b = 0;
				if(is(Unknown))
					b += 10;
				if(is(SingleUse))
					b += 35 - level * 5;
				else {
					switch(id.type) {
					case Spell:
						player_rang = player.get((spell_s)id.value);
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
							sb.adds("¬ам удалось пон€ть, что это [%-1].", temp);
						}
					} else {
						if(player.isactive())
							player.act("ќднако, вам так и не удалось ничего пон€ть.");
					}
					consume = false;
				} else {
					switch(v.type) {
					case Spell:
						if(is(SingleUse)) {
							if(is(Blessed) || is(Artifact))
								result = true;
							if(!result || !player.cast((spell_s)v.value, level, this))
								player.act("%герой вытащил%а %-1 и громко прочитал%а.", getname());
						} else {

						}
						break;
					}
				}
				if(consume) {
					act("%герой превратил%ась в пыль и рассыпал%ась.");
					clear();
				}
			}
			break;
		}
		break;
	}
	return true;
}

bool creature::apply(creature& player, variant id, int v, int order, bool run) {
	spelli* si;
	skilli* sk;
	switch(id.type) {
	case Spell:
		if(finds(id))
			return false; // Not allow two spells be effected
		switch(id.value) {
		case ArmorSpell:
			if(run) {
				add(Armor, id, 1, false, 30 * v);
				act("%герой озарил%ась белым си€нием.");
			}
			break;
		case BlessSpell:
			if(run) {
				add(Attack, id, v * 5, false, 30);
				add(Damage, id, 2, false, 30);
				act("%герой испытал%а небывалый прилив сил.");
			}
			break;
		case CharmPerson:
			if(ismatch(Animal) || get(ResistCharm)>=100)
				return false;
			if(run) {
				if(charmresist())
					return false;
				if(player.is(Hostile))
					add(Hostile, 1, false);
				else
					add(Hostile, -1, false);
				setguard(Blocked);
				say("%1, друг мой, € тебе помогу!", player.getname());
			}
			break;
		case FearSpell:
			if(get(ResistCharm) >= 100)
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
				damage(-v, Magic, 0, true);
			break;
		case ShieldSpell:
			if(run) {
				add(Protection, id, 20 * v, false, 20);
				act("¬округ %геро€ по€вилось защитное поле.");
			}
			break;
		case SlowMonster:
			if(run) {
				add(Movement, id, v, false, 60);
			}
		default:
			si = bsmeta<spelli>::elements + id.value;
			if(!si->bonus)
				return false;
			return apply(player, si->bonus, v, order, run);
		}
		break;
	case Harm:
		if(run)
			damage(v, (damage_s)id.value, 0, true);
		break;
	case State:
		if(v >= 0) {
			if(is((state_s)id.value))
				return false;
		} else {
			if(!is((state_s)id.value))
				return false;
		}
		if(run)
			add(id, v, true);
		break;
	case Skill:
		sk = bsmeta<skilli>::elements + id.value;
		switch(id.value) {
		case Diplomacy:
			break;
		case HideInShadow:
			if(is(Invisible))
				return false;
			if(run) {
				if(!player.roll((skill_s)id.value)) {
					if(isactive())
						sb.add("ѕопытка не удалась.");
					wait(xrand(2, 6));
					return false;
				}
				add(Invisible, 1, true);
			}
			break;
		}
		break;
	}
	return true;
}
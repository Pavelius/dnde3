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
			if(!is(KnownMagic) || !is(Mundane))
				return false;
			if(run) {
				act("%герой заискрил%ась многими желтыми огоньками.");
				set(Blessed);
			}
			break;
		case DetectMagic:
			if(is(KnownMagic) || !is(Blessed))
				return false;
			if(run) {
				act("%герой засветил%ась синим светом.");
				set(KnownMagic);
			}
			break;
		case DetectEvil:
			if(is(KnownMagic) || !is(Cursed))
				return false;
			if(run) {
				act("%герой засветил%ась [-красным] светом.");
				set(KnownPower);
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
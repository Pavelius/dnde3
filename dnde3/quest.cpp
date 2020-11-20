#include "main.h"

static quest generate_character[] = {{1, {}, "Выбирайте, где вы родились?"},
{1, {Human}, "Вы родились в одной из деревень **Гиганского леса**.", 2},
{1, {Dwarf}, "Вашей родиной стали глубокие подземелья **Угрюмых утесов** - могущественной цитадели гномов.", 2},
{1, {Human}, "Вы родились на островах далеко за океаном.", 2},
{2, {}, "Кто были ваши родители?"},
{2, {}, "Члены семьи правителя, приблежанный к королю. Каждого знали в лицо во всем королевстве.", 3},
{2, {}, "Торговцы с рынка ценными товарами.", 3},
{2, {}, "Бедными попрашайками, живущими в трущебах, которым еле-еле хватало на еду. Они часто тебя били и употребляли спиртные напитки.", 3},
{}};

void quest::contexti::add(answeri& an, const quest* p) const {
	an.add((int)p, p->name);
}

void quest::contexti::add(const quest* p) const {
	sb.add(p->name);
}

const quest* quest::find(int index) const {
	for(auto p = this; *p; p++) {
		if(p->index == index)
			return p;
	}
	return 0;
}

static const quest* skip(const quest* p) {
	while(*p) {

	}
}

const quest* quest::choose(contexti& ei) const {
	answeri an;
	auto index = -1;
	for(auto p = this; *p; p++) {
		if(index == -1 && p->next)
			continue;
		if(index != -1 && (p->index != index || !p->next))
			break;
		if(!ei.apply(p, false))
			continue;
		index = p->index;
		if(p->next)
			ei.add(an, p);
		else {
			ei.apply(p, true);
			ei.add(p);
			while(!p[1].next)
				p++;
		}
	}
	if(!an)
		return 0;
	return (quest*)an.choosev(true, true, false, sb);
}

void quest::play(contexti& ei) const {
	auto p = this;
	while(p) {
		auto p1 = p->choose(ei);
		if(p1)
			ei.apply(p1, true);
		if(!p1 || p1->next==-1)
			break;
		p = find(p1->next);
	}
}

void gamei::begin() {
	quest::contexti ei;
	generate_character->play(ei);
}
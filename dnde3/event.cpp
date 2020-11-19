#include "main.h"

BSDATA(eventi) = {{1, 0, {Road}, "На обочине дороги вы замечаете одинокую повозку и осторожно к ней приближаетесь.\nЕдва вы подходите ближе, как веселый путник в широкопалой шляпе машет вам рукой.\n - Эй, там! Вы случайно ось чинить не умеете? Еду я по дороге, она вроде ровная без ухабов, а затем, БАХ - Мужчина хлопает в ладоши - разлетелась пополам. Мне через два дня нужно доставить весь этот товар на север, и я в толк не возьму что мне с этой бедой делать.", "Попытаться помочь путнику, починив сломанную ось.", "Связать путника и забрать товар, о котором он так печется."},
{1, 1, {Fighter, Dwarf, AddReputation}, "Пока вы работаете, путник не замолкает, но где-то через час-полтора вам удается починить ось, и он отбывает в прекрасном настроении, рассыпаясь в благодарностях."},
{1, 1, {}, "Вы с радостью соглашаетесь помочь, но быстро понимаете, что не знаете, что делать. Путник всеравно вас благодарит, и вы отправляетесь дальше."},
{1, 2, {AddMoney10, LoseReputation}, "Вы широко улыбаетесь и подходите к путнику. Он даже не сопротивляется. Первое, что вы делаете, - затыкаете его, прекращая бесконечную болтовню. Вы, выбирая самое легкое что у него есть из ценностей, и забираете с собой, оставив мужчину валяться связанным на обочине. Пожалуй, его день был не таким уж плохим, пока он не встретил вас."},
};
DECLFULL(eventi)

static eventi* find_start(int number, int type) {
	for(auto& e : bsmeta<eventi>()) {
		if(!e || e.index != number || e.type != type)
			continue;
		if(!e.isallow())
			continue;
		return &e;
	}
	return 0;
}

static bool iscondition(variant_s v) {
	return v == Race || v == Gender || v == Class || v == Tile;
}

bool eventi::isallow() const {
	auto deny = iscondition(actions[0].type);
	for(auto v : actions) {
		if(!v)
			break;
		switch(v.type) {
		case Tile:
			if(deny && game.isoverland()) {
				if(loc.gettile(game.getposition()) == v.value)
					deny = false;
			}
			break;
		case Race:
		case Gender:
		case Class:
			if(deny && game.is(v))
				deny = false;
			break;
		case Command:
			break;
		}
	}
	return !deny;
}

void eventi::apply() {
}

void eventi::play(int number) {
	auto p = find_start(number, 0);
	if(!p)
		return;
	answeri an;
	an.add(1, p->answer1);
	an.add(2, p->answer2);
	auto i = an.choosev(true, true, false, p->text);
	p = find_start(number, i);
	if(!p)
		return;
	sb.add(p->text);
}
#include "main.h"

struct chati {
	variant				v1;
	dialogi*			dialog;
	constexpr explicit operator bool() const { return dialog != 0; }
};

static dialogi party_member[] = {{1, Say, {Common}, "Чего тебе?"},
{1, Say, {Common}, "Что надо, друг?"},
{1, Say, {}, "Что там?"},
{1, Ask, {}, "Охраняй это место.", {GuardPosition}},
{1, Ask, {}, "Все нормально. Так просто решуил поболтать."},
{}};
static dialogi neutral_character[] = {{1, Say, {Common}, "Чего тебе?"},
{1, Say, {Common}, "Я рад теб приветствовать. Чем могу быть полезен?"},
{1, Say, {}, "Да?"},
{1, Ask, {}, "Присоединяйся к нам. Нам нужны отчаяные люди как ты."},
{1, Ask, {}, "Что ты тут делаешь?"},
{1, Ask, {}, "Пожалуй я пойду."},
{}};
static dialogi good_character[] = {{1, Say, {Common}, "Чего тебе?"},
{1, Say, {Common}, "Я рад теб приветствовать. Чем могу быть полезен?"},
{1, Say, {}, "Да?"},
{1, Ask, {}, "Присоединяйся к нам. Будем нести добро вместе."},
{1, Ask, {}, "Что ты тут делаешь?"},
{1, Ask, {}, "Пожалуй я пойду."},
{}};
static dialogi chat_commoner[] = {{1, Say, {Common}, "Хороший день, правда?"},
{1, Say, {Common}, "Знаете какие удои у коров были меяц назад?"},
{1, Say, {}, "Сена в этом году мало, как бы заготовить побольше."},
{1, Say, {}, "Вы знаете, наш управитель что-то скрывает?"},
{}};
static dialogi chat_child[] = {{1, Say, {Common}, "Мама мне не разреает общаться с незнакомцами."},
{1, Say, {Common}, "Как тебя зовут?"},
{1, Say, {Common}, "Ты такой большой."},
{1, Say, {}, "Не хочу разговаривать. Ты страшный."},
{}};
static dialogi chat_shopkeeper[] = {{1, Say, {Common}, "Как твой бизнес, друг?"},
{1, Say, {Uncommon}, "Хороший день, да?", {}, 3},
{1, Say, {}, "Чую запах денег."},
{1, Ask, {}, "Дай мне что-нибудь бесплатно.", {}, 2},
{1, Ask, {}, "Все хорошо. Было бы еще лучше если бы твои глупые шутки не орошали сегодняшний эфир.", {}, 2},
{2, Say, {}, "И почему я это должен сделать?"},
{2, Ask, {Strenght}, "Потому что я смогу сделать тебе больно. Подходит?"},
{2, Ask, {Diplomacy, 60}, "Так ты сделашь свой вклад в борьбу с хаосом, который сеeтся в округе."},
{2, Ask, {}, "И то верно. Причин нет. Видимо мне пора идти."},
{3, Say, {}, "Поэтому предлагаю такой хороший день не превращать в побиение моей охраной еще одного воришки и прохиндея. Договорились?"},
{3, Ask, {}, "Смело. Но ты хозяин и вправе заказывать музыку."},
{}};

static const chati* find_chat(const chati* pb, creature& player, creature& opponent) {
	for(auto p = pb; *p; p++) {
		if(opponent.ismatch(p->v1))
			return p;
	}
	return 0;
}

static dialogi* addask(creature& player, creature& opponent, const dialogi* source, int index) {
	answeri an;
	for(auto p = source; *p; p++) {
		if(p->index != index)
			continue;
		if(p->type != Ask)
			continue;
		if(!player.ismatch(opponent, p->conditions))
			continue;
		if(!player.ismatch(opponent, p->actions))
			continue;
		an.add((int)p, "\"%2\"", player.getname(), p->text);
	}
	if(an)
		return (dialogi*)an.choosev(true, true, false, sb);
	return 0;
}

static int addsay(creature& player, creature& opponent, const dialogi* source, int index) {
	for(auto p = source; *p; p++) {
		if(p->index != index)
			continue;
		if(p->type != Say)
			continue;
		if(!player.ismatch(opponent, p->conditions))
			continue;
		if(!player.ismatch(opponent, p->actions))
			continue;
		opponent.say(p->text);
		if(p->next) {
			player.pause();
			return p->next;
		}
		auto p1 = addask(player, opponent, source, index);
		if(p1) {
			for(auto v : p1->actions) {
				switch(v.type) {
				case Action: opponent.execute((action_s)v.value, true); break;
				}
			}
			return p1->next;
		}
		break;
	}
	return 0;
}

void creature::chat(creature& opponent, const dialogi* source) {
	auto index = 1;
	auto prev_index = -1;
	while(index > 0) {
		auto next = addsay(*this, opponent, source, index);
		if(next == -1) {
			next = prev_index;
			prev_index = 0;
		} else
			prev_index = index;
		index = next;
	}
}

void creature::chat(creature& opponent) {
	static chati available_chats[] = {{Shopkeeper, chat_shopkeeper},
	{HumanChild, chat_child},
	{HumanMale, chat_commoner},
	{}};
	if(saybusy())
		return;
	auto pd = find_chat(available_chats, *this, opponent);
	const dialogi* dg = 0;
	if(pd)
		dg = pd->dialog;
	else if(opponent.is(Friendly) && is(Friendly))
		dg = party_member;
	if(dg)
		chat(opponent, dg);
}

void creature::chat() {
	creaturea creatures(*this);
	bsmeta<spelli>::elements[ChatPerson].target.use(*this, creatures, ChatPerson, 0, true);
}
#include "main.h"

struct chati {
	variant				v1;
	const quest*		dialog;
	constexpr explicit operator bool() const { return dialog != 0; }
};

static quest party_member[] = {{1, {Common}, "Чего тебе?"},
{1, {Common}, "Что надо, друг?"},
{1, {}, "Что там?"},
{1, {Opponent, GuardPosition}, "Охраняй это место.", 2},
{1, {Opponent, StopGuardPosition}, "Пошли со мной.", 2},
{1, {}, "Все нормально. Так просто решил%а поболтать.", -1},
{}};
static quest neutral_character[] = {{1, {Common}, "Чего тебе?"},
{1, {Common}, "Я рад теб приветствовать. Чем могу быть полезен?"},
{1, {}, "Да?"},
{1, {}, "Присоединяйся к нам. Нам нужны отчаяные люди как ты.", 2},
{1, {}, "Что ты тут делаешь?", 2},
{1, {}, "Пожалуй я пойду.", -1},
{}};
static quest chat_commoner[] = {{1, {Common}, "Хороший день, правда?"},
{1, {Common}, "Знаете какие удои у коров были меяц назад?"},
{1, {Common}, "Сена в этом году мало, как бы заготовить побольше."},
{1, {}, "Вы знаете, наш управитель что-то скрывает?"},
{}};
static quest chat_child[] = {{1, {Common}, "Мама мне не разреает общаться с незнакомцами."},
{1, {Common}, "Как тебя зовут?"},
{1, {Common}, "Ты такой большой."},
{1, {}, "Не хочу разговаривать. Ты страшный."},
{}};
static quest chat_shopkeeper[] = {{1, {Common}, "Как твой бизнес, друг?"},
{1, {Uncommon}, "Хороший день, да?", 3},
{1, {}, "Чую запах денег."},
{1, {}, "Дай мне что-нибудь бесплатно.", 2},
{1, {}, "Все хорошо. Было бы еще лучше если бы твои глупые шутки не орошали сегодняшний эфир.", 2},
{2, {}, "И почему я это должен сделать?"},
{2, {Strenght}, "Потому что я смогу сделать тебе больно. Подходит?", 3},
{2, {Diplomacy}, "Так ты сделашь свой вклад в борьбу с хаосом, который сеeтся в округе.", 3},
{2, {}, "И то верно. Причин нет. Видимо мне пора идти.", -1},
{3, {}, "Поэтому предлагаю такой хороший день не превращать в побиение моей охраной еще одного воришки и прохиндея. Договорились?"},
{3, {}, "Смело. Но ты хозяин - и вправе заказывать музыку."},
{}};

static const chati* find_chat(const chati* pb, creature& player, creature& opponent) {
	for(auto p = pb; *p; p++) {
		if(opponent.ismatch(p->v1))
			return p;
	}
	return 0;
}

static void apply(creature& player, creature& opponent, const varianta& source) {
	for(auto v : source) {
		if(v.type != Action)
			continue;
		player.execute((action_s)v.value, true);
	}
}

void creature::chat(creature& opponent, const quest* source) {
	struct contexti : quest::contexti {
		creature*	opponent;
		creature*	player;
		void add(const quest* p) const override {
			opponent->say(p->name);
		}
		void add(answeri& an, const quest* p) const override {
			if(!an.addv((int)p, 0))
				return;
			auto& sb = an.getsb();
			sb.add(" -");
			player->actev(sb, p->name, 0);
		}
		bool match(const quest* p) const override {
			auto target = player;
			for(auto v : p->bonus) {
				if(!v)
					break;
				if(v.type == Modifier) {
					switch(v.value) {
					case Opponent: target = opponent; break;
					}
				} else {
					if(!target->ismatch(v))
						return false;
					target = player;
				}
			}
			return true;
		}
		void apply(const quest* p) override {
			auto target = player;
			for(auto v : p->bonus) {
				if(!v)
					break;
				if(v.type == Modifier) {
					switch(v.value) {
					case Opponent: target = opponent; break;
					}
				} else {
					if(v.type==Action)
						target->execute((action_s)v.value, true);
					target = player;
				}
			}
		}
	};
	contexti context;
	context.player = this;
	context.opponent = &opponent;
	source->play(context);
}

void creature::chat(creature& opponent) {
	static chati available_chats[] = {{Shopkeeper, chat_shopkeeper},
	{HumanChild, chat_child},
	{HumanMale, chat_commoner},
	{HumanFemale, chat_commoner},
	{}};
	if(saybusy())
		return;
	auto pd = find_chat(available_chats, *this, opponent);
	const quest* dg = 0;
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
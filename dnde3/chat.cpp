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
{1, {Opponent, UseLongActionSkill}, "Мы планируем немного отдохнуть. Не трать зря время и прямо сейчас займись чем-то долгим, на что ранше не хватало времени.", 3},
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
{1, {Uncommon}, "Хороший день, да?"},
{1, {}, "Чую запах денег."},
{1, {}, "Дай мне что-нибудь бесплатно.", 2},
{1, {}, "Все хорошо. Было бы еще лучше если бы твои глупые шутки не орошали сегодняшний эфир."},
{2, {}, "И почему я это должен сделать?"},
{2, {Strenght, Opponent, MakeDiscount}, "Потому что я смогу сделать тебе больно. Подходит?", -1},
{2, {Diplomacy, Opponent, MakeDiscount}, "Так ты сделашь свой вклад в борьбу с хаосом, который сеeтся в округе.", -1},
{2, {}, "И то верно. Причин нет. Видимо мне пора идти.", -1},
//{3, {}, "Уговорил!"},
//{3, {}, "Предлагаю такой хороший день не превращать в побиение моей охраной еще одного воришки и прохиндея. Договорились?"},
//{4, {MakeDiscount}, "Борбу с хаосом в округе? Хаос это Я!"},
{}};

static const chati* find_chat(const chati* pb, creature& player, creature& opponent) {
	for(auto p = pb; *p; p++) {
		if(opponent.match(p->v1))
			return p;
	}
	return 0;
}

void creature::chat(creature& opponent, const quest* source) {
	struct contexti : quest::contexti {
		creature*	opponent;
		creature*	player;
		char		base_bonus;
		void add(const quest* p) const override {
			opponent->say(p->name);
		}
		void add(answeri& an, const quest* p) const override {
			if(!an.addv((int)p, 0))
				return;
			auto& sb = an.getsb();
			sb.add("\"");
			player->actev(sb, p->name, 0, false);
			sb.add("\"");
		}
		bool apply(const quest* p, bool run) override {
			contexti push = *this;
			for(auto v : p->bonus) {
				if(!v)
					break;
				switch(v.type) {
				case Modifier:
					switch(v.value) {
					case Opponent: player = push.opponent; opponent = push.player; break;
					case Easy: base_bonus = 30; break;
					case Hard: base_bonus = 80; break;
					}
					continue;
				case Rarity:
					if(!run) {
						if(d100() > bsmeta<rarityi>::elements[v.value].chance) {
							*this = push;
							return false;
						}
					}
					break;
				case Skill:
					if(!player->ismatch(*opponent, (skill_s)v.value, base_bonus)) {
						*this = push;
						return false;
					}
					break;
				case Ability:
					if(player->get((ability_s)v.value) < opponent->get((ability_s)v.value)) {
						*this = push;
						return false;
					}
					break;
				case Action:
					if(!player->execute((action_s)v.value, run)) {
						*this = push;
						return false;
					}
					break;
				default:
					if(!player->match(v)) {
						*this = push;
						return false;
					}
					break;
				}
				*this = push;
			}
			return true;
		}
	};
	contexti ei;
	ei.player = this;
	ei.opponent = &opponent;
	ei.base_bonus = 50;
	source->play(ei);
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
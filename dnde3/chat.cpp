#include "main.h"

static quest party_member[] = {{1, {}, "Чего тебе?"},
	{1, {}, "Что надо, друг?"},
	{1, {}, "Что там?"},
	{1, {Opponent, GuardPosition}, "Охраняй это место.", 2},
	{1, {Opponent, StopGuardPosition}, "Пошли со мной.", 2},
	{1, {Opponent, UseLongActionSkill}, "Мы планируем немного отдохнуть. Не трать зря время и прямо сейчас займись чем-то долгим, на что ранше не хватало времени.", 3},
	{1, {}, "Все нормально. Так просто решил%а поболтать.", -1},
	{}};
static quest all_chat[] = {
	{1, {Random, Opponent, ShopWeaponAndArmor}, "Мечи самое сбалансированное оружие. Есть огромный выбор. Двуручный меч медленный, но наносит больше всего урона. Кинжал можно использовать во второй руке и он достаточно быстрый."},
	{1, {Random, Opponent, ShopFood}, "Хлеб гномов та еще пища - напоминает вкус земли. Но для настоящего гнома это тот еще деликатес! Любой гном его обажает, ведь на них он сказывается благосклонно."},
	{1, {Random, Opponent, ShopFood}, "Моя рекомендация - есть больше яблок. Чем больше ты их будешь есть, тем [сильнее] станешь."},
	{1, {Random, Opponent, ShopFood}, "Колбаса наиболее питательный продукт. Она укрепляет здоровье и повышает самочуствие. Еш больше колбасы и у тебя будет высокое [телосложение]."},
	{1, {Random, Opponent, ShopFood}, "Если где найдешь хлеб хоббитов, помни - этот продукт сделан из экологически чистого материала и очень хорошо усваивается организмом. Говорят, что те кто его едят много [хорошо выглядят] и имеют необычайный заряд бодрости."},
	{1, {Opponent, Owner}, "Заходи. Смотри. Выбирай."},
	{1, {Random, Opponent, HumanChild}, "Мама мне не разреает общаться с незнакомцами."},
	{1, {Random, Opponent, HumanChild}, "Как тебя зовут?"},
	{1, {Random, Opponent, HumanChild}, "Ты такой большой!"},
	{1, {Opponent, HumanChild}, "Не хочу разговаривать. Ты страшный."},
	{1, {Random, Opponent, HumanGuard}, "Я за тобой слежу."},
	{1, {Random, Opponent, HumanGuard}, "Я есть закон."},
	{1, {Random, Opponent, HumanGuard}, "Что? Куда он побежал?"},
	{1, {Random, Opponent, HumanGuard}, "Нужны проблемы?"},
	{1, {Random, Opponent, HumanFemale}, "Знаете какие удои у коров были меяц назад?"},
	{1, {Random, Opponent, HumanMale}, "Сена в этом году мало, как бы заготовить побольше."},
	{1, {Random}, "Чего-то хотите?"},
	{1, {}, "Хороший день, да?"},
	{}};

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
					case Random: if(d100() < 30) return false; break;
					case Opponent: player = push.opponent; opponent = push.player; break;
					case Easy: base_bonus = 30; break;
					case Hard: base_bonus = 80; break;
					}
					continue;
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
	if(saybusy())
		return;
	const quest* dg = all_chat;
	if(opponent.is(Friendly) && is(Friendly))
		dg = party_member;
	chat(opponent, dg);
}

void creature::chat() {
	creaturea creatures(*this);
	bsdata<spelli>::elements[ChatPerson].target.use(*this, creatures, ChatPerson, 0, true);
}
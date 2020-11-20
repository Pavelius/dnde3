#include "main.h"

struct chati {
	variant				v1;
	const quest*		dialog;
	constexpr explicit operator bool() const { return dialog != 0; }
};

static quest party_member[] = {{1, {Common}, "���� ����?"},
{1, {Common}, "��� ����, ����?"},
{1, {}, "��� ���?"},
{1, {Opponent, GuardPosition}, "������� ��� �����.", 2},
{1, {Opponent, StopGuardPosition}, "����� �� ����.", 2},
{1, {Opponent, UseLongActionSkill}, "�� ��������� ������� ���������. �� ����� ��� ����� � ����� ������ ������� ���-�� ������, �� ��� ����� �� ������� �������.", 3},
{1, {}, "��� ���������. ��� ������ �����%� ���������.", -1},
{}};
static quest neutral_character[] = {{1, {Common}, "���� ����?"},
{1, {Common}, "� ��� ��� ��������������. ��� ���� ���� �������?"},
{1, {}, "��?"},
{1, {}, "������������� � ���. ��� ����� �������� ���� ��� ��.", 2},
{1, {}, "��� �� ��� �������?", 2},
{1, {}, "������� � �����.", -1},
{}};
static quest chat_commoner[] = {{1, {Common}, "������� ����, ������?"},
{1, {Common}, "������ ����� ���� � ����� ���� ���� �����?"},
{1, {Common}, "���� � ���� ���� ����, ��� �� ���������� ��������."},
{1, {}, "�� ������, ��� ���������� ���-�� ��������?"},
{}};
static quest chat_child[] = {{1, {Common}, "���� ��� �� �������� �������� � ������������."},
{1, {Common}, "��� ���� �����?"},
{1, {Common}, "�� ����� �������."},
{1, {}, "�� ���� �������������. �� ��������."},
{}};
static quest chat_shopkeeper[] = {{1, {Common}, "��� ���� ������, ����?"},
{1, {Uncommon}, "������� ����, ��?"},
{1, {}, "��� ����� �����."},
{1, {}, "��� ��� ���-������ ���������.", 2},
{1, {}, "��� ������. ���� �� ��� ����� ���� �� ���� ������ ����� �� ������� ����������� ����."},
{2, {}, "� ������ � ��� ������ �������?"},
{2, {Strenght, Opponent, MakeDiscount}, "������ ��� � ����� ������� ���� ������. ��������?", -1},
{2, {Diplomacy, Opponent, MakeDiscount}, "��� �� ������� ���� ����� � ������ � ������, ������� ��e��� � ������.", -1},
{2, {}, "� �� �����. ������ ���. ������ ��� ���� ����.", -1},
//{3, {}, "��������!"},
//{3, {}, "��������� ����� ������� ���� �� ���������� � �������� ���� ������� ��� ������ ������� � ���������. ������������?"},
//{4, {MakeDiscount}, "����� � ������ � ������? ���� ��� �!"},
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
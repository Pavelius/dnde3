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
{1, {Uncommon}, "������� ����, ��?", 3},
{1, {}, "��� ����� �����."},
{1, {}, "��� ��� ���-������ ���������.", 2},
{1, {}, "��� ������. ���� �� ��� ����� ���� �� ���� ������ ����� �� ������� ����������� ����.", 2},
{2, {}, "� ������ � ��� ������ �������?"},
{2, {Strenght}, "������ ��� � ����� ������� ���� ������. ��������?", 3},
{2, {Diplomacy}, "��� �� ������� ���� ����� � ������ � ������, ������� ��e��� � ������.", 3},
{2, {}, "� �� �����. ������ ���. ������ ��� ���� ����.", -1},
{3, {}, "������� ��������� ����� ������� ���� �� ���������� � �������� ���� ������� ��� ������ ������� � ���������. ������������?"},
{3, {}, "�����. �� �� ������ - � ������ ���������� ������."},
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
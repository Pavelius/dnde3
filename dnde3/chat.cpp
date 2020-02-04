#include "main.h"

struct chati {
	variant				v1;
	dialogi*			dialog;
	constexpr explicit operator bool() const { return dialog != 0; }
};

static dialogi party_member[] = {{1, Say, {Common}, {}, "���� ����?"},
{1, Say, {Common}, {}, "��� ����, ����?"},
{1, Say, {}, {}, "��� ���?"},
{1, Ask, {}, {GuardPosition}, "������� ��� �����."},
{1, Ask, {}, {}, "��� ���������. ��� ������ ������ ���������."},
{}};
static dialogi neutral_character[] = {{1, Say, {Common}, {}, "���� ����?"},
{1, Say, {Common}, {}, "� ��� ��� ��������������. ��� ���� ���� �������?"},
{1, Say, {}, {}, "��?"},
{1, Ask, {}, {}, "������������� � ���. ��� ����� �������� ���� ��� ��."},
{1, Ask, {}, {}, "��� �� ��� �������?"},
{1, Ask, {}, {}, "������� � �����."},
{}};
static dialogi chat_commoner[] = {{1, Say, {Common}, {}, "������� ����, ������?"},
{1, Say, {Common}, {}, "������ ����� ���� � ����� ���� ���� �����?"},
{1, Say, {Common}, {}, "���� � ���� ���� ����, ��� �� ���������� ��������."},
{1, Say, {}, {}, "�� ������, ��� ���������� ���-�� ��������?"},
{}};
static dialogi chat_child[] = {{1, Say, {Common}, {}, "���� ��� �� �������� �������� � ������������."},
{1, Say, {Common}, {}, "��� ���� �����?"},
{1, Say, {Common}, {}, "�� ����� �������."},
{1, Say, {}, {}, "�� ���� �������������. �� ��������."},
{}};
static dialogi chat_shopkeeper[] = {{1, Say, {Common}, {}, "��� ���� ������, ����?"},
{1, Say, {Uncommon}, {}, "������� ����, ��?", 3},
{1, Say, {}, {}, "��� ����� �����."},
{1, Ask, {}, {}, "��� ��� ���-������ ���������.", 2},
{1, Ask, {}, {}, "��� ������. ���� �� ��� ����� ���� �� ���� ������ ����� �� ������� ����������� ����.", 2},
{2, Say, {}, {}, "� ������ � ��� ������ �������?"},
{2, Ask, {Strenght}, {}, "������ ��� � ����� ������� ���� ������. ��������?"},
{2, Ask, {Diplomacy, 60}, {}, "��� �� ������� ���� ����� � ������ � ������, ������� ��e��� � ������."},
{2, Ask, {}, {}, "� �� �����. ������ ���. ������ ��� ���� ����."},
{3, Say, {}, {}, "������� ��������� ����� ������� ���� �� ���������� � �������� ���� ������� ��� ������ ������� � ���������. ������������?"},
{3, Ask, {}, {}, "�����. �� �� ������ � ������ ���������� ������."},
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

static dialogi* addask(creature& player, creature& opponent, const dialogi* source, int index) {
	answeri an;
	for(auto p = source; *p; p++) {
		if(p->index != index)
			continue;
		if(p->type != Ask)
			continue;
		if(!player.ismatch(opponent, p->actions))
			continue;
		if(!opponent.ismatch(player, p->opponent_actions))
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
		if(!player.ismatch(opponent, p->actions))
			continue;
		if(!opponent.ismatch(player, p->opponent_actions))
			continue;
		opponent.say(p->text);
		if(p->next) {
			player.pause();
			return p->next;
		}
		auto p1 = addask(player, opponent, source, index);
		if(p1) {
			apply(player, opponent, p1->actions);
			apply(opponent, player, p1->opponent_actions);
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
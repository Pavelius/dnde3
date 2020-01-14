#include "main.h"

static dialogi chat_shopkeeper[] = {{1, Say, {Common}, "��� ���� ������, ����?"},
{1, Say, {Uncommon}, "������� ����, ��?", {}, 3},
{1, Say, {}, "��� ����� �����."},
{1, Ask, {}, "��� ��� ���-������ ���������.", {}, 2},
{1, Ask, {}, "��� ������. ���� �� ��� ����� ���� �� ���� ������ ����� �� ������� ����������� ����.", {}, 2},
{2, Say, {}, "� ������ � ��� ������ �������?"},
{2, Ask, {Strenght}, "������ ��� � ����� ������� ���� ������. ��������?"},
{2, Ask, {Diplomacy, 60}, "��� �� ������� ���� ����� � ������ � ������, ������� ��e��� � ������."},
{2, Ask, {}, "� �� �����. ������ ���. ������ ��� ���� ����."},
{3, Say, {}, "������� ��������� ����� ������� ���� �� ���������� � �������� ���� ������� ��� ������ ������� � ���������. ������������?"},
{3, Ask, {}, "�����. �� �� ������ � ������ ���������� ������."},
{}};

static dialogi* addask(creature& player, creature& opponent, const dialogi* source, int index) {
	answeri an;
	for(auto p = source; *p; p++) {
		if(p->index != index)
			continue;
		if(p->type != Ask)
			continue;
		if(!player.ismatch(opponent, p->conditions))
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
		opponent.say(p->text);
		if(p->next) {
			player.pause();
			return p->next;
		}
		auto p1 = addask(player, opponent, source, index);
		if(p1)
			return p1->next;
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
	if(saybusy())
		return;
	chat(opponent, chat_shopkeeper);
}

void creature::chat() {
	creaturea creatures(*this);
	bsmeta<spelli>::elements[ChatPerson].target.use(*this, creatures, ChatPerson, 0, true);
}
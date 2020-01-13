#include "main.h"

static dialogi chat_shopkeeper[] = {{1, Say, {Uncommon}, "��� ���� ������, ����?"},
{1, Say, {Uncommon}, "��� ����� �����."},
{1, Say, {}, "������� ����, ��?"},
{1, Ask, {}, "��� ��� ���-������ ���������.", {}, 2},
{2, Say, {}, "� ������ � ��� ������ �������?"},
{2, Ask, {Strenght}, "������ ��� � ����� ������� ���� ������. ��������?"},
{2, Ask, {Diplomacy, 60}, "��� �� ������� ���� ����� � ������ � ������, ������� ������ � ������."},
{2, Ask, {}, "� �� �����. ������ ���. ������ ��� ���� ����."},
{}};

static void addask(answeri& an, creature& player, creature& opponent, const dialogi* source, int index) {
	for(auto p = source; *p; p++) {
		if(p->index != index)
			continue;
		if(p->type != Ask)
			continue;
		if(!player.ismatch(opponent, p->conditions))
			continue;
		an.add((int)&p, p->text);
	}
}

static int addsay(creature& player, creature& opponent, const dialogi* source, int index) {
	for(auto p = source; *p; p++) {
		if(p->index != index)
			continue;
		if(p->type != Say)
			continue;
		if(!player.ismatch(opponent, p->conditions))
			continue;
		answeri answers;
		opponent.say(p->text);
		//addask(answers, )
		return p->next;
	}
	return -1;
}

void creature::chat(creature& opponent, const dialogi* source) {
	auto index = 1;
	auto prev_index = -1;
	while(index != -1) {
		auto next = addsay(*this, opponent, source, index);
		if(!next) {
			next = prev_index;
			prev_index = -1;
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
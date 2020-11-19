#include "main.h"

BSDATA(eventi) = {{1, 0, {Road}, "�� ������� ������ �� ��������� �������� ������� � ��������� � ��� �������������.\n���� �� ��������� �����, ��� ������� ������ � ����������� ����� ����� ��� �����.\n - ��, ���! �� �������� ��� ������ �� ������? ��� � �� ������, ��� ����� ������ ��� ������, � �����, ��� - ������� ������� � ������ - ����������� �������. ��� ����� ��� ��� ����� ��������� ���� ���� ����� �� �����, � � � ���� �� ������ ��� ��� � ���� ����� ������.", "���������� ������ �������, ������� ��������� ���.", "������� ������� � ������� �����, � ������� �� ��� �������."},
{1, 1, {Fighter, Dwarf, AddReputation}, "���� �� ���������, ������ �� ���������, �� ���-�� ����� ���-������� ��� ������� �������� ���, � �� �������� � ���������� ����������, ���������� � ��������������."},
{1, 1, {}, "�� � �������� ������������ ������, �� ������ ���������, ��� �� ������, ��� ������. ������ �������� ��� ����������, � �� ������������� ������."},
{1, 2, {AddMoney10, LoseReputation}, "�� ������ ���������� � ��������� � �������. �� ���� �� ��������������. ������, ��� �� �������, - ��������� ���, ��������� ����������� ��������. ��, ������� ����� ������ ��� � ���� ���� �� ���������, � ��������� � �����, ������� ������� �������� ��������� �� �������. �������, ��� ���� ��� �� ����� �� ������, ���� �� �� �������� ���."},
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
#include "main.h"

static quest party_member[] = {{1, {}, "���� ����?"},
	{1, {}, "��� ����, ����?"},
	{1, {}, "��� ���?"},
	{1, {Opponent, GuardPosition}, "������� ��� �����.", 2},
	{1, {Opponent, StopGuardPosition}, "����� �� ����.", 2},
	{1, {Opponent, UseLongActionSkill}, "�� ��������� ������� ���������. �� ����� ��� ����� � ����� ������ ������� ���-�� ������, �� ��� ����� �� ������� �������.", 3},
	{1, {}, "��� ���������. ��� ������ �����%� ���������.", -1},
	{}};
static quest all_chat[] = {
	{1, {Random, Opponent, ShopWeaponAndArmor}, "���� ����� ���������������� ������. ���� �������� �����. ��������� ��� ���������, �� ������� ������ ����� �����. ������ ����� ������������ �� ������ ���� � �� ���������� �������."},
	{1, {Random, Opponent, ShopFood}, "���� ������ �� ��� ���� - ���������� ���� �����. �� ��� ���������� ����� ��� ��� ��� ���������! ����� ���� ��� �������, ���� �� ��� �� ����������� ������������."},
	{1, {Random, Opponent, ShopFood}, "��� ������������ - ���� ������ �����. ��� ������ �� �� ������ ����, ��� [�������] �������."},
	{1, {Random, Opponent, ShopFood}, "������� �������� ����������� �������. ��� ��������� �������� � �������� �����������. �� ������ ������� � � ���� ����� ������� [������������]."},
	{1, {Random, Opponent, ShopFood}, "���� ��� ������� ���� ��������, ����� - ���� ������� ������ �� ������������ ������� ��������� � ����� ������ ����������� ����������. �������, ��� �� ��� ��� ���� ����� [������ ��������] � ����� ����������� ����� ��������."},
	{1, {Opponent, Owner}, "������. ������. �������."},
	{1, {Random, Opponent, HumanChild}, "���� ��� �� �������� �������� � ������������."},
	{1, {Random, Opponent, HumanChild}, "��� ���� �����?"},
	{1, {Random, Opponent, HumanChild}, "�� ����� �������!"},
	{1, {Opponent, HumanChild}, "�� ���� �������������. �� ��������."},
	{1, {Random, Opponent, HumanGuard}, "� �� ����� �����."},
	{1, {Random, Opponent, HumanGuard}, "� ���� �����."},
	{1, {Random, Opponent, HumanGuard}, "���? ���� �� �������?"},
	{1, {Random, Opponent, HumanGuard}, "����� ��������?"},
	{1, {Random, Opponent, HumanFemale}, "������ ����� ���� � ����� ���� ���� �����?"},
	{1, {Random, Opponent, HumanMale}, "���� � ���� ���� ����, ��� �� ���������� ��������."},
	{1, {Random}, "����-�� ������?"},
	{1, {}, "������� ����, ��?"},
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
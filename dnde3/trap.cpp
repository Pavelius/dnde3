#include "main.h"

BSDATA(trapi) = {{"", "trap/trap_a"},
	{"�������� ������", "trap/trap_a", 10, 1, {2, 4}, GoblinWarrior, "�� ���� ����� ������� ����!"},
	{"��� � ��������", "trap/trap_ac", 0, 1, {2, 8}, Acid, "%����� ��������%��� � ��� � ��������."},
	{"���������", "trap/trap_ar", -10, 1, {1, 4}, Piercing, "�� ����� �������� ������.", Bolt},
	{"��� � ������", "trap/trap_c", -20, 1, {1, 6}, Magic, "����� ��������� ���������� ��������."},
	{"������������� �������", "trap/trap_ce", -20, 1, {1, 8}, Electricity, "�������� ������� ������ ������� ������."},
	{"�������� �������", "trap/trap_f", -10, 1, {2, 12}, Electricity, "����� ��������� �����, ��������� ������� �����."},
	{"�������� �������", "trap/trap_l", -5, 1, {}, Dazzled, "�������� ��������� ������� �������������� �����."},
	{"�������� ���", "pit", 20, 1, {1, 6}, Bludgeon, "%����� ��������%��� � �������� ���."},
	{"��� � �������", "trap/trap_pc", 0, 1, {2, 12}, Piercing, ""},
	{"������ �� �����", "trap/trap_s", 0, 1, {1, 8}, Piercing, "�� ����� ������� ������."},
	{"������ �� ����", "trap/trap_st", 0, 1, {3, 18}, Slashing, "�� ���� ������� ������ ������."},
	{"���� ��������", "trap/trap_t", 0, 1, {}, {}, ""},
	{"������� �������", "trap/trap_w", 0, 1, {3, 18}, WaterAttack, "������ ����� ���� ��������� ������."},
};
assert_enum(trapi, TrapWater)

void creature::usetrap() {
	auto i = getposition();
	if(i == Blocked)
		return;
	auto t = loc.gettrap(i);
	if(!t)
		return;
	auto& ei = bsdata<trapi>::elements[t];
	int chance = ei.modifier;
	if(!loc.is(i, Hidden))
		chance += 40;
	if(rolld(Dexterity, Alertness, chance)) {
		if(loc.is(i, Hidden) && is(Friendly)) {
			act("%����� ���������%� �������.");
			loc.remove(i, Hidden);
		}
	} else {
		loc.remove(i, Hidden);
		act(ei.text_use);
		auto c = ei.damage.roll();
		switch(ei.effect.type) {
		case Role:
			while(c-- > 0) {
				auto p = loc.add(i, (role_s)ei.effect.value);
				if(p)
					p->add(Hostile, 1, false);
			}
			break;
		default:
			add(ei.effect, c, true);
			break;
		}
		if(ei.loot && d100() < 20)
			loc.drop(i, ei.loot);
	}
}
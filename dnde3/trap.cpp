#include "main.h"

BSDATA(trapi) = {{"", "trap/trap_a"},
	{"«меинное логово", "trap/trap_a", 10, 1, {2, 4}, GoblinWarrior, "—о всех щелей полезли змеи!"},
	{"яма с кислотой", "trap/trap_ac", 0, 1, {2, 8}, Acid, "%герой провалил%ась в €му с кислотой."},
	{"—амострел", "trap/trap_ar", -10, 1, {1, 4}, Piercing, "»з стены вылетела стрела.", Bolt},
	{"яма с эфиром", "trap/trap_c", -20, 1, {1, 6}, Magic, "—низу по€вилось фиолетовое свечение."},
	{"Ёлектрическа€ ловушка", "trap/trap_ce", -20, 1, {1, 8}, Electricity, "–аздалс€ громкий хлопок разр€да молнии."},
	{"ќгненна€ ловушка", "trap/trap_f", -10, 1, {2, 12}, Electricity, "—низу вырвалось пл€м€, коготорое опалило стены."},
	{"—ветова€ ловушка", "trap/trap_l", -5, 1, {}, Dazzled, "¬незапно по€ивлась вспышка ослепительного света."},
	{"√лубока€ €ма", "pit", 20, 1, {1, 6}, Bludgeon, "%герой провалил%ась в глубокую €му."},
	{"яма с коль€ми", "trap/trap_pc", 0, 1, {2, 12}, Piercing, ""},
	{"ƒротик из стены", "trap/trap_s", 0, 1, {1, 8}, Piercing, "»з стены вылетел дротик."},
	{"Ћезви€ из пола", "trap/trap_st", 0, 1, {3, 18}, Slashing, "»з пола выехали острые лезви€."},
	{"јура ржавчины", "trap/trap_t", 0, 1, {}, {}, ""},
	{"¬од€на€ ловушка", "trap/trap_w", 0, 1, {3, 18}, WaterAttack, "ћощный поток воды обрушилс€ сверху."},
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
			act("%герой обнаружил%а ловушку.");
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
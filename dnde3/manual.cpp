#include "main.h"

static int compare_skills(const void* p1, const void* p2) {
	auto e1 = *((skill_s*)p1);
	auto e2 = *((skill_s*)p2);
	return strcmp(bsmeta<skilli>::elements[e1].name, bsmeta<skilli>::elements[e2].name);
}

static void choose_children(stringbuilder& sb, manual& mn, answeri& an) {
	for(auto& e : bsmeta<manual>()) {
		if(e.parent == mn.value)
			an.add((int)&e, e.getname());
	}
}

static void ability_skills(stringbuilder& sb, manual& mn, answeri& an) {
	adat<skill_s, LastResist + 1> source;
	for(auto& e : bsmeta<skilli>()) {
		if(e.abilities[0] == mn.value.value || e.abilities[1] == mn.value.value)
			source.add(e.getid());
	}
	qsort(source.data, source.count, sizeof(source.data[0]), compare_skills);
	if(!source.count)
		return;
	sb.addn("[Навыки]: ");
	auto p = sb.get();
	for(auto i : source) {
		if(p[0])
			sb.add(", ");
		sb.add(bsmeta<skilli>::elements[i].name);
	}
	sb.add(".");
}

static manual::proc ability_list[] = {choose_children};
static manual::proc ability_procs[] = {ability_skills};
manual bsmeta<manual>::elements[] = {{Variant, Ability, "Способности персонажей", "Каждый персонаж имеет **6** базовых способностей, которые его характеризуют.", ability_list},
{Ability, Strenght, 0, "Сила является мерой мускульной силы, физической стойкости и выносливости персонажа. Эта способность является главной для воинов, так как они должны быть сильными, чтобы носить доспехи и владеть тяжелым оружием.", ability_procs},
{Ability, Dexterity, 0, "Ловкость характеризует различные физические особенности персонажа, включая координацию движений, проворство, скорость реакции, рефлексы и вестибулярный аппарат. Ловкость влияет на реакцию персонажа на опасность или неожиданность, на его умение обращаться с метательным оружием и луками, а также на его увертливость от ударов.", ability_procs},
{Ability, Constitution, 0, "Телосложение персонажа характеризует его физические данные, гармоничность комплекции, здоровье и устойчивость к неудобствам, ранениям и заболеваниям.", ability_procs},
{Ability, Intellegence, 0, "Интеллект характеризует память, рассудительность персонажа и его способность к обучению, включая области вне того, что можно выразить печатным словом.", ability_procs},
{Ability, Wisdow, 0, "Мудрость характеризует совокупность просвещенности, рассудительности, силы воли, здравого смысла и интуиции персонажа.", ability_procs},
{Ability, Charisma, 0, "Обаяние характеризует способность персонажа к убеждению, его личную привлекательность и лидерские качества. Это не отражает физической привлекательности, хотя она, разумеется, также играет роль. Обаяние важно для всех персонажей, а особенно для тех, кто собирается иметь дело с неигровыми персонажами, наемными воинами, слугами и разумными животными.", ability_procs},
{Skill, Acrobatics, 0, "Добавляет **1%%** к шансу промазать по персонажу за каждые 4 единицы. Также позволяет подзаработать в таверне."},
{Skill, Riding, 0, "Позволяет передвигаться быстрее на глобальной карте."},
};
DECLFULL(manual);

const char* manual::getname() const {
	if(name)
		return name;
	return value.getname();
}

void gamei::help() {
	adat<manual*, 32> pages;
	auto pm = bsmeta<manual>::elements;
	while(pm) {
		char temp[2048]; stringbuilder sb(temp); answeri an;
		sb.add(pm->text);
		for(auto p : pm->procs)
			p(sb, *pm, an);
		if(pages.getcount() == pages.getmaximum())
			pages.remove(0);
		pages.add(pm);
		pm = (manual*)an.dialogv(true, pm->getname(), sb);
		if(!pm && pages.getcount()>1) {
			pages.remove(pages.getcount() - 1);
			pm = pages[pages.getcount() - 1];
			pages.remove(pages.getcount() - 1);
		}
	}
}
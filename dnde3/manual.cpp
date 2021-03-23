#include "main.h"

static void choose_children(stringbuilder& sb, manual& mn, answeri& an) {
	for(auto& e : bsdata<manual>()) {
		if(e.parent == mn.value)
			an.add((int)&e, e.getname());
	}
}

static void choose_children_sort(stringbuilder& sb, manual& mn, answeri& an) {
	choose_children(sb, mn, an);
	an.sort();
}

static void add_children(stringbuilder& sb, manual& mn, answeri& an) {
	for(auto& e : bsdata<manual>()) {
		if(e.parent == mn.value) {
			sb.addn("[%1]: ", e.name);
			sb.adds(e.text);
		}
	}
}

static void race_ability(stringbuilder& sb, manual& mn, answeri& an) {
	if(mn.value.type != Race)
		return;
	sb.addn("[Артибуты]: ");
	auto& ei = bsdata<racei>::elements[mn.value.value];
	for(auto i = Strenght; i <= Charisma; i = ability_s(i + 1)) {
		if(i != Strenght)
			sb.add(", ");
		sb.add("%1i %2", ei.abilities[i - Strenght], getstr(i));
	}
	sb.add(".");
}

static bool have(const varianta& source, variant v) {
	for(auto e : source) {
		if(e == v)
			return true;
	}
	return false;
}

static void race_skills(stringbuilder& sb, manual& mn, answeri& an) {
	auto count = 0;
	if(mn.value.type == Skill) {
		const char* header = "Присутствует";
		auto skill = (skill_s)mn.value.value;
		for(auto& ei : bsdata<racei>()) {
			if(!ei.is(skill))
				continue;
			if(!count)
				sb.addn("[%1]: ", header);
			else
				sb.add(", ");
			sb.add("%+1", ei.name);
			count++;
		}
		for(auto& ei : bsdata<classi>()) {
			if(!have(ei.bonuses, skill))
				continue;
			if(!count)
				sb.addn("[%1]: ", header);
			else
				sb.add(", ");
			sb.add("%+1", ei.name);
			count++;
		}
	} else if(mn.value.type == Race) {
		const char* header = "Бонусы";
		auto race = (race_s)mn.value.value;
		for(auto v : bsdata<racei>::elements[race].bonuses) {
			if(!count)
				sb.addn("[%1]: ", header);
			else
				sb.add(", ");
			sb.add("%+1", v.getname());
			count++;
		}
	}
	if(count)
		sb.add(".");
}

static void item_example(stringbuilder& sb, manual& mn, answeri& an) {
	auto count = 0;
	if(mn.value.type == Skill) {
		auto id = (skill_s)mn.value.value;
		const char* header = "Применимо";
		for(auto& ei : bsdata<itemi>()) {
			if(ei.skill != id)
				continue;
			if(!count)
				sb.addn("[%1]: ", header);
			else
				sb.add(", ");
			sb.add("%+1", ei.name);
			count++;
		}
	}
}

static void weapon_skill_ability(stringbuilder& sb, manual& mn, answeri& an) {
	auto count = 0;
	if(mn.value.type == Skill) {
		auto id = (skill_s)mn.value.value;
		auto& ei = bsdata<skilli>::elements[id];
		if(ei.weapon.attack)
			sb.addn("* Дает +1%% к шансу попасть за каждые [%1i%%] навыка.", ei.weapon.attack);
		if(ei.weapon.damage)
			sb.addn("* Дает +1 к повреждениям за каждые [%1i%%] навыка.", ei.weapon.damage);
		if(ei.weapon.speed)
			sb.addn("* Дает +1 к скорости атаки за каждые [%1i%%] навыка.", ei.weapon.speed);
	}
}

static void ability_example(stringbuilder& sb, manual& mn, answeri& an) {
	if(mn.value.type != Ability)
		return;
	sb.addn("[Значения]: ");
	auto count = 0;
	for(auto i = 5; i <= 20; i++) {
		for(auto& e : bsdata<racei>()) {
			if(e.abilities[mn.value.value] != i)
				continue;
			if(count > 0)
				sb.add(", ");
			sb.add("%1i %-2", i, e.name);
			count++;
			break;
		}
	}
	sb.add(".");
}

static void ability_skills(stringbuilder& sb, manual& mn, answeri& an) {
	skilla source;
	for(auto& e : bsdata<skilli>()) {
		if(e.is(ability_s(mn.value.value)))
			source.add(e.getid());
	}
	if(!source.count)
		return;
	source.sort();
	sb.addn("[Навыки]: ");
	auto p = sb.get();
	for(auto i : source) {
		if(p[0])
			sb.add(", ");
		sb.add(bsdata<skilli>::elements[i].name);
	}
	sb.add(".");
}

static void ability_list(stringbuilder& sb, manual& mn, answeri& an) {
	choose_children(sb, mn, an);
}

static void ability_procs(stringbuilder& sb, manual& mn, answeri& an) {
	add_children(sb, mn, an);
	ability_example(sb, mn, an);
	ability_skills(sb, mn, an);
}

static void general_list(stringbuilder& sb, manual& mn, answeri& an) {
	choose_children(sb, mn, an);
}

static void manual_list(stringbuilder& sb, manual& mn, answeri& an) {
	choose_children(sb, mn, an);
}

static void skill_list(stringbuilder& sb, manual& mn, answeri& an) {
	choose_children_sort(sb, mn, an);
}

static void skill_proc(stringbuilder& sb, manual& mn, answeri& an) {
	race_skills(sb, mn, an);
}

static void weapon_skill(stringbuilder& sb, manual& mn, answeri& an) {
	weapon_skill_ability(sb, mn, an);
	item_example(sb, mn, an);
	race_skills(sb, mn, an);
}

static void state_list(stringbuilder& sb, manual& mn, answeri& an) {
	choose_children_sort(sb, mn, an);
}

static void race_ability_list(stringbuilder& sb, manual& mn, answeri& an) {
	race_ability(sb, mn, an);
	race_skills(sb, mn, an);
}

BSDATA(manual) = {
	{NoVariant, Variant, "Мануал", "Содержит справочную информацию по правилам игры.", manual_list},
	{Variant, Ability, "Способности персонажей", "Каждый персонаж имеет [6] базовых способностей, которые его характеризуют. От базовых атрибутов зависят множество характеристик, плюс время от времени происходит **тест** атрибута. Шанс пройти такой тест (в процентах) равен удвоенному значению атрибута плюс различные модификаторы.", ability_list},
	{Ability, Strenght, 0, "Сила является мерой мускульной силы, физической стойкости и выносливости персонажа. Эта способность является главной для воинов, так как они должны быть сильными, чтобы носить доспехи и владеть тяжелым оружием.", ability_procs},
	{Ability, Dexterity, 0, "Ловкость характеризует различные физические особенности персонажа, включая координацию движений, проворство, скорость реакции, рефлексы и вестибулярный аппарат. Ловкость влияет на реакцию персонажа на опасность или неожиданность, на его умение обращаться с метательным оружием и луками, а также на его увертливость от ударов.", ability_procs},
	{Ability, Constitution, 0, "Телосложение персонажа характеризует его физические данные, гармоничность комплекции, здоровье и устойчивость к неудобствам, ранениям и заболеваниям.", ability_procs},
	{Ability, Intellegence, 0, "Интеллект характеризует память, рассудительность персонажа и его способность к обучению, включая области вне того, что можно выразить печатным словом.", ability_procs},
	{Ability, Wisdow, 0, "Мудрость характеризует совокупность просвещенности, рассудительности, силы воли, здравого смысла и интуиции персонажа.", ability_procs},
	{Ability, Charisma, 0, "Обаяние характеризует способность персонажа к убеждению, его личную привлекательность и лидерские качества. Это не отражает физической привлекательности, хотя она, разумеется, также играет роль. Обаяние важно для всех персонажей, а особенно для тех, кто собирается иметь дело с неигровыми персонажами, наемными воинами, слугами и разумными животными.", ability_procs},
	{Dexterity, 0, "Стрельба", "Добавляется к шансу попасть по врагу дистанционным оружием."},
	{Dexterity, 0, "Шанс избежать ловушки", "Если вы наступили на ловушку делается тест ловкости. В случае успеха, вы успешно ее обошли."},
	{Dexterity, 0, "Бонус уклонения", "Как описано в навыке акробатики, добавляет [++1%%] за каждые 2 единицы к шансу избежать урона, когда по вам бъет враг."},
	{Intellegence, 0, "Дополнительный опыт", "Персонаж получает [++1%%] опыта за каждую единицу интеллекта более **10** и [--1%%] опыта за каждую единицу интеллекта менее 10."},
	{Intellegence, 0, "Повышение навыков", "Персонаж получает **Интллект/2** повышений наввыков на каждом уровне."},
	{Intellegence, 0, "Диалоги", "Во время диалога все персонажи добавляют к сложности для сравнения навыка значение **ИнтеллектХ2-20**."},
	{Variant, Trap, "Ловушки", "В любом подземелье находится множество ловушек. По-умолчанию все ловушки скрыты и когда герой становится на клетке с ловушкой делается тестирование [Ловкости]. Если тест удался герой обнаруживает ловушку и избегает ее эффекта. Иначе срабатывает эффект ловушки и она становится видимой.\nПри повторном наступании на ловушку также делается тот же тест, но с бонусом [++40%%]."},
	{Variant, Skill, "Навыки персонажей", "Каждый персонаж имеет определенное количество навыков. Их число зависит от класса и расы.", skill_list},
	{Variant, State, "Состояния персонажей", "Во время приключений персонажи сталкиваются с множетсвом опасностей.", state_list},
	{Skill, Acrobatics, 0, "Добавляет [++1%%] к шансу промазать по персонажу за каждые 4 единицы.", skill_proc},
	{Skill, Alertness, 0, "Добавляет [++1%%] к шансу обнаружить ловушку за каждые 4 единицы. Добавляет [++1%%] к шансу обнаружить скрытые двери за каждые 3 единицы при активном поиске.", skill_proc},
	{Skill, Alchemy, 0, "Позволяет определить зелье. Также можно изготавливать зелья по рецептам.", skill_proc},
	{Skill, Athletics, 0, "Персонаж с этим навыком разносит двери в щепки. При повышении уровня имеется шанс повысить **ловкость** или **силу**. Также повышает скорость движения персонажа в локации.", skill_proc},
	{Skill, Backstabbing, 0, "Добавляется к шансу попадания и увеличивает урон в процентах равный утроенному навыку, если атака делается из невидимого состояния. После такой атаки вы становитесь видимым.", skill_proc},
	{Skill, Climbing, 0, "При движении по горам делается тест и если он не пройден затраты времени на движения увеличивается на 50%%. Такой тест делается для каждого героя в партии. Кроме того тестируется при попытке вылезти из **паутины** или ловушки **ямы**.", skill_proc},
	{Skill, Concetration, 0, "Скорость восстановления маны персонажа.", skill_proc},
	{Skill, DisarmTraps, 0, "Убирает ловушку и дает немного опыта.", skill_proc},
	{Skill, FindWeakness, 0, "Шанс попасть в уязвимое место врага и нанести [++30%%] урона, а также в зависимости от типа оружия дополнительный эффект:\n* Колющее оружие причиняет +40%% урона вместо +30%% и игнорирует броню.\n* Режущее оружие заставляет противника истекать кровью\n* Ударное оружие наносит оглушительный удар после которого соперник должен прийти в себя чтобы ходить дальше.\nИз шанса попасть в уязвимое место вычитается покрытость врага броней.", skill_proc},
	{Skill, Gambling, 0, "Шанс выиграть деньги в азартной игре (кубики, карты, фишки). Можно играть только в заведении и только с достойным соперником, у которого также имеется данный навык.", skill_proc},
	{Skill, Healing, 0, "Скорость восстановления здоровья персонажа.", skill_proc},
	{Skill, HearNoises, 0, "Шанс услышать движение невидимого врага и сделать его видимым.", skill_proc},
	{Skill, Herbalism, 0, "Шанс удачно собрать урожай с цветов и получить качественное растение в свое распоряжение. Конкретный тип собранного растения определяется случайным образом.", skill_proc},
	{Skill, HideInShadow, 0, "Позволяет спрятаться и оставаться незамеченым. Вы просто исчезаете из вида у всех на виду.", skill_proc},
	{Skill, Literacy, 0, "Самый важный навык для мага. Когда вы находите свиток или книгу показывает шанс опознать и изучить то, что вы нашли. Также оторажает шанс изучить заклинание - правда чем выше ваш уровень в заклинении, тем сложнее из книги можно будет почерпнуть что-то новое.", skill_proc},
	{Skill, Lockpicking, 0, "Открывает закрытую дверь и дает немного опыта.", skill_proc},
	{Skill, MoveSilently, 0, "Шанс двигаться тихо оставаясь незамеченным в невидимом состоянии.", skill_proc},
	{Skill, PickPockets, 0, "В любом городском здании можно разжиться монетами обворовывая обывателей.", skill_proc},
	{Skill, Religion, 0, "Отображает знание религие и принадлежность своей вере. От религии зависит количество очков веры персонажа.", skill_proc},
	{Skill, Riding, 0, "Позволяет передвигаться быстрее на глобальной карте.", skill_proc},
	{Skill, FocusAxes, 0, "Позволяет применять сложные приемы с таким оружием как топор.", weapon_skill},
	{Skill, FocusSwords, 0, "Позволяет более эффективно сражаться на мечах.", weapon_skill},
	{Skill, FocusBows, 0, "Позволяет более эффективно стрелять из лука.", weapon_skill},
	{Skill, FocusTwohanded, 0, "Позволяет более эффективно сражаться двуручным оружием.", weapon_skill},
	{Skill, TwoWeaponFighting, 0, "Позволяет попадаеть более точно по противнику, при сражении с оружием в каждой руке.", skill_proc},
	{State, Poisoned, 0, "Отравленный персонаж теряет по одному очку жизни каждые 5 минут если не выполнит сопротивление яду. При удачном сопротивлении уменьшается уровень яда."},
	{State, Sick, 0, "Больной персонаж не восстанавливает очки жизни природным путем. При лечение количество восстанавливаемых хитов делится на три."},
	{Variant, Race, "Расы", "От выбранной расы зависят стартовые атрибуты к которым добавяется случайный модификатор от [--2] до [++2].", general_list},
	{Race, Human, 0, "Люди - самая распространённая раса в Забытых Королевствах. Их главное преимущество перед всеми остальными расами то, что они могут выбрать любой класс и специализацию. В среднем они живут около 70 лет и достигают роста 180 см.", race_ability_list},
	{Race, Dwarf, 0, "Дворфы — низкий, коренастый народ, их легко узнать по размеру и осанке. В среднем они от 4 до 4 1/2 футов ростом. У них румяные щёки, тёмные глаза и тёмные волосы. В основном дворфы живут 350 лет. Обычно дворфы угрюмы и неразговорчивы. Они увлекаются тяжёлой работой, их мало заботит юмор. Они сильные и храбрые. Они любят пиво, эль, мёд и более крепкие напитки. Их главная любовь, однако, драгоценный металл, в частности золото. Они ценят самоцветы, особенно алмазы, и непрозрачные камни (кроме жемчужин). Дворфам нравится земля и не нравится море. Они не слишком любят эльфов и испытывают лютую ненависть к оркам и гоблинам. Их низкие коренастые тела не позволяют ездить на лошадях или других крупных ездовых (хотя на пони или кабанах не испытывают трудностей), и они относятся с лёгким подозрением и осторожностью к этим созданиям.Они питают неприязнь к магии и мало способны к ней, но наслаждаются боем, военным искусством и научными ремёслами, такими как инженерное дело.", race_ability_list},
};
DECLFULL(manual);

const char* manual::getname() const {
	if(name)
		return name;
	return value.getname();
}

void gamei::help() {
	adat<manual*, 32> pages;
	auto pm = bsdata<manual>::elements;
	while(pm) {
		char temp[2048]; stringbuilder sb(temp); answeri an;
		sb.add(pm->text);
		if(pm->description)
			pm->description(sb, *pm, an);
		if(pages.getcount() == pages.getmaximum())
			pages.remove(0);
		pages.add(pm);
		pm = (manual*)an.dialogv(true, pm->getname(), sb);
		if(!pm && pages.getcount() > 1) {
			pages.remove(pages.getcount() - 1);
			pm = pages[pages.getcount() - 1];
			pages.remove(pages.getcount() - 1);
		}
	}
}
#include "main.h"

abilityi bsmeta<abilityi>::elements[] = {{"Strenght", "Сила", "СЛ", "силы", "слабости"},
{"Dexterity", "Ловкость", "ЛВ", "ловкости", "неуклюжести"},
{"Constitution", "Телосложение", "ТЛ", "телосложения", "болезни"},
{"Intellegence", "Интеллект", "ИН", "интеллекта", "глупости"},
{"Wisdow", "Мудрость", "МД", "мудрости", "наивности"},
{"Charisma", "Харизма", "ХР", "харизмы", "уродства"},
//
{"AttackMelee", "Шанс удачного удара", "", "попадания", "промазывания", {40}},
{"AttackRanged", "Шанс удачного выстрела", "", "меткости", "косоглазия", {40, Dexterity}},
{"DamageMelee", "Урон рукопашный", "", "урона", "лечения", {Strenght, Divide4, -2}},
{"DamageRanged", "Урон дистанция", "", "урона", "лечения"},
{"Pierce", "Прокалывание", "", "проникания", "затупления"},
{"Deflect", "Уклонение", "", "уклонения", "", {Acrobatics, Divide4}},
{"Armor", "Броня", "", "брони", ""},
{"Speed", "Скорость", "", "скорости", "замедления"},
{"Visibility", "Обзор", "", "обзора", "слепоты"},
//
{"Level", "Уровень", "", "опыта", ""},
{"LifePoints", "Жизнь", "", "здоровья", "", {Constitution}},
{"LifeRate", "Восстановление жизни", "", "магии", ""},
{"ManaPoints", "Мана", "", "регенерации", "", {Intellegence, Concetration, Divide4}},
{"ManaRate", "Восстановление маны", "", "воодружения", ""},
};
assert_enum(ability, ManaRate);

ability_s abilityi::getid() const {
	return ability_s(this - bsmeta<abilityi>::elements);
}
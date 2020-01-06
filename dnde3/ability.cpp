#include "main.h"

abilityi bsmeta<abilityi>::elements[] = {{"Strenght", "Сила", "СЛ", "силы", "слабости"},
{"Dexterity", "Ловкость", "ЛВ", "ловкости", "неуклюжести"},
{"Constitution", "Телосложение", "ТЛ", "телосложения", "болезни"},
{"Intellegence", "Интеллект", "ИН", "интеллекта", "глупости"},
{"Wisdow", "Мудрость", "МД", "мудрости", "наивности"},
{"Charisma", "Харизма", "ХР", "харизмы", "уродства"},
//
{"AttackMelee", "Шанс удачного удара", "", "попадания", "промазывания", {Strenght, Multiply2, -20}},
{"AttackRanged", "Шанс удачного выстрела", "", "меткости", "косоглазия", {Dexterity, Multiply2, -20}},
{"DamageMelee", "Урон рукопашный", "", "урона", "лечения", {Strenght, Divide2, -5}},
{"DamageRanged", "Урон дистанция", "", "урона", "лечения"},
{"Pierce", "Прокалывание", "", "проникания", "затупления"},
{"Deflect", "Уклонение", "", "уклонения", "неловкости", {Acrobatics, Divide4}},
{"Armor", "Броня", "", "брони", ""},
{"Speed", "Скорость", "", "скорости", "замедления", {95, Athletics, Divide10, Dexterity, Divide2}},
{"Visibility", "Обзор", "", "обзора", "слепоты", {6}},
//
{"Level", "Уровень", "", "опыта", ""},
{"LifePoints", "Жизнь", "", "здоровья", "", {Constitution}},
{"LifeRate", "Восстановление жизни", "", "исцеления", "", {Healing}},
{"ManaPoints", "Мана", "", "регенерации", "", {Wisdow}},
{"ManaRate", "Восстановление маны", "", "воодружения", "", {Concetration}},
};
assert_enum(ability, ManaRate);

ability_s abilityi::getid() const {
	return ability_s(this - bsmeta<abilityi>::elements);
}
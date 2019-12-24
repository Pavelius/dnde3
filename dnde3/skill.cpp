#include "main.h"

static const char* talk_subjects[] = {"гномов", "хоббитов", "эльфов", "рыцарей", "троллей", "дракона", "колдуна", "трех друзей"};
static const char* talk_object[] = {"сокровище", "волшебное кольцо", "проклятый артефакт", "гору", "истинную любовь", "прекрасную куртизанку"};
static const char* talk_location[] = {"библиотеку", "ратушу", "магазин", "таверну", "храм"};
static const char* talk_games[] = {"кубики", "карты", "наперстки", "шарады"};

skilli bsmeta<skilli>::elements[] = {{"Торговля", "торговли", {Charisma, Intellegence}},
{"Блеф", "обмана", {Charisma, Dexterity}},
{"Дипломатия", "дипломатии", {Charisma, Wisdow}},
//
{"Акробатика", "акробатики", {Dexterity, Dexterity}},
{"Внимательность", "внимательности", {Wisdow, Dexterity}},
{"Атлетика", "атлетики", {Strenght, Dexterity}},
{"Убийство", "убийства", {Dexterity, Dexterity}},
{"Концентрация", "концетрации", {Wisdow, Wisdow}},
{"Обезвредить ловушки", "ловушек", {Dexterity, Intellegence}},
{"Слышать звуки", "слуха", {Wisdow, Intellegence}},
{"Прятаться в тени", "скрытности", {Dexterity, Dexterity}},
{"Открыть замок", "взлома", {Dexterity, Intellegence}},
{"Очистить карманы", "воровства", {Dexterity, Charisma}},
{"Алхимия", "алхимии", {Intellegence, Intellegence}},
{"Танцы", "танцев", {Dexterity, Charisma}},
{"Инженерное дело", "инженерии", {Intellegence, Intellegence}},
{"Азартные игры", "азартных игр", {Charisma, Dexterity}},
{"История", "истории", {Intellegence, Intellegence}},
{"Лечение", "лечения", {Wisdow, Intellegence}},
{"Грамотность", "письма и чтения", {Intellegence, Intellegence}},
{"Шахтерское дело", "шахтерского дела", {Strenght, Constitution}},
{"Езда верхом", "езды верхом", {Dexterity, Constitution}},
{"Кузнечное дело", "кузнечного дела", {Strenght, Intellegence}},
{"Выживание", "выживания", {Wisdow, Constitution}},
{"Плавание", "плавания", {Strenght, Constitution}},
//
{"Владение луком", "стрельбы из лука", {Dexterity, Dexterity}},
{"Владение мечом", "сражения на мечах", {Strenght, Dexterity}},
{"Владение топором", "сражения на топорах", {Strenght, Constitution}},
{"Сражение двумя оружиями", "ужасного оружия", {Strenght, Dexterity}},
//
{"Сопротивление кислоте", "кислоты", {Dexterity, Constitution}},
{"Сопротивление шарму", "красоты и любви", {Wisdow, Wisdow}},
{"Сопротивление холоду", "холода", {Constitution, Strenght}},
{"Сопротивление электричеству", "молнии", {Dexterity, Dexterity}},
{"Сопротивление огню", "огня", {Constitution, Dexterity}},
{"Сопротивление параличу", "параличу", {Constitution, Dexterity}},
{"Сопротивление яду", "яда", {Constitution, Constitution}},
{"Дыхание водой", "воды", {Strenght, Constitution}},
};
assert_enum(skill, ResistWater);

skill_s	skilli::getid() const {
	return skill_s(this - bsmeta<skilli>::elements);
}
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
{"Лазание", "лазания", {Dexterity, Strenght}},
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
{"Заживание ран", "здоровья", {Wisdow, Constitution}},
{"Травознавство", "травознания", {Wisdow, Intellegence}},
{"Грамотность", "письма и чтения", {Intellegence, Intellegence}},
{"Шахтерское дело", "шахтерского дела", {Strenght, Constitution}},
{"Езда верхом", "езды верхом", {Dexterity, Constitution}},
{"Кузнечное дело", "кузнечного дела", {Strenght, Intellegence}},
{"Выживание", "выживания", {Wisdow, Constitution}},
{"Плавание", "плавания", {Strenght, Constitution}},
//
{"Владение луком", "стрельбы из лука", {Dexterity, Dexterity}, {40, 2}},
{"Владение ударным", "ударного оружия", {Strenght, Strenght}, {30, 1}},
{"Владение мечом", "сражения на мечах", {Strenght, Dexterity}, {35, 1}},
{"Владение топором", "сражения на топорах", {Strenght, Constitution}, {30, 1}},
{"Владение колющим оружием", "фехтования", {Dexterity, Dexterity}, {40, 2}},
{"Владение двуручным оружием", "сражения двуручным оружием", {Strenght, Strenght}, {30, 1}},
{"Владение посохом", "сражения посохом", {Strenght, Dexterity}, {40, 2}},
{"Рукопашный бой", "рукопашного боя", {Strenght, Dexterity}, {60, 3}},
{"Сражение двумя оружиями", "ужасного оружия", {Strenght, Dexterity}, {40, 2}},
//
{"Сопротивление кислоте", "кислоты", {Dexterity, Constitution}},
{"Сопротивление шарму", "красоты и любви", {Wisdow, Wisdow}},
{"Сопротивление холоду", "холода", {Constitution, Strenght}},
{"Сопротивление электричеству", "молнии", {Dexterity, Dexterity}},
{"Сопротивление огню", "огня", {Constitution, Dexterity}},
{"Сопротивление параличу", "паралича", {Constitution, Dexterity}},
{"Сопротивление яду", "яда", {Constitution, Constitution}},
{"Дыхание водой", "воды", {Strenght, Constitution}},
};
assert_enum(skill, ResistWater);

skill_s	skilli::getid() const {
	return skill_s(this - bsmeta<skilli>::elements);
}

const char* skilli::getusetext() const {
	switch(getid()) {
	case Healing: return "Используется автоматически восстанавливая 1 очко жизни за определенный промежуток времени.";
	case Concetration: return "Используется автоматически восстанавливая 1 очко маны за определенный промежуток времени.";
	default: return "Этот навык используется автоматически.";
	}
}

int	skilli::weaponi::get(int v) const {
	auto r = base;
	switch(divider) {
	case 0: break;
	case 1: r += (2 * v) / 3; break;
	default: r += v / divider; break;
	}
	return r;
}
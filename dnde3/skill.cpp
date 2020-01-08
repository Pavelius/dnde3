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
{"Обезвредить ловушки", "ловушек", {Dexterity, Intellegence}, {Trap}},
{"Удар в уязвимое место", "боевых трюков", {Dexterity, Intellegence}},
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
{"Владение луком", "стрельбы из лука", {Dexterity, Dexterity}, {3, 30, 30}},
{"Владение мечом", "сражения на мечах", {Strenght, Dexterity}, {4, 20, 25}},
{"Владение топором", "сражения на топорах", {Strenght, Constitution}, {5, 15, 30}},
{"Владение двуручным оружием", "двуручного оружия", {Strenght, Strenght}, {4, 10, 35}},
{"Сражение двумя оружиями", "ужасного оружия", {Strenght, Dexterity}, {5, 30, 15}},
};
assert_enum(skill, LastSkill);

skill_s	skilli::getid() const {
	return skill_s(this - bsmeta<skilli>::elements);
}

const char* skilli::getusetext() const {
	switch(getid()) {
	case Healing: return "Используется автоматически восстанавливая 1 очко жизни за определенный промежуток времени.";
	case Concetration: return "Используется автоматически восстанавливая 1 очко маны за определенный промежуток времени.";
	default:
		if(isweapon())
			return "Навык владения оружием влияет на шанс попадания, наносимый урон, скорость проведения удара и используется автоматически где это применимо.";
		return "Этот навык используется автоматически.";
	}
}
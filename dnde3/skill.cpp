#include "main.h"

static const char* talk_subjects[] = {"гномов", "хоббитов", "эльфов", "рыцарей", "троллей", "дракона", "колдуна", "трех друзей"};
static const char* talk_object[] = {"сокровище", "волшебное кольцо", "проклятый артефакт", "гору", "истинную любовь", "прекрасную куртизанку"};
static const char* talk_location[] = {"библиотеку", "ратушу", "магазин", "таверну", "храм"};
static const char* talk_games[] = {"кубики", "карты", "наперстки", "шарады"};

skilli bsmeta<skilli>::elements[] = {{"Торговля", "торговли", {Charisma, Intellegence}, {}, {Creature, {NotYou}, SingleTarget, Close}},
{"Блеф", "обмана", {Charisma, Dexterity}, {}, {Creature, {NotYou}, SingleTarget, Close}},
{"Дипломатия", "дипломатии", {Charisma, Wisdow}, {}, {Creature, {NotYou}, SingleTarget, Close}},
//
{"Акробатика", "акробатики", {Dexterity, Dexterity}},
{"Внимательность", "внимательности", {Wisdow, Dexterity}},
{"Атлетика", "атлетики", {Strenght, Dexterity}, {}, {Object, {}, SingleTarget, Close}},
{"Убийство", "убийства", {Dexterity, Dexterity}, {}, {Creature, {NotYou, Friends}, SingleTarget, Close}},
{"Лазание", "лазания", {Dexterity, Strenght}},
{"Концентрация", "концетрации", {Wisdow, Wisdow}},
{"Обезвредить ловушки", "ловушек", {Dexterity, Intellegence}, {}, {Object, {}, SingleTarget, Close}},
{"Удар в уязвимое место", "боевых трюков", {Dexterity, Intellegence}},
{"Слышать звуки", "слуха", {Wisdow, Intellegence}},
{"Прятаться в тени", "скрытности", {Dexterity, Dexterity}, {}, {Creature}},
{"Открыть замок", "взлома", {Dexterity, Intellegence}, {}, {Object, {}, SingleTarget, Close}},
{"Очистить карманы", "воровства", {Dexterity, Charisma}, {}, {Creature, {NotYou}, SingleTarget, Close}},
{"Алхимия", "алхимии", {Intellegence, Intellegence}},
{"Танцы", "танцев", {Dexterity, Charisma}},
{"Инженерное дело", "инженерии", {Intellegence, Intellegence}},
{"Азартные игры", "азартных игр", {Charisma, Dexterity}, {}, {Creature, {Friends}, SingleTarget, Close}},
{"История", "истории", {Intellegence, Intellegence}, {}, {Object, {}, SingleTarget, Reach}},
{"Заживание ран", "здоровья", {Wisdow, Constitution}},
{"Травознавство", "травознания", {Wisdow, Intellegence}, {}, {Object, {}, SingleTarget, Close}},
{"Грамотность", "письма и чтения", {Intellegence, Intellegence}},
{"Шахтерское дело", "шахтерского дела", {Strenght, Constitution}, {}, {Object, {}, SingleTarget, Close}},
{"Езда верхом", "езды верхом", {Dexterity, Constitution}},
{"Кузнечное дело", "кузнечного дела", {Strenght, Intellegence}, {}, {Object, {}, SingleTarget, Close}},
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
#include "main.h"

skilli bsmeta<skilli>::elements[] = {{"Торговля", "торговли", {Charisma, Intellegence}, {}, {Creature, {NotYou}, Close, "С кем хотите поторговаться?"}},
{"Блеф", "обмана", {Charisma, Dexterity}, {}, {Creature, {NotYou}, Close, "Кого будете обманывать?"}},
{"Дипломатия", "дипломатии", {Charisma, Wisdow}, {}, {Creature, {NotYou}, Close, "С кем будете вести переговоры?"}},
//
{"Акробатика", "акробатики", {Dexterity, Dexterity}},
{"Внимательность", "внимательности", {Wisdow, Dexterity}},
{"Атлетика", "атлетики", {Strenght, Dexterity}, {}, {Object, {}, Close, "Какую дверь выбить?"}},
{"Убийство", "убийства", {Dexterity, Dexterity}, {}, {Creature, {NotYou, Friends}, Close, "Кого убить?"}},
{"Лазание", "лазания", {Dexterity, Strenght}},
{"Концентрация", "концетрации", {Wisdow, Wisdow}},
{"Обезвредить ловушки", "ловушек", {Dexterity, Intellegence}, {}, {Object, {}, Close, "Какую ловушку обезвредить?"}},
{"Удар в уязвимое место", "боевых трюков", {Dexterity, Intellegence}},
{"Слышать звуки", "слуха", {Wisdow, Intellegence}},
{"Прятаться в тени", "скрытности", {Dexterity, Intellegence}, {}, {Creature}},
{"Открыть замок", "взлома", {Dexterity, Intellegence}, {}, {Object, {}, Close, "Какой замок вскрыть?"}},
{"Двигаться тихо", "тихого движения", {Dexterity, Dexterity}},
{"Очистить карманы", "воровства", {Dexterity, Charisma}, {}, {Creature, {NotYou}, Close, "Кого обворовать?"}},
{"Алхимия", "алхимии", {Intellegence, Intellegence}, {}, {Item, {AlwaysChoose}, You, "Какое зелье хотите попробывать?"}},
{"Готовка еды", "куховарения", {Charisma, Intellegence}, {}, {Item, {AlwaysChoose}, You, "Что хотите приготовить?"}},
{"Танцы", "танцев", {Dexterity, Charisma}},
{"Инженерное дело", "инженерии", {Intellegence, Intellegence}},
{"Азартные игры", "азартных игр", {Charisma, Dexterity}, {}, {Creature, {Friends, NotYou}, Close, "С кем поиграть?"}},
{"История", "истории", {Intellegence, Intellegence}, {}, {Object, {}, Reach, "Какой объект изучить?"}},
{"Заживание ран", "здоровья", {Wisdow, Constitution}},
{"Травознавство", "травознания", {Wisdow, Intellegence}, {}, {Object, {}, Close, "Где собрать урожай?"}},
{"Грамотность", "письма и чтения", {Intellegence, Intellegence}, {}, {Item, {AlwaysChoose}, You, "Что хотите прочитать?"}},
{"Шахтерское дело", "шахтерского дела", {Strenght, Constitution}, {}, {Object, {}, Close, "Где добывать руду?"}},
{"Езда верхом", "езды верхом", {Dexterity, Constitution}},
{"Кузнечное дело", "кузнечного дела", {Strenght, Intellegence}, {}, {Object, {}, Close, "Что хотите отремонтировать?"}},
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
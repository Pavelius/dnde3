#include "main.h"

racei bsmeta<racei>::elements[] = {{"Человек", {9, 9, 9, 9, 9, 9}, {11, 11, 11, 11, 11, 11}, {Bargaining, Gambling, Swimming}},
{"Гном", {11, 6, 13, 9, 9, 9}, {14, 9, 15, 11, 11, 11}, {Smithing, Mining, Athletics}, {{ResistPoison, 30}}},
{"Эльф", {8, 12, 6, 11, 10, 11}, {10, 14, 8, 13, 12, 13}, {Survival, WeaponFocusBows, Swimming}, {{ResistCharm, 30}}},
{"Полурослик", {6, 11, 10, 9, 10, 9}, {8, 13, 11, 11, 12, 11}, {HideInShadow, Acrobatics, Swimming}},
//
{"Гоблин", {5, 12, 8, 6, 9, 6}, {7, 14, 10, 8, 11, 8}, {HideInShadow, Acrobatics, Swimming}},
{"Кобольд", {5, 13, 7, 6, 9, 6}, {7, 15, 9, 8, 11, 8}, {HideInShadow, Acrobatics, Swimming}},
{"Орк", {12, 9, 11, 6, 9, 6}, {15, 11, 13, 8, 11, 8}, {Athletics, Mining, Swimming}},
{"Гнолл", {13, 9, 11, 5, 9, 5}, {16, 11, 13, 7, 11, 7}, {Athletics, Survival, Swimming}},
//
{"Зверь", {6, 6, 6, 2, 12, 2}, {8, 9, 9, 4, 15, 4}},
{"Насекомое", {4, 5, 4, 1, 1, 1}, {6, 9, 6, 1, 1, 1}},
{"Мертвец", {10, 6, 10, 4, 1, 1}, {12, 8, 13, 5, 1, 1}},
};
assert_enum(race, Undead);
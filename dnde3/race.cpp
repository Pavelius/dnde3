#include "main.h"

racei bsmeta<racei>::elements[] = {{"�������", {10, 10, 10, 10, 10, 10}, {Bargaining, Gambling, Swimming}},
{"����", {12, 8, 13, 10, 10, 9}, {Smithing, Mining, Athletics}, {{ResistPoison, 30}}},
{"����", {9, 12, 8, 11, 10, 11}, {Survival, WeaponFocusBows, Swimming}, {{ResistCharm, 30}}},
{"����������", {7, 12, 11, 10, 8, 10}, {HideInShadow, Acrobatics, Swimming}},
//
{"������", {5, 12, 8, 6, 9, 6}, {HideInShadow, Acrobatics, Swimming}},
{"�������", {5, 13, 7, 6, 9, 6}, {HideInShadow, Acrobatics, Swimming}},
{"���", {12, 9, 11, 6, 9, 6}, {Athletics, Mining, Swimming}},
{"�����", {13, 9, 11, 5, 9, 5}, {Athletics, Survival, Swimming}},
//
{"�����", {6, 6, 6, 2, 12, 2}},
{"���������", {4, 5, 4, 1, 1, 1}},
{"�������", {10, 6, 10, 4, 1, 1}},
};
assert_enum(race, Undead);
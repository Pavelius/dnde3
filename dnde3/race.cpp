#include "main.h"

racei bsmeta<racei>::elements[] = {{"�������", {10, 10, 10, 10, 10, 10}, {Bargaining, Gambling, Swimming}, {}},
{"����", {11, 8, 12, 10, 10, 9}, {Smithing, Mining, Athletics}, {{ResistPoison, 30}}, {Darkvision}},
{"����", {9, 12, 8, 10, 10, 11}, {MoveSilently, FocusBows, Swimming}, {{ResistCharm, 30}, {Visibility, 1}}, {}},
{"����������", {7, 13, 12, 10, 8, 10}, {HideInShadow, MoveSilently, Swimming}, {{ResistPoison, 30}}, {}},
//
{"������", {5, 12, 8, 6, 9, 6}, {HideInShadow, MoveSilently, Swimming}, {}, {Darkvision}},
{"�������", {5, 13, 7, 6, 9, 6}, {HideInShadow, MoveSilently, Swimming}, {}, {Darkvision}},
{"���", {17, 8, 13, 6, 9, 6}, {Athletics, Mining, Survival}, {}, {Darkvision}},
{"���", {12, 9, 11, 6, 9, 6}, {Athletics, Mining, Swimming}, {}, {Darkvision}},
{"�����", {13, 9, 11, 5, 9, 5}, {HearNoises, Survival, Swimming}, {}, {Darkvision}},
//
{"�����", {6, 6, 10, 2, 12, 2}, {Alertness, HearNoises, Swimming}, {{ResistCharm, 20}}, {}},
{"���������", {4, 5, 4, 1, 1, 1}, {Athletics, Survival, Swimming}, {{ResistCharm, 100}}, {}},
{"�������", {10, 6, 10, 4, 1, 1}, {Athletics, MoveSilently, HearNoises}, {{ResistCharm, 100}, {ResistPoison, 100}}, {}},
};
assert_enum(race, Undead);
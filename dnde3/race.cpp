#include "main.h"

BSDATA(racei) = {
	{"�������", {10, 10, 10, 10, 10, 10}, "human", {Bargaining, Gambling, Swimming}},
	{"����", {11, 8, 12, 10, 10, 9}, "dwarf", {Smithing, Mining, Athletics, Resist, Poison, Darkvision}},
	{"����", {9, 12, 8, 10, 10, 11}, "helf", {MoveSilently, FocusSwords, Swimming, Visibility, Resist, Charm}},
	{"����������", {7, 13, 12, 10, 8, 10}, "hurth", {HideInShadow, MoveSilently, Swimming, Resist, Poison}},
	{"��������", {9, 11, 12, 7, 9, 8}, "ratling", {HideInShadow, Healing, Gambling, Resist, Poison, Darkvision}},
	{"���", {12, 9, 11, 6, 9, 6}, "orc", {Athletics, Mining, Swimming, Darkvision}},
	{"������", {5, 12, 8, 6, 9, 6}, 0, {HideInShadow, MoveSilently, Swimming, Darkvision}},
	{"�������", {5, 13, 7, 6, 9, 6}, 0, {HideInShadow, MoveSilently, Swimming, Darkvision}},
	{"���", {17, 8, 13, 6, 9, 6}, 0, {Athletics, Mining, Survival, Darkvision}},
	{"�����", {13, 9, 11, 5, 9, 5}, 0, {HearNoises, Survival, Swimming, Darkvision}},
	{"�����", {6, 6, 10, 2, 12, 2}, 0, {Alertness, HearNoises, Swimming, Resist, Charm}},
	{"���������", {4, 5, 4, 1, 1, 1}, 0, {Athletics, Survival, Swimming, Immune, Charm}},
	{"�������", {10, 6, 10, 4, 1, 1}, 0, {Athletics, MoveSilently, HearNoises, Immune, Charm, Poison}},
};
assert_enum(racei, Undead)
#include "main.h"

statei bsmeta<statei>::elements[] = {{"Anger", "����", "������", true},
{"Dazzled", "���������", "�����������", true},
{"Drunken", "������", "���������", true},
{"Friendly", "������������", "������", false},
{"Hostile", "���������", "������", true},
{"Hunger", "�������", "������", true},
{"Invisible", "�������", "�����������", false},
{"Poisoned", "��������", "����������", true},
{"Sick", "�����", "�������", true},
{"Sleeped", "����", "���", true},
{"Wounded", "�����", "����������", true},
};
assert_enum(state, LastState);
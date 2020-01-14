#include "main.h"

struct gender_change_string {
	const char*	id;
	const char*	female;
	const char*	male;
	const char*	multiply;
};
static gender_change_string player_gender[] = {{"а", "а", "", "и"},
{"ла", "ла", "", "ли"},
{"ась", "ась", "ся", "ись"},
{"ая", "ая", "ый", "ые"},
{"ей", "ей", "ему", "им"},
{"нее", "нее", "него", "них"},
{"она", "она", "он", "они"},
{"ее", "ее", "его", "их"},
{"стерва", "стерва", "ублюдок", "ублюдки"},
{"леди", "леди", "лорды", "лорды"},
{"миледи", "миледи", "милорд", "милорды"},
{"госпожа", "госпожа", "господин", "господа"},
{"женщина", "женщина", "мужчина", "господа"},
};
static gender_change_string opponent_gender_change[] = {{"А", "а", "", "и"},
{"ЛА", "ла", "", "ли"},
{"ОНА", "она", "он", "они"},
{"ЕЕ", "ее", "его", "их"},
};

static void addsquad(string& sb, const char* name, int count) {
	if(count <= 1)
		sb.add(name);
	else if(count <= 4) {
		sb.add("%1i ", count);
		sb.addof(name);
	} else {
		sb.add("%1i ", count);
		sb.addof(name);
	}
}

static void addsquadof(string& sb, const char* name, int count) {
	if(count <= 1)
		sb.addof(name);
	else if(count <= 4) {
		sb.add("%1i ", count);
		sb.addof(name);
	} else {
		sb.add("%1i ", count);
		sb.addof(name);
	}
}

void string::addidentifier(const char* identifier) {
	if(name && strcmp(identifier, "герой") == 0)
		add(name);
	else if(name && strcmp(identifier, "героя") == 0)
		addof(name);
	else if(name && strcmp(identifier, "герою") == 0)
		addto(name);
	else if(opponent_name && strcmp(identifier, "оппонент") == 0)
		add(opponent_name);
	else if(opponent_name && strcmp(identifier, "оппонента") == 0)
		addof(opponent_name);
	else if(opponent_name && strcmp(identifier, "оппоненту") == 0)
		addto(opponent_name);
	else if(player && strcmp(identifier, "раса") == 0)
		add(bsmeta<racei>::elements[player->getrace()].name);
	else if(player && strcmp(identifier, "класс") == 0)
		add(player->getclass().name);
	else if(player && strcmp(identifier, "имя") == 0)
		add(player->getname());
	else {
		for(auto& e : player_gender) {
			if(strcmp(e.female, identifier) != 0)
				continue;
			if(gender == NoGender)
				add(e.multiply);
			else if(gender == Female)
				add(e.female);
			else
				add(e.male);
			return;
		}
		for(auto& e : opponent_gender_change) {
			if(strcmp(e.id, identifier) != 0)
				continue;
			if(opponent_gender == NoGender)
				add(e.multiply);
			else if(opponent_gender == Female)
				add(e.female);
			else
				add(e.male);
			return;
		}
		stringbuilder::addidentifier(identifier);
	}
}
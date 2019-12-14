#include "main.h"

gender_s nameable::getgender() const {
	return Male;
}

const char* nameable::getname() const {
	return "Павел";
}

void nameable::actv(stringbuilder& sb, const char* format, const char* param) const {

}

void nameable::actv(stringbuilder& sb, nameable& e, const char* format, const char* param) const {

}
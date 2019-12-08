#include "answer.h"

int answeri::compare(const void* p1, const void* p2) {
	auto e1 = (answeri::element*)p1;
	auto e2 = (answeri::element*)p2;
	return strcmp(e1->text, e2->text);
}

void answeri::sort() {
	qsort(elements.data, elements.getcount(), sizeof(elements[0]), compare);
}

void answeri::addv(int param, int priority, const char* format, const char* format_param) {
	if(elements.getcount() >= elements.getmaximum())
		return;
	if(get() >= end())
		return;
	if(get() > begin())
		addsz();
	auto pe = elements.add();
	pe->param = param;
	pe->priority = param;
	pe->text = get();
	stringbuilder::addv(format, format_param);
	*((char*)pe->text) = stringbuilder::upper(pe->text[0]);
}

void answeri::add(int param, const char* format, ...) {
	addv(param, 0, format, xva_start(format));
}

void answeri::addp(int param, int priority, const char* format, ...) {
	addv(param, priority, format, xva_start(format));
}

int	answeri::choose(bool interactive, bool clear_text, const char* format, ...) const {
	char temp[512]; stringbuilder sb(temp);
	sb.addv(format, xva_start(format));
	return choosev(interactive, clear_text, false, sb);
}

int	answeri::choose() const {
	return choosev(true, true, false, 0);
}
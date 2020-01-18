#include "main.h"
#include "archive.h"

static void getfilename(stringbuilder& sb, indext index, int level) {
	sb.add("game/%05i%03i", index, level);
}

static bool serial(location& e, const char* url, bool write_mode) {
	io::file file(url, write_mode ? StreamWrite : StreamRead);
	if(!file)
		return false;
	archive a(file, write_mode);
	a.set(e);
	a.set(bsmeta<outdoor>::source);
	a.set(bsmeta<creature>::source);
	return true;
}

static void remove_party() {
	boosti boost_source[64];
	array source(boost_source);
}

bool location::write(const char* url) const {
	return serial(*const_cast<location*>(this), url, true);
}

bool location::write(indext index, int level) {
	char temp[260]; stringbuilder sb(temp); getfilename(sb, index, level);
	return write(temp);
}

bool location::read(const char* url) {
	return serial(*const_cast<location*>(this), url, false);
}

bool location::read(indext index, int level) {
	char temp[260]; stringbuilder sb(temp); getfilename(sb, index, level);
	return read(temp);
}
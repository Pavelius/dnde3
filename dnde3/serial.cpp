#include "main.h"
#include "archive.h"

static void getfilename(stringbuilder& sb, indext index, int level) {
	sb.add("game/%1.5i%2.3i.loc", index, level);
}

static bool serial(location& e, const char* url, bool write_mode) {
	io::file file(url, write_mode ? StreamWrite : StreamRead);
	if(!file)
		return false;
	archive a(file, write_mode);
	a.set(e);
	a.set(bsmeta<creature>::source);
	a.set(bsmeta<site>::source);
	a.set(bsmeta<boosti>::source);
	a.set(bsmeta<itemground>::source);
	return true;
}

static bool serial(gamei& e, bool write_mode) {
	io::file file("game/game.dat", write_mode ? StreamWrite : StreamRead);
	if(!file)
		return false;
	archive a(file, write_mode);
	a.set(e);
	return true;
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

bool gamei::read() {
	if(!serial(*this, false))
		return false;
	if(!loc.read(getposition(), getlevel()))
		return false;
	return true;
}

bool gamei::write() {
	if(!serial(*this, true))
		return false;
	if(!loc.write(getposition(), getlevel()))
		return false;
	return true;
}
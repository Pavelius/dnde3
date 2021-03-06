#include "main.h"
#include "archive.h"

static void getfilename(stringbuilder& sb, indext index, int level) {
	sb.add("game/%1.5i%2.3i.loc", index, level);
}

static bool serial(location& e, const char* url, bool write_mode, bool overland) {
	io::file file(url, write_mode ? StreamWrite : StreamRead);
	if(!file)
		return false;
	archive a(file, write_mode);
	a.set(e);
	a.set(bsdata<creature>::source);
	if(!overland) {
		a.set(bsdata<site>::source);
		a.set(bsdata<boosti>::source);
		a.set(bsdata<itemground>::source);
	}
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

bool location::write(const char* url, bool overland) {
	saveround = game.getrounds();
	return serial(*this, url, true, overland);
}

bool location::write(indext index, int level) {
	char temp[260]; stringbuilder sb(temp); getfilename(sb, index, level);
	return write(temp, false);
}

bool location::read(const char* url, bool overland) {
	return serial(*const_cast<location*>(this), url, false, overland);
}

bool location::read(indext index, int level) {
	char temp[260]; stringbuilder sb(temp); getfilename(sb, index, level);
	return read(temp, false);
}

bool gamei::read() {
	if(!serial(*this, false))
		return false;
	if(isoverland())
		return loc.read("game/overland.loc", true);
	else
		return loc.read(getposition(), getlevel());
}

bool gamei::write() {
	if(!serial(*this, true))
		return false;
	if(isoverland())
		return loc.write("game/overland.loc", true);
	else
		return loc.write(getposition(), getlevel());
}
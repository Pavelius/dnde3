#include "main.h"
#include "archive.h"

static bool serial(location& e, const char* url, bool write_mode) {
	io::file file(url, write_mode ? StreamWrite : StreamRead);
	if(!file)
		return false;
	archive a(file, write_mode);
	a.set(e);
	return true;
}

bool location::write(const char* url) const {
	return serial(*const_cast<location*>(this), url, false);
}

bool location::read(const char* url) {
	return serial(*const_cast<location*>(this), url, true);
}
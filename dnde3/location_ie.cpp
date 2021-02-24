#include "main.h"
#include "io.h"

class archivet {
	typedef char identifiert[260];
	io::file&		file;
	const char*		pf;
	const char*		p;
	void write(const char* v) {
		file.write(v, zlen(v));
	}
	bool readind(identifiert& id) {
		if(!stringbuilder::ischa(*p))
			return false;
		p = psidn(p, id, id + sizeof(id));
		skipsp();
		return true;
	}
	bool skip(const char* sym) {
		auto i = 0;
		while(sym[i]) {
			if(p[i] != sym[i])
				return false;
			i++;
		}
		p += i;
		return true;
	}
	void skipsp() {
		while(*p && *p == ' ' || *p == '\t')
			p++;
	}
public:
	archivet(io::file& file) : file(file), pf(0), p(0) { }
	archivet(const char* url) : file(file), pf(loadt(url)) { p = pf; }
	~archivet() {
		if(pf)
			delete pf;
	}
	bool isread() const { return p != 0; }
	tile_s findtile(char symbol) const {
		for(auto& e : bsdata<tilei>()) {
			if(e.symbol == symbol)
				return (tile_s)(&e - bsdata<tilei>::elements);
		}
		return Plain;
	}
	void set(location& e) {
		if(!p) {
			for(auto y = 0; y < mmy; y++) {
				for(auto x = 0; x < mmx; x++) {
					auto i = loc.get(x, y);
					if(i == Blocked)
						continue;
					auto t = loc.gettile(i);
					file.write(&bsdata<tilei>::elements[t].symbol, sizeof(bsdata<tilei>::elements[t].symbol));
				}
				write("\r\n");
			}
		} else {
			int x = 0, y = 0;
			while(*p) {
				if(*p == 13 || *p == 10) {
					p = zskipspcr(p);
					y++; x = 0;
				} else {
					auto i = findtile(*p++);
					auto index = loc.get(x, y);
					loc.set(index, (tile_s)i);
					x++;
				}
			}
		}
	}
};

void location::serialx(const char* url, bool write_mode) {
	if(write_mode) {
		io::file file(url, StreamWrite);
		if(!file)
			return;
		archivet a(file);
		a.set(*this);
	} else {
		archivet a(url);
		if(!a.isread())
			return;
		a.set(*this);
	}
}
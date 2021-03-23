export module stringbuilder;

#define xva_start(v) ((const char*)&v + sizeof(v))

extern "C" int memcmp(const void* p1, const void* p2, unsigned size);
extern "C" void* memmove(void* destination, const void* source, unsigned size);
extern "C" void* memcpy(void* destination, const void* source, unsigned size);

static const char spaces[] = {" \n\t\r.,!?;:"};

export constexpr const char* zend(const char* p) {
	while(*p)
		p++;
	return p;
}

export constexpr unsigned zlen(const char* p) {
	return zend(p) - p;
}

static const char* word_end(const char* ps) {
	while(*ps) {
		for(auto e : spaces) {
			if(*ps == e)
				return ps;
		}
		ps++;
	};
	return ps;
}

static const char* skip_space(const char* ps) {
	while(*ps) {
		for(auto e : spaces) {
			if(*ps != e)
				return ps;
		}
		ps++;
	};
	return ps;
}

static inline bool is_space(char sym) {
	for(auto e : spaces) {
		if(sym == e)
			return true;
	}
	return false;
}

struct grammar {
	const char*		name;
	const char*		change;
	unsigned		name_size;
	unsigned		change_size;
	constexpr grammar() : name(0), change(0), name_size(0), change_size(0) {}
	grammar(const char* name, const char* change) :
		name(name), change(change), name_size(zlen(name)), change_size(zlen(change)) {
	}
	operator bool() const { return name != 0; }
};

export unsigned char upper(unsigned char sym) {
	if(sym >= 0x61 && sym <= 0x7A)
		return sym - 0x61 + 0x41;
	else if(sym >= 0xE0)
		return sym - 0xE0 + 0xC0;
	return sym;
}

export unsigned char lower(unsigned char sym) {
	if(sym >= 0x41 && sym <= 0x5A)
		return sym - 0x41 + 0x61;
	else if(sym >= 0xC0 && sym <= 0xDF)
		return sym - 0xC0 + 0xE0;
	return sym;
}

export bool	ischa(unsigned char sym) {
	return (sym >= 'A' && sym <= 'Z') || (sym >= 'a' && sym <= 'z') || sym >= 0xC0;
}

export bool	isnum(unsigned char sym) {
	return sym >= '0' && sym <= '9';
}

// Convert unsigned integer value to string
export char* adduint(char* result, const char* result_maximum, unsigned value, int precision, const int radix) {
	char temp[32]; int i = 0;
	if(!value) {
		if(result < result_maximum)
			*result++ = '0';
		if(result < result_maximum)
			*result = 0;
		return result;
	}
	if(!result_maximum)
		result_maximum = result + 32;
	while(value) {
		temp[i++] = (value % radix);
		value /= radix;
	}
	while(precision-- > i) {
		if(result < result_maximum)
			*result++ = '0';
	}
	while(i) {
		auto v = temp[--i];
		if(result < result_maximum) {
			if(v < 10)
				*result++ = '0' + v;
			else
				*result++ = 'A' + (v - 10);
		}
	}
	result[0] = 0;
	return result;
}

// Convert integer value to string
export char* addint(char* result, const char* result_maximum, int value, int precision, const int radix) {
	if(value < 0) {
		if(result < result_maximum)
			*result++ = '-';
		value = -value;
	}
	return adduint(result, result_maximum, value, precision, radix);
}

export class stringbuilder {
	char* p;
	char* pb;
	const char*	pe;
	const char*	readformat(const char* src, const char* vl) {
		if(*src == '%') {
			src++;
			if(p < pe)
				*p++ = '%';
			*p = 0;
			return src;
		}
		*p = 0;
		char prefix = 0;
		if(*src == '+' || *src == '-')
			prefix = *src++;
		if(*src >= '0' && *src <= '9') {
			// Если число, просто подставим нужный параметр
			int pn = 0, pnp = 0;
			while(isnum(*src))
				pn = pn * 10 + (*src++) - '0';
			if(src[0] == '.' && (src[1] >= '0' && src[1] <= '9')) {
				src++;
				while(*src >= '0' && *src <= '9')
					pnp = pnp * 10 + (*src++) - '0';
			}
			if(*src == 'i') {
				src++;
				auto value = ((int*)vl)[pn - 1];
				if(prefix == '+' && value >= 0) {
					if(p < pe)
						*p++ = '+';
				}
				p = addint(p, pe, value, pnp, 10);
			} else if(*src == 'h') {
				src++;
				p = adduint(p, pe, (unsigned)(((int*)vl)[pn - 1]), pnp, 16);
			} else {
				if(((char**)vl)[pn - 1]) {
					auto p0 = p;
					auto p1 = ((char**)vl)[pn - 1];
					while(*p1 && p < pe)
						*p++ = *p1++;
					if(p < pe)
						*p = 0;
					switch(prefix) {
					case '-': *p0 = lower(*p0); break;
					case '+': *p0 = upper(*p0); break;
					default: break;
					}
				}
			}
		} else {
			auto p0 = p;
			src = readvariable(src);
			switch(prefix) {
			case '-': *p0 = lower(*p0); break;
			case '+': *p0 = upper(*p0); break;
			default: break;
			}
		}
		return src;
	}
	const char*	readvariable(const char* format) {
		char temp[260];
		auto ps = temp;
		auto pe = temp + sizeof(temp) - 1;
		if(*p == '(') {
			p++;
			while(*p && *p != ')') {
				if(ps < pe)
					*ps++ = *p;
			}
			if(*p == ')')
				p++;
		} else {
			while(*p && (ischa(*p) || isnum(*p) || *p == '_')) {
				if(ps < pe)
					*ps++ = *p++;
				else
					break;
			}
		}
		*ps = 0;
		addidentifier(temp);
		return p;
	}
	void add(const char* s, const grammar* source, const char* def = 0) {
		auto ps = skip_space(s);
		while(*ps) {
			auto pw = word_end(ps);
			unsigned s1 = pw - ps;
			auto found = false;
			for(auto pg = source; *pg; pg++) {
				auto s2 = pg->name_size;
				if(pg->name_size > s1)
					continue;
				if(memcmp(pw - s2, pg->name, s2) == 0) {
					auto s3 = pg->change_size;
					memcpy(p, ps, s1 - s2);
					memcpy(p + (s1 - s2), pg->change, s3);
					p += (s1 - s2 + s3);
					if(pw[0] == 0)
						def = 0;
					found = true;
					break;
				}
			}
			if(!found) {
				memcpy(p, ps, pw - ps);
				p += pw - ps;
			}
			ps = pw;
			while(*ps) {
				if(is_space(*ps))
					*p++ = *ps++;
				break;
			}
		}
		p[0] = 0;
		if(def)
			add(def);
	}
public:
	constexpr stringbuilder(char* pb, const char* pe) : pb(pb), p(pb), pe(pe) {
	}
	template<unsigned N> constexpr stringbuilder(char(&result)[N]) : stringbuilder(result, result + N - 1) {
	}
	constexpr operator char*() const {
		return pb;
	}
	constexpr explicit operator bool() const {
		return p != pb;
	}
	constexpr char* begin() {
		return pb;
	}
	constexpr const char* begin() const {
		return pb;
	}
	void clear() {
		pb[0] = 0;
		p = pb;
	}
	const char* end() const {
		return pe;
	}
	void add(const char* format, ...) {
		addv(format, xva_start(format));
	}
	void add(char symbol) {
		if(p < pe)
			*p++ = symbol;
		*p = 0;
	}
	void addby(const char* s) {
		static grammar map[] = {{"ая", "ой"},
			{"ый", "ым"}, {"ое", "ым"}, {"ой", "ым"},
			{"би", "би"},
			{"ий", "им"},
			{"ец", "цем"},
			{"ки", "ками"},
			{"й", "ем"}, {"ь", "ем"}, {"е", "ем"},
			{"а", "ой"},
			{"ч", "чем"},
			{}};
		add(s, map, "ом");
	}
	void addcn(const char* name, int count) {
		if(!count)
			return;
		else if(count == 1)
			add(name);
		else if(count <= 4) {
			add("%1i ", count);
			addof(name);
		} else {
			add("%1i ", count);
			addby(name);
		}
	}
	virtual void addidentifier(const char* identifier) {
		addv("[-", 0);
		addv(identifier, 0);
		addv("]", 0);
	}
	void addicon(const char* id, int value) {
		if(value < 0)
			adds(":%1:[-%2i]", id, -value);
		else
			adds(":%1:%2i", id, value);
	}
	void addn(const char* format, ...) {
		addx('\n', format, xva_start(format));
	}
	void addof(const char* s) {
		static grammar map[] = {{"ый", "ого"},
			{"ий", "ого"},
			{"ое", "ого"},
			{"ая", "ой"},
			{"би", "би"},
			{"ты", "т"},
			{"сы", "сов"},
			{"ны", "н"},
			{"ок", "ка"},
			{"а", "ы"},
			{"ь", "и"},
			{"о", "а"},
			{"я", "и"},
			{}};
		add(s, map, "а");
	}
	static const char* addof(const stringbuilder& sbn, const char* s) {
		auto sb = sbn;
		sb.addof(s);
		return sb;
	}
	void adds(const char* format, ...) {
		addx(' ', format, xva_start(format));
	}
	void addsep(char separator) {
		if(p <= pb || p >= pe)
			return;
		if(p[-1] == separator)
			return;
		switch(separator) {
		case ' ':
			if(p[-1] == '\n' || p[-1] == '\t')
				return;
			break;
		case '.':
			if(p[-1] == '?' || p[-1] == '!' || p[-1] == ':')
				return;
			break;
		}
		*p++ = separator;
		*p = 0;
	}
	void addsz() {
		if(p < pe)
			*p++ = 0;
	}
	void addto(const char* s) {
		static grammar map[] = {{"а", "е"},
			{"о", "у"},
			{"ы", "ам"},
			{}
		};
		add(s, map, "у");
	}
	static const char* addto(const stringbuilder& sbn, const char* s) {
		auto sb = sbn;
		sb.addto(s);
		return sb;
	}
	void addv(const char* format, const char* format_param) {
		if(!p)
			return;
		p[0] = 0;
		if(!format)
			return;
		while(true) {
			switch(*format) {
			case 0: *p = 0; return;
			case '%': format = readformat(format + 1, format_param); break;
			default:
				if(p < pe)
					*p++ = *format;
				format++;
				break;
			}
		}
	}
	// Add string with separator
	void addx(char separator, const char* format, const char* format_param) {
		if(!format || format[0] == 0)
			return;
		addsep(separator);
		addv(format, format_param);
	}
	// Add separator if string is not emphty, then add string
	void addx(const char* separator, const char* format, const char* format_param) {
		if(!format || format[0] == 0)
			return;
		if(p != pb)
			add(separator);
		addv(format, format_param);
	}
	char* get() const {
		return p;
	}
	bool ispos(const char* v) const {
		return p == v;
	}
	void normalize() {
		bool need_upper_case = true;
		for(auto p = pb; *p && p < pe; p++) {
			if(*p == ' ' || *p == 9)
				continue;
			if(*p == 10 || *p == 13 || *p == '.' || *p == '?' || *p == '!') {
				need_upper_case = true;
				continue;
			}
			if(need_upper_case) {
				*p = upper(*p);
				need_upper_case = false;
			}
		}
	}
	void set(char* v) {
		p = v;
		p[0] = 0;
	}
};

export typedef const char* (*fntext)(const void* object, stringbuilder& sb);

char* szprint(char* result, const char* result_maximum, const char* src, ...) {
	stringbuilder e(result, result_maximum);
	e.addv(src, xva_start(src));
	return e;
}

char* szprintv(char* result, const char* result_maximum, const char* format, const char* format_param) {
	stringbuilder e(result, result_maximum);
	e.addv(format, format_param);
	return e;
}
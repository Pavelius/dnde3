#pragma once

#define xva_start(v) ((const char*)&v + sizeof(v))

class stringbuilder {
	struct grammar;
	char*				p;
	char*				pb;
	const char*			pe;
	const char*			readformat(const char* format, const char* format_param);
	const char*			readvariable(const char* format);
	void				add(const char* s, const grammar* source, const char* def = 0);
public:
	constexpr stringbuilder(char* pb, const char* pe) : pb(pb), p(pb), pe(pe) {}
	template<unsigned N> constexpr stringbuilder(char(&result)[N]) : stringbuilder(result, result + N - 1) {}
	constexpr operator char*() const { return pb; }
	constexpr explicit operator bool() const { return p!=pb; }
	void				add(const char* format, ...) { addv(format, xva_start(format)); }
	void				add(char symbol);
	void				addby(const char* s);
	void				addcn(const char* name, int count);
	virtual void		addidentifier(const char* identifier);
	void				addicon(const char* id, int value);
	static char*		addint(char* result, const char* result_maximum, int value, int precision, const int radix);
	void				addn(const char* format, ...) { addx('\n', format, xva_start(format)); }
	void				addof(const char* s);
	static const char*	addof(const stringbuilder& sbn, const char* s);
	void				adds(const char* format, ...) { addx(' ', format, xva_start(format)); }
	void				addsep(char separator);
	void				addsz() { if(p < pe) *p++ = 0; }
	void				addto(const char* s);
	static const char*	addto(const stringbuilder& sbn, const char* s);
	void				addv(const char* format, const char* format_param);
	void				addx(char separator, const char* format, const char* format_param);
	void				addx(const char* separator, const char* format, const char* format_param);
	static char*		adduint(char* result, const char* result_maximum, unsigned value, int precision, const int radix);
	char*				begin() { return pb; }
	const char*			begin() const { return pb; }
	void				clear() { pb[0] = 0; p = pb; }
	const char*			end() const { return pe; }
	char*				get() const { return p; }
	static bool			ischa(unsigned char sym) { return (sym >= 'A' && sym <= 'Z') || (sym >= 'a' && sym <= 'z') || sym>=0xC0; }
	static bool			isnum(unsigned char sym) { return sym >= '0' && sym <= '9'; }
	bool				ispos(const char* v) const { return p == v; }
	static unsigned char lower(unsigned char sym);
	void				normalize();
	void				set(char* v) { p = v; p[0] = 0; }
	static unsigned char upper(unsigned char sym);
};
typedef const char* (*fntext)(const void* object, stringbuilder& sb);
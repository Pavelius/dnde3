#include "crt.h"
#include "stringbuilder.h"

#pragma once

class answeri : stringbuilder {
	struct element {
		int						param;
		int						priority;
		const char*				text;
	};
	char						buffer[4096];
	adat<element, 32>			elements;
	int							paint(int x, int y, int width, int i, int& maximum_width) const;
public:
	typedef void(*tipspr)(stringbuilder& sb, int param);
	typedef void(*callpr)();
	explicit operator bool() const { return elements.count != 0; }
	constexpr answeri() : stringbuilder(buffer, buffer + sizeof(buffer) - 1), buffer(), elements() {}
	void						add(int param, const char* format, ...);
	void						addp(int param, int priority, const char* format, ...);
	void						addv(int param, int priority, const char* format, const char* format_param);
	int							choose() const;
	int							choose(bool interactive, bool clear_text, const char* format, ...) const;
	int							choosev(bool interactive, bool clear_text, bool return_single, const char* format) const;
	void						clear() { stringbuilder::clear(); elements.clear(); }
	static int					compare(const void* p1, const void* p2);
	int							paint(int x, int y, int width) const;
	void						sort();
};

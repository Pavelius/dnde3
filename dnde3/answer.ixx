export module answer;
import collections;
import stringbuilder;

#define xva_start(v) ((const char*)&v + sizeof(v))

extern "C" int strcmp(const char* s1, const char* s2); // Compare two strings
extern "C" void qsort(void* base, unsigned num, unsigned size, int(*compar)(const void*, const void*));

export class answeri : stringbuilder {
	struct element {
		int						param;
		int						priority;
		const char*				text;
	};
	char						buffer[4096];
	adat<element, 48>			elements;
	int							paint(int x, int y, int width, int i, int& maximum_width) const;
	int							paint(int x, int y, int width, const char* format, int& maximum_width) const;
	static const char*			getnmshortcut(const void* object, stringbuilder& sb);
	void						information(const char* text, fntext fparam, int width) const;
public:
	typedef void(*tipspr)(stringbuilder& sb, int param);
	typedef void(*callpr)();
	explicit operator bool() const {
		return elements.count != 0;
	}
	constexpr answeri() : stringbuilder(buffer, buffer + sizeof(buffer) - 1), buffer(), elements() {
	}
	void add(int param, const char* format, ...) {
		addv(param, 0, format, xva_start(format));
	}
	void addp(int param, int priority, const char* format, ...) {
		addv(param, priority, format, xva_start(format));
	}
	bool addv(int param, int priority) {
		if(elements.getcount() >= elements.getmaximum())
			return false;
		if(get() >= end())
			return false;
		if(get() > begin())
			addsz();
		auto pe = elements.add();
		pe->param = param;
		pe->priority = priority;
		pe->text = get();
		return true;
	}
	void addv(int param, int priority, const char* format, const char* format_param) {
		if(addv(param, priority)) {
			auto p = get();
			stringbuilder::addv(format, format_param);
			p[0] = upper(p[0]);
		}
	}
	int	choose() const {
		return choosev(true, true, false, 0);
	}
	int choose(bool interactive, bool clear_text, const char* format, ...) const {
		char temp[512]; stringbuilder sb(temp);
		sb.addv(format, xva_start(format));
		return choosev(interactive, clear_text, false, sb);
	}
	int	choosev(bool interactive, bool clear_text, bool return_single, const char* format) const;
	void clear() {
		stringbuilder::clear();
		elements.clear();
	}
	static int compare(const void* p1, const void* p2) {
		auto e1 = (answeri::element*)p1;
		auto e2 = (answeri::element*)p2;
		return strcmp(e1->text, e2->text);
	}
	int dialogv(bool allow_cancel, const char* title, const char* format) const;
	static int getindex(char v) {
		if(v >= '1' && v <= '9')
			return v - '1';
		return v - 'A';
	}
	char getkey(int v) {
		if(v < 9)
			return '1' + v;
		return 'A' + (v - 9);
	}
	stringbuilder& getsb() {
		return *this;
	}
	int							menuv(bool allow_cancel, const char* format) const;
	void						shortcuts(const char* text) const { information(text, getnmshortcut, 100); }
	void sort() {
		qsort(elements.data, elements.getcount(), sizeof(elements[0]), compare);
	}
};
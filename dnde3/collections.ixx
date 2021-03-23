export module collections;
import stringbuilder;

//extern "C" int memcmp(const void* p1, const void* p2, unsigned size);
//extern "C" void* memmove(void* destination, const void* source, unsigned size);
//extern "C" void* memcpy(void* destination, const void* source, unsigned size);
extern "C" void	free(void* pointer);
extern "C" void* malloc(unsigned size);
export extern "C" void* memset(void* destination, int value, unsigned size);
extern "C" int rand(void);
extern "C" void* realloc(void *ptr, unsigned size);
extern "C" int strcmp(const char* s1, const char* s2); // Compare two strings

export enum codepages {
	CPNONE, CP1251, CPUTF8, CPU16BE, CPU16LE
};
export namespace metrics {
const codepages		code = CP1251;
}

namespace std {
export template<class T>
class initializer_list { // list of pointers to elements
	const T*				first;
	const T*				last;
public:
	typedef T				value_type;
	typedef const T&		reference;
	typedef const T&		const_reference;
	typedef unsigned		size_type;
	typedef const T*		iterator;
	typedef const T*		const_iterator;
	constexpr initializer_list() noexcept : first(0), last(0) {}
	constexpr initializer_list(const T *first_arg, const T *last_arg) noexcept : first(first_arg), last(last_arg) {}
	constexpr const T*		begin() const noexcept { return first; }
	constexpr const T*		end() const noexcept { return last; }
	constexpr unsigned		size() const noexcept { return last - first; }
};
}

export unsigned rmoptimal(unsigned need_count) {
	const unsigned mc = 256 * 256 * 256;
	unsigned m = 16;
	while(m < mc) {
		if(need_count < m)
			return m;
		m = m << 1;
	}
	return m;
}

export void* rmreserve(void* data, unsigned new_size) {
	if(data)
		return realloc(data, new_size);
	return malloc(new_size);
}

export void rmreserve(void** data, unsigned count, unsigned& count_maximum, unsigned size) {
	if(count >= count_maximum) {
		count_maximum = rmoptimal(count + 1);
		*data = rmreserve(*data, count_maximum * size);
	}
}

export void rmremove(void* data, unsigned size, unsigned index, unsigned& count, int elements_count) {
	if(index >= count)
		return;
	count -= elements_count;
	if(index >= count)
		return;
	memmove((char*)data + index * size, (char*)data + (index + elements_count) * size, (count - index) * size);
}

export unsigned szget(const char** input, codepages code = metrics::code) {
	const unsigned char* p;
	unsigned result;
	switch(code) {
	case CPUTF8:
		p = (unsigned char*)*input;
		result = *p++;
		if(result >= 192 && result <= 223)
			result = (result - 192) * 64 + (*p++ - 128);
		else if(result >= 224 && result <= 239) {
			result = (result - 224) * 4096 + (p[0] - 128) * 64 + (p[1] - 128);
			p += 2;
		}
		*input = (const char*)p;
		return result;
	case CPU16LE:
		p = (unsigned char*)*input;
		result = p[0] | (p[1] << 8);
		*input = (const char*)(p + 2);
		return result;
	case CP1251:
		result = (unsigned char)*(*input)++;
		if(((unsigned char)result >= 0xC0))
			return result - 0xC0 + 0x410;
		else switch(result) {
		case 0xB2: return 0x406;
		case 0xAF: return 0x407;
		case 0xB3: return 0x456;
		case 0xBF: return 0x457;
		}
		return result;
	default:
		return *(*input)++;
	}
}

export void szput(char** output, unsigned value, codepages code = metrics::code) {
	char* p;
	switch(code) {
	case CPUTF8:
		p = *output;
		if(((unsigned short)value) < 128)
			*p++ = (unsigned char)value;
		else if(((unsigned short)value) < 2047) {
			*p++ = (unsigned char)(192 + (((unsigned short)value) / 64));
			*p++ = (unsigned char)(128 + (((unsigned short)value) % 64));
		} else {
			*p++ = (unsigned char)(224 + (((unsigned short)value) / 4096));
			*p++ = (unsigned char)(128 + ((((unsigned short)value) / 64) % 64));
			*p++ = (unsigned char)(224 + (((unsigned short)value) % 64));
		}
		*output = p;
		break;
	case CP1251:
		if(value >= 0x410 && value <= 0x44F)
			value = value - 0x410 + 0xC0;
		else switch(value) {
		case 0x406: value = 0xB2; break; // I
		case 0x407: value = 0xAF; break; // ¯
		case 0x456: value = 0xB3; break;
		case 0x457: value = 0xBF; break;
		}
		*(*output)++ = (unsigned char)value;
		break;
	case CPU16LE:
		*(*output)++ = (unsigned char)(value & 0xFF);
		*(*output)++ = (unsigned char)(((unsigned)value >> 8));
		break;
	case CPU16BE:
		*(*output)++ = (unsigned char)(((unsigned)value >> 8));
		*(*output)++ = (unsigned char)(value & 0xFF);
		break;
	default:
		*(*output)++ = (unsigned char)value;
		break;
	}
}

export char* szupper(char* p, int count) {
	char* s1 = p;
	const char* p1 = p;
	while(count-- > 0)
		szput(&s1, upper(szget(&p1)));
	return p;
}

export int getdigitscount(unsigned number) {
	if(number < 10)
		return 1;
	if(number < 100)
		return 2;
	if(number < 1000)
		return 3;
	if(number < 10000)
		return 4;
	if(number < 100000)
		return 5;
	if(number < 1000000)
		return 6;
	if(number < 10000000)
		return 7;
	if(number < 100000000)
		return 8;
	return 9;
}

export void szlower(char* p, int count) {
	char* s1 = p;
	const char* p1 = p;
	if(count == -1) {
		while(true) {
			unsigned sym = szget(&p1);
			if(!sym)
				break;
			szput(&s1, lower(sym));
		}
		szput(&s1, 0);
	} else {
		while(count-- > 0)
			szput(&s1, lower(szget(&p1)));
	}
}

export char* szput(char* result, unsigned sym, codepages page = metrics::code) {
	char* p = result;
	szput(&p, sym, page);
	*p = 0;
	return result;
}

export void szencode(char* output, int output_count, codepages output_code, const char* input, int input_count, codepages input_code = metrics::code) {
	char* s1 = output;
	char* s2 = s1 + output_count;
	const char* p1 = input;
	const char* p2 = p1 + input_count;
	while(p1 < p2 && s1 < s2)
		szput(&s1, szget(&p1, input_code), output_code);
	if(s1 < s2) {
		s1[0] = 0;
		if((output_code == CPU16BE || output_code == CPU16LE) && (s1 + 1) < s2)
			s1[1] = 0;
	}
}

export int szcmpi(const char* p1, const char* p2) {
	while(*p2 && *p1) {
		unsigned s1 = upper(szget(&p1));
		unsigned s2 = upper(szget(&p2));
		if(s1 == s2)
			continue;
		return s1 - s2;
	}
	unsigned s1 = upper(szget(&p1));
	unsigned s2 = upper(szget(&p2));
	return s1 - s2;
}

export int szcmpi(const char* p1, const char* p2, int max_count) {
	while(*p2 && *p1 && max_count-- > 0) {
		unsigned s1 = upper(szget(&p1));
		unsigned s2 = upper(szget(&p2));
		if(s1 == s2)
			continue;
		return s1 - s2;
	}
	if(!max_count)
		return 0;
	unsigned s1 = upper(szget(&p1));
	unsigned s2 = upper(szget(&p2));
	return s1 - s2;
}

export bool matchuc(const char* name, const char* filter) {
	if(!name || name[0] == 0)
		return false;
	auto sym = upper(szget(&filter));
	auto pn = name;
	while(pn[0]) {
		auto sym1 = upper(szget(&pn));
		if(sym1 == sym) {
			auto pf = filter;
			auto ps = pn;
			while(true) {
				if(pf[0] == 0)
					return true;
				auto sym2 = upper(szget(&pf));
				auto sym1 = upper(szget(&pn));
				if(sym1 != sym2)
					break;
			}
			pn = ps;
		}
	}
	return false;
}

export template<typename T>
T imax(T a, T b) {
	return a > b ? a : b;
}

export template<typename T>
T imin(T a, T b) {
	return a < b ? a : b;
}

export template<typename T>
T iabs(T a) {
	return a > 0 ? a : -a;
}

export template<typename T>
void iswap(T & a, T & b) {
	T i = a;
	a = b;
	b = i;
}

export template<typename T, T v>
struct static_value {
	static constexpr T value = v;
};

export template<int v>
struct static_int : static_value<int, v> {
};

// Storge like vector
export template<typename T, int count_max = 128>
struct adat {
	T			data[count_max];
	unsigned	count;
	constexpr adat() : count(0) {}
	constexpr adat(const std::initializer_list<T> list) : count(0) { for(auto& e : list) *add() = e; }
	constexpr const T& operator[](unsigned index) const { return data[index]; }
	constexpr T& operator[](unsigned index) { return data[index]; }
	explicit operator bool() const { return count != 0; }
	T* add() {
		if(count < count_max)
			return data + (count++);
		return 0;
	}
	void add(const T& e) {
		if(count < count_max)
			data[count++] = e;
	}
	T* begin() {
		return data;
	}
	const T* begin() const {
		return data;
	}
	void clear() {
		count = 0;
	}
	T* end() {
		return data + count;
	}
	const T* end() const {
		return data + count;
	}
	T* endof() const {
		return (T*)data + count_max;
	}
	int getcount() const {
		return count;
	}
	int getmaximum() const {
		return count_max;
	}
	int indexof(const T* e) const {
		if(e >= data && e < data + count)
			return e - data;
		return -1;
	}
	int	indexof(const T t) const {
		for(unsigned i = 0; i < count; i++)
			if(data[i] == t)
				return i;
		return -1;
	}
	bool is(const T t) const {
		return indexof(t) != -1;
	}
	T random() const {
		if(count)
			return data[rand() % count];
		return T();
	}
	void remove(int index, int remove_count = 1) {
		if(index < 0)
			return;
		if(index<int(count - 1))
			memcpy(data + index, data + index + 1, sizeof(data[0]) * (count - index - 1));
		count--;
	}
	void shuffle() {
		zshuffle(data, count);
	}
};

// Reference to array with dymanic size
export template<typename T>
struct aref {
	T* data;
	unsigned count;
	constexpr aref() = default;
	constexpr aref(T* source, unsigned count) : data(source), count(count) {
	}
	template<unsigned N> constexpr aref(T(&data)[N]) : data(data), count(N) {
	}
	template<unsigned N> constexpr aref(adat<T, N>& source) : data(source.data), count(source.count) {
	}
	constexpr T& operator[](int index) {
		return data[index];
	}
	constexpr const T& operator[](int index) const {
		return data[index];
	}
	explicit operator bool() const {
		return count != 0;
	}
	constexpr T* begin() {
		return data;
	}
	constexpr const T* begin() const {
		return data;
	}
	constexpr T* end() {
		return data + count;
	}
	constexpr const T* end() const {
		return data + count;
	}
	int getcount() const {
		return count;
	}
	int indexof(const T* t) const {
		if(t<data || t>data + count)
			return -1;
		return t - data;
	}
	int indexof(const T t) const {
		for(unsigned i = 0; i < count; i++)
			if(data[i] == t)
				return i;
		return -1;
	}
	bool is(const T t) const {
		return indexof(t) != -1;
	}
};

// Abstract flag storage
export template<unsigned c>
class flagable {
	unsigned char			data[c];
public:
	constexpr flagable() : data{0} {}
	constexpr explicit operator bool() const {
		for(auto e : data)
			if(e)
				return true;
		return false;
	}
	template<class T> constexpr flagable(const std::initializer_list<T> v) : data{0} {
		for(auto e : v)
			set(e);
	}
	void clear() {
		memset(this, 0, sizeof(*this));
	}
	constexpr bool is(short unsigned v) const {
		return (data[v / 8] & (1 << (v % 8))) != 0;
	}
	constexpr unsigned getcount() const {
		unsigned r = 0;
		for(unsigned i = 0; i < c * 8; i++)
			if(is(i))
				r++;
		return r;
	}
	constexpr void remove(short unsigned v) {
		data[v / 8] &= ~(1 << (v % 8));
	}
	constexpr void set(const flagable& e) {
		for(unsigned i = 0; i < c; i++)
			data[i] |= e.data[i];
	}
	constexpr void set(short unsigned v) {
		data[v / 8] |= 1 << (v % 8);
	}
	constexpr void set(short unsigned v, bool activate) {
		if(activate)
			set(v);
		else
			remove(v);
	}
};

// Abstract flag data bazed on enumerator
export template<typename T, typename DT = unsigned>
struct cflags {
	DT data;
	constexpr cflags() : data(0) {
	}
	constexpr cflags(const std::initializer_list<T> list) : data() {
		for(auto e : list)
			add(e);
	}
	constexpr void add(const T id) {
		data |= 1 << id;
	}
	constexpr void clear() {
		data = 0;
	}
	constexpr bool is(const T id) const {
		return (data & (1 << id)) != 0;
	}
	constexpr void remove(T id) {
		data &= ~(1 << id);
	}
};

// Abstract value collection
export template<class T>
struct casev {
	T		id;
	char	value;
};

// Abstract value collection
export template<class T, int N>
struct aset {
	char data[N];
	constexpr aset() : data{} {}
	constexpr aset(const std::initializer_list<casev<T>> v) : data{} {
		for(auto e : v)
			set(e.id, e.value);
	}
	constexpr char get(T id) {
		return data[id];
	}
	constexpr void set(T id, int v) {
		data[id] = v;
	}
};

// Abstract array vector
export class array {
	void*					data;
	unsigned				size;
	unsigned				count;
	unsigned				count_maximum;
	bool					growable;
	friend struct archive;
public:
	constexpr array(const array& e) = default;
	constexpr array() : data(0), size(0), count(0), count_maximum(0), growable(false) {
	}
	constexpr array(unsigned size) : data(0), size(size), count(0), count_maximum(0), growable(true) {
	}
	constexpr array(void* data, unsigned size, unsigned count) : data(data), size(size), count(count), count_maximum(0), growable(false) {
	}
	constexpr array(void* data, unsigned size, unsigned count, unsigned count_maximum) : data(data), size(size), count(count), count_maximum(count_maximum), growable(false) {
	}
	template<typename T, unsigned N>
	constexpr array(T(&e)[N]) : array(e, sizeof(T), N) {
	}
	~array() {
		if(isgrowable())
			clear();
	}
	void* add() {
		if(count >= count_maximum) {
			if(isgrowable())
				reserve(count + 1);
			else
				return (char*)data;
		}
		return (char*)data + size * (count++);
	}
	void* add(const void* element) {
		auto p = add();
		memcpy(p, element, getsize());
		return p;
	}
	char* begin() const {
		return (char*)data;
	}
	void clear() {
		count = 0;
		if(!isgrowable())
			return;
		count_maximum = 0;
		if(data)
			delete (char*)data;
		data = 0;
	}
	char* end() const {
		return (char*)data + size * count;
	}
	int	find(const char* value, unsigned offset) const {
		auto m = getcount();
		for(unsigned i = 0; i < m; i++) {
			auto p = (const char**)((char*)ptr(i) + offset);
			if(!(*p))
				continue;
			if(strcmp(*p, value) == 0)
				return i;
		}
		return -1;
	}
	unsigned getmaximum() const {
		return count_maximum;
	}
	unsigned getcount() const {
		return count;
	}
	unsigned getsize() const {
		return size;
	}
	int	indexof(const void* element) const {
		if(element >= data && element < ((char*)data + size * count))
			return ((char*)element - (char*)data) / size;
		return -1;
	}
	void* insert(int index, const void* element) {
		auto count_before = getcount(); add();
		memmove((char*)data + (index + 1) * size, (char*)data + index * size, (count_before - index) * size);
		void* p = ptr(index);
		if(element)
			memcpy(p, element, size);
		else
			memset(p, 0, size);
		return p;
	}
	bool isgrowable() const {
		return growable;
	}
	void* ptr(int index) const {
		return (char*)data + size * index;
	}
	int random() const {
		return count ? (rand() % count) : 0;
	}
	void remove(int index, int elements_count) {
		if(((unsigned)index) >= count)
			return;
		if((unsigned)index < count - elements_count)
			memcpy(ptr(index), ptr(index + elements_count), (count - (index + elements_count)) * getsize());
		count -= elements_count;
	}
	void setcount(unsigned value) {
		count = value;
	}
	void setup(unsigned size) {
		if(!isgrowable())
			return;
		clear();
		this->size = size;
	}
	void shift(int i1, int i2, unsigned c1, unsigned c2) {
		if(i2 < i1) {
			iswap(i2, i1);
			iswap(c1, c2);
		}
		unsigned char* a1 = (unsigned char*)ptr(i1);
		unsigned char* a2 = (unsigned char*)ptr(i2);
		unsigned s1 = c1 * size;
		unsigned s2 = c2 * size;
		unsigned s = (a2 - a1) + s2 - 1;
		for(unsigned i = 0; i < s1; i++) {
			auto a = a1[0];
			memcpy(a1, a1 + 1, s);
			a1[s] = a;
		}
	}
	void sort(int i1, int i2, int(*compare)(const void* p1, const void* p2, void* param), void* param) {
		for(int i = i2; i > i1; i--) {
			for(int j = i1; j < i; j++) {
				auto t1 = ptr(j);
				auto t2 = ptr(j + 1);
				if(compare(t1, t2, param) > 0)
					swap(j, j + 1);
			}
		}
	}
	void swap(int i1, int i2) {
		unsigned char* a1 = (unsigned char*)ptr(i1);
		unsigned char* a2 = (unsigned char*)ptr(i2);
		for(unsigned i = 0; i < size; i++) {
			char a = a1[i];
			a1[i] = a2[i];
			a2[i] = a;
		}
	}
	void reserve(unsigned count) {
		if(!isgrowable())
			return;
		if(!size)
			return;
		if(data && count < count_maximum)
			return;
		count_maximum = rmoptimal(count);
		if(data)
			data = realloc(data, count_maximum * size);
		else
			data = malloc(count_maximum * size);
	}
};

export template<typename T>
struct bsdata {
	typedef T	data_type;
	static T	elements[];
	static array	source;
	static constexpr array*	source_ptr = &source;
	static T* addz() {
		for(auto& e : bsdata())
			if(!e)
				return &e;
		return add();
	}
	static T* add() {
		return (T*)source.add();
	}
	static T* begin() {
		return (T*)source.begin();
	}
	static T* end() {
		return (T*)source.end();
	}
};
template<> struct bsdata<int> {
	typedef int	data_type;
	static constexpr array*	source_ptr = 0;
};
template<> struct bsdata<const char*> : bsdata<int> {
	typedef const char*		data_type;
};
template<> struct bsdata<unsigned char> : bsdata<int> {};
template<> struct bsdata<char> : bsdata<int> {};
template<> struct bsdata<unsigned short> : bsdata<int> {};
template<> struct bsdata<short> : bsdata<int> {};
template<> struct bsdata<unsigned> : bsdata<int> {};
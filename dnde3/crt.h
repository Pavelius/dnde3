#include "my_initialize_list.h"

#pragma once

#define maptbl(t, id) (t[imax((unsigned)0, imin((unsigned)id, (sizeof(t)/sizeof(t[0])-1)))])
#define maprnd(t) t[rand()%(sizeof(t)/sizeof(t[0]))]
#define lenof(t) (sizeof(t)/sizeof(t[0]))
#define zendof(t) (t + sizeof(t)/sizeof(t[0]) - 1)
#define BSDATA(e) e bsdata<e>::elements[]
#define DECLFULL(e) template<> array bsdata<e>::source(bsdata<e>::elements);
#define assert_enum(e, last) static_assert(sizeof(bsdata<e>::elements) / sizeof(bsdata<e>::elements[0]) == last + 1, "Invalid count of " #e " elements"); DECLFULL(e)
#define DECLENUM(e) template<> struct bsdata<e##_s> : bsdata<e##i> {}
#define BSDATAC(e, n) e bsdata<e>::elements[n];\
template<> array bsdata<e>::source(bsdata<e>::elements, sizeof(bsdata<e>::elements[0]), 0, sizeof(bsdata<e>::elements)/sizeof(bsdata<e>::elements[0]));

extern "C" int						atexit(void(*func)(void));
extern "C" void*					bsearch(const void* key, const void *base, unsigned num, unsigned size, int(*compar)(const void *, const void *));
extern "C" unsigned					clock(); // Returns the processor time consumed by the program.
extern "C" void						exit(int exit_code);
extern "C" int						memcmp(const void* p1, const void* p2, unsigned size);
extern "C" void*					memmove(void* destination, const void* source, unsigned size);
extern "C" void*					memcpy(void* destination, const void* source, unsigned size);
extern "C" void*					memset(void* destination, int value, unsigned size);
extern "C" void						qsort(void* base, unsigned num, unsigned size, int(*compar)(const void*, const void*));
extern "C" int						rand(void); // Get next random value
extern "C" void						srand(unsigned seed); // Set random seed
extern "C" int						strcmp(const char* s1, const char* s2); // Compare two strings
extern "C" long long				time(long long* seconds);

template<class T> inline T			imax(T a, T b) { return a > b ? a : b; }
template<class T> inline T			imin(T a, T b) { return a < b ? a : b; }
template<class T> inline T			iabs(T a) { return a > 0 ? a : -a; }
template<class T> inline void		iswap(T & a, T & b) { T i = a; a = b; b = i; }
template<class T> inline const T*	zchr(const T * p, T e) { while(*p) { if(*p == e) return p; p++; } return 0; }
template<class T> inline void		zcpy(T * p1, const T * p2) { while(*p2) *p1++ = *p2++; *p1 = 0; }
template<class T> inline void		zcpy(T * p1, const T * p2, int max_count) { while(*p2 && max_count-- > 0) *p1++ = *p2++; *p1 = 0; }
template<class T> inline T*			zend(T * p) { while(*p) p++; return p; }
template<class T> inline void		zcat(T * p1, const T e) { p1 = zend(p1); p1[0] = e; p1[1] = 0; }
template<class T> inline void		zcat(T * p1, const T * p2) { zcpy(zend(p1), p2); }
template<class T> inline int		zlen(T * p) { return zend(p) - p; }
template<unsigned N> inline char*	zprint(char(&result)[N], const char* format, ...) { return szprintv(result, result + N - 1, format, (const char*)&format + sizeof(format)); }
template<class T> inline T*			zskipsp(T * p) { if(p) while(*p == 32 || *p == 9) p++; return p; }
template<class T> inline T*			zskipspcr(T * p) { if(p) while(*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++; return p; }template<class T> inline void		zshuffle(T * p, int count) { for(int i = 0; i < count; i++) iswap(p[i], p[rand() % count]); }

enum codepages { CPNONE, CP1251, CPUTF8, CPU16BE, CPU16LE };
namespace metrics {
const codepages						code = CP1251;
}
// Untility structures
template<typename T, T v> struct static_value { static constexpr T value = v; };
template<int v> struct static_int : static_value<int, v> {};
// Storge like vector
template<class T, int count_max = 128>
struct adat {
	T						data[count_max];
	unsigned				count;
	constexpr adat() : count(0) {}
	constexpr adat(const std::initializer_list<T> list) : count(0) { for(auto& e : list) *add() = e; }
	constexpr const T& operator[](unsigned index) const { return data[index]; }
	constexpr T& operator[](unsigned index) { return data[index]; }
	explicit operator bool() const { return count != 0; }
	T*						add() { if(count < count_max) return data + (count++); return 0; }
	void					add(const T& e) { if(count < count_max) data[count++] = e; }
	T*						begin() { return data; }
	const T*				begin() const { return data; }
	void					clear() { count = 0; }
	T*						end() { return data + count; }
	const T*				end() const { return data + count; }
	T*						endof() const { return (T*)data + count_max; }
	int						getcount() const { return count; }
	int						getmaximum() const { return count_max; }
	int						indexof(const T* e) const { if(e >= data && e < data + count) return e - data; return -1; }
	int						indexof(const T t) const { for(unsigned i = 0; i < count; i++) if(data[i] == t) return i; return -1; }
	bool					is(const T t) const { return indexof(t) != -1; }
	T						random() const { if(count) return data[rand() % count]; return T(); }
	void					remove(int index, int remove_count = 1) { if(index < 0) return; if(index<int(count - 1)) memcpy(data + index, data + index + 1, sizeof(data[0])*(count - index - 1)); count--; }
	void					shuffle() { zshuffle(data, count); }
};
// Reference to array with dymanic size
template<class T>
struct aref {
	T*						data;
	unsigned				count;
	constexpr aref() = default;
	constexpr aref(T* source, unsigned count) : data(source), count(count) {}
	template<unsigned N> constexpr aref(T(&data)[N]) : data(data), count(N) {}
	template<unsigned N> constexpr aref(adat<T, N>& source) : data(source.data), count(source.count) {}
	constexpr T& operator[](int index) { return data[index]; }
	constexpr const T& operator[](int index) const { return data[index]; }
	explicit operator bool() const { return count != 0; }
	constexpr T*			begin() { return data; }
	constexpr const T*		begin() const { return data; }
	constexpr T*			end() { return data + count; }
	constexpr const T*		end() const { return data + count; }
	int						getcount() const { return count; }
	int						indexof(const T* t) const { if(t<data || t>data + count) return -1; return t - data; }
	int						indexof(const T t) const { for(unsigned i = 0; i < count; i++) if(data[i] == t) return i; return -1; }
	bool					is(const T t) const { return indexof(t) != -1; }
};
// Abstract value collection
template<class T> struct casev {
	T						id;
	char					value;
};
// Abstract value collection
template<class T, int N> struct aset {
	char					data[N];
	constexpr aset() : data{} {}
	constexpr aset(const std::initializer_list<casev<T>> v) : data{} {
		for(auto e : v)
			set(e.id, e.value);
	}
	constexpr char			get(T id) { return data[id]; }
	constexpr void			set(T id, int v) { data[id] = v; }
};
// Abstract flag data bazed on enumerator
template<typename T, typename DT = unsigned>
struct cflags {
	DT						data;
	constexpr cflags() : data(0) {}
	constexpr cflags(const std::initializer_list<T> list) : data() { for(auto e : list) add(e); }
	constexpr void			add(const T id) { data |= 1 << id; }
	constexpr void			clear() { data = 0; }
	constexpr bool			is(const T id) const { return (data & (1 << id)) != 0; }
	constexpr void			remove(T id) { data &= ~(1 << id); }
};
// Simple slice object
template<class T>
class slice {
	T*						data;
	size_t					count;
public:
	typedef T data_type;
	constexpr slice() : data(0), count(0) {}
	template<size_t N> constexpr slice(T(&v)[N]) : data(v), count(N) {}
	template<int N> constexpr slice(adat<T, N>& v) : data(v), count(v.count) {}
	constexpr slice(T* data, unsigned count) : data(data), count(count) {}
	constexpr T*			begin() const { return data; }
	constexpr T*			end() const { return data + count; }
	constexpr unsigned		size() const { return count; }
};
// Abstract array vector
class array {
	unsigned				size;
	unsigned				count;
	unsigned				count_maximum;
	bool					growable;
	friend struct archive;
public:
	void*					data;
	constexpr array(const array& e) = default;
	constexpr array() : data(0), size(0), count(0), count_maximum(0), growable(false) {}
	constexpr array(unsigned size) : data(0), size(size), count(0), count_maximum(0), growable(true) {}
	constexpr array(void* data, unsigned size, unsigned count) : data(data), size(size), count(count), count_maximum(0), growable(false) {}
	constexpr array(void* data, unsigned size, unsigned count, unsigned count_maximum) : data(data), size(size), count(count), count_maximum(count_maximum), growable(false) {}
	template<typename T, unsigned N> constexpr array(T(&e)[N]) : array(e, sizeof(T), N) {}
	~array();
	void*					add();
	void*					add(const void* element);
	char*					begin() const { return (char*)data; }
	void					clear();
	char*					end() const { return (char*)data + size * count; }
	int						find(const char* value, unsigned offset) const;
	unsigned				getmaximum() const { return count_maximum; }
	unsigned				getcount() const { return count; }
	unsigned				getsize() const { return size; }
	int						indexof(const void* element) const;
	void*					insert(int index, const void* element);
	bool					isgrowable() const { return growable; }
	void*					ptr(int index) const { return (char*)data + size * index; }
	int						random() const { return count ? (rand() % count) : 0; }
	template<class T> slice<T> records() const { return slice<T>((T*)data, count); }
	void					remove(int index, int elements_count);
	void					setcount(unsigned value) { count = value; }
	void					setup(unsigned size);
	void					shift(int i1, int i2, unsigned c1, unsigned c2);
	void					sort(int i1, int i2, int(*compare)(const void* p1, const void* p2, void* param), void* param);
	void					swap(int i1, int i2);
	void					reserve(unsigned count);
};
// Abstract data access class
template<typename T> struct bsdata {
	static T				elements[];
	static array			source;
	static constexpr array*	source_ptr = &source;
	static T*				add() { return (T*)source.add(); }
	static T*				addz() { for(auto& e : bsdata<T>()) if(!e) return &e; return add(); }
	static constexpr T*		begin() { return (T*)source.data; }
	static constexpr T*		end() { return (T*)source.data + source.getcount(); }
	static constexpr bool	have(const void* p) { return source.have(p); }
	static constexpr T&		get(int i) { return begin()[i]; }
};
template<> struct bsdata<int> {
	typedef int				data_type;
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
//
int									getdigitscount(unsigned number); // Get digits count of number. For example if number=100, result be 3.
const char*							getnm(const char* id);
template<class T> const char*		getstr(T e); // Template to return string of small class
void								initialize_translation(const char* locale);
bool								ischa(unsigned u); // is alphabetical character?
inline bool							isnum(unsigned u) { return u >= '0' && u <= '9'; } // is numeric character?
void*								loadb(const char* url, int* size = 0, int additional_bytes_alloated = 0); // Load binary file.
char*								loadt(const char* url, int* size = 0); // Load text file and decode it to system codepage.
bool								matchuc(const char* name, const char* filter);
const char*							psidn(const char* p, char* result, char* result_end); // safe parse identifier name from string
inline const char*					psidn(const char* p, char* result) { return psidn(p, result, result + 128); }
const char*							psnum(const char* p, int& value); // Parse number from string
const char*							psstr(const char* p, char* value, char end_symbol = '\"'); // Parse string from string (like c format "Some\nstring")
unsigned							rmoptimal(unsigned need_count);
void								rmremove(void* data, unsigned size, unsigned index, unsigned& count, int elements_count);
void*								rmreserve(void* data, unsigned new_size);
void								rmreserve(void** data, unsigned count, unsigned& count_maximum, unsigned size);
float								sqrt(const float x); // Return aquare root of 'x'
int									sz2num(const char* p1, const char** pp1 = 0);
void								szencode(char* output, int output_count, codepages output_code, const char* input, int input_count, codepages input_code);
unsigned							szget(const char** input, codepages page = metrics::code);
int									szcmpi(const char* p1, const char* p2);
int									szcmpi(const char* p1, const char* p2, int count);
const char*							szdup(const char *text);
const char*							szext(const char* path);
const char*							szfname(const char* text); // Get file name from string (no fail, always return valid value)
char*								szfnamewe(char* result, const char* name); // get file name without extension (no fail)
unsigned							szlower(unsigned u); // to lower reg
void								szlower(char* p, int count = 1); // to lower reg
bool								szmatch(const char* text, const char* name); //
char*								sznum(char* result, int num, int precision = 0, const char* empthy = 0, int radix = 10);
char*								sznum(char* result, float f, int precision = 0, const char* empthy = "0.00");
bool								szpmatch(const char* text, const char* pattern);
char*								szprint(char* result, const char* result_maximum, const char* format, ...);
char*								szprintv(char* result, const char* result_maximum, const char* format, const char* format_param);
void								szput(char** output, unsigned u, codepages page = metrics::code);
char*								szput(char* output, unsigned u, codepages page = metrics::code); // Fast symbol put function. Return 'output'.
const char*							skipcr(const char* p);
inline const char*					skipsp(const char* p) { if(p) while(*p == 32 || *p == 9) p++; return p; }
unsigned							szupper(unsigned u);
char*								szupper(char* p, int count = 1); // to upper reg
char*								szurl(char* p, const char* path, const char* name, const char* ext = 0, const char* suffix = 0);
char*								szurlc(char* p1);
inline int							xrand(int n1, int n2) { return n1 + rand() % (n2 - n1 + 1); }
inline int							d100() { return rand() % 100; }
// Common used templates
inline int							ifloor(double n) { return (int)n; }
// Inline sequence functions
template<class T> inline void		seqclear(T* p) { T* z = p->next; while(z) { T* n = z->next; z->next = 0; delete z; z = n; } p->next = 0; } // Use to clean up sequenced resources if destructor. Use it like 'clear(this)'.
template<class T> inline T*			seqlast(T* p) { while(p->next) p = p->next; return p; } // Return last element in sequence.
template<class T> inline void		seqlink(T* p) { p->next = 0; if(!T::first) T::first = p; else seqlast(T::first)->next = p; }
// Inline strings functions
template<class T> const char*		getstr(const T e) { return bsdata<T>::elements[e].name; }
template<class T> int				getbsid(const T* e) { return e - bsdata<T>::elements; }
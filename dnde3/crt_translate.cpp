#include "crt.h"
#include "io.h"
#include "log.h"
#include "stringbuilder.h"

namespace {
struct translate {
	const char*			id;
	const char*			name;
};
}

static const char* main_locale;
static array source_name(sizeof(translate));
static array source_nameof(sizeof(translate));
static array source_namepl(sizeof(translate));
static array source_text(sizeof(translate));

static int compare(const void* v1, const void* v2) {
	auto p1 = (const translate*)v1;
	auto p2 = (const translate*)v2;
	return strcmp(p1->id, p2->id);
}

static void update_elements(array& ei) {
	if(ei.getcount()==0)
		return;
	qsort(ei.data, ei.getcount(), ei.getsize(), compare);
}

static const char* read_string_v1(const char* p, char* ps, const char* pe) {
	char sym;
	while(*p && *p != '\n' && *p != '\r') {
		if(p[0] == '\\' && p[1] == 'n') {
			sym = '\n';
			p += 2;
		} else {
			sym = *p;
			p++;
		}
		switch(sym) {
		case 17: sym = '-'; break;
		}
		if(ps < pe)
			*ps++ = sym;
	}
	*ps = 0;
	while(*p == '\n' || *p == '\r') {
		p = skipcr(p);
		p = skipsp(p);
	}
	return p;
}

static const char* read_identifier(const char* p, char* ps, const char* pe) {
	while(*p && (ischa(*p) || isnum(*p) || *p == '_' || *p == ' ')) {
		if(ps < pe)
			*ps++ = *p++;
		else
			break;
	}
	*ps = 0;
	return p;
}

static void apply_value(array& source, const char* id, const char* name) {
	id = szdup(id);
	name = szdup(name);
	auto p = (translate*)source.add();
	p->id = id;
	p->name = name;
}

static void readl(const char* url, array& source, bool required) {
	auto p_alloc = (const char*)loadt(url);
	if(!p_alloc) {
		if(required) {
			log::seturl(0);
			log::error(0, "Can't find file `%1`", url);
		}
		return;
	}
	auto p = p_alloc;
	char name[128], value[8192];
	auto records_read = 0;
	while(*p) {
		p = read_identifier(p, name, name + sizeof(name) - 1);
		if(p[0] != ':')
			break;
		p = skipsp(p + 1);
		p = read_string_v1(p, value, value + sizeof(value) - 1);
		apply_value(source, name, value);
		records_read++;
	}
	delete p_alloc;
	update_elements(source);
}

static void savel(const char* url, array& source) {
	io::file file(url, StreamText | StreamWrite);
	if(!file)
		return;
	auto records_write = 0;
	for(auto& e : source.records<translate>()) {
		file << e.id << ": ";
		if(e.name) {
			file << e.name;
			records_write++;
		}
		file << "\r\n";
	}
}

static void setfile(array& source, const char* id, const char* locale, bool write_mode, bool required) {
	char temp[260]; stringbuilder sb(temp);
	sb.clear(); sb.add("locale/%1/%2.txt", locale, id);
	if(write_mode)
		savel(temp, source);
	else
		readl(temp, source, required);
}

static void deinitialize() {
	setfile(source_name, "Names", main_locale, true, false);
}

static void check(array& source, const char* locale, const char* url) {
	log::seturl(url);
	for(auto& e : source.records<translate>()) {
		if(e.name && e.name[0])
			continue;
		log::error(0, " Define translate for `%1`", e.id);
	}
}

void check_translation() {
	atexit(deinitialize);
	check(source_name, main_locale, "Names.txt");
}

void initialize_translation(const char* locale) {
	if(main_locale)
		return;
	main_locale = szdup(locale);
	setfile(source_name, "Names", main_locale, false, true);
	setfile(source_text, "Descriptions", main_locale, false, false);
	setfile(source_nameof, "NamesOf", main_locale, false, false);
	setfile(source_namepl, "NamesPl", main_locale, false, false);
}

const char* getnm(const char* id) {
	if(!id || id[0]==0)
		return "";
	translate key = {id, 0};
	auto p = (translate*)bsearch(&key, source_name.data, source_name.getcount(), source_name.getsize(), compare);
	if(!p) {
#ifdef _DEBUG
		// Only in debug mode collect new strings
		p = (translate*)source_name.add();
		memset(p, 0, sizeof(*p));
		p->id = szdup(id);
		update_elements(source_name);
#endif
		return id;
	}
	if(!p->name || !p->name[0])
		return id;
	return p->name;
}

const char* getnmof(const char* id) {
	if(!id || id[0] == 0)
		return "";
	translate key = {id, 0};
	auto p = (translate*)bsearch(&key, source_nameof.data, source_nameof.getcount(), source_nameof.getsize(), compare);
	if(!p || !p->name || !p->name[0])
		return id;
	return p->name;
}

const char* getnmpl(const char* id) {
	if(!id || id[0] == 0)
		return "";
	translate key = {id, 0};
	auto p = (translate*)bsearch(&key, source_namepl.data, source_namepl.getcount(), source_namepl.getsize(), compare);
	if(!p || !p->name || !p->name[0])
		return id;
	return p->name;
}

const char* getdescription(const char* id) {
	if(!id || id[0] == 0)
		return 0;
	translate key = {id, 0};
	auto p = (translate*)bsearch(&key, source_text.data, source_text.getcount(), source_text.getsize(), compare);
	if(!p || !p->name)
		return 0;
	return p->name;
}

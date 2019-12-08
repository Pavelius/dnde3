#include "crt.h"
#include "io.h"

void io::stream::writescan(void* p, int width, int height, int scan_line, int element_size) {
	char* pc = (char*)p;
	while(height > 0) {
		write(pc, width*element_size);
		pc += scan_line;
		height--;
	}
}

unsigned char io::stream::get() {
	unsigned char r = 0;
	read(&r, 1);
	return r;
}

unsigned short io::stream::getLE16() {
	unsigned char u2 = get();
	unsigned char u1 = get();
	return (u2 << 8) | u1;
}

unsigned io::stream::getLE32() {
	unsigned char u4 = get();
	unsigned char u3 = get();
	unsigned char u2 = get();
	unsigned char u1 = get();
	return (u4 << 24) | (u3 << 16) | (u2 << 8) | u1;
}

unsigned io::stream::getsize() {
	unsigned s = seek(0, SeekCur);
	unsigned r = seek(0, SeekEnd);
	seek(s, SeekSet);
	return r;
}

void io::stream::puttext(const char* text) {
	int m = text ? zlen(text) : 0;
	write(m);
	if(m)
		write(text, m);
}

void io::stream::gettext(char* result, int max_buffer) {
	int m = 0;
	result[0] = 0;
	read(&m, sizeof(m));
	if(!m)
		return;
	max_buffer = imin(m, max_buffer - 1);
	read(result, max_buffer);
	result[max_buffer] = 0;
	m -= max_buffer;
	if(m)
		seek(m, SeekCur);
}

void* loadb(const char* url, int* size, int additional) {
	void* p = 0;
	if(size)
		*size = 0;
	if(!url || url[0] == 0)
		return 0;
	io::file file(url, StreamRead);
	if(!file)
		return 0;
	int s = file.seek(0, SeekEnd) + additional;
	file.seek(0, SeekSet);
	p = new char[s];
	memset(p, 0, s);
	file.read(p, s);
	if(size)
		*size = s;
	return p;
}

char* loadt(const char* url, int* size) {
	int s1;
	if(size)
		*size = 0;
	unsigned char* p = (unsigned char*)loadb(url, &s1, 1);
	if(!p)
		return 0;
	if(p[0] == 0xEF && p[1] == 0xBB && p[2] == 0xBF) {
		// UTF8
		// Перекодируем блок через декодировщик. Может быть только меньше,
		// так как системная кодировка ANSI
		szencode((char*)p, s1, metrics::code, (char*)p + 3, s1, CPUTF8);
	}
	if(size)
		*size = s1;
	return (char*)p;
}
export module win_io_file;
import collections;
import stream;
import stringbuilder;
#include "win.h"

export void printcnf(const char* text) {
	unsigned result;
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), text, zlen(text), &result, 0);
}

namespace io {
export class file : public stream {
	int					handle;
public:
	class find {
		char			path[261];
		char			reserved[512];
		void*			handle;
	public:
		find(const char* url) {
			path[0] = 0; stringbuilder s1(path); s1.add(url);
			char temp[261]; stringbuilder s2(temp);
			s2.add(path);
			s2.add("/*.*");
			handle = FindFirstFileA(temp, (WIN32_FIND_DATA*)&reserved);
			if(handle == ((void*)-1))
				handle = 0;
		}
		~find() {
			if(handle)
				FindClose(handle);
			handle = 0;
		}
		const char*	name() {
			return ((WIN32_FIND_DATA*)&reserved)->cFileName;
		}
		const char* fullname(char* result) {
			stringbuilder sb(result, result + 260);
			sb.add(path);
			if(sb)
				sb.add("/");
			sb.add(name());
			return result;
		}
		void next() {
			if(!handle)
				return;
			if(!FindNextFileA(handle, (WIN32_FIND_DATA*)&reserved)) {
				FindClose(handle);
				handle = 0;
			}
		}
		operator bool() const { return handle != 0; }
	};
	file() : handle(0) {
	}
	file(const char* url, unsigned flags = StreamRead) : file() {
		create(url, flags);
	}
	~file() {
		close();
	}
	explicit operator bool() const {
		return handle != 0;
	}
	void close() {
		CloseHandle((void*)handle);
		handle = 0;
	}
	bool create(const char* url, unsigned flags) {
		if(handle)
			return true;
		handle = (int)CreateFileA(url,
			(flags & StreamWrite) ? GENERIC_WRITE : GENERIC_READ,
			0,
			0,
			(flags & StreamWrite) ? CREATE_ALWAYS : OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			0);
		if(handle == -1)
			handle = 0;
		else {
			if((flags & (StreamText | StreamWrite)) == (StreamText | StreamWrite)) {
				static unsigned char header_utf8[] = {0xEF, 0xBB, 0xBF};
				write(header_utf8, sizeof(header_utf8));
			}
		}
		return operator bool();
	}
	static bool exist(const char* url) {
		return GetFileAttributesA(url) != 0xFFFFFFFF;
	}
	static char* getdir(char* url, int size) {
		if(!GetCurrentDirectoryA(size, url))
			return 0;
		return url;
	}
	static char* getmodule(char* url, int size) {
		if(!GetModuleFileNameA(0, url, size))
			return 0;
		return url;
	}
	static bool	makedir(const char* url) {
		return CreateDirectoryA(url, 0) != 0;
	}
	int read(void* result, int count) override {
		unsigned opsize;
		if(!ReadFile((void*)handle, result, count, &opsize, 0))
			return 0;
		return opsize;
	}
	static bool remove(const char* url) {
		return DeleteFileA(url) != 0;
	}
	int seek(int count, int rel) override {
		int mm = FILE_BEGIN;
		switch(rel) {
		case SeekCur: mm = FILE_CURRENT; break;
		case SeekEnd: mm = FILE_END; break;
		}
		return SetFilePointer(handle, count, 0, mm);
	}
	static bool setdir(const char* url) {
		return SetCurrentDirectoryA(url) != 0;
	}
	int write(const void* result, int count) override {
		unsigned opsize;
		if(!WriteFile((void*)handle, result, count, &opsize, 0))
			return 0;
		return opsize;
	}
};
}

export void* loadb(const char* url, int* size = 0, int additional = 0) {
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

export char* loadt(const char* url, int* size = 0) {
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
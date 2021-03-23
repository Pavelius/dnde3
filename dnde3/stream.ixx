export module stream;
import collections;
import stringbuilder;

export enum seek_flags {
	SeekSet, SeekCur, SeekEnd
};
export enum stream_flags {
	StreamRead = 1,
	StreamWrite = 2,
	StreamText = 4,
};

namespace io {
// Abstract bi-stream interface
export struct stream {
	virtual int	read(void* result, int count) = 0;
	virtual int	write(const void* result, int count) = 0;
	virtual int	seek(int count, int rel = SeekCur) { return 0; };
	unsigned char get() {
		unsigned char r = 0;
		read(&r, 1);
		return r;
	}
	unsigned short getLE16() {
		unsigned char u2 = get();
		unsigned char u1 = get();
		return (u2 << 8) | u1;
	}
	unsigned getLE32() {
		unsigned char u4 = get();
		unsigned char u3 = get();
		unsigned char u2 = get();
		unsigned char u1 = get();
		return (u4 << 24) | (u3 << 16) | (u2 << 8) | u1;
	}
	unsigned getsize() {
		unsigned s = seek(0, SeekCur);
		unsigned r = seek(0, SeekEnd);
		seek(s, SeekSet);
		return r;
	}
	void gettext(char* result, int max_buffer) {
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
	void read(bool& value) {
		read(&value, sizeof(value));
	}
	void read(int& value) {
		read(&value, sizeof(value));
	}
	void read(unsigned& value) {
		read(&value, sizeof(value));
	}
	void read(short& value) {
		read(&value, sizeof(value));
	}
	void read(unsigned short& value) {
		read(&value, sizeof(value));
	}
	void read(unsigned char& value) {
		read(&value, sizeof(value));
	}
	int	readsig(char* signature) {
		return read(signature, 4);
	}
	void puttext(const char* text) {
		int m = text ? zlen(text) : 0;
		write(m);
		if(m)
			write(text, m);
	}
	void write(bool value) {
		write(&value, sizeof(value));
	}
	void write(int value) {
		write(&value, sizeof(value));
	}
	void write(unsigned value) {
		write(&value, sizeof(value));
	}
	void write(short value) {
		write(&value, sizeof(value));
	}
	void write(unsigned short value) {
		write(&value, sizeof(value));
	}
	void write(unsigned char value) {
		write(&value, sizeof(value));
	}
	void writescan(void* p, int width, int height, int scan_line, int element_size) {
		char* pc = (char*)p;
		while(height > 0) {
			write(pc, width * element_size);
			pc += scan_line;
			height--;
		}
	}
	int	writesig(const char* signature) {
		return write(signature, 4);
	}
};
}
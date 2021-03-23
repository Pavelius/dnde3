export module archive;
import collections;
import stream;
import stringbuilder;

export struct archive {

	struct dataset {
		void*		data;
		template<class T, unsigned N> constexpr dataset(T(&data)[N]) : data(&data), size(sizeof(T)), count(current_count), current_count(N), maximum_count(N) {}
		template<class T, unsigned N> constexpr dataset(adat<T, N>& data) : data(&data.data), size(sizeof(T)), count(data.count), current_count(0), maximum_count(N) {}
		void*		get(int index) const { return (char*)data + index * size; }
		int			indexof(void* p) const { if(((char*)p) >= ((char*)data) && ((char*)p) < ((char*)data + size * count)) return ((char*)p - (char*)data) / size; return -1; }
	private:
		unsigned	size;
		unsigned	maximum_count;
		unsigned	current_count;
		unsigned&	count;
	};
	io::stream&		source;
	bool			writemode;
	aref<dataset>	pointers;

	archive(io::stream& source, bool writemode) : source(source), writemode(writemode), pointers() {}
	archive(io::stream& source, bool writemode, const aref<dataset>& pointers) : source(source), writemode(writemode), pointers(pointers) {}

	void setpointer(void** value) {
		unsigned pid;
		if(writemode) {
			pid = -1;
			auto j = 0;
			for(auto& e : pointers) {
				auto i = e.indexof(*value);
				if(i != -1) {
					pid = (j << 24) | i;
					break;
				}
				j++;
			}
			source.write(&pid, sizeof(pid));
		} else {
			*value = 0;
			source.read(&pid, sizeof(pid));
			if(pid != -1) {
				auto bi = pid >> 24;
				auto ii = pid & 0xFFFFFF;
				*value = pointers[bi].get(ii);
			}
		}
	}
	bool signature(const char* id) {
		char temp[4] = {};
		if(writemode) {
			stringbuilder sb(temp); sb.add(id);
			source.write(temp, sizeof(temp));
		} else {
			source.read(temp, sizeof(temp));
			if(szcmpi(temp, id) != 0)
				return false;
		}
		return true;
	}
	// Check if file have at least this version
	bool version(short major, short minor) {
		short major_reader = major;
		short minor_reader = minor;
		set(major_reader);
		set(minor_reader);
		if(!writemode) {
			if(major_reader < major)
				return false;
			else if(major_reader == major && minor_reader < minor)
				return false;
		}
		return true;
	}
	// Any pointer class
	template<class T> void set(T*& value) {
		setpointer((void**)&value);
	}
	// Full specialization for strings
	template<> void set<const char>(const char*& e) {
		if(writemode) {
			unsigned len = zlen(e);
			source.write(&len, sizeof(len));
			if(len)
				source.write(e, len);
		} else {
			unsigned len;
			char temp[128 * 128];
			source.read(&len, sizeof(len));
			e = 0;
			if(len) {
				source.read(temp, len);
				temp[len] = 0;
				//e = szdup(temp);
			}
		}
	}
	// Array with fixed count
	template<typename T, unsigned N> void set(T(&value)[N]) {
		for(int i = 0; i < N; i++)
			set(value[i]);
	};
	// Fixed data collection
	template<typename T, unsigned N> void set(adat<T, N>& value) {
		set(value.count);
		for(auto& e : value)
			set(e);
	}
	// Custom aref collection
	template<typename T> void set(aref<T>& value) {
		set(value.count);
		set(value.data);
	}
	// All simple types and requisites
	template<class T> void set(T& value) {
		if(writemode)
			source.write(&value, sizeof(value));
		else
			source.read(&value, sizeof(value));
	}
	// Full specialization for arrays
	void set(array& e) {
		set(e.count);
		if(e.count > 0) {
			if(writemode)
				source.write(e.data, e.size * e.count);
			else
				source.read(e.data, e.size * e.count);
		}
	}
};
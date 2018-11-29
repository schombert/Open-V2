#pragma once
#include "simple_fs\\simple_fs.h"
#include <string>
#include <vector>


class directory_representation;

class file_representation {
public:
	const std::u16string name;
	directory_representation* const parent;

	const size_t size;
	const char* const data;

	template<typename T, size_t N>
	file_representation(const std::u16string& n, directory_representation &p, T(&a)[N]);
	template<size_t N>
	file_representation(const std::u16string& n, directory_representation &p, const char(&a)[N]);
};

class directory_representation {
private:
	static std::u16string get_proper_name(const std::u16string& i) {
		if (i.length() != 0 && i[0] == u'\\')
			return i;
		else
			return std::u16string(u"\\") + i;
	}
public:
	const std::u16string name;
	directory_representation* const parent;

	std::vector<directory_representation*> sub_dir;
	std::vector<file_representation*> files;

	directory_representation(const std::u16string& n, directory_representation &p) : name(get_proper_name(n)), parent(&p) {
		p.sub_dir.push_back(this);
	}
	directory_representation(const std::u16string& n) : name(n), parent(nullptr) {}
	std::u16string get_path() const {
		if (parent)
			return parent->get_path() + name;
		return name;
	}
};

void set_default_root(directory_representation& d);

template<typename T, size_t N>
file_representation::file_representation(const std::u16string & n, directory_representation & p, T(&a)[N]) : name(n), parent(&p), size(N * sizeof(T)), data((const char*)a) {
	p.files.push_back(this);
}

template<size_t N>
file_representation::file_representation(const std::u16string & n, directory_representation & p, const char(&a)[N]) : name(n), parent(&p), size(N - 1), data((const char*)a) {
	p.files.push_back(this);
}

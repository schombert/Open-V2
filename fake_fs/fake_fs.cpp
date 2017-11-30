#include "simple_fs\\simple_fs.h"
#include "fake_fs.h"

#include <vector>
#include <Windows.h>
#include <algorithm>
#include <numeric>


class _file {
public:
	file_representation* const f;
	_file(file_representation* i) : f(i) {};
};


class _unopened_file {
public:
	const std::u16string _file_path;
	const std::u16string _file_name;
	file_representation* const target;

	_unopened_file(const std::u16string &path, const std::u16string &name, file_representation* t) : _file_path(path), _file_name(name), target(t) {};
};

class _directory {
public:
	std::vector<directory_representation*> d;
	_directory(const _directory& other) : d(other.d) {}
	_directory(std::vector<directory_representation*>&& din) : d(std::move(din)) {}
	_directory() {}
};

class _file_system {
public:
	directory root_dir;

	_file_system(directory_representation* r) : root_dir(std::make_unique<_directory>()) {
		root_dir.impl->d.push_back(r);
	}
};

directory_representation* representation_from_full_path(directory_representation& root, const std::u16string& path) {
	directory_representation* cdir = &root;
	for (auto first = std::find(path.begin(), path.end(), u'\\');
		first != path.end();
		first = std::find(first + 1, path.end(), u'\\')) {
		std::u16string subdir(first, std::find(first + 1, path.end(), u'\\'));

		directory_representation* next = nullptr;
		for (auto d : cdir->sub_dir) {
			if (d->name == subdir)
				next = d;
		}
		if (!next)
			return nullptr;
		cdir = next;
	}
	return cdir;
}


directory_representation* default_root = nullptr;

void set_default_root(directory_representation& d) {
	default_root = &d;
}



file::file(std::unique_ptr<_file> i) : impl(std::move(i)) {}
file::file(file &&other) noexcept : impl(std::move(other.impl)) {}

unopened_file::unopened_file(std::unique_ptr<_unopened_file> i) : impl(std::move(i)) {}


unopened_file::unopened_file(const unopened_file& other) : impl(std::make_unique<_unopened_file>(*(other.impl))) {}
unopened_file::unopened_file(unopened_file&& other) noexcept : impl(std::move(other.impl)) {};
unopened_file& unopened_file::operator=(const unopened_file& other) {
	impl = std::make_unique<_unopened_file>(*(other.impl));
	return *this;
}
unopened_file& unopened_file::operator=(unopened_file&& other) noexcept {
	impl = std::move(other.impl);
	return *this;
}

file& file::operator=(file &&other) noexcept {
	impl = std::move(other.impl);
	return *this;
};

file_system::file_system() {
	wchar_t buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);
	const auto f = representation_from_full_path(*default_root, (char16_t*)buffer);
	if (!f)
		std::abort();
	impl = std::make_unique<_file_system>(f);
}

void file_system::pop_root() {
	impl->root_dir.impl->d.pop_back();
}

file_system::file_system(const file_system& other) : impl(std::make_unique<_file_system>(*(other.impl))) {}
file_system::file_system(file_system&& other) noexcept : impl(std::move(other.impl)) {};
file_system& file_system::operator=(const file_system& other) {
	impl = std::make_unique<_file_system>(*(other.impl));
	return *this;
}
file_system& file_system::operator=(file_system&& other) noexcept {
	impl = std::move(other.impl);
	return *this;
}


void file_system::set_root(const char* location, const char* location_t) {
	impl->root_dir.impl->d.clear();

	const auto size = MultiByteToWideChar(CP_UTF8, 0, location, (int)(location_t - location), nullptr, 0);
	wchar_t* buffer = new wchar_t[size];
	MultiByteToWideChar(CP_UTF8, 0, location, (int)(location_t - location), buffer, size);

	const auto f = representation_from_full_path(*default_root, std::u16string((char16_t*)buffer, size));
	if (!f)
		std::abort();
	impl->root_dir.impl->d.emplace_back(f);
}

void file_system::set_root(const char16_t* location, const char16_t* location_t) {
	impl->root_dir.impl->d.clear();
	const auto f = representation_from_full_path(*default_root, std::u16string(location, location_t));
	if (!f)
		std::abort();
	impl->root_dir.impl->d.emplace_back(f);
}

void file_system::set_root(const std::u16string& location) {
	impl->root_dir.impl->d.clear();
	const auto f = representation_from_full_path(*default_root, location);
	if (!f)
		std::abort();
	impl->root_dir.impl->d.emplace_back(f);
}



void file_system::add_root(const char* location, const char* location_t) {
	const auto size = MultiByteToWideChar(CP_UTF8, 0, location, (int)(location_t - location), nullptr, 0);
	wchar_t* buffer = new wchar_t[size];
	MultiByteToWideChar(CP_UTF8, 0, location, (int)(location_t - location), buffer, size);

	add_root((char16_t*)buffer);
}

void file_system::add_root(const char16_t* location, const char16_t* location_t) {
	add_root(std::u16string(location, location_t));
}

void file_system::add_root(const std::u16string& location) {
	const auto f = representation_from_full_path(*default_root, location);
	if (f)
		impl->root_dir.impl->d.emplace_back(f);
}

void file_system::add_root_relative(const char* location, const char* location_t) {
	const auto size = MultiByteToWideChar(CP_UTF8, 0, location, (int)(location_t - location), nullptr, 0);
	wchar_t* buffer = new wchar_t[size];
	MultiByteToWideChar(CP_UTF8, 0, location, (int)(location_t - location), buffer, size);

	add_root_relative(std::u16string((char16_t*)buffer, size));

	delete[] buffer;
}

void file_system::add_root_relative(const char16_t* location, const char16_t* location_t) {
	add_root_relative(std::u16string(location, location_t));
}

void file_system::add_root_relative(const std::u16string& location) {
	std::u16string fname;
	if (location.length() == 0 || location[0] != u'\\')
		fname += u'\\';
	fname += location;

	std::vector<directory_representation*> new_roots;

	for (auto d : impl->root_dir.impl->d) {
		//const auto f = representation_from_full_path(*d, fname);
		//if (f)
		//	new_roots.emplace_back(f);
		for (auto s : d->sub_dir) {
			if (s->name == fname) {
				new_roots.emplace_back(s);
			}
		}
	}

	impl->root_dir.impl->d.insert(impl->root_dir.impl->d.end(), new_roots.begin(), new_roots.end());
}

directory::directory(std::unique_ptr<_directory> i) : impl(std::move(i)) {}

directory::directory(const directory& p, const char16_t* name, const char16_t* name_t) : directory(p, std::u16string(name, name_t)) {}
directory::directory(const directory& p, const std::u16string& name) {
	impl = std::make_unique<_directory>();
	for (auto& dir : p.impl->d) {
		for (auto& sd : dir->sub_dir) {
			if (sd->name == name) {
				impl->d.push_back(sd);
			}
		}
	}
}

directory::directory(const file_system& p) : directory(p.get_root()) {
}

const directory& file_system::get_root() const {
	return impl->root_dir;
}

const std::u16string& directory::name() const {
	return impl->d.front()->name;
}

directory::directory(const directory& other) : impl(std::make_unique<_directory>(*(other.impl))) {}
directory::directory(directory&& other) noexcept : impl(std::move(other.impl)) {};
directory& directory::operator=(const directory& other) {
	impl = std::make_unique<_directory>(*(other.impl));
	return *this;
}
directory& directory::operator=(directory&& other) noexcept {
	impl = std::move(other.impl);
	return *this;
}

directory directory::get_directory(const char* name, const char* name_t) const {
	std::u16string full_name;
	if (name == name_t || *name != '\\') {
		full_name += u'\\';
	}

	const auto size = MultiByteToWideChar(CP_UTF8, 0, name, (int)(name_t - name), nullptr, 0);
	wchar_t* buffer = new wchar_t[size];
	MultiByteToWideChar(CP_UTF8, 0, name, (int)(name_t - name), buffer, size);
	full_name += std::u16string((char16_t*)buffer, size);
	delete[] buffer;

	return directory(*this, full_name);
}

directory directory::get_directory(const char16_t* name, const char16_t* name_t) const {
	if (name != name_t && *name == u'\\')
		return directory(*this, name, name_t);
	else {
		std::u16string full_name(u"\\");
		full_name.append(name, name_t);
		return directory(*this, full_name);
	}
}

directory directory::get_directory(const std::u16string& name) const {
	if (name.length() != 0 && name[0] == u'\\')
		return directory(*this, name);
	else {
		std::u16string pref(u"\\");
		return directory(*this, pref + name);
	}
}

std::vector<directory> directory::list_directories() const {
	const auto all_paths = list_paths();
	std::vector<std::pair<std::u16string, std::vector<directory_representation*>>> accumulated_results;
	for (auto& dir : impl->d) {
		for (auto& subdir : dir->sub_dir) {
			auto matching = std::find_if(accumulated_results.begin(), accumulated_results.end(), [subdir](auto& p) {return p.first == subdir->name; });
			if (matching == accumulated_results.end())
				accumulated_results.emplace_back(subdir->name, std::vector<directory_representation*>{ subdir });
			else
				matching->second.push_back(subdir);
		}
	}
	std::vector<directory> dir_results;
	for (auto& n : accumulated_results) {
		dir_results.emplace_back(std::make_unique<_directory>(std::move(n.second)));
	}

	return dir_results;
}

std::vector<unopened_file> directory::list_files(const char16_t* extension) const {
	std::vector<std::pair<std::u16string, file_representation*>> accumulated_results;
	std::u16string ext(extension);

	for(auto dit = impl->d.rbegin(); dit != impl->d.rend(); ++dit) {
		for (auto& f : (*dit)->files) {
			if (f->name.length() >= ext.length() && std::equal(ext.rbegin(), ext.rend(), f->name.rbegin())) {
				if (std::find_if(accumulated_results.begin(), accumulated_results.end(), [f](auto& p) {
					return p.first == f->name;
				}) == accumulated_results.end()) {
					accumulated_results.emplace_back(f->name, f);
				}
			}
		}
	}
	std::vector<unopened_file> results;
	for (auto& r : accumulated_results) {
		results.emplace_back(std::make_unique<_unopened_file>(r.second->parent->get_path(), r.first, r.second));
	}

	return results;
}

std::optional<file> unopened_file::open_file() const {
	return std::optional<file>(std::in_place_t(), std::make_unique<_file>(impl->target));
}

const std::u16string& unopened_file::file_path() const {
	return impl->_file_path;
}
const std::u16string& unopened_file::file_name() const {
	return impl->_file_name;
}



std::optional<file> directory::open_file_internal(const std::u16string& suffix_name) const {
	for (auto dit = impl->d.rbegin(); dit != impl->d.rend(); ++dit) {
		for (auto f : (*dit)->files) {
			if(f->name == suffix_name)
				return std::optional<file>(std::in_place_t(), std::make_unique<_file>(f));
		}
	}
	return std::optional<file>();
}

std::optional<file> directory::open_file(const char* name, const char* name_t) const {
	return open_file_internal(std::u16string(name, name_t));
}


std::optional<file> directory::open_file(const char16_t* name, const char16_t* name_t) const {
	return open_file_internal(std::u16string(name, name_t));
}

std::optional<file> directory::open_file(const std::u16string& name) const {
	return open_file_internal(name);
}

std::u16string directory::to_string() const {
	if (impl->d.size() == 1) {
		return impl->d[0]->get_path();
	} else {
		return std::accumulate(impl->d.begin() + 1, impl->d.end(), impl->d.front()->get_path(), [](const auto& a, const auto& b) { return a + u";" + b->get_path(); });
	}
}

std::vector<std::u16string> directory::list_paths() const {
	std::vector<std::u16string> results;

	for (auto d : impl->d) {
		results.emplace_back(d->get_path());
	}

	return results;
}

size_t file::size() const {
	return impl->f->size;;
}
void file::read_to_buffer(char* buffer, size_t buffer_size) const {
	memcpy(buffer, impl->f->data, buffer_size);
}

file::~file() {}
directory::~directory() {}
file_system::~file_system() {}
unopened_file::~unopened_file() {}
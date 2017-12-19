#include "simple_fs.h"
#include <Windows.h>
#include <numeric>
#include <algorithm>

class _file {
public:
	void* handle;
	_file(void* h) : handle(h) {};
	~_file();
};

_file::~_file() {
	if (handle)
		CloseHandle((HANDLE)handle);
	handle = nullptr;
}

file::file(std::unique_ptr<_file> i) : impl(std::move(i)) {}
file::file(file &&other) noexcept : impl(std::move(other.impl)) {}

class _unopened_file {
public:
	std::u16string _file_path;
	std::u16string _file_name;

	_unopened_file(const std::u16string &path, const std::u16string &name) : _file_path(path), _file_name(name) {};
};

class _directory {
public:
	const std::u16string this_directory;
	const std::variant<const directory*, const file_system*> parent;

	_directory(const file_system& p) : this_directory(u""), parent(&p) {}
	_directory(const directory& p, const char16_t* name, const char16_t* name_t) : this_directory(name, name_t), parent(&p) {}
	_directory(const directory& p, const std::u16string& name) : this_directory(name), parent(&p) {}
};

class _file_system {
public:
	std::vector<std::u16string> roots;
	const directory root;

	_file_system(const file_system& parent) : root(parent) {}
};


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

file_system::file_system() : impl(std::make_unique<_file_system>(*this)) {
	wchar_t buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);
	impl->roots.emplace_back((char16_t*)buffer);
}

void file_system::pop_root() {
	impl->roots.pop_back();
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
	impl->roots.clear();
	const auto size = MultiByteToWideChar(CP_UTF8, 0, location, (int)(location_t - location), nullptr, 0);
	wchar_t* buffer = new wchar_t[size];
	MultiByteToWideChar(CP_UTF8, 0, location, (int)(location_t - location), buffer, size);
	impl->roots.emplace_back((char16_t*)buffer, size);
	delete[] buffer;
}

void file_system::set_root(const char16_t* location, const char16_t* location_t) {
	impl->roots.clear();
	impl->roots.emplace_back(location, location_t - location);
}

void file_system::set_root(const std::u16string& location) {
	impl->roots.clear();
	impl->roots.emplace_back(location);
}

void file_system::add_root(const char* location, const char* location_t) {
	const auto size = MultiByteToWideChar(CP_UTF8, 0, location, (int)(location_t - location), nullptr, 0);
	wchar_t* buffer = new wchar_t[size];
	MultiByteToWideChar(CP_UTF8, 0, location, (int)(location_t - location), buffer, size);
	impl->roots.emplace_back((char16_t*)buffer, size);
	delete[] buffer;
}

void file_system::add_root(const char16_t* location, const char16_t* location_t) {
	impl->roots.emplace_back(location, location_t - location);
}

void file_system::add_root(const std::u16string& location) {
	impl->roots.emplace_back(location);
}

void file_system::add_root_relative(const char* location, const char* location_t) {
	if (!impl->roots.empty()) {
		std::u16string first = impl->roots.front();

		if (location != location_t && *location != '\\')
			first += u'\\';

		const auto size = MultiByteToWideChar(CP_UTF8, 0, location, (int)(location_t - location), nullptr, 0);
		wchar_t* buffer = new wchar_t[size];
		MultiByteToWideChar(CP_UTF8, 0, location, (int)(location_t - location), buffer, size);
		first.append((char16_t*)buffer, size);
		delete[] buffer;

		impl->roots.emplace_back(std::move(first));
	}
}

void file_system::add_root_relative(const char16_t* location, const char16_t* location_t) {
	if (!impl->roots.empty()) {
		std::u16string first = impl->roots.front();

		if (location != location_t && *location != u'\\')
			first += u'\\';

		first.append(location, location_t - location);
		impl->roots.emplace_back(std::move(first));
	}
}

void file_system::add_root_relative(const std::u16string& location) {
	if (!impl->roots.empty()) {
		std::u16string first = impl->roots.front();
		if (location.size() != 0 && location[0] != u'\\')
			first += u'\\';
		first.append(location);
		impl->roots.emplace_back(std::move(first));
	}
}


const directory& file_system::get_root() const {
	return impl->root;
}

directory::directory(std::unique_ptr<_directory> i) : impl(std::move(i)) {}
directory::directory(const directory& p, const char16_t* name, const char16_t* name_t) : impl(std::make_unique<_directory>(p, name, name_t))  {}
directory::directory(const directory& p, const std::u16string& name) : impl(std::make_unique<_directory>(p, name)) {}
directory::directory(const file_system& p) : impl(std::make_unique<_directory>(p)) {
}

const std::u16string& directory::name() const {
	return impl->this_directory;
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
	full_name.append((char16_t*)buffer, size);
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
	std::vector<directory> accumulated_results;
	for (const auto& path : all_paths) {
		const auto appended_path = path + u"\\*";
		WIN32_FIND_DATA find_result;
		auto find_handle = FindFirstFile((wchar_t*)(appended_path.c_str()), &find_result);
		if (find_handle != INVALID_HANDLE_VALUE) {
			do {
				if (find_result.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					const std::u16string prefixed_name = std::u16string(u"\\") + (char16_t*)find_result.cFileName;
					if (find_result.cFileName[0] != u'.' && std::find_if(accumulated_results.begin(), accumulated_results.end(), [&prefixed_name](const auto& s){
						return s.impl->this_directory.compare(prefixed_name) == 0;
					}) == accumulated_results.end()) {
						accumulated_results.emplace_back(*this, prefixed_name);
					}
				}
			} while (FindNextFile(find_handle, &find_result) != 0);
			FindClose(find_handle);
		}
	}
	return accumulated_results;
}

std::vector<unopened_file> directory::list_files(const char16_t* extension) const {
	std::vector<unopened_file> accumulated_results;
	const auto all_paths = list_paths();
	for (const auto& path : all_paths) {
		const auto appended_path = path + u"\\*" + extension;
		WIN32_FIND_DATA find_result;
		auto find_handle = FindFirstFile((wchar_t*)(appended_path.c_str()), &find_result);
		if (find_handle != INVALID_HANDLE_VALUE) {
			do {
				if (!(find_result.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					const std::u16string prefixed_name = std::u16string(u"\\") + (char16_t*)find_result.cFileName;
					if (auto search_result = std::find_if(accumulated_results.begin(), accumulated_results.end(), [n = (char16_t*)find_result.cFileName](const auto& f) {
						return f.impl->_file_name.compare(n) == 0;
					}); search_result != accumulated_results.end()) {
						search_result->impl->_file_path = path;
					} else {
						accumulated_results.emplace_back(std::make_unique<_unopened_file>(path, (char16_t*)find_result.cFileName));
					}
				}
			} while (FindNextFile(find_handle, &find_result) != 0);
			FindClose(find_handle);
		}
	}
	return accumulated_results;
}

std::optional<file> unopened_file::open_file() const {
	const auto full = impl->_file_path + u'\\' + impl->_file_name;
	const auto handle = CreateFile((wchar_t*)full.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (handle != INVALID_HANDLE_VALUE) {
		return std::optional<file>(std::in_place_t(), std::make_unique<_file>((void*)handle));
	}
	return std::optional<file>();
}

const std::u16string& unopened_file::file_path() const {
	return impl->_file_path;
}
const std::u16string& unopened_file::file_name() const {
	return impl->_file_name;
}

std::optional<file> directory::open_file_internal(const std::u16string& suffix_name) const {
	const auto all_paths = list_paths();
	
	for (auto p = all_paths.crbegin(); p != all_paths.crend(); ++p) {
		const auto full = *p + suffix_name;
		const auto handle = CreateFile((wchar_t*)full.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (handle != INVALID_HANDLE_VALUE) {
			return std::optional<file>(std::in_place_t(), std::make_unique<_file>((void*)handle));
		}
	}

	return std::optional<file>();
}
bool sfs_file_exists(const char16_t* szPath) {
	DWORD dwAttrib = GetFileAttributes((const wchar_t*)szPath);
	return dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

std::optional<unopened_file> directory::peek_file_internal(const std::u16string& suffix_name) const {
	const auto all_paths = list_paths();
	for (auto p = all_paths.crbegin(); p != all_paths.crend(); ++p) {
		const auto full = *p + suffix_name;
		if (sfs_file_exists(full.c_str())) {
			return std::optional<unopened_file>(std::in_place_t(), std::make_unique<_unopened_file>(*p, suffix_name.substr(1)));
		}
	}
	return std::optional<unopened_file>();
}

std::optional<unopened_file> directory::peek_file(const char* name, const char* name_t) const {
	std::u16string suffix_name;
	if (name != name_t && *name != '\\')
		suffix_name += u'\\';

	const auto size = MultiByteToWideChar(CP_UTF8, 0, name, (int)(name_t - name), nullptr, 0);
	wchar_t* buffer = new wchar_t[size];
	MultiByteToWideChar(CP_UTF8, 0, name, (int)(name_t - name), buffer, size);
	suffix_name.append((char16_t*)buffer, (char16_t*)(buffer + size));
	delete[] buffer;

	return peek_file_internal(suffix_name);
}

std::optional<unopened_file> directory::peek_file(const char16_t* name, const char16_t* name_t) const {
	return peek_file(std::u16string(name, name_t));
}

std::optional<unopened_file> directory::peek_file(const std::u16string& name) const {
	if (name.length() != 0 && name[0] == u'\\')
		return peek_file_internal(name);
	else
		return peek_file_internal(std::u16string(u"\\") + name);
}

std::optional<file> directory::open_file(const char* name, const char* name_t) const {
	std::u16string suffix_name;
	if (name != name_t && *name != '\\')
		suffix_name += u'\\';

	const auto size = MultiByteToWideChar(CP_UTF8, 0, name, (int)(name_t - name), nullptr, 0);
	wchar_t* buffer = new wchar_t[size];
	MultiByteToWideChar(CP_UTF8, 0, name, (int)(name_t - name), buffer, size);
	suffix_name.append((char16_t*)buffer, (char16_t*)(buffer + size));
	delete[] buffer;

	return open_file_internal(suffix_name);
}

std::optional<file> directory::open_file(const char16_t* name, const char16_t* name_t) const {
	return open_file(std::u16string(name, name_t));
}

std::optional<file> directory::open_file(const std::u16string& name) const {
	if (name.length() != 0 && name[0] == u'\\')
		return open_file_internal(name);
	else
		return open_file_internal(std::u16string(u"\\") + name);
}

std::u16string directory::to_string() const {
	const auto all = list_paths();
	if (all.size() == 1)
		return all[0];
	else
		return std::accumulate(all.begin() + 1, all.end(), all.front(), [](const auto& a, const auto& b) { return a + u";" + b; });
}

template<typename... T>
struct overloaded;

template<typename T, typename... REST>
struct overloaded<T, REST...> : T, overloaded<REST...> {
	overloaded(T t, REST ...rest) : T(t), overloaded<REST...>(rest...) {};
	using T::operator();
	using overloaded<REST...>::operator();
};
template<>
struct overloaded<> { void operator()() {}; };

template<typename... T>
auto make_overloaded(T... t) {
	return overloaded<T...>(t... );
}

std::vector<std::u16string> directory::list_paths() const {
	std::vector<std::u16string> path_vector;

	std::visit(make_overloaded( [&path_vector, _this = this](const directory* p) {
		const auto parent_result = p->list_paths();
		for (const auto& path : parent_result) {
			path_vector.emplace_back(path + _this->impl->this_directory);
		}
	}, [&path_vector](const file_system* p) {
		path_vector.insert(path_vector.end(), p->impl->roots.begin(), p->impl->roots.end());
	} ), impl->parent);

	return path_vector;
}



size_t file::size() const {
	LARGE_INTEGER file_size;
	GetFileSizeEx((HANDLE)(impl->handle), &file_size);
	return file_size.QuadPart;
}
void file::read_to_buffer(char* buffer, size_t buffer_size) const {
	ReadFile((HANDLE)(impl->handle), buffer, (DWORD)buffer_size, NULL, NULL);
}

file::~file() {}
directory::~directory() {}
file_system::~file_system() {}
unopened_file::~unopened_file() {}
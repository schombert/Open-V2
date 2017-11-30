#pragma once
#include <memory>
#include <string>
#include <vector>
#include <variant>
#include <optional>


class _file;
class _unopened_file;
class _directory;
class _file_system;

class file {
private:
public:
	std::unique_ptr<_file> impl;

	file(std::unique_ptr<_file> i);
	file(file &&other) noexcept;
	file(const file& other) = delete;
	file& operator=(file &&other) noexcept;

	size_t size() const;
	void read_to_buffer(char* buffer, size_t buffer_size) const;

	~file();
};

class unopened_file {
public:
	std::unique_ptr<_unopened_file> impl;

	unopened_file(std::unique_ptr<_unopened_file> i);
	unopened_file(const unopened_file& other);
	unopened_file(unopened_file&& other) noexcept;
	unopened_file& operator=(const unopened_file& other);
	unopened_file& operator=(unopened_file&& other) noexcept;
	~unopened_file();

	std::optional<file> open_file() const;
	const std::u16string& file_path() const;
	const std::u16string& file_name() const;

	friend class directory;
};

class file_system;

class directory {
private:
	std::optional<file> open_file_internal(const std::u16string& prefix_name) const;
public:
	std::unique_ptr<_directory> impl;

	directory(const file_system& p);
	directory(const directory& p, const char16_t* name, const char16_t* name_t);
	directory(const directory& p, const std::u16string& name);
	~directory();

	directory(std::unique_ptr<_directory> i);
	directory(const directory& other);
	directory(directory&& other) noexcept;
	directory& operator=(const directory& other);
	directory& operator=(directory&& other) noexcept;

	std::vector<unopened_file> list_files(const char16_t* extension) const;
	std::vector<directory> list_directories() const;
	directory get_directory(const char* name, const char* name_t) const;
	directory get_directory(const char16_t* name, const char16_t* name_t) const;
	directory get_directory(const std::u16string& name) const;
	std::optional<file> open_file(const char* name, const char* name_t) const;
	std::optional<file> open_file(const char16_t* name, const char16_t* name_t) const;
	std::optional<file> open_file(const std::u16string& name) const;
	std::vector<std::u16string> list_paths() const;
	std::u16string to_string() const;
	const std::u16string& name() const;
};


class file_system {
public:
	std::unique_ptr<_file_system> impl;

	file_system();
	file_system(const file_system& other);
	file_system(file_system&& other) noexcept;
	file_system& operator=(const file_system& other);
	file_system& operator=(file_system&& other) noexcept;
	~file_system();

	void set_root(const char* location, const char* location_t);
	void set_root(const char16_t* location, const char16_t* location_t);
	void set_root(const std::u16string& location);
	void add_root(const char* location, const char* location_t);
	void add_root(const char16_t* location, const char16_t* location_t);
	void add_root(const std::u16string& location);
	void add_root_relative(const char* location, const char* location_t);
	void add_root_relative(const char16_t* location, const char16_t* location_t);
	void add_root_relative(const std::u16string& location);
	void pop_root();
	const directory& get_root() const;
};

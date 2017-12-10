#pragma once
#include <string>
#include <atomic>
#include <vector>
#include "common\\common.h"
#include "simple_fs\\simple_fs.h"
#include <map>

class texture {
private:
	uint32_t texture_handle = 0;
	int width = 0;
	int height = 0;
	int channels = 0;
	std::atomic<unsigned char*> filedata = nullptr;
public:
	const std::string filename;
	
	texture(const std::string& fn);
	texture(const texture& o) noexcept : texture_handle(o.texture_handle), width(o.width), height(o.height), channels(o.channels), filename(o.filename) {
		filedata.store(o.filedata.load(std::memory_order_relaxed), std::memory_order_relaxed);
	}
	texture(texture&& o) noexcept : texture_handle(o.texture_handle), width(o.width), height(o.height), channels(o.channels), filename(std::move(o.filename)) {
		filedata.store(o.filedata.load(std::memory_order_relaxed), std::memory_order_relaxed);
	}
	texture& operator=(const texture& o) noexcept {
		this->~texture();
		new (this) texture(o);
		return *this;
	}
	texture& operator=(texture&& o) noexcept {
		this->~texture();
		new (this) texture(std::move(o));
		return *this;
	}

	uint32_t handle();
	void load();
	void load_filedata();
	void free();
};

class data_texture {
private:
	uint32_t texture_handle = 0;
	uint8_t* _data = nullptr;
	std::atomic<bool> write_pending = true;
public:
	const uint16_t size = 0;
	const uint16_t channels = 3;

	data_texture(int s, int c) : _data(new uint8_t[s * c]), size(s), channels(c) {
		memset(_data, 0, size * channels * sizeof(uint8_t));
	};
	data_texture(const data_texture& o) noexcept : _data(new uint8_t[o.size * o.channels]), size(o.size), channels(o.channels) {
		memcpy(_data, o._data, size * channels * sizeof(uint8_t)); }
	data_texture(data_texture&& o) noexcept : texture_handle(o.texture_handle), _data(o._data), size(o.size), channels(o.channels) { o._data = nullptr; }
	data_texture& operator=(data_texture&& o) noexcept { this->~data_texture(), new (this) data_texture(std::move(o)); return *this; }
	data_texture& operator=(const data_texture& o) noexcept { this->~data_texture(), new (this) data_texture(o); return *this; }
	~data_texture() { if(_data) delete[] _data; }

	void create();
	uint32_t handle();
	uint8_t* data() const { return _data; }
	void data_ready() { write_pending.store(true, std::memory_order_release); }
};

class texture_manager {
private:
	std::vector<texture> textures;
	std::vector<char> file_names;
	std::map<vector_backed_string<char>, uint16_t, vector_backed_string_less_ci> fname_map;
public:
	texture_manager() : fname_map(vector_backed_string_less_ci(file_names)) {}
	auto count() { return textures.size(); }
	uint16_t retrieve_by_name(const directory& root, const char* start, const char* end);
	texture& retrieve_by_key(uint16_t key);
	void load_all_texture_files();
};
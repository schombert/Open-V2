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
	texture(const texture& o) noexcept : texture_handle(o.texture_handle), width(o.width), height(o.height), channels(o.channels) {
		filedata.store(o.filedata.load(std::memory_order_relaxed), std::memory_order_relaxed);
	}
	texture(texture&& o) noexcept : texture_handle(o.texture_handle), width(o.width), height(o.height), channels(o.channels) {
		filedata.store(o.filedata.load(std::memory_order_relaxed), std::memory_order_relaxed);
	}
	texture& operator=(const texture& o) noexcept  {
		texture_handle = o.texture_handle; width = o.width; height = o.height; channels = o.channels;
		filedata.store(o.filedata.load(std::memory_order_relaxed), std::memory_order_relaxed);
		return *this;
	}
	texture& operator=(texture&& o) noexcept {
		texture_handle = o.texture_handle; width = o.width; height = o.height; channels = o.channels;
		filedata.store(o.filedata.load(std::memory_order_relaxed), std::memory_order_relaxed);
		return *this;
	}

	uint32_t handle();
	void load();
	void load_filedata();
	void free();
};

class texture_manager {
private:
	std::vector<texture> textures;
	std::vector<char> file_names;
	std::map<vector_backed_string<char>, uint16_t, vector_backed_string_less_ci> fname_map;
public:
	texture_manager() : fname_map(vector_backed_string_less_ci(file_names)) {}
	uint16_t retrieve_by_name(directory& root, const char* start, const char* end);
	texture& retrieve_by_key(uint16_t key);
	void load_all_texture_files();
};
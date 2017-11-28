#pragma once
#include <string>
#include <atomic>

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

	uint32_t handle();
	void load();
	void load_filedata();
	void free();
};
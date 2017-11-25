#pragma once
#include <string>

class texture {
private:
	uint32_t texture_handle = 0;
	int width;
	int height;
public:
	const std::string filename;
	
	texture(const std::string& fn);

	uint32_t handle();
	void load();
	void free();
};
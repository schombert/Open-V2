#include "texture.h"
#define GLEW_STATIC

#include "glew.h"
#include "wglew.h"
#include <Windows.h>
#include <wingdi.h>

#include "soil\\SOIL.h"

texture::texture(const std::string& fn) : filename(fn) {};

void texture::free() {
	glDeleteTextures(1, &texture_handle);
}

void texture::load() {
	if (texture_handle != 0)
		return;

	int channels = 4;
	const auto data = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);

	glGenTextures(1, &texture_handle);
	glBindTexture(GL_TEXTURE_2D, texture_handle);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	SOIL_free_image_data(data);
}

uint32_t texture::handle() {
	load();
	return texture_handle;
}
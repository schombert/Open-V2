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

	unsigned char* expected = nullptr;
	if (filedata.compare_exchange_strong(expected, (unsigned char*)1, std::memory_order::memory_order_release, std::memory_order::memory_order_acquire)) {
		texture_handle = SOIL_load_OGL_texture(filename.c_str(), 0, 0, SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_DDS_LOAD_DIRECT);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	} else {
		const auto fdcopy = filedata.load(std::memory_order::memory_order_relaxed);

		glGenTextures(1, &texture_handle);
		glBindTexture(GL_TEXTURE_2D, texture_handle);

		if (channels == 3) {
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, width, height);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, fdcopy);
		} else if (channels == 4) {
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, width, height);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, fdcopy);
		} else  if (channels == 2) {
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_COMPRESSED_RG11_EAC, width, height);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RG, GL_UNSIGNED_BYTE, fdcopy);
		} else {
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_COMPRESSED_R11_EAC, width, height);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, fdcopy);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		SOIL_free_image_data(fdcopy);
	}
}

void texture::load_filedata() {
	if (filedata.load(std::memory_order::memory_order_acquire) == nullptr) {
		auto data = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);

		unsigned char* expected = nullptr;
		if (!filedata.compare_exchange_strong(expected, data, std::memory_order::memory_order_release, std::memory_order::memory_order_acquire)) {
			SOIL_free_image_data(data);
		}
	}
}

uint32_t texture::handle() {
	load();
	return texture_handle;
}
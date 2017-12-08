#include "texture.h"
#define GLEW_STATIC

#include "glew.h"
#include "wglew.h"
#include <Windows.h>
#include <wingdi.h>
#include <thread>

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
		glGenTextures(1, &texture_handle);
		glBindTexture(GL_TEXTURE_2D, texture_handle);

		if (channels == 3) {
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, width, height);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, expected);
		} else if (channels == 4) {
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, width, height);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, expected);
		} else  if (channels == 2) {
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_COMPRESSED_RG11_EAC, width, height);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RG, GL_UNSIGNED_BYTE, expected);
		} else {
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_COMPRESSED_R11_EAC, width, height);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, expected);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		SOIL_free_image_data(expected);
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

uint16_t texture_manager::retrieve_by_name(directory& root, const char* start, const char* end) {
	char* const temp_cpy = (char*)_alloca(end - start + 1);
	uint32_t t_pos = 0;
	for (uint32_t i = 0; i < (end - start); ++i, ++t_pos) {
		if (start[i] != '\\')
			temp_cpy[t_pos] = start[i];
		else if (i + 1 < (end - start) && start[i+1] == '\\') {
			temp_cpy[t_pos] = start[i];
			++i;
		}
	}
	temp_cpy[t_pos] = 0;

	const auto find_result = fname_map.find(temp_cpy);
	if (find_result != fname_map.end()) {
		_freea(temp_cpy);
		return find_result->second;
	} else {
		const auto new_key = textures.size() + 1;

		const auto full_fn = root.peek_file(temp_cpy, temp_cpy + t_pos);
		if (full_fn) {
			std::u16string full_path = full_fn->file_path() + u'\\' + full_fn->file_name();
			textures.emplace_back(std::string(full_path.begin(), full_path.end()));
		} else if(t_pos > 3) {
			temp_cpy[t_pos - 1] = 's';
			temp_cpy[t_pos - 2] = 'd';
			temp_cpy[t_pos - 3] = 'd';

			const auto full_fn_b = root.peek_file(temp_cpy, temp_cpy + t_pos);
			if (full_fn_b) {
				std::u16string full_path = full_fn_b->file_path() + u'\\' + full_fn_b->file_name();
				textures.emplace_back(std::string(full_path.begin(), full_path.end()));
			} else {
#ifdef _DEBUG
				OutputDebugStringA("texture file not found: ");
				OutputDebugStringA(temp_cpy);
				OutputDebugStringA("\n");
#endif
				_freea(temp_cpy);
				return 0;
			}
		}

		fname_map.emplace(vector_backed_string<char>(temp_cpy, temp_cpy + t_pos, file_names), new_key);

		_freea(temp_cpy);
		return new_key;
	}
}

texture& texture_manager::retrieve_by_key(uint16_t key) {
	return textures[key - 1];
}

void texture_manager::load_all_texture_files() {
	std::thread loading_thread([_this = this](){
		for (auto& t : _this->textures) {
			t.load_filedata();
		}
	});
	loading_thread.detach();
}
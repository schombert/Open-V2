#include "texture.h"
#define GLEW_STATIC

#include "glew.h"
#include "wglew.h"
#include <Windows.h>
#include <wingdi.h>
#include <thread>
#include "Parsers/parsers.h"

#include "soil\\SOIL.h"

namespace graphics {
	texture::texture(const std::string& fn) : filename(fn) {};

	void data_texture::create() {
		if (texture_handle != 0)
			return;

		glGenTextures(1, &texture_handle);
		glBindTexture(GL_TEXTURE_2D, texture_handle);

		if (channels == 3) {
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, size, 1);
		} else if (channels == 4) {
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, size, 1);
		} else  if (channels == 2) {
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RG8, size, 1);
		} else {
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, size, 1);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	uint32_t data_texture::handle() {
		create();
		bool expected = true;
		if (write_pending.compare_exchange_strong(expected, false, std::memory_order_release, std::memory_order_acquire)) {
			glBindTexture(GL_TEXTURE_2D, texture_handle);
			if (channels == 3) {
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size, 1, GL_RGB, GL_UNSIGNED_BYTE, _data);
			} else if (channels == 4) {
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size, 1, GL_RGBA, GL_UNSIGNED_BYTE, _data);
			} else  if (channels == 2) {
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size, 1, GL_RG, GL_UNSIGNED_BYTE, _data);
			} else {
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size, 1, GL_RED, GL_UNSIGNED_BYTE, _data);
			}
		}
		return texture_handle;
	}

	void texture::free() {
		const auto th = texture_handle.load(std::memory_order_acquire);
		texture_handle.store(0, std::memory_order_release);
		glDeleteTextures(1, &th);
	}

	void texture::load() {
		if (texture_handle.load(std::memory_order_acquire) != 0)
			return;

		unsigned char* expected = nullptr;
		if (filedata.compare_exchange_strong(expected, (unsigned char*)1, std::memory_order_release, std::memory_order_acquire)) {
			int channels = 4;
			expected = SOIL_load_image(filename.c_str(), &width, &height, &channels, 4);
		}
		
		unsigned int new_th;
		glGenTextures(1, &new_th);
		glBindTexture(GL_TEXTURE_2D, new_th);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, expected);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		texture_handle.store(new_th, std::memory_order_release);
		SOIL_free_image_data(expected);
	}

	void texture::load_filedata() {
		if (filedata.load(std::memory_order_acquire) == nullptr) {
			int channels = 4;
			auto data = SOIL_load_image(filename.c_str(), &width, &height, &channels, 4);

			unsigned char* expected = nullptr;
			if (!filedata.compare_exchange_strong(expected, data, std::memory_order_release, std::memory_order_acquire)) {
				SOIL_free_image_data(data);
			}
		}
	}

	uint32_t texture::handle() {
		load();
		return texture_handle.load(std::memory_order_acquire);
	}

	color_rgba texture::get_pixel(float x, float y) {
		const auto h = texture_handle.load(std::memory_order_acquire);
		if (h != 0) {
			color_rgba* result = (color_rgba*)_alloca(sizeof(color_rgba) * static_cast<size_t>(width * height));

			glGetTextureImage(h, 0, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<int32_t>(sizeof(color_rgba)) * width * height, result);

			const auto final_result = result[width * std::clamp(static_cast<int32_t>(y * height), 0, height - 1) + std::clamp(static_cast<int32_t>(x * width), 0, width - 1)];
			return final_result;
		} else {
			return color_rgba{ 0,0,0,0 };
		}
	}

	texture_tag texture_manager::load_texture(const directory& root, const char* start, const char* end) {
		texture_tag new_key;
		const auto file_name = vector_backed_string<char>::create_unique(start, end, file_names);

		
		if ((end - start) > 3) {
			const auto full_fn_b = root.peek_file(std::u16string(start, end - 3) + u"dds");
			if (full_fn_b) {
				std::u16string full_path = full_fn_b->file_path() + u'\\' + full_fn_b->file_name();
				new_key = textures.emplace_back(UTF16toUTF8(full_path));
			} else {
				const auto full_fn = root.peek_file(start, end);
				if(full_fn) {
					std::u16string full_path = full_fn->file_path() + u'\\' + full_fn->file_name();
					new_key = textures.emplace_back(UTF16toUTF8(full_path));
				} else {
					new_key = texture_tag();
				}
			}
		}
		if(is_valid_index(new_key))
			fname_map.emplace(file_name, new_key);
		return new_key;
	}

	void texture_manager::load_standard_textures(const directory& root) {
		static const char small_tiles_dialog[] = "gfx\\interface\\small_tiles_dialog.tga";
		standard_small_tiles_dialog = load_texture(root, small_tiles_dialog, small_tiles_dialog + (sizeof(small_tiles_dialog) - 1));

		static const char transparency[] = "gfx\\interface\\transparency.tga";
		standard_transparency = load_texture(root, transparency, transparency + (sizeof(transparency) - 1));

		static const char tiles_dialog[] = "gfx\\interface\\tiles_dialog.tga";
		standard_tiles_dialog = load_texture(root, tiles_dialog, tiles_dialog + (sizeof(tiles_dialog) - 1));

		static const char cursor[] = "\\gfx\\interface\\edit_cursor.tga";
		edit_cursor = load_texture(root, cursor, cursor + (sizeof(cursor) - 1));
	}

	texture_tag texture_manager::retrieve_by_name(const directory& root, const char* start, const char* end) {
		char* const temp_cpy = (char*)_alloca(static_cast<size_t>(end - start + 1));
		uint32_t t_pos = 0;
		for (uint32_t i = 0; i < (end - start); ++i, ++t_pos) {
			temp_cpy[t_pos] = start[i];
			if (start[i] == '\\' && i + 1 < (end - start) && start[i + 1] == '\\')
				++i;
		}
		temp_cpy[t_pos] = 0;

		const auto find_result = fname_map.find(temp_cpy);
		if (find_result != fname_map.end()) {;
			return find_result->second;
		} else {
			const auto new_key = load_texture(root, temp_cpy, temp_cpy + t_pos);
			return new_key;
		}
	}

	const texture& texture_manager::retrieve_by_key(texture_tag key) const {
		return textures[key];
	}
	texture& texture_manager::retrieve_by_key(texture_tag key) {
		return textures[key];
	}

	void texture_manager::load_all_texture_files() {
		std::thread loading_thread([_this = this]() {
			for (auto& t : _this->textures) {
				t.load_filedata();
			}
		});
		loading_thread.detach();
	}
}

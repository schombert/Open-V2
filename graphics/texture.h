#pragma once
#include <string>
#include <atomic>
#include <vector>
#include "common\\common.h"
#include "simple_fs\\simple_fs.h"
#include <map>
#include "common\\shared_tags.h"
#include "simple_serialize\\simple_serialize.hpp"

namespace graphics {
	class texture {
	private:
		std::atomic<uint32_t> texture_handle = 0;
		int width = 0;
		int height = 0;
		int channels = 0;
		std::atomic<unsigned char*> filedata = nullptr;
	public:
		std::string filename;

		texture() {}
		texture(const std::string& fn);
		texture(const texture& o) noexcept : width(o.width), height(o.height), channels(o.channels), filename(o.filename) {
			texture_handle.store(o.texture_handle.load(std::memory_order_relaxed), std::memory_order_relaxed);
			filedata.store(o.filedata.load(std::memory_order_relaxed), std::memory_order_relaxed);
		}
		texture(texture&& o) noexcept : width(o.width), height(o.height), channels(o.channels), filename(std::move(o.filename)) {
			texture_handle.store(o.texture_handle.load(std::memory_order_relaxed), std::memory_order_relaxed);
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

		color_rgba get_pixel(float x, float y);
		
		int32_t get_width() const { return width; }
		int32_t get_height() const { return height; }
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

		data_texture(uint16_t s, uint16_t c) : _data(new uint8_t[s * c]), size(s), channels(c) {
			memset(_data, 0, size * channels * sizeof(uint8_t));
		}
		data_texture(const data_texture& o) noexcept : _data(new uint8_t[o.size * o.channels]), size(o.size), channels(o.channels) {
			memcpy(_data, o._data, size * channels * sizeof(uint8_t));
		}
		data_texture(data_texture&& o) noexcept : texture_handle(o.texture_handle), _data(o._data), size(o.size), channels(o.channels) { o._data = nullptr; }
		data_texture& operator=(data_texture&& o) noexcept { this->~data_texture(); new (this) data_texture(std::move(o)); return *this; }
		data_texture& operator=(const data_texture& o) noexcept { this->~data_texture(); new (this) data_texture(o); return *this; }
		~data_texture() { if (_data) delete[] _data; }

		void create();
		uint32_t handle();
		uint8_t* data() const { return _data; }
		void data_ready() { write_pending.store(true, std::memory_order_release); }
	};

	class texture_manager {
	private:
		std::vector<char> file_names;
		std::map<vector_backed_string<char>, texture_tag, vector_backed_string_less_ci> fname_map;
	public:
		tagged_vector<texture, texture_tag> textures;

		texture_tag standard_tiles_dialog;
		texture_tag standard_transparency;
		texture_tag standard_small_tiles_dialog;

		texture_manager() : fname_map(vector_backed_string_less_ci(file_names)) {}
		auto count() { return textures.size(); }
		texture_tag load_texture(const directory& root, const char* start, const char* end);
		void load_standard_textures(const directory& root);
		texture_tag retrieve_by_name(const directory& root, const char* start, const char* end);
		const texture& retrieve_by_key(texture_tag key) const;
		texture& retrieve_by_key(texture_tag key);
		void load_all_texture_files();
	};
}

template<>
class serialization::serializer<graphics::texture> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, graphics::texture const& obj) {
		serialize(output, obj.filename);
	}
	template<typename ... CONTEXT>
	static void deserialize_object(std::byte const* &input, graphics::texture& obj, CONTEXT&& ... c) {
		deserialize(input, obj.filename);
	}
	static size_t size(graphics::texture const& obj) {
		return serialize_size(obj.filename);
	}
};

template<>
class serialization::serializer<graphics::texture_manager> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, graphics::texture_manager const& obj) {
		serialize(output, obj.textures);
		serialize(output, obj.standard_tiles_dialog);
		serialize(output, obj.standard_transparency);
		serialize(output, obj.standard_small_tiles_dialog);
	}
	template<typename ... CONTEXT>
	static void deserialize_object(std::byte const* &input, graphics::texture_manager& obj, CONTEXT&& ... c) {
		deserialize(input, obj.textures);
		deserialize(input, obj.standard_tiles_dialog);
		deserialize(input, obj.standard_transparency);
		deserialize(input, obj.standard_small_tiles_dialog);
	}
	static size_t size(graphics::texture_manager const& obj) {
		return serialize_size(obj.textures) +
			serialize_size(obj.standard_tiles_dialog) +
			serialize_size(obj.standard_transparency) +
			serialize_size(obj.standard_small_tiles_dialog);
	}
};

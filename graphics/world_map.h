#pragma once
#include "common\\common.h"
#include "open_gl_wrapper.h"

namespace graphics {
	class color_maps {
	private:
		uint8_t * primary_colors;
		uint8_t* secondary_colors;

		uint32_t primary_handle = 0;
		uint32_t secondary_handle = 0;

		std::atomic<bool> update_pending = false;

		int32_t size = 0;
	public:
		color_maps() {}
		color_maps(uint32_t count) : size(static_cast<int32_t>(count)) {
			primary_colors = new uint8_t[count * 3];
			secondary_colors = new uint8_t[count * 3];
		}
		void bind_colors();
		void create_color_textures();
		void init_color_data(uint32_t count) {
			size = static_cast<int32_t>(count);
			primary_colors = new uint8_t[count * 3];
			secondary_colors = new uint8_t[count * 3];
		}
		uint8_t* primary_color_data() const {
			return primary_colors;
		}
		uint8_t* secondary_color_data() const {
			return secondary_colors;
		}
		void update_ready() {
			update_pending.store(true, std::memory_order_release);
		}
		~color_maps() {
			delete[] primary_colors;
			delete[] secondary_colors;
		}
	};

	struct map_data_textures {
		uint16_t* primary_data = nullptr;
		uint32_t primary_handle = 0;
		int32_t width = 0;
		int32_t height = 0;
	};

	class map_display {
	private:
		uint32_t shader_handle = 0;
		uint32_t vertex_buffer = 0;
		uint32_t vao = 0;
		uint32_t element_buffer = 0;
		bool ready = false;
		float top_lat;
		float lat_step;
		float left_long;
		float long_step;
	public:
		color_maps colors;
		map_data_textures data_textures;

		void initialize(open_gl_wrapper&, const boost::container::flat_map<uint32_t, uint16_t>& colors_map, uint8_t* map_data, int32_t width, int32_t height, float left_longitude, float top_latitude, float bottom_latitude);
		void render(open_gl_wrapper&, float scale, float long_rotation, float lat_rotation, int32_t width, int32_t height);
	};

	uint16_t get_value_from_data(int32_t i, int32_t j, uint16_t* data, int32_t width, int32_t height);
	void color_map_creation_stub(boost::container::flat_map<uint32_t, uint16_t>& color_mapping, color_maps& cm, uint8_t* color_data, int32_t width, int32_t height);
	map_data_textures create_data_textures(const boost::container::flat_map<uint32_t, uint16_t>& color_mapping, uint8_t* color_data, int32_t width, int32_t height);
}

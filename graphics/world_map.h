#pragma once
#include "common\\common.h"
#include "open_gl_wrapper.h"
#include "simple_fs\\simple_fs.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wunused-template"
#pragma clang diagnostic ignored "-Wcast-align"
#pragma clang diagnostic ignored "-Wshadow"

#include "Eigen\\Dense"
#include "Eigen\\Geometry"

#pragma clang diagnostic pop

namespace graphics {
	class color_maps {
	private:
		uint8_t* primary_colors = nullptr;
		uint8_t* secondary_colors = nullptr;

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
			if (primary_colors)
				delete[] primary_colors;
			if(secondary_colors)
				delete[] secondary_colors;
		}
	};

	struct map_data_textures {
		uint16_t const* primary_data = nullptr;
		uint32_t handle = 0;
		uint32_t corner_handle = 0;
		uint32_t shadows_handle = 0;
		int32_t width = 0;
		int32_t height = 0;
	};

	enum class projection_type {
		standard_map,
		spherical
	};

	struct map_state {
	private:
		Eigen::Matrix3f _rotation;
		Eigen::Matrix3f inverse_rotation;

		float _aspect = 1.0f;
	public:
		float scale = 1.0f;
		projection_type projection = projection_type::standard_map;

		map_state();
		void resize(int32_t x, int32_t y);
		void rotate(float longr, float latr);
		const Eigen::Matrix3f& rotation() const { return _rotation; }
		float aspect() const { return _aspect; }
		std::pair<float, float> normalize_screen_coordinates(int32_t x, int32_t y, int32_t width, int32_t height) const;
		void move_vector_to(const Eigen::Vector3f& start, const Eigen::Vector3f& destination);
		Eigen::Vector3f get_vector_for(const std::pair<float, float>& in) const;
		Eigen::Vector3f get_unrotated_vector_for(const std::pair<float, float>& in) const;
	};

	class map_display {
	private:
		uint32_t shader_handle = 0;
		uint32_t vertex_buffer = 0;
		uint32_t vao = 0;
		uint32_t element_buffer = 0;
		int32_t map_height = 0;
		int32_t map_width = 0;
		bool ready = false;
		float top_lat = 0.0f;
		float lat_step = 0.0f;
		float left_long = 0.0f;
		float long_step = 0.0f;
		int32_t triangle_vertex_count = 0;
	public:
		color_maps colors;
		map_data_textures data_textures;
		map_state state;

		std::pair<int32_t, int32_t> map_coordinates_from_screen(std::pair<float, float> const& normalized_screen_coordinates) const;
		void initialize(open_gl_wrapper&, std::string shadows_file, uint16_t const* map_data, int32_t width, int32_t height, float left_longitude, float top_latitude, float bottom_latitude);
		void render(open_gl_wrapper&);
	};

	uint16_t get_value_from_data(int32_t i, int32_t j, uint16_t* data, int32_t width, int32_t height);
	map_data_textures create_data_textures(uint16_t const* map_data, int32_t width, int32_t height);
}

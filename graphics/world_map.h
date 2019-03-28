#pragma once
#include "common\\common.h"
#include "open_gl_wrapper.h"
#include "simple_fs\\simple_fs.h"
#include "scenario\scenario.h"

class world_state;

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
			primary_colors = nullptr;
			secondary_colors = nullptr;
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

	struct borders_manager {
		struct province_border_info {
			int16_t size;
			int16_t offset;
			provinces::province_tag a;
			provinces::province_tag b;
		};

		struct border_block {
			std::vector<province_border_info> province_borders;

			uint32_t vertices_handle = 0;
			uint32_t indices_handle = 0;

			int32_t coastal_borders_size = 0;
			int32_t state_borders_size = 0;
			int32_t province_borders_size = 0;
		};

		std::vector<border_block> borders;
	};

	struct globe_mesh {
		struct vertex {
			float x;
			float y;
			float z;
		};

		std::vector<vertex> vertices;
		std::vector<float> transformed_buffer;

		void update_transformed_buffer(float aspect, float scale, Eigen::Matrix3f const& rotation);

		template<typename FUNC>
		void for_each_visible_block(int32_t width, int32_t height, FUNC&& f);
	};

	struct map_state {
	private:
		Eigen::Matrix3f _rotation;
		Eigen::Matrix3f inverse_rotation;

		float _aspect = 1.0f;
		float scale = 1.0f;
	public:
		globe_mesh globe;
		projection_type projection = projection_type::standard_map;

		map_state();
		void resize(int32_t x, int32_t y);
		void rescale_by(float multiplier);
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
		borders_manager borders;

		std::pair<int32_t, int32_t> map_coordinates_from_screen(std::pair<float, float> const& normalized_screen_coordinates) const;
		void initialize(open_gl_wrapper&, std::string shadows_file, uint16_t const* map_data, int32_t width, int32_t height, float left_longitude, float top_latitude, float bottom_latitude);
		void populate_borders(scenario::scenario_manager const& s, uint16_t const* map_data, int32_t width, int32_t height);
		void render(open_gl_wrapper&);
	};

	uint16_t get_value_from_data(int32_t i, int32_t j, uint16_t* data, int32_t width, int32_t height);
	map_data_textures create_data_textures(uint16_t const* map_data, int32_t width, int32_t height);
	void update_map_colors(graphics::map_display& map, world_state& ws);
}

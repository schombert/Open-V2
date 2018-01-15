#include "world_map.h"
#define GLEW_STATIC

#include "glew.h"
#include "wglew.h"
#include <Windows.h>
#undef min
#undef max
#include <map>
#include <deque>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wunused-template"
#pragma clang diagnostic ignored "-Wcast-align"

#include "Eigen\\Dense"
#include "Eigen\\Geometry"

#pragma clang diagnostic pop

namespace graphics {
	struct map_vertex_data {
		float x;
		float y;
		uint16_t value;
	};

	struct map_vertex_data_3d {
		float x;
		float y;
		float z;
		uint16_t value;
	};

	uint16_t get_value_from_data(int32_t i, int32_t j, const uint16_t* data, int32_t width, int32_t height) {
		if ((j < 0) | (j >= height)) {
			return 0ui16;
		}
		if (i < 0)
			i += width;
		if (i >= width)
			i -= width;
		return data[i + j * width];
	}

	uint16_t get_value_from_data_h(int32_t i, int32_t j, const uint16_t* data, int32_t width) {
		if (i >= width)
			i -= width;
		return data[i + j * width];
	}


	static const map_vertex_data edge_points[] = {
		map_vertex_data{ 0.0f, 0.0f, 1 }, //0
		map_vertex_data{ 0.5f, 0.0f, 0 }, //1
		map_vertex_data{ 1.0f, 0.0f, 2 }, //2

		map_vertex_data{ 1.0f, 0.5f, 0 }, //3
		map_vertex_data{ 0.0f, 0.5f, 0 }, //4

		map_vertex_data{ 0.0f, 1.0f, 4 }, //5
		map_vertex_data{ 0.5f, 1.0f, 0 }, //6
		map_vertex_data{ 1.0f, 1.0f, 3 }, //7
	};

	static const uint32_t map_triangle_indices[] = {
		0, 2, 5,
		7, 2, 5
	};

	namespace map_parameters {
		constexpr GLuint aspect           = 0;
		constexpr GLuint scale            = 1;
		constexpr GLuint latitude_step    = 2;
		constexpr GLuint latitude_base    = 3;
		constexpr GLuint longitude_step   = 4;
		constexpr GLuint longitude_base   = 5;
		constexpr GLuint width_count      = 6;
		constexpr GLuint rotation_matrix  = 7;
	}

	static const char* map_vertex_shader = 
		"#version 430 core\n"
		"layout (location = 0) in vec2 vertex_position;\n"
		"layout (location = 2) in unsigned short v_value;\n"
		"\n"
		"flat out unsigned short s_value;\n"
		"flat out int instance_x;\n"
		"flat out int instance_y;\n"
		"\n"
		"layout(location = 0) uniform float aspect;\n" // = w/h
		"layout(location = 1) uniform float scale;\n"
		"layout(location = 2) uniform float latitude_step;\n"
		"layout(location = 3) uniform float latitude_base;\n"
		"layout(location = 4) uniform float longitude_step;\n"
		"layout(location = 5) uniform float longitude_base;\n"
		"layout(location = 6) uniform int width_count;\n"
		"layout(location = 7) uniform mat3 rotation_matrix;\n"
		"\n"
		"const float hpi = 1.5707963267f;\n"
		"\n"
		"void main() {\n"
		"   instance_x = gl_InstanceID % width_count;\n"
		"   instance_y = gl_InstanceID / width_count;\n"
		"   vec2 real_position = vec2((vertex_position.x + instance_x) * longitude_step + longitude_base,"
		        " (vertex_position.y + instance_y) * latitude_step + latitude_base);\n"
		"   vec3 sphere_position = vec3(cos(real_position.x) * cos(real_position.y), sin(real_position.x) * cos(real_position.y), sin(real_position.y));\n"
		"   vec3 rotated_position = rotation_matrix * sphere_position;\n"
		"   vec2 base_projection = vec2(atan(rotated_position.y, rotated_position.x), asin(rotated_position.z));\n"
		"   float rdiv = base_projection.y / 3.14159265358f;\n"
		"	gl_Position = vec4(scale * base_projection.x * sqrt(1.0f - (3.0f*rdiv*rdiv)) / 3.14159265358f, base_projection.y * scale * aspect / 3.14159265358f, "
		"        (abs(base_projection.x))/16.0f + 0.5f, 1.0f);\n"
		//"	gl_Position = vec4(scale * rotated_position.y, rotated_position.z * scale * aspect, "
		//"        -rotated_position.x/16.0f + 0.5f, 1.0f);\n"
		"	s_value = v_value;\n"
		"}\n";

	static const char* map_fragment_shader =
		"#version 430 core\n"
		"\n"
		"flat in unsigned short s_value;\n"
		"flat in int instance_x;\n"
		"flat in int instance_y;\n"
		"layout (location = 0) out vec4 frag_color;\n"
		"\n"
		"layout (binding = 0) uniform usampler2D value_map;\n"
		"layout (binding = 1) uniform sampler2D terrain_texture;\n"
		"layout (binding = 2) uniform sampler1D primary_colors;\n"
		"layout (binding = 3) uniform sampler1D secondary_colors;\n"
		"\n"
		"layout(location = 0) uniform float aspect;\n" // = w/h
		"layout(location = 1) uniform float scale;\n"
		"layout(location = 2) uniform float latitude_step;\n"
		"layout(location = 3) uniform float latitude_base;\n"
		"layout(location = 4) uniform float longitude_step;\n"
		"layout(location = 5) uniform float longitude_base;\n"
		"layout(location = 6) uniform int width_count;\n"
		"\n"
		"void main() {\n"
		//"   if(int(texelFetch(value_map, ivec2(instance_x, instance_y), 0).r) % 4 == 0)\n"
		//"       frag_color = vec4(0.0f,0.0f,0.0f,1.0f);\n"
		//"   else\n"
		//"	    frag_color = vec4((instance_x * longitude_step) / 6.28f, 0.0, (instance_y * latitude_step + latitude_base + 3.14159) /  6.28f, 1.0);\n"
		"   frag_color = texelFetch(primary_colors, int(texelFetch(value_map, ivec2(instance_x, instance_y), 0).r), 0);\n"
		"}\n";

	inline auto generate_rotation_matrix(float long_rotation, float lat_rotation) {
		Eigen::Matrix3f m;
		m = Eigen::AngleAxisf(lat_rotation, Eigen::Vector3f::UnitX()) * Eigen::AngleAxisf(long_rotation, Eigen::Vector3f::UnitZ());
		return m;
	}

	constexpr int32_t block_size = 64;

	inline uint32_t pair_to_border_index(uint32_t a, uint32_t b) {
		return a == b ? 0 : (a < b ? ((a << 16) | b) : ((b << 16) | a));
	}

	map_vertex_data_3d generate_3d_map_point(int32_t x_off, int32_t y_off, float base_lat, float lat_step, float long_step, uint16_t value) {
		const float vx_pos = x_off * long_step;
		const float vy_pos = y_off * lat_step + base_lat;
		const float cos_vy = cos(vy_pos);
		const float sin_vy = sin(vy_pos);
		return map_vertex_data_3d{ cos(vx_pos) * cos_vy , sin(vx_pos) * cos_vy, sin_vy, value };
	}

	//.....................

	class i_block {
	public:
		boost::container::small_vector<uint32_t, 8, concurrent_allocator<uint32_t>> intersects[block_size];
	};

	uint32_t pack_ushort_pair(uint16_t x, uint16_t y) {
		return (static_cast<uint32_t>(y) << 16) | static_cast<uint32_t>(x);
	}
	uint32_t pack_unordered_ushort_pair(uint16_t x, uint16_t y) {
		return y < x ? ((static_cast<uint32_t>(y) << 16) | static_cast<uint32_t>(x)) : ((static_cast<uint32_t>(x) << 16) | static_cast<uint32_t>(y));
	}
	std::pair<uint16_t, uint16_t> unpack_ushort_pair(uint32_t v) {
		return std::make_pair(static_cast<uint16_t>(v & 0xFFFF), static_cast<uint16_t>(v >> 16));
	}

	enum class slope_direction {
		left = 1,
		straight = 0,
		right = 2
	};
	
	uint16_t direction_pair_to_next_position(const std::pair<slope_direction, uint16_t>& in) {
		switch (in.first) {
			case slope_direction::left: return in.second - 1ui16;
			case slope_direction::right: return in.second + 1ui16;
			case slope_direction::straight: return in.second;
		}
	}
	uint32_t pack_position_and_direction(slope_direction d, uint16_t p) {
		return pack_ushort_pair(p, static_cast<uint16_t>(d));
	}
	std::pair<slope_direction, uint16_t> unpack_position_and_direction(uint32_t v) {
		std::make_pair(static_cast<slope_direction>(v >> 16), static_cast<uint16_t>(v & 0xFFFF));
	}

	using block_set_t = boost::container::flat_map<uint32_t, i_block, concurrent_allocator<std::pair<uint32_t, i_block>>>;

	void intersects_to_blocks(
		v_vector<uint32_t> &intersects,
		block_set_t& block_set) {

		for (uint32_t it = 0; it < intersects.index.size(); ++it) {
			uint16_t previous = (uint16_t )-1;
			const auto bound = intersects.get_row(it);
			for (auto iit = bound.first; iit != bound.second; ++iit) {
				if (previous == (uint16_t)-1) {
					const uint16_t xblock = unpack_position_and_direction(*iit).second / block_size;
					auto& blk = block_set[pack_ushort_pair(xblock, static_cast<uint16_t>(it / block_size))];
					blk.intersects[it % block_size].push_back(*iit);
					previous = xblock;
				} else {
					const uint16_t xblock = unpack_position_and_direction(*iit).second / block_size;
					if (xblock == previous) {
						auto& blk = block_set[pack_ushort_pair(xblock, static_cast<uint16_t>(it / block_size))];
						blk.intersects[it % block_size].push_back(*iit);
						previous = (uint16_t)-1;
					} else {
						auto& prv = block_set[pack_ushort_pair(previous, static_cast<uint16_t>(it / block_size))];
						prv.intersects[it % block_size].push_back(pack_position_and_direction(slope_direction::straight, previous * block_size + block_size));
						for (uint16_t i = previous + 1; i < xblock; ++i) {
							auto& blk = block_set[pack_ushort_pair(i, static_cast<uint16_t>(it / block_size))];
							blk.intersects[it % block_size].push_back(pack_position_and_direction(slope_direction::straight, i*block_size));
							blk.intersects[it % block_size].push_back(pack_position_and_direction(slope_direction::straight, i*block_size + block_size));
						}
						auto& blk = block_set[pack_ushort_pair(xblock, static_cast<uint16_t>(it / block_size))];
						blk.intersects[it % block_size].push_back(pack_position_and_direction(slope_direction::straight, xblock * block_size));
						blk.intersects[it % block_size].push_back(*iit);
						previous = (uint16_t)-1;
					}
				}
			}
		}

	}

	struct i_polyline {
		std::deque<uint32_t, concurrent_allocator<uint32_t>> points;
	};

	bool in_polyline_list(const std::vector<typename i_polyline, concurrent_allocator<i_polyline>>& plines, uint32_t in) noexcept {
		for (const auto& pl : plines) {
			if (std::find(pl.points.cbegin(), pl.points.cend(), in) != pl.points.cend())
				return true;
		}
		return false;
	}


	void blocks_to_polylines(uint16_t top, const i_block& block, std::vector<i_polyline, concurrent_allocator<i_polyline>>& plines) noexcept {
		int previoussize = 0;
		std::vector<i_polyline> generatedlines;

		for (uint16_t vertindex = 0; vertindex < block_size; ++vertindex) {
			bool testcontinuity = false;
			if (previoussize == (((block.intersects[vertindex].size() + 1) / 2) << 1)) {
				int indx = 0;
				for (const auto pt : block.intersects[vertindex]) {
					const auto unpacked_pt = unpack_position_and_direction(pt);
					if (indx % 2 == 0) {
						const auto front_pair = unpack_ushort_pair(generatedlines[indx / 2].points.front());
						if (unpacked_pt.second + 1 < front_pair.first || unpacked_pt.second > front_pair.first + 1) {
							testcontinuity = true;
							break;
						}
					} else {
						const auto back_pair = unpack_ushort_pair(generatedlines[indx / 2].points.back());
						if (unpacked_pt.second + 1 < back_pair.first || unpacked_pt.second > back_pair.first + 1) {
							testcontinuity = true;
							break;
						}
					}
					++indx;
				}
			}

			if (testcontinuity || previoussize != (((block.intersects[vertindex].size() + 1) / 2) << 1)) {
				if (previoussize != 0) {
					int indx = 0;
					for (const auto& pt : block.intersects[vertindex - 1]) {
						const auto unpacked_pt = unpack_position_and_direction(pt);
						if (indx % 2 == 0)
							generatedlines[indx / 2].points.push_front(pack_ushort_pair(direction_pair_to_next_position(unpacked_pt), vertindex + top));
						else
							generatedlines[indx / 2].points.push_back(pack_ushort_pair(direction_pair_to_next_position(unpacked_pt), vertindex + top));
						++indx;
					}
					for (auto& gl : generatedlines)
						plines.emplace_back(std::move(gl));
					generatedlines.clear();
				}
				generatedlines.resize((block.intersects[vertindex].size() + 1) / 2);
				previoussize = static_cast<int>(((block.intersects[vertindex].size() + 1) / 2) << 1);
			}

			int indx = 0;
			for (const auto& pt : block.intersects[vertindex]) {
				const auto unpacked_pt = unpack_position_and_direction(pt);
				if (indx % 2 == 0) {
					generatedlines[indx / 2].points.push_front(pack_ushort_pair(unpacked_pt.second, vertindex + top));
					generatedlines[indx / 2].points.push_front(pack_ushort_pair(direction_pair_to_next_position(unpacked_pt), vertindex + 1 + top));
				} else {
					generatedlines[indx / 2].points.push_back(pack_ushort_pair(unpacked_pt.second, vertindex + top));
					generatedlines[indx / 2].points.push_back(pack_ushort_pair(direction_pair_to_next_position(unpacked_pt), vertindex + 1 + top));
				}
				++indx;
			}
		}

		if (previoussize != 0) {
			int indx = 0;
			for (const auto& pt : block.intersects[block_size - 1]) {
				const auto unpacked_pt = unpack_position_and_direction(pt);
				if (indx % 2 == 0)
					generatedlines[indx / 2].points.push_front(pack_ushort_pair(direction_pair_to_next_position(unpacked_pt), block_size + top));
				else
					generatedlines[indx / 2].points.push_back(pack_ushort_pair(direction_pair_to_next_position(unpacked_pt), block_size + top));
				++indx;
			}
			for (auto& gl : generatedlines)
				plines.emplace_back(std::move(gl));
		}
	}


	void reduce_polyline(i_polyline& ln) {
		if (ln.points.size() < 4)
			return;

		bool up = true;
		auto it = ln.points.begin();
		auto last_point = unpack_ushort_pair(*it);
		int slopex = block_size * 2;
		int slopey = block_size * 2;

		++it;
		while (it != ln.points.end()) {
			auto current_point = unpack_ushort_pair(*it);
			if (current_point.first == last_point.first && current_point.second == last_point.second) {
				--it;
				it = ln.points.erase(it);
			} else if (up && current_point.second < last_point.second) {
				up = false;
			} else if (current_point.first - last_point.first == slopex && current_point.second - last_point.second == slopey) {
				--it;
				it = ln.points.erase(it);
			} else {
				slopex = current_point.first - last_point.first;
				slopey = current_point.second - last_point.second;
			}
			last_point = current_point;
			++it;
		}
	}

	int32_t inline triangle_sign(uint32_t packed_pt_1, uint32_t packed_pt_2, uint32_t packed_pt_3) {
		const auto unpacked_pt_1 = unpack_ushort_pair(packed_pt_1);
		const auto unpacked_pt_2 = unpack_ushort_pair(packed_pt_2);
		const auto unpacked_pt_3 = unpack_ushort_pair(packed_pt_3);

		return (static_cast<int32_t>(unpacked_pt_1.first) * static_cast<int32_t>(unpacked_pt_2.second)
			- static_cast<int32_t>(unpacked_pt_2.first) * static_cast<int32_t>(unpacked_pt_1.second)
			+ static_cast<int32_t>(unpacked_pt_2.first) * static_cast<int32_t>(unpacked_pt_3.second)
			- static_cast<int32_t>(unpacked_pt_3.first) * static_cast<int32_t>(unpacked_pt_2.second)
			+ static_cast<int32_t>(unpacked_pt_3.first) * static_cast<int32_t>(unpacked_pt_1.second)
			- static_cast<int32_t>(unpacked_pt_1.first) * static_cast<int32_t>(unpacked_pt_3.second));
	}

	struct graphics_data_creation {
		boost::container::flat_map<uint32_t, uint32_t> packed_coordinare_to_index;
		std::vector<map_vertex_data_3d> vertices;
		std::vector<uint32_t> indices;

		void place_lead_point(uint32_t pt, uint16_t value, float base_lat, float lat_step, float long_step) {
			if (auto it = packed_coordinare_to_index.find(pt); it != packed_coordinare_to_index.end()) {
				if (vertices[it->second].value == (uint16_t)-1) {
					vertices[it->second].value = value;
					indices.emplace_back(it->second);
				} else if(vertices[it->second].value == value) {
					indices.emplace_back(it->second);
				} else {
					const auto unpacked = unpack_ushort_pair(pt);
					vertices.emplace_back(generate_3d_map_point(static_cast<int32_t>(unpacked.first), static_cast<int32_t>(unpacked.second), base_lat, lat_step, long_step, value));
					const auto new_index = static_cast<uint32_t>(vertices.size() - 1);
					indices.emplace_back(new_index);
				}
			} else {
				const auto unpacked = unpack_ushort_pair(pt);
				vertices.emplace_back(generate_3d_map_point(static_cast<int32_t>(unpacked.first), static_cast<int32_t>(unpacked.second), base_lat, lat_step, long_step, value));
				const auto new_index = static_cast<uint32_t>(vertices.size() - 1);
				indices.emplace_back(new_index);
				packed_coordinare_to_index.emplace(std::make_pair(pt, new_index));
			}
		}

		void place_point(uint32_t pt, float base_lat, float lat_step, float long_step) {
			if (auto it = packed_coordinare_to_index.find(pt); it != packed_coordinare_to_index.end()) {
				indices.emplace_back(it->second);
			} else {
				const auto unpacked = unpack_ushort_pair(pt);
				vertices.emplace_back(generate_3d_map_point(static_cast<int32_t>(unpacked.first), static_cast<int32_t>(unpacked.second), base_lat, lat_step, long_step, (uint16_t)-1));
				const auto new_index = static_cast<uint32_t>(vertices.size() - 1);
				indices.emplace_back(new_index);
				packed_coordinare_to_index.emplace(std::make_pair(pt, new_index));
			}
		}
	};

	void monotone_to_triangles(graphics_data_creation& data_out, i_polyline& ln, uint16_t value, float base_lat, float lat_step, float long_step) {
		if (ln.points.size() < 3)
			return;

		struct pointrecord {
			uint32_t pt;
			bool left;
			pointrecord(uint32_t p, bool l) : pt(p), left(l) {}
		};

		std::vector<pointrecord> sorted;
		bool left = true;
		uint16_t lasty = unpack_ushort_pair(ln.points.front()).second;
		auto it = ln.points.begin();
		auto rit = it;
		for (; it != ln.points.end(); ++it) {
			const auto newy = unpack_ushort_pair(*it).second;
			if (newy > lasty) {
				--it;
				rit = it;
				break;
			}
			lasty = newy;
		}

		auto lit = decltype(ln.points)::reverse_iterator(rit);
		while (lit != ln.points.rend() && rit != ln.points.end()) {
			if (unpack_ushort_pair(*lit).second <= unpack_ushort_pair(*rit).second) {
				sorted.emplace_back(*lit, true);
				++lit;
			} else {
				sorted.emplace_back(*rit, false);
				++rit;
			}
		}

		while (lit != ln.points.rend()) {
			sorted.emplace_back(*lit, true);
			++lit;
		}

		while (rit != ln.points.end()) {
			sorted.emplace_back(*rit, false);
			++rit;
		}

		std::vector<pointrecord> stack;
		size_t indx = 2;
		stack.push_back(sorted[0]);
		stack.push_back(sorted[1]);

		while (indx < sorted.size()) {
			if (stack.back().left != sorted[indx].left) {
				pointrecord t = stack.back();

				if (sorted[indx].left) {
					while (stack.size() >= 2) {
						data_out.place_lead_point(sorted[indx].pt, value, base_lat, lat_step, long_step);
						data_out.place_point(stack.back().pt, base_lat, lat_step, long_step);
						stack.pop_back();
						data_out.place_point(stack.back().pt, base_lat, lat_step, long_step);
					}
				} else {
					while (stack.size() >= 2) {
						data_out.place_lead_point(stack.back().pt, value, base_lat, lat_step, long_step);
						data_out.place_point(sorted[indx].pt, base_lat, lat_step, long_step);
						stack.pop_back();
						data_out.place_point(stack.back().pt, base_lat, lat_step, long_step);
					}
				}
				stack.pop_back();
				stack.push_back(t);
				stack.push_back(sorted[indx]);
			} else {
				if (sorted[indx].left) {
					while (stack.size() >= 2) {
						pointrecord t = stack.back();
						stack.pop_back();

						if (triangle_sign(sorted[indx].pt, t.pt, stack.back().pt) > 0) {
							data_out.place_lead_point(t.pt, value, base_lat, lat_step, long_step);
							data_out.place_point(sorted[indx].pt, base_lat, lat_step, long_step);
							data_out.place_point(stack.back().pt, base_lat, lat_step, long_step);
						} else {
							stack.push_back(t);
							break;
						}
					}
					stack.push_back(sorted[indx]);
				} else {
					while (stack.size() >= 2) {
						pointrecord t = stack.back();
						stack.pop_back();

						if (triangle_sign(sorted[indx].pt, t.pt, stack.back().pt) < 0) {
							data_out.place_lead_point(stack.back().pt, value, base_lat, lat_step, long_step);
							data_out.place_point(sorted[indx].pt, base_lat, lat_step, long_step);
							data_out.place_point(t.pt, base_lat, lat_step, long_step);
						} else {
							stack.push_back(t);
							break;
						}
					}
					stack.push_back(sorted[indx]);
				}
			}
			++indx;
		}
	}

	

	void intersects_to_triangles(v_vector<uint32_t>& intersects, int top, graphics_data_creation& data_out) noexcept {
		block_set_t block_set;
		intersects_to_blocks(intersects, block_set);
		std::vector<i_polyline, concurrent_allocator<i_polyline>> plines;
		for (const auto& bk : block_set) {
			blocks_to_polylines(top + bk.first.second * block_size, bk.second, plines);
		}
		for (auto& pl : plines) {
			reduce_polyline(pl);
			monotone_to_triangles(data_out, pl);
		}
	}

	using edge = uint32_t;

	uint64_t pack_unordered_uint_pair(uint32_t x, uint32_t y) {
		return y < x ? ((static_cast<uint64_t>(y) << 32ui64) | static_cast<uint64_t>(x)) : ((static_cast<uint64_t>(x) << 32ui64) | static_cast<uint64_t>(y));
	}
	std::pair<uint32_t, uint32_t> unpack_uint_pair(uint64_t v) {
		return std::make_pair(static_cast<uint32_t>(v & 0xFFFFFFFF), static_cast<uint32_t>(v >> 32));
	}
	uint64_t pack_unordered_coordindates_pairs(int32_t a, int32_t b, int32_t c, int32_t d) {
		return pack_unordered_uint_pair(
			pack_ushort_pair(static_cast<uint16_t>(a), static_cast<uint16_t>(b)),
			pack_ushort_pair(static_cast<uint16_t>(c), static_cast<uint16_t>(d)));
	}

	void add_segment_to_map(boost::container::flat_map<uint32_t, std::vector<uint64_t>>& unordered_prov_pair_to_segments, uint32_t unordered_prov_pair, uint64_t packed_segment) {
		if (unordered_prov_pair_to_segments.find(unordered_prov_pair) == unordered_prov_pair_to_segments.cend())
			unordered_prov_pair_to_segments.emplace(unordered_prov_pair, std::vector<uint64_t>());
		unordered_prov_pair_to_segments[unordered_prov_pair].push_back(packed_segment);
	}

	using borders_set = boost::container::flat_multimap<uint16_t, std::vector<uint32_t>>;

	inline std::pair<uint16_t, uint16_t> adjust_for_edge(std::pair<uint16_t, uint16_t> in, int32_t edge_width, bool shift_to_zero) {
		if (!shift_to_zero)
			return in;
		if (static_cast<int32_t>(in.second) != edge_width)
			return in;
		return std::make_pair(0ui16, in.second);
	}

	void make_intersects(v_vector<uint32_t> &intersect, decltype(std::declval<borders_set>().lower_bound(0ui16)) first_border, decltype(std::declval<borders_set>().upper_bound(0ui16)) last_border, int32_t edge_width) {
		int32_t max_y = 0;
		int32_t min_y = std::numeric_limits<int32_t>::max();
		int32_t max_x = 0;
		int32_t min_x = std::numeric_limits<int32_t>::max();
		bool seen_max_width = false;
		bool edge_width_moved_to_zero = false;

		for (auto it = first_border; it != last_border; ++it) {
			for (auto v : it->second) {
				auto xy = unpack_ushort_pair(v);
				max_y = std::max(max_y, static_cast<int32_t>(xy.second));
				min_y = std::min(min_y, static_cast<int32_t>(xy.second));
				if (static_cast<int32_t>(xy.first) != edge_width)
					max_x = std::max(max_y, static_cast<int32_t>(xy.first));
				else
					seen_max_width = true;
				min_x = std::min(min_y, static_cast<int32_t>(xy.first));
			}
		}
		if (seen_max_width) {
			if (max_x < edge_width / 2) {
				min_x = 0;
				edge_width_moved_to_zero = true;
			} else {
				max_x = edge_width;
			}
		}

		for (auto it = first_border; it != last_border; ++it) {
			const int32_t border_size = static_cast<int32_t>(it->second.size());

			for (int32_t i = border_size - 2; i >= 0; --i) {
				auto xy = adjust_for_edge(unpack_ushort_pair(it->second[i]), edge_width, edge_width_moved_to_zero);
				auto next_xy = adjust_for_edge(unpack_ushort_pair(it->second[i + 1]), edge_width, edge_width_moved_to_zero);

				if (xy.second > next_xy.second) {
					if (next_xy.first < xy.first) {
						for (int32_t k = 0; k < xy.second - next_xy.second; ++k)
							intersect.add_to_row(next_xy.second - min_y + k, pack_position_and_direction(slope_direction::right, next_xy.first + k));
					} else if (next_xy.first > xy.first) {
						for (int32_t k = 0; k < xy.second - next_xy.second; ++k)
							intersect.add_to_row(next_xy.second - min_y + k, pack_position_and_direction(slope_direction::left, next_xy.first - k));
					} else {
						for (int32_t k = 0; k < xy.second - next_xy.second; ++k)
							intersect.add_to_row(next_xy.second - min_y + k, pack_position_and_direction(slope_direction::straight, next_xy.first));
					}
				} else if (xy.second < next_xy.second) {
					if (next_xy.first < xy.first) {
						for (int32_t k = 0; k < next_xy.second - xy.second; ++k)
							intersect.add_to_row(next_xy.second - min_y + k, pack_position_and_direction(slope_direction::left, next_xy.first - k));
					} else if (next_xy.first > xy.first) {
						for (int32_t k = 0; k < next_xy.second - xy.second; ++k)
							intersect.add_to_row(next_xy.second - min_y + k, pack_position_and_direction(slope_direction::right, next_xy.first + k));
					} else {
						for (int32_t k = 0; k < next_xy.second - xy.second; ++k)
							intersect.add_to_row(next_xy.second - min_y + k, pack_position_and_direction(slope_direction::straight, next_xy.first));
					}
				}
			}
		}

		for (int32_t i = static_cast<int32_t>(intersect.row_size()) - 1; i >= 0; --i) {
			const auto bound = intersect.get_row(static_cast<uint32_t>(i));
			std::sort(bound.first, bound.second, [](uint32_t a, uint32_t b) {return (a & 0xFFFF) < (b & 0xFFFF); });
		}
	}

	void make_borders(
		borders_set& borders,
		const uint16_t* map_data, int32_t width, int32_t height) {

		boost::container::flat_map<uint32_t, std::vector<uint64_t>> unordered_prov_pair_to_segments;

		for (int32_t j = -1; j <= height; j++) {
			for (int32_t i = 0; i <= width; i++) {
				const uint16_t a = get_value_from_data(i, j, map_data, width, height);
				const uint16_t b = get_value_from_data(i + 1, j, map_data, width, height);
				const uint16_t c = get_value_from_data(i, j + 1, map_data, width, height);
				const uint16_t d = get_value_from_data(i + 1, j + 1, map_data, width, height);

				if (a == b && a == c && a == d) { // same, no connections

				} else if (a == b && a == c) { //two colors
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(a, d),
						pack_unordered_coordindates_pairs(i * 2 + 2, j * 2 + 3, i * 2 + 3, j * 2 + 2));
				} else if (c == b && c == d) {
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(a, d),
						pack_unordered_coordindates_pairs(i * 2 + 1, j * 2 + 2, i * 2 + 2, j * 2 + 1));
				} else if (c == a && a == d) {
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(b, c),
						pack_unordered_coordindates_pairs(i * 2 + 3, j * 2 + 2, i * 2 + 2, j * 2 + 1));
				} else if (b == a && b == d) {
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(b, c),
						pack_unordered_coordindates_pairs(i * 2 + 1, j * 2 + 2, i * 2 + 2, j * 2 + 3));
				} else if (c == a && b == d) {
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(b, c),
						pack_unordered_coordindates_pairs(i * 2 + 2, j * 2 + 1, i * 2 + 2, j * 2 + 3));
				} else if (b == a && c == d) {
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(b, c),
						pack_unordered_coordindates_pairs(i * 2 + 1, j * 2 + 2, i * 2 + 3, j * 2 + 2));
				} else if (a == d && b == c) {
					if ((j + i / 3) % 2 == 0) {
						add_segment_to_map(
							unordered_prov_pair_to_segments,
							pack_unordered_ushort_pair(a, b),
							pack_unordered_coordindates_pairs(i * 2 + 1, j * 2 + 2, i * 2 + 2, j * 2 + 3));
						add_segment_to_map(
							unordered_prov_pair_to_segments,
							pack_unordered_ushort_pair(a, b),
							pack_unordered_coordindates_pairs(i * 2 + 2, j * 2 + 1, i * 2 + 3, j * 2 + 2));
					} else {
						add_segment_to_map(
							unordered_prov_pair_to_segments,
							pack_unordered_ushort_pair(a, b),
							pack_unordered_coordindates_pairs(i * 2 + 2, j * 2 + 1, i * 2 + 1, j * 2 + 2));
						add_segment_to_map(
							unordered_prov_pair_to_segments,
							pack_unordered_ushort_pair(a, b),
							pack_unordered_coordindates_pairs(i * 2 + 3, j * 2 + 2, i * 2 + 2, j * 2 + 3));
					}
				} else if (b == a) { // three colors
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(a, c),
						pack_unordered_coordindates_pairs(i * 2 + 1, j * 2 + 2, i * 2 + 2, j * 2 + 2));
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(d, b),
						pack_unordered_coordindates_pairs(i * 2 + 3, j * 2 + 2, i * 2 + 2, j * 2 + 2));
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(c, d),
						pack_unordered_coordindates_pairs(i * 2 + 2, j * 2 + 3, i * 2 + 2, j * 2 + 2));
				} else if (b == d) {
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(a, b),
						pack_unordered_coordindates_pairs(i * 2 + 2, j * 2 + 1, i * 2 + 2, j * 2 + 2));
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(c, d),
						pack_unordered_coordindates_pairs(i * 2 + 2, j * 2 + 3, i * 2 + 2, j * 2 + 2));
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(a, c),
						pack_unordered_coordindates_pairs(i * 2 + 1, j * 2 + 2, i * 2 + 2, j * 2 + 2));
				} else if (c == d) {
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(a, b),
						pack_unordered_coordindates_pairs(i * 2 + 2, j * 2 + 1, i * 2 + 2, j * 2 + 2));
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(a, c),
						pack_unordered_coordindates_pairs(i * 2 + 1, j * 2 + 2, i * 2 + 2, j * 2 + 2));
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(b, d),
						pack_unordered_coordindates_pairs(i * 2 + 3, j * 2 + 2, i * 2 + 2, j * 2 + 2));
				} else if (a == c) {
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(a, b),
						pack_unordered_coordindates_pairs(i * 2 + 2, j * 2 + 1, i * 2 + 2, j * 2 + 2));
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(c, d),
						pack_unordered_coordindates_pairs(i * 2 + 2, j * 2 + 3, i * 2 + 2, j * 2 + 2));
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(b, d),
						pack_unordered_coordindates_pairs(i * 2 + 3, j * 2 + 2, i * 2 + 2, j * 2 + 2));
				} else if (a == d) {
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(a, b),
						pack_unordered_coordindates_pairs(i * 2 + 2, j * 2 + 1, i * 2 + 3, j * 2 + 2));
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(a, c),
						pack_unordered_coordindates_pairs(i * 2 + 1, j * 2 + 2, i * 2 + 2, j * 2 + 3));
				} else if (c == b) {
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(c, a),
						pack_unordered_coordindates_pairs(i * 2 + 1, j * 2 + 2, i * 2 + 2, j * 2 + 1));
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(c, d),
						pack_unordered_coordindates_pairs(i * 2 + 2, j * 2 + 3, i * 2 + 3, j * 2 + 2));
				} else { //four colors
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(a, b),
						pack_unordered_coordindates_pairs(i * 2 + 2, j * 2 + 1, i * 2 + 2, j * 2 + 2));
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(c, d),
						pack_unordered_coordindates_pairs(i * 2 + 2, j * 2 + 3, i * 2 + 2, j * 2 + 2));
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(a, c),
						pack_unordered_coordindates_pairs(i * 2 + 1, j * 2 + 2, i * 2 + 2, j * 2 + 2));
					add_segment_to_map(
						unordered_prov_pair_to_segments,
						pack_unordered_ushort_pair(b, d),
						pack_unordered_coordindates_pairs(i * 2 + 3, j * 2 + 2, i * 2 + 2, j * 2 + 2));
				}
			}
		}

		for (auto& t : unordered_prov_pair_to_segments) {
			const auto[loca, locb] = unpack_ushort_pair(t.first);

			for (uint32_t place = 0; place < t.second.size(); ++place) {
				if (t.second[place] == uint32_t(-1)) {
					continue;
				}

				auto[first_coordinate, second_coordinate] = unpack_uint_pair(t.second[place]);

				std::deque<uint32_t> packed_v;

				packed_v.push_back(first_coordinate);
				packed_v.push_back(second_coordinate);

				t.second[place] = uint32_t(-1);

				for (uint32_t inx = place + 1; inx < t.second.size(); ++inx) {
					const auto[inner_first_coordinate, inner_second_coordinate] = unpack_uint_pair(t.second[inx]);

					if (inner_first_coordinate == first_coordinate) {
						packed_v.push_front(inner_second_coordinate);
						first_coordinate = inner_second_coordinate;
						t.second[inx] = uint32_t(-1);
						inx = place + 1;
					} else if (inner_second_coordinate == first_coordinate) {
						packed_v.push_front(inner_first_coordinate);
						first_coordinate = inner_first_coordinate;
						t.second[inx] = uint32_t(-1);
						inx = place + 1;
					}

					if (inner_first_coordinate == second_coordinate) {
						packed_v.push_back(inner_second_coordinate);
						second_coordinate = inner_second_coordinate;
						t.second[inx] = uint32_t(-1);
						inx = place + 1;
					} else if (inner_second_coordinate == second_coordinate) {
						second_coordinate = inner_first_coordinate;
						packed_v.push_back(inner_first_coordinate);
						t.second[inx] = uint32_t(-1);
						inx = place + 1;
					}
				}

				borders.emplace(loca, std::vector<uint32_t>(packed_v.begin(), packed_v.end()));
				borders.emplace(locb, std::vector<uint32_t>(packed_v.begin(), packed_v.end())); // one border for each
			}
		}

	}

	//......................

	void create_block_borders(int32_t block_x, int32_t block_y, uint32_t* h_borders, uint32_t* v_borders, const uint16_t* map_data, int32_t width, int32_t height) {
		const int32_t x_start = block_x * block_size;
		const int32_t y_start = block_y * block_size;
		const int32_t x_limit = std::min(width, (block_x + 1) * block_size + 1);
		const int32_t y_limit = std::min(height - 1, (block_y + 1) * block_size + 1);

		for (int32_t j = y_start; j < y_limit; ++j) {
			for (int32_t i = x_start; i < x_limit; ++i) {
				const uint16_t center_value = map_data[i + j * width];
				const uint16_t right_value = get_value_from_data_h(i + 1, j, map_data, width);
				const uint16_t bottom_value = map_data[i + (j + 1) * width];

				const uint32_t dest_x = static_cast<uint32_t>(i - x_start);
				const uint32_t dest_y = static_cast<uint32_t>(j - y_start);
				h_borders[dest_x + dest_y * (block_size + 1)] = pair_to_border_index(center_value, right_value);
				v_borders[dest_x + dest_y * (block_size + 1)] = pair_to_border_index(center_value, bottom_value);
			}
		}
	}

	void color_map_creation_stub(boost::container::flat_map<uint32_t, uint16_t>& color_mapping, color_maps& cm, uint8_t* color_data, int32_t width, int32_t height) {
		uint16_t counter = 0;
		const auto pcolors = cm.primary_color_data();
		const auto scolors = cm.secondary_color_data();

		for (int32_t t = width * height - 1; t >= 0; --t) {
			uint32_t color_index =
				static_cast<uint32_t>(color_data[t * 3 + 0]) +
				(static_cast<uint32_t>(color_data[t * 3 + 1]) << 8) +
				(static_cast<uint32_t>(color_data[t * 3 + 2]) << 16);
			if (auto it = color_mapping.find(color_index); it == color_mapping.end()) {
				const auto new_index = ++counter;
				color_mapping.insert(std::make_pair(color_index, new_index));
				pcolors[new_index * 3 + 0] = color_data[t * 3 + 0];
				pcolors[new_index * 3 + 1] = color_data[t * 3 + 1];
				pcolors[new_index * 3 + 2] = color_data[t * 3 + 2];
				scolors[new_index * 3 + 0] = static_cast<uint8_t>(255 - color_data[t * 3 + 0]);
				scolors[new_index * 3 + 1] = static_cast<uint8_t>(255 - color_data[t * 3 + 1]);
				scolors[new_index * 3 + 2] = static_cast<uint8_t>(255 - color_data[t * 3 + 2]);
			}
		}

		cm.update_ready();
	}


	map_data_textures create_data_textures(const boost::container::flat_map<uint32_t, uint16_t>& color_mapping, uint8_t* color_data, int32_t width, int32_t height) {
		map_data_textures result;
		result.width = width;
		result.height = height;
		result.primary_data = new uint16_t[static_cast<size_t>(width * height)];
		//uint16_t* local_data = new uint16_t[static_cast<size_t>(width * height) * sizeof(uint16_t)];

		for (int32_t t = width * height - 1; t >= 0; --t) {
			uint32_t color_index =
				static_cast<uint32_t>(color_data[t * 3 + 0]) +
				(static_cast<uint32_t>(color_data[t * 3 + 1]) << 8) +
				(static_cast<uint32_t>(color_data[t * 3 + 2]) << 16);
			if (auto it = color_mapping.find(color_index); it != color_mapping.end())
				result.primary_data[t] = it->second;
			else
				result.primary_data[t] = 0;
		}

		glGenTextures(1, &result.primary_handle);
		glBindTexture(GL_TEXTURE_2D, result.primary_handle);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_R16UI, width, height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED_INTEGER, GL_UNSIGNED_SHORT, result.primary_data);

		/*
		for (int32_t j = height - 1; j >= 0; --j) {
			for (int32_t i = width - 1; i >= 0; --i) {
				const auto base_color = get_value_from_data(i, j, result.primary_data, width, height);
				const auto corner_color = get_value_from_data(i + 1, j + 1, result.primary_data, width, height);
				if (base_color == corner_color) {
					local_data[i + j * width] = base_color;
				} else if(
					corner_color == get_value_from_data(i, j + 1, result.primary_data, width, height) &&
					corner_color == get_value_from_data(i + 1, j, result.primary_data, width, height)) {
					local_data[i + j * width] = corner_color;
				} else {
					local_data[i + j * width] = base_color;
				}
			}
		}

		glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 3, width, height, 1, GL_RED_INTEGER, GL_UNSIGNED_SHORT, local_data); // bottom right

		for (int32_t j = height - 1; j >= 0; --j) {
			for (int32_t i = width - 1; i >= 0; --i) {
				const auto base_color = get_value_from_data(i, j, result.primary_data, width, height);
				const auto corner_color = get_value_from_data(i - 1, j + 1, result.primary_data, width, height);
				if (base_color == corner_color) {
					local_data[i + j * width] = base_color;
				} else if (
					corner_color == get_value_from_data(i, j + 1, result.primary_data, width, height) &&
					corner_color == get_value_from_data(i - 1, j, result.primary_data, width, height)) {
					local_data[i + j * width] = corner_color;
				} else {
					local_data[i + j * width] = base_color;
				}
			}
		}

		glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 4, width, height, 1, GL_RED_INTEGER, GL_UNSIGNED_SHORT, local_data); // bottom left

		for (int32_t j = height - 1; j >= 0; --j) {
			for (int32_t i = width - 1; i >= 0; --i) {
				const auto base_color = get_value_from_data(i, j, result.primary_data, width, height);
				const auto corner_color = get_value_from_data(i - 1, j - 1, result.primary_data, width, height);
				if (base_color == corner_color) {
					local_data[i + j * width] = base_color;
				} else if (
					corner_color == get_value_from_data(i, j - 1, result.primary_data, width, height) &&
					corner_color == get_value_from_data(i - 1, j, result.primary_data, width, height)) {
					local_data[i + j * width] = corner_color;
				} else {
					local_data[i + j * width] = base_color;
				}
			}
		}

		glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 1, width, height, 1, GL_RED_INTEGER, GL_UNSIGNED_SHORT, local_data); // top left

		for (int32_t j = height - 1; j >= 0; --j) {
			for (int32_t i = width - 1; i >= 0; --i) {
				const auto base_color = get_value_from_data(i, j, result.primary_data, width, height);
				const auto corner_color = get_value_from_data(i + 1, j - 1, result.primary_data, width, height);
				if (base_color == corner_color) {
					local_data[i + j * width] = base_color;
				} else if (
					corner_color == get_value_from_data(i, j - 1, result.primary_data, width, height) &&
					corner_color == get_value_from_data(i + 1, j, result.primary_data, width, height)) {
					local_data[i + j * width] = corner_color;
				} else {
					local_data[i + j * width] = base_color;
				}
			}
		}

		glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 2, width, height, 1, GL_RED_INTEGER, GL_UNSIGNED_SHORT, local_data); // top right
		delete[] local_data;*/
		return result;
	}

	void color_maps::bind_colors() {
		bool expected = true;
		if (update_pending.compare_exchange_strong(expected, false, std::memory_order_release, std::memory_order_acquire)) {
			glBindTexture(GL_TEXTURE_1D, primary_handle);
			glTexSubImage1D(GL_TEXTURE_1D, 0, 0, size, GL_RGB, GL_UNSIGNED_BYTE, primary_colors);
			glBindTexture(GL_TEXTURE_1D, secondary_handle);
			glTexSubImage1D(GL_TEXTURE_1D, 0, 0, size, GL_RGB, GL_UNSIGNED_BYTE, secondary_colors);
		}

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_1D, primary_handle);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_1D, secondary_handle);
	}

	void color_maps::create_color_textures() {
		glGenTextures(1, &primary_handle);
		glBindTexture(GL_TEXTURE_1D, primary_handle);

		glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGB8, size);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glGenTextures(1, &secondary_handle);
		glBindTexture(GL_TEXTURE_1D, secondary_handle);

		glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGB8, size);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	void map_display::initialize(open_gl_wrapper&, const boost::container::flat_map<uint32_t, uint16_t>& colors_map, uint8_t* map_data, int32_t width, int32_t height, float left_longitude, float top_latitude, float bottom_latitude) {

		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

#ifdef _DEBUG
		if (vertex_shader == 0 || fragment_shader == 0) {
			MessageBox(nullptr, L"shader creation failed", L"OpenGL error", MB_OK);
			std::abort();
		}
#endif
		const GLchar* array_a[] = { map_vertex_shader };

		glShaderSource(vertex_shader, 1, array_a, nullptr);
		glCompileShader(vertex_shader);

#ifdef _DEBUG
		GLint result;
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			MessageBox(nullptr, L"vertex shader compilation failed", L"OpenGL error", MB_OK);

			GLint logLen;
			glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &logLen);

			char * log = new char[static_cast<size_t>(logLen)];
			GLsizei written;
			glGetShaderInfoLog(vertex_shader, logLen, &written, log);

			MessageBoxA(nullptr, log, "OpenGL error", MB_OK);
			delete[] log;

			std::abort();
		}
#endif


		const GLchar* array_b[] = { map_fragment_shader };

		glShaderSource(fragment_shader, 1, array_b, nullptr);
		glCompileShader(fragment_shader);

#ifdef _DEBUG
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			MessageBox(nullptr, L"fragment shader compilation failed", L"OpenGL error", MB_OK);

			GLint logLen;
			glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &logLen);

			char * log = new char[static_cast<size_t>(logLen)];
			GLsizei written;
			glGetShaderInfoLog(fragment_shader, logLen, &written, log);

			MessageBoxA(nullptr, log, "OpenGL error", MB_OK);
			delete[] log;

			std::abort();
		}
#endif

		shader_handle = glCreateProgram();

#ifdef _DEBUG
		if (shader_handle == 0) {
			MessageBox(nullptr, L"shader program creation failed", L"OpenGL error", MB_OK);
			std::abort();
		}
#endif

		glAttachShader(shader_handle, vertex_shader);
		glAttachShader(shader_handle, fragment_shader);
		glLinkProgram(shader_handle);

#ifdef _DEBUG
		glGetProgramiv(shader_handle, GL_LINK_STATUS, &result);
		if (result == GL_FALSE) {
			MessageBox(nullptr, L"shader program linking failed", L"OpenGL error", MB_OK);

			GLint logLen;
			glGetProgramiv(shader_handle, GL_INFO_LOG_LENGTH, &logLen);

			char * log = new char[static_cast<size_t>(logLen)];
			GLsizei written;
			glGetProgramInfoLog(shader_handle, logLen, &written, log);
			MessageBoxA(nullptr, log, "OpenGL error", MB_OK);

			delete[] log;
			std::abort();
		}
#endif
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		lat_step = (bottom_latitude - top_latitude) / static_cast<float>(height);
		top_lat = top_latitude;
		long_step = 6.28318530718f / static_cast<float>(width);
		left_long = left_longitude;

		data_textures = create_data_textures(colors_map, map_data, width, height);
		colors.create_color_textures();

		glGenBuffers(1, &element_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(map_triangle_indices), map_triangle_indices, GL_STATIC_DRAW);

		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(edge_points), edge_points, GL_STATIC_DRAW);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0); //position
		glEnableVertexAttribArray(1); //layer_value

		glBindVertexBuffer(0, vertex_buffer, 0, sizeof(map_vertex_data));

		glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, 0); //position
		glVertexAttribFormat(1, 1, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(GLfloat) * 2); //layer_value
		glVertexAttribBinding(0, 0); //position -> to array zero
		glVertexAttribBinding(1, 0); //layer_value -> to array zero 

		ready = true;
	}

	void map_display::render(open_gl_wrapper &, float scale, float long_rotation, float lat_rotation, int32_t width, int32_t height) {
		if (ready) {
			glUseProgram(shader_handle);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, data_textures.primary_handle);

			colors.bind_colors();

			glUniform1f(map_parameters::aspect, static_cast<float>(width) / static_cast<float>(height));
			glUniform1f(map_parameters::scale, scale);
			glUniform1f(map_parameters::latitude_step, lat_step);
			glUniform1f(map_parameters::latitude_base, top_lat);
			glUniform1f(map_parameters::longitude_step, long_step);
			glUniform1f(map_parameters::longitude_base, left_long);
			glUniform1i(map_parameters::width_count, data_textures.width);

			const auto m = generate_rotation_matrix(long_rotation, lat_rotation);
			glUniformMatrix3fv(map_parameters::rotation_matrix, 1, GL_FALSE, m.data());

			glBindVertexArray(vao);
			glBindVertexBuffer(0, vertex_buffer, 0, sizeof(map_vertex_data));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);

			glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
			glEnable(GL_DEPTH_TEST);
			glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, data_textures.width * data_textures.height);
			glDisable(GL_DEPTH_TEST);
		}
	}

}

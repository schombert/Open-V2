#include "world_map.h"
#define GLEW_STATIC

#include "glew.h"
#include "wglew.h"
#include <Windows.h>
#include <map>

namespace graphics {
	struct vertex_data {
		float x;
		float y;
		float z;
		uint16_t value;
	};

	struct map_data_textures {
		uint16_t* primary_data = nullptr;
		uint32_t primary_handle = 0;
	};

	uint16_t get_color_value_from_data(int32_t i, int32_t j, uint16_t* map_data, int32_t width, int32_t height) {
		if ((j < 0) | (j >= width) | (i < 0) | (i >= height)) {
			return 0ui16;
		} else {
			uint16_t* index = map_data + (j + i * width) * sizeof(uint16_t);
			return *index;
		}
	}

	uint16_t get_value_from_data(int32_t i, int32_t j, uint16_t* data, int32_t width, int32_t height) {
		if ((j < 0) | (j >= height)) {
			return 0ui16;
		}
		if (i < 0)
			i += width;
		if (i >= width)
			i -= width;
		return data[i + j * width];
	}

	vertex_data vertex_from_lat_long(float latitude, float longitude, uint16_t value);

	map_data_textures create_data_textures(const std::map<uint32_t, uint16_t>& color_mapping, uint8_t* color_data, int32_t width, int32_t height) {
		map_data_textures result;

		result.primary_data = new uint16_t[width * height];
		uint16_t* local_data = (uint16_t*)_alloca(width * height * sizeof(uint16_t));

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

		glTexStorage3D(GL_TEXTURE_2D, 1, GL_RG8, width, height, 5);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexSubImage3D(GL_TEXTURE_2D, 0, 0, 0, 0, width, height, 1, GL_RG, GL_UNSIGNED_BYTE, result.primary_data);

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

		glTexSubImage3D(GL_TEXTURE_2D, 0, 0, 0, 1, width, height, 1, GL_RG, GL_UNSIGNED_BYTE, local_data); // bottom right

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

		glTexSubImage3D(GL_TEXTURE_2D, 0, 0, 0, 2, width, height, 1, GL_RG, GL_UNSIGNED_BYTE, local_data); // bottom left

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

		glTexSubImage3D(GL_TEXTURE_2D, 0, 0, 0, 3, width, height, 1, GL_RG, GL_UNSIGNED_BYTE, local_data); // top left

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

		glTexSubImage3D(GL_TEXTURE_2D, 0, 0, 0, 4, width, height, 1, GL_RG, GL_UNSIGNED_BYTE, local_data); // top right
		_freea(local_data);
		return result;
	}

	std::pair<uint32_t, uint32_t> build_region_arrays(uint16_t* map_data, int32_t width, int32_t height, float top_latitude, float bottom_latitude, float left_longitude) {
		struct vertex_map_element {
			uint32_t bottom_center;
			uint32_t bottom_right;
			uint32_t right;
			uint16_t value_mapped;
		};
		vertex_map_element* vbuf_a = (vertex_map_element*)_alloca(sizeof(vertex_map_element) * static_cast<size_t>(width + 1));
		vertex_map_element* vbuf_b = (vertex_map_element*)_alloca(sizeof(vertex_map_element) * static_cast<size_t>(width + 1));

		memset(vbuf_a, 0xFFFFFFFF, sizeof(vertex_map_element) * static_cast<size_t>(width + 1));
		vertex_map_element* old_elements = vbuf_a;
		vertex_map_element* new_elements = vbuf_b;

		std::vector<vertex_data> region_vertex_buffer;
		std::vector<uint32_t> region_index_buffer;

		const float latitude_increment = (top_latitude - bottom_latitude) / static_cast<float>(height);
		const float longitude_increment = static_cast<float>(width) / 360.0f;

		for (int32_t i = 0; i < height; ++i) {
			for (int32_t j = 0; j < width; ++j) {
				uint16_t top_left_val = old_elements[j].value_mapped != (uint16_t)-1 ? old_elements[j].value_mapped : 0ui16;
				uint16_t top_center_val = old_elements[j+1].value_mapped != (uint16_t)-1 ? old_elements[j+1].value_mapped : 0ui16;;
				uint16_t top_right_val = old_elements[j+2].value_mapped != (uint16_t)-1 ? old_elements[j+2].value_mapped : 0ui16;;

				uint16_t left_val = j != 0 ?
					(new_elements[j].value_mapped != (uint16_t)-1 ? new_elements[j].value_mapped : 0ui16) :
					get_color_value_from_data(i, width-1, map_data, width, height);
				uint16_t center_val = get_color_value_from_data(i, j, map_data, width, height);
				uint16_t right_val = get_color_value_from_data(i, j + 1, map_data, width, height);

				uint16_t bottom_left_val = get_color_value_from_data(i+1,j-1, map_data, width, height);
				uint16_t bottom_center_val = get_color_value_from_data(i + 1, j, map_data, width, height);
				uint16_t bottom_right_val = get_color_value_from_data(i + 1, j + 1, map_data, width, height);

				const bool top_left_corner = (center_val != top_left_val) && (top_left_val == top_center_val) && (top_left_val == left_val);
				const bool top_right_corner = (center_val != top_right_val) && (top_right_val == top_center_val) && (top_right_val == right_val);
				const bool bottom_left_corner = (center_val != bottom_left_val) && (bottom_left_val == bottom_center_val) && (bottom_left_val == left_val);
				const bool bottom_right_corner = (center_val != bottom_right_val) && (bottom_right_val == bottom_center_val) && (bottom_right_val == right_val);

				//center region -- top left
				if (top_left_corner) {
					if (new_elements[j].right != (uint32_t)-1) {
						const auto old_index = new_elements[j].right;
						region_index_buffer.push_back(old_index);
						region_vertex_buffer[old_index].value = center_val;
					} else {
						region_vertex_buffer.push_back(vertex_from_lat_long(top_latitude + i * latitude_increment + latitude_increment / 2.0f, left_longitude + j * longitude_increment, center_val));
						const auto new_index = static_cast<uint32_t>(region_vertex_buffer.size() - 1);
						region_index_buffer.push_back(new_index);
						new_elements[j].right = new_index;
					}
					if (old_elements[j + 1].bottom_center != (uint32_t)-1) {
						region_index_buffer.push_back(old_elements[j + 1].bottom_center);
					} else {
						region_vertex_buffer.push_back(vertex_from_lat_long(top_latitude + i * latitude_increment, left_longitude + j * longitude_increment + longitude_increment / 2.0f, center_val));
						const auto new_index = static_cast<uint32_t>(region_vertex_buffer.size() - 1);
						region_index_buffer.push_back(new_index);
						old_elements[j + 1].bottom_center = new_index;
					}
				} else {
					if (old_elements[j + 1].bottom_right != (uint32_t)-1) {
						const auto old_index = old_elements[j + 1].bottom_right;
						region_index_buffer.push_back(old_index);
						region_vertex_buffer[old_index].value = center_val;
					} else {
						region_vertex_buffer.push_back(vertex_from_lat_long(top_latitude + i * latitude_increment, left_longitude + j * longitude_increment, center_val));
						region_index_buffer.push_back(static_cast<uint32_t>(region_vertex_buffer.size() - 1));
					}

					if (top_right_corner) {
						if (old_elements[j + 1].bottom_center != (uint32_t)-1) {
							region_index_buffer.push_back(old_elements[j + 1].bottom_center);
						} else {
							region_vertex_buffer.push_back(vertex_from_lat_long(top_latitude + i * latitude_increment, left_longitude + j * longitude_increment + longitude_increment / 2.0f, center_val));
							const auto new_index = static_cast<uint32_t>(region_vertex_buffer.size() - 1);
							region_index_buffer.push_back(new_index);
							old_elements[j + 1].bottom_center = new_index;
						}
					}
				}

				// center region -- top right
				if (top_right_corner) {
					region_vertex_buffer.push_back(vertex_from_lat_long(top_latitude + i * latitude_increment + latitude_increment / 2.0f, left_longitude + (j + 1) * longitude_increment, center_val));
					const auto new_index = static_cast<uint32_t>(region_vertex_buffer.size() - 1);
					region_index_buffer.push_back(new_index);
					new_elements[j + 1].right = new_index;
				} else {
					if (old_elements[j + 1].bottom_right != (uint32_t)-1) {
						region_index_buffer.push_back(old_elements[j + 1].bottom_right);
					} else {
						region_vertex_buffer.push_back(vertex_from_lat_long(top_latitude + i * latitude_increment, left_longitude + (j + 1) * longitude_increment, center_val));
						const auto new_index = static_cast<uint32_t>(region_vertex_buffer.size() - 1);
						region_index_buffer.push_back(new_index);
						old_elements[j + 1].bottom_right = new_index;
					}

					if (bottom_right_corner) {
						region_vertex_buffer.push_back(vertex_from_lat_long(top_latitude + i * latitude_increment + latitude_increment / 2.0f, left_longitude + (j + 1) * longitude_increment, center_val));
						const auto new_index = static_cast<uint32_t>(region_vertex_buffer.size() - 1);
						region_index_buffer.push_back(new_index);
						new_elements[j + 1].right = new_index;
					} else {
						new_elements[j + 1].right = (uint32_t)-1;
					}
				}

				//center region -- bottom right
				if (bottom_right_corner) {
					region_vertex_buffer.push_back(vertex_from_lat_long(top_latitude + (i + 1) * latitude_increment, left_longitude + j * longitude_increment + longitude_increment / 2.0f, center_val));
					const auto new_index = static_cast<uint32_t>(region_vertex_buffer.size() - 1);
					region_index_buffer.push_back(new_index);
					new_elements[j + 1].bottom_center = new_index;
					new_elements[j + 1].bottom_right = (uint32_t)-1;
				} else {
					region_vertex_buffer.push_back(vertex_from_lat_long(top_latitude + (i + 1) * latitude_increment, left_longitude + (j + 1) * longitude_increment, center_val));
					const auto new_index = static_cast<uint32_t>(region_vertex_buffer.size() - 1);
					region_index_buffer.push_back(new_index);
					new_elements[j + 1].bottom_right = new_index;

					if (bottom_left_corner) {
						region_vertex_buffer.push_back(vertex_from_lat_long(top_latitude + (i + 1) * latitude_increment, left_longitude + j * longitude_increment + longitude_increment / 2.0f, center_val));
						const auto new_index = static_cast<uint32_t>(region_vertex_buffer.size() - 1);
						region_index_buffer.push_back(new_index);
						new_elements[j + 1].bottom_center = new_index;
					} else {
						new_elements[j + 1].bottom_center = (uint32_t)-1;;
					}
				}

				//center region -- bottom left
				if (bottom_left_corner) {
					if (!top_left_corner) {
						if (new_elements[j].right != (uint32_t)-1) {
							region_index_buffer.push_back(new_elements[j].right);
						} else {
							region_vertex_buffer.push_back(vertex_from_lat_long(top_latitude + i * latitude_increment + latitude_increment / 2.0f, left_longitude + j * longitude_increment, center_val));
							const auto new_index = static_cast<uint32_t>(region_vertex_buffer.size() - 1);
							region_index_buffer.push_back(new_index);
							new_elements[j].right = new_index;
						}
					}
				} else {
					if (new_elements[j].bottom_right != (uint32_t)-1) {
						region_index_buffer.push_back(new_elements[j].bottom_right);
					} else {
						region_vertex_buffer.push_back(vertex_from_lat_long(top_latitude + (i + 1) * latitude_increment, left_longitude + j * longitude_increment, center_val));
						const auto new_index = static_cast<uint32_t>(region_vertex_buffer.size() - 1);
						region_index_buffer.push_back(new_index);
						new_elements[j].bottom_right = new_index;
					}
				}

				//center region complete
				region_index_buffer.push_back((uint32_t)-1);

				//top left region
				if (top_left_corner) {
					const auto old_val = old_elements[j].bottom_right;
					if (old_val != (uint32_t)-1) {
						region_vertex_buffer[old_val].value = top_left_val;
						region_index_buffer.push_back(old_val);
					} else {
						region_vertex_buffer.push_back(vertex_from_lat_long(top_latitude + i * latitude_increment, left_longitude + j * longitude_increment, top_left_val));
						const auto new_index = static_cast<uint32_t>(region_vertex_buffer.size() - 1);
						region_index_buffer.push_back(new_index);
						old_elements[j].bottom_right = new_index;
					}

					region_index_buffer.push_back(old_elements[j + 1].bottom_center);
					region_index_buffer.push_back(new_elements[j].right);
					region_index_buffer.push_back((uint32_t)-1);
				}

				//top right region
				if (top_right_corner) {
					region_index_buffer.push_back(old_elements[j + 1].bottom_center);
					region_vertex_buffer[old_elements[j + 1].bottom_center].value = top_right_val;

					const auto old_val = old_elements[j+1].bottom_right;
					if (old_val != (uint32_t)-1) {
						region_index_buffer.push_back(old_val);
					} else {
						region_vertex_buffer.push_back(vertex_from_lat_long(top_latitude + i * latitude_increment, left_longitude + (j+1) * longitude_increment, top_right_val));
						const auto new_index = static_cast<uint32_t>(region_vertex_buffer.size() - 1);
						region_index_buffer.push_back(new_index);
						old_elements[j+1].bottom_right = new_index;
					}

					region_index_buffer.push_back(new_elements[j+1].right);
					region_index_buffer.push_back((uint32_t)-1);
				}

				//bottom right region
				if (bottom_right_corner) {
					const auto old_val = new_elements[j + 1].bottom_right;
					if (old_val != (uint32_t)-1) {
						region_index_buffer.push_back(old_val);
					} else {
						region_vertex_buffer.push_back(vertex_from_lat_long(top_latitude + (i + 1) * latitude_increment, left_longitude + (j + 1) * longitude_increment, bottom_right_val));
						const auto new_index = static_cast<uint32_t>(region_vertex_buffer.size() - 1);
						region_index_buffer.push_back(new_index);
						new_elements[j + 1].bottom_right = new_index;
					}

					region_index_buffer.push_back(new_elements[j + 1].bottom_center);
					region_vertex_buffer[new_elements[j + 1].bottom_center].value = bottom_right_val;

					region_index_buffer.push_back(new_elements[j + 1].right);
					region_index_buffer.push_back((uint32_t)-1);
				}

				//bottom left region
				if (bottom_left_corner) {
					region_index_buffer.push_back(new_elements[j + 1].bottom_center);
					region_vertex_buffer[new_elements[j + 1].bottom_center].value = bottom_left_val;

					const auto old_val = new_elements[j].bottom_right;
					if (old_val != (uint32_t)-1) {
						region_index_buffer.push_back(old_val);
					} else {
						region_vertex_buffer.push_back(vertex_from_lat_long(top_latitude + (i + 1) * latitude_increment, left_longitude + j  * longitude_increment, bottom_left_val));
						const auto new_index = static_cast<uint32_t>(region_vertex_buffer.size() - 1);
						region_index_buffer.push_back(new_index);
						new_elements[j].bottom_right = new_index;
					}

					region_index_buffer.push_back(new_elements[j].right);
					region_index_buffer.push_back((uint32_t)-1);
				}
			}

			//end of row
			const auto temp = new_elements;
			new_elements = old_elements;
			old_elements = temp;
		}
		

		_freea(vbuf_a);
		_freea(vbuf_b);
	}
}
#include "world_map.h"
#define GLEW_STATIC

#include "glew.h"
#include "wglew.h"
#include <Windows.h>
#include <map>

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

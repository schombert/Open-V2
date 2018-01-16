#include "world_map.h"
#include "concurrency_tools\\concurrency_tools.hpp"
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

	struct map_vertex_data_3d {
		float x;
		float y;
		float z;
		int32_t x_value;
		int32_t y_value;
	};

	constexpr int32_t block_size = 8;

	uint16_t get_value_from_data(int32_t i, int32_t j, const uint16_t* data, int32_t width, int32_t height);
	uint16_t get_value_from_data_h(int32_t i, int32_t j, const uint16_t* data, int32_t width);

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

	namespace map_parameters {
		constexpr GLuint aspect = 0;
		constexpr GLuint scale = 1;
		constexpr GLuint division_count = 2;
		constexpr GLuint inner_step = 3;
		constexpr GLuint rotation_matrix = 4;
	}

	//static map_vertex_data_3d test_data[] = {
	//	map_vertex_data_3d{1.0f,0.0f,0.0,0},
	//	map_vertex_data_3d{ 0.0f,1.0f,0.0,100},
	//	map_vertex_data_3d{0.0f,0.0f,1.0f,0},
	//	map_vertex_data_3d{ 0.0f,0.0f,-1.0f,0 }
	//};
	//static uint32_t test_indices[] = { 0, 1, 2, 1, 2, 3 };

	static const char* map_vertex_shader =
		"#version 430 core\n"
		"layout (location = 0) in vec3 vertex_position;\n"
		"layout (location = 1) in ivec2 value;\n"
		"\n"
		"out ivec2 t_value;\n"
		"\n"
		"layout(location = 0) uniform float aspect;\n" // = w/h
		"layout(location = 1) uniform float scale;\n"
		"layout(location = 2) uniform int division_count;\n"
		"layout(location = 3) uniform int inner_step;\n"
		"layout(location = 4) uniform mat3 rotation_matrix;\n"
		"\n"
		"const float hpi = 1.5707963267f;\n"
		"\n"
		"void main() {\n"
		"   vec3 rotated_position = rotation_matrix * vertex_position;\n"
		"   vec2 base_projection = vec2(atan(rotated_position.y, rotated_position.x), asin(rotated_position.z));\n"
		"   float rdiv = base_projection.y / 3.14159265358f;\n"
		"	gl_Position = vec4(scale * base_projection.x * sqrt(1.0f - (3.0f*rdiv*rdiv)) / 3.14159265358f, base_projection.y * scale * aspect / 3.14159265358f, "
		"        (abs(base_projection.x))/16.0f + 0.5f, 1.0f);\n"
		//"	gl_Position = vec4(scale * rotated_position.y, rotated_position.z * scale * aspect, "
		//"        -rotated_position.x/16.0f + 0.5f, 1.0f);\n"
		"	t_value = value;\n"
		"}\n";

	static const char* map_geometry_shader =
		"#version 430 core\n"
		"layout (lines_adjacency) in;\n"
		"layout (triangle_strip, max_vertices = 12) out;\n"
		"\n"
		"layout(location = 2) uniform int division_count;\n"
		"layout(location = 3) uniform int inner_step;\n"
		"\n"
		"in ivec2 t_value[];\n"
		"\n"
		"layout (binding = 0) uniform usampler2D data_texture;\n"
		"\n"
		"flat out ivec2 g_value;\n"
		"\n"
		"void main() {\n"
		"    const float max_x = max(max(gl_in[0].gl_Position.x, gl_in[1].gl_Position.x), max(gl_in[3].gl_Position.x, gl_in[2].gl_Position.x));\n"
		"    const float max_y = max(max(gl_in[0].gl_Position.y, gl_in[1].gl_Position.y), max(gl_in[3].gl_Position.y, gl_in[2].gl_Position.y));\n"
		"    const float min_x = min(min(gl_in[0].gl_Position.x, gl_in[1].gl_Position.x), min(gl_in[3].gl_Position.x, gl_in[2].gl_Position.x));\n"
		"    const float min_y = min(min(gl_in[0].gl_Position.y, gl_in[1].gl_Position.y), min(gl_in[3].gl_Position.y, gl_in[2].gl_Position.y));\n"
		"    if(min_x <= 1.0f && max_x >= -1.0f && min_y <= 1.0f && max_y >= -1.0f) {\n"
		"    const float inv_div_count = 1.0f / float(division_count);\n"
		"    for(int i = 0; i < division_count; ++i) {\n"
		"        const vec4 top_left = mix(gl_in[0].gl_Position, gl_in[3].gl_Position, inv_div_count * float(i));\n"
		"        const vec4 top_center = mix(gl_in[0].gl_Position, gl_in[3].gl_Position, inv_div_count * (float(i) + 0.5f));\n"
		"        const vec4 top_right = mix(gl_in[0].gl_Position, gl_in[3].gl_Position, inv_div_count * float(i + 1));\n"
		"        const vec4 bottom_left = mix(gl_in[1].gl_Position, gl_in[2].gl_Position, inv_div_count * float(i));\n"
		"        const vec4 bottom_center = mix(gl_in[1].gl_Position, gl_in[2].gl_Position, inv_div_count * (float(i) + 0.5f));\n"
		"        const vec4 bottom_right = mix(gl_in[1].gl_Position, gl_in[2].gl_Position, inv_div_count * float(i + 1));\n"
		"        for(int j = 0; j < division_count; ++j) {\n"
		"            const ivec2 passed_value = ivec2(t_value[0].x + i * inner_step, t_value[0].y + j * inner_step);\n"
		"            const vec4 center_left = mix(top_left, bottom_left, inv_div_count * (float(j) + 0.5f));\n"
		"            const vec4 center_right = mix(top_right, bottom_right, inv_div_count * (float(j) + 0.5f)); \n"
		"\n"
		"            gl_Position = mix(top_left, bottom_left, inv_div_count * float(j));\n"
		"            g_value = passed_value;\n"
		"            EmitVertex();\n"
		"            gl_Position = top_center;\n"
		"            g_value = passed_value;\n"
		"            EmitVertex();\n"
		"            gl_Position = center_left;\n"
		"            g_value = passed_value;\n"
		"            EmitVertex();\n"
		"            gl_Position = center_right;\n"
		"            g_value = passed_value;\n"
		"            EmitVertex();\n"
		"            gl_Position = bottom_center;\n"
		"            g_value = passed_value;\n"
		"            EmitVertex();\n"
		"            gl_Position = mix(top_right, bottom_right, inv_div_count * float(j + 1));\n"
		"            g_value = passed_value;\n"
		"            EmitVertex();\n"
		"            EndPrimitive();\n"
		"            gl_Position = center_left;\n"
		"            g_value = passed_value;\n"
		"            EmitVertex();\n"
		"            gl_Position = mix(top_left, bottom_left, inv_div_count * float(j + 1));\n"
		"            g_value = passed_value;\n"
		"            EmitVertex();\n"
		"            gl_Position = bottom_center;\n"
		"            g_value = passed_value;\n"
		"            EmitVertex();\n"
		"            EndPrimitive();\n"
		"            gl_Position = top_center;\n"
		"            g_value = passed_value;\n"
		"            EmitVertex();\n"
		"            gl_Position = mix(top_right, bottom_right, inv_div_count * float(j));\n"
		"            g_value = passed_value;\n"
		"            EmitVertex();\n"
		"            gl_Position = center_right;\n"
		"            g_value = passed_value;\n"
		"            EmitVertex();\n"
		"            EndPrimitive();\n"
		"        }\n"
		"    }\n"
		"    }\n"
		"}\n";

	static const char* map_fragment_shader =
		"#version 430 core\n"
		"\n"
		"flat in ivec2 g_value;\n"
		"layout (location = 0) out vec4 frag_color;\n"
		"\n"
		"layout (binding = 0) uniform usampler2D data_texture;\n"
		"layout (binding = 1) uniform sampler1D primary_colors;\n"
		"layout (binding = 2) uniform sampler1D secondary_colors;\n"
		"\n"
		"void main() {\n"
		"   frag_color = texelFetch(primary_colors, int(texelFetch(data_texture, g_value, 0).r), 0);\n"
		//"   frag_color = vec4(float(g_value.x) / 5600.0f,0.0f,float(g_value.y) / 2160.0f,1.0f );\n"
		"}\n";

	inline auto generate_rotation_matrix(float long_rotation, float lat_rotation) {
		Eigen::Matrix3f m;
		m = Eigen::AngleAxisf(lat_rotation, Eigen::Vector3f::UnitX()) * Eigen::AngleAxisf(long_rotation, Eigen::Vector3f::UnitZ());
		return m;
	}

	
	inline map_vertex_data_3d generate_3d_map_point(int32_t x_off, int32_t y_off, float base_lat, float lat_step, float long_step) {
		const float vx_pos = x_off * long_step;
		const float vy_pos = y_off * lat_step + base_lat;
		const float cos_vy = cos(vy_pos);
		const float sin_vy = sin(vy_pos);
		//return map_vertex_data_3d{ cos(vx_pos) * cos_vy , sin(vx_pos) * cos_vy, sin_vy, x_off, y_off };
		return map_vertex_data_3d{ cos(vx_pos) * cos_vy , sin(vx_pos) * cos_vy, sin_vy, x_off, y_off };
	}

	std::tuple<uint32_t, uint32_t, uint32_t> create_patches_buffers(int32_t width, int32_t height, float base_lat, float lat_step, float long_step);

	std::tuple<uint32_t, uint32_t, uint32_t> create_patches_buffers(int32_t width, int32_t height, float base_lat, float lat_step, float long_step) {
		std::vector<uint32_t> indices;
		std::vector<map_vertex_data_3d> vertices;

		const auto wblocks = (width + block_size - 1) / block_size;
		const auto hblocks = (height + block_size - 1) / block_size;

		for (int32_t j = 0; j <= hblocks; ++j) {
			for (int32_t i = 0; i <= wblocks; ++i) {
				vertices.emplace_back(generate_3d_map_point(i * block_size, j * block_size, base_lat, lat_step, long_step));
			}
		}
		
		for (int32_t j = 0; j < hblocks; ++j) {
			for (int32_t i = 0; i < wblocks; ++i) {
				indices.push_back(static_cast<uint32_t>(i + j * (wblocks + 1)));
				indices.push_back(static_cast<uint32_t>(i + (j + 1) * (wblocks + 1)));
				indices.push_back(static_cast<uint32_t>((i + 1) + (j + 1) * (wblocks + 1)));
				indices.push_back(static_cast<uint32_t>((i + 1) + j * (wblocks + 1)));
			}
		}
		uint32_t element_buffer = 0;
		uint32_t vertex_buffer = 0;

		glGenBuffers(1, &element_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int64_t>(indices.size() * sizeof(uint32_t)), indices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, static_cast<int64_t>(vertices.size() * sizeof(map_vertex_data_3d)), vertices.data(), GL_STATIC_DRAW);

		return std::make_tuple(vertex_buffer, element_buffer, static_cast<uint32_t>(indices.size()));
	}
	//.....................


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

		glGenTextures(1, &result.handle);
		glBindTexture(GL_TEXTURE_2D, result.handle);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_R16UI, width, height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,  GL_RED_INTEGER, GL_UNSIGNED_SHORT, result.primary_data);

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

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_1D, primary_handle);
		glActiveTexture(GL_TEXTURE2);
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

		glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
		glEnable(GL_DEPTH_TEST);

		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		GLuint geom_shader = glCreateShader(GL_GEOMETRY_SHADER);

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

		const GLchar* array_c[] = { map_geometry_shader };
		glShaderSource(geom_shader, 1, array_c, nullptr);
		glCompileShader(geom_shader);

#ifdef _DEBUG
		glGetShaderiv(geom_shader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			MessageBox(nullptr, L"geometry shader compilation failed", L"OpenGL error", MB_OK);

			GLint logLen;
			glGetShaderiv(geom_shader, GL_INFO_LOG_LENGTH, &logLen);

			char * log = new char[static_cast<size_t>(logLen)];
			GLsizei written;
			glGetShaderInfoLog(geom_shader, logLen, &written, log);

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
		glAttachShader(shader_handle, geom_shader);
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
		glDeleteShader(geom_shader);
		glDeleteShader(fragment_shader);

		lat_step = (bottom_latitude - top_latitude) / static_cast<float>(height);
		top_lat = top_latitude;
		long_step = 6.28318530718f / static_cast<float>(width);
		left_long = left_longitude;

		data_textures = create_data_textures(colors_map, map_data, width, height);
		colors.create_color_textures();

		const auto [v, e, c] = create_patches_buffers(width, height, top_lat, lat_step, long_step);
		vertex_buffer = v;
		element_buffer = e;
		triangle_vertex_count = static_cast<int32_t>(c);

		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0); //position
		glEnableVertexAttribArray(1); //layer_value

		glBindVertexBuffer(0, vertex_buffer, 0, sizeof(map_vertex_data_3d));

		glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0); //position
		glVertexAttribIFormat(1, 2, GL_INT, sizeof(GLfloat) * 3); //layer_value
		glVertexAttribBinding(0, 0); //position -> to array zero
		glVertexAttribBinding(1, 0); //layer_value -> to array zero 

		glDisable(GL_DEPTH_TEST);

		ready = true;
	}

	void map_display::render(open_gl_wrapper &, float scale, float long_rotation, float lat_rotation, int32_t width, int32_t height) {
		if (ready) {
			glUseProgram(shader_handle);


			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, data_textures.handle);

			colors.bind_colors();

			glUniform1f(map_parameters::aspect, static_cast<float>(width) / static_cast<float>(height));
			glUniform1f(map_parameters::scale, scale);

			const int32_t divisions = 1;

			glUniform1i(map_parameters::division_count, divisions);
			glUniform1i(map_parameters::inner_step, block_size / divisions);

			const auto m = generate_rotation_matrix(long_rotation, lat_rotation);
			glUniformMatrix3fv(map_parameters::rotation_matrix, 1, GL_FALSE, m.data());

			glBindVertexArray(vao);
			glBindVertexBuffer(0, vertex_buffer, 0, sizeof(map_vertex_data_3d));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);

			glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
			glEnable(GL_DEPTH_TEST);
			glDrawElements(GL_LINES_ADJACENCY, triangle_vertex_count, GL_UNSIGNED_INT, nullptr);
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
			glDisable(GL_DEPTH_TEST);
		}
	}

}

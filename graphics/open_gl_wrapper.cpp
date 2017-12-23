#include "open_gl_wrapper.hpp"
#define GLEW_STATIC

#include "glew.h"
#include "wglew.h"
#include <Windows.h>
#include <wingdi.h>
#include <string>
#include <atomic>
#include <thread>
#include <algorithm>
#include <chrono>
#include "v2_window.hpp"

#undef max
#undef min

namespace graphics {
#ifdef _DEBUG
	void debug_callback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* param) {

		std::string source_str;
		switch (source) {
			case GL_DEBUG_SOURCE_API:
				source_str = "OpenGL API call"; break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
				source_str = "Window system API"; break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER:
				source_str = "Shading language compiler"; break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:
				source_str = "Application associated with OpenGL"; break;
			case GL_DEBUG_SOURCE_APPLICATION:
				source_str = "User generated"; break;
			case GL_DEBUG_SOURCE_OTHER:
				source_str = "Unknown source"; break;
		}
		std::string error_type;
		switch (type) {
			case GL_DEBUG_TYPE_ERROR:
				error_type = "General error"; break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				error_type = "Deprecated behavior"; break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				error_type = "Undefined behavior"; break;
			case GL_DEBUG_TYPE_PORTABILITY:
				error_type = "Portability issue"; break;
			case GL_DEBUG_TYPE_PERFORMANCE:
				error_type = "Performance issue"; break;
			case GL_DEBUG_TYPE_MARKER:
				error_type = "Command stream annotation"; break;
			case GL_DEBUG_TYPE_PUSH_GROUP:
				error_type = "Error group push"; break;
			case GL_DEBUG_TYPE_POP_GROUP:
				error_type = "Error group pop"; break;
			case GL_DEBUG_TYPE_OTHER:
				error_type = "Uknown error type"; break;
		}
		std::string severity_str;
		switch (severity) {
			case GL_DEBUG_SEVERITY_HIGH:
				severity_str = "High"; break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				severity_str = "Medium"; break;
			case GL_DEBUG_SEVERITY_LOW:
				severity_str = "Low"; break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				severity_str = "Notification"; break;
		}
		std::string full_message("OpenGL error ");
		full_message += std::to_string(id);
		full_message += " ";
		full_message += " source: ";
		full_message += source_str;
		full_message += " type: ";
		full_message += error_type;
		full_message += " severity: ";
		full_message += severity_str;
		full_message += "; ";
		full_message += message;
		full_message += "\n";

		OutputDebugStringA(full_message.c_str());
	}
#endif


	namespace parameters {
		constexpr GLuint screen_width = 0;
		constexpr GLuint screen_height = 1;
		constexpr GLuint drawing_rectangle = 2;

		constexpr GLuint border_size = 6;
		constexpr GLuint inner_color = 7;

		constexpr GLuint enabled = 4;
		constexpr GLuint disabled = 3;
		constexpr GLuint border_filter = 0;
		constexpr GLuint filter = 1;
		constexpr GLuint no_filter = 2;
		constexpr GLuint sub_sprite = 5;
		constexpr GLuint use_mask = 6;
		constexpr GLuint progress_bar = 7;
		constexpr GLuint frame_stretch = 8;
		constexpr GLuint piechart = 9;
		constexpr GLuint barchart = 10;
		constexpr GLuint linegraph = 11;
	}

	char tquad_vertex_shader[] =
		"#version 430 core\n"
		"layout (location = 0) in vec2 vertex_position;\n"
		"layout (location = 1) in vec2 v_tex_coord;\n"
		"\n"
		"out vec2 tex_coord;\n"
		"\n"
		"layout(location = 0) uniform float screen_width;\n"
		"layout(location = 1) uniform float screen_height;\n"
		"layout(location = 2) uniform vec4 d_rect;\n"
		""
		"void main() {\n"
		"	gl_Position = vec4(-1.0 + (2.0 * ((vertex_position.x * d_rect.z)  + d_rect.x) / screen_width), "
		"1.0 - (2.0 * ((vertex_position.y * d_rect.w)  + d_rect.y) / screen_height), "
		"0.0, 1.0);\n"
		"	tex_coord = v_tex_coord;\n"
		"}\n";


	char tquad_fragment_shader[] =
		"#version 430 core\n"
		"\n"
		"#define M_PI 3.1415926535897932384626433832795\n"
		"subroutine vec4 color_function_class(vec4 color_in);\n"
		"layout(location = 0) subroutine uniform color_function_class coloring_function;\n"
		"\n"
		"subroutine vec4 font_function_class(vec2 tc);\n"
		"layout(location = 1) subroutine uniform font_function_class font_function;\n"
		"\n"
		"in vec2 tex_coord;\n"
		"layout (location = 0) out vec4 frag_color;\n"
		"\n"
		"layout (binding = 0) uniform sampler2D texture_sampler;\n"
		"layout (binding = 1) uniform sampler2D secondary_texture_sampler;\n"
		"layout(location = 2) uniform vec4 d_rect;\n"
		"layout(location = 6) uniform float border_size;\n"
		"layout(location = 7) uniform vec3 inner_color;\n"
		"\n"
		"layout(index = 0) subroutine(font_function_class)\n"
		"vec4 border_filter(vec2 tc) {\n"
		"	vec4 color_in = texture(texture_sampler, tc);\n"
		"	if(color_in.r > 0.5 + border_size / 4.0) {"
		"		return vec4(inner_color, 1.0);\n"
		"	} else if(color_in.r > 0.5 - border_size / 2.0) {"
		"		float sm_val = smoothstep(0.5, 0.5 + border_size / 4.0, color_in.r);\n"
		"		return vec4(mix(vec3(1.0, 1.0, 1.0) - inner_color, inner_color, sm_val), 1.0);"
		"	} else {\n"
		"		float sm_val = smoothstep(0.5 - border_size * 3.0 / 4.0, 0.5 - border_size / 2.0, color_in.r);\n"
		"		return vec4(vec3(1.0, 1.0, 1.0) - inner_color, sm_val);\n"
		"	}\n"
		"}\n"
		"\n"
		"layout(index = 1) subroutine(font_function_class)\n"
		"vec4 color_filter(vec2 tc) {\n"
		"	vec4 color_in = texture(texture_sampler, tc);\n"
		"	float sm_val = smoothstep(0.5 - border_size / 2.0, 0.5 + border_size / 2.0, color_in.r);\n"
		"	return vec4(inner_color, sm_val);\n"
		"}\n"
		"\n"
		"layout(index = 2) subroutine(font_function_class)\n"
		"vec4 no_filter(vec2 tc) {\n"
		"	return texture(texture_sampler, tc);\n"
		"}\n"
		"\n"
		"layout(index = 5) subroutine(font_function_class)\n"
		"vec4 subsprite(vec2 tc) {\n"
		"	return texture(texture_sampler, vec2(tc.x * inner_color.y + inner_color.x, tc.y));\n"
		"}\n"
		"\n"
		"layout(index = 6) subroutine(font_function_class)\n"
		"vec4 use_mask(vec2 tc) {\n"
		"	return vec4(texture(texture_sampler, tc).rgb, texture(secondary_texture_sampler, tc).a);\n"
		"}\n"
		"\n"
		"layout(index = 7) subroutine(font_function_class)\n"
		"vec4 progress_bar(vec2 tc) {\n"
		"	return mix( texture(texture_sampler, tc), texture(secondary_texture_sampler, tc), step(border_size, tc.x));\n"
		"}\n"
		"\n"
		"layout(index = 8) subroutine(font_function_class)\n"
		"vec4 frame_stretch(vec2 tc) {\n"
		"	const float realx = tc.x * d_rect.z;\n"
		"	const float realy = tc.y * d_rect.w;\n"
		"	const vec2 tsize = textureSize(texture_sampler, 0);\n"
		"	float xout = 0.0;\n"
		"	float yout = 0.0;\n"
		"	if(realx <= border_size)\n"
		"		xout = realx / tsize.x;\n"
		"	else if(realx >= (d_rect.z - border_size))"
		"		xout = (1.0 - border_size / tsize.x) + (border_size - (d_rect.z - realx))  / tsize.x;\n"
		"	else\n"
		"		xout = border_size / tsize.x + (1.0 - 2.0 * border_size / tsize.x) * (realx - border_size) / (d_rect.z * 2.0 * border_size);\n"
		"	if(realy <= border_size)\n"
		"		yout = realy / tsize.y;\n"
		"	else if(realy >= (d_rect.w - border_size))"
		"		yout = (1.0 - border_size / tsize.y) + (border_size - (d_rect.w - realy))  / tsize.y;\n"
		"	else\n"
		"		yout = border_size / tsize.y + (1.0 - 2.0 * border_size / tsize.y) * (realy - border_size) / (d_rect.w * 2.0 * border_size);\n"
		"	return texture(texture_sampler, vec2(xout, yout));\n"
		"}\n"
		"\n"
		"layout(index = 9) subroutine(font_function_class)\n"
		"vec4 piechart(vec2 tc) {\n"
		"	if(((tc.x - 0.5) * (tc.x - 0.5) + (tc.y - 0.5) * (tc.y - 0.5)) > 0.25)\n"
		"		return vec4(0.0, 0.0, 0.0, 0.0);\n"
		"	else\n"
		"		return texture(texture_sampler, vec2((atan((tc.y - 0.5), (tc.x - 0.5) ) + M_PI) / (2.0 * M_PI), 0.5));\n"
		"}\n"
		"\n"
		"layout(index = 10) subroutine(font_function_class)\n"
		"vec4 barchart(vec2 tc) {\n"
		"	vec4 color_in = texture(texture_sampler, vec2(tc.x, 0.5));\n"
		"	return vec4(color_in.rgb, step(color_in.a, tc.y));\n"
		"}\n"
		"\n"
		"layout(index = 11) subroutine(font_function_class)\n"
		"vec4 linegraph(vec2 tc) {\n"
		"	return mix(vec4(1.0, 0.0, 0.0, 1.0), vec4(0.0, 1.0, 0.0, 1.0), tc.y);\n"
		"}\n"
		"\n"
		"layout(index = 3) subroutine(color_function_class)\n"
		"vec4 disabled_color(vec4 color_in) {\n"
		"	const float amount = (color_in.r + color_in.g + color_in.b) / 4.0;\n"
		"	return vec4(amount, amount, amount, color_in.a);\n"
		"}\n"
		"\n"
		"layout(index = 4) subroutine(color_function_class)\n"
		"vec4 enabled_color(vec4 color_in) {\n"
		"	return color_in;\n"
		"}\n"
		"\n"
		"void main() {\n"
		"	frag_color = coloring_function(font_function(tex_coord));\n"
		"}\n";

	GLuint general_shader = 0;

	GLuint global_square_vao = 0;
	GLuint global_sqaure_buffer = 0;
	GLuint global_sqaure_right_buffer = 0;
	GLuint global_sqaure_left_buffer = 0;

	GLfloat global_square_data[] = {
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	GLfloat global_square_right_data[] = {
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.0f
	};

	GLfloat global_square_left_data[] = {
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f
	};

	GLuint sub_sqaure_buffers[64] = { 0 };

	void create_global_square() {
		glGenBuffers(1, &global_sqaure_buffer);

		// Populate the position buffer
		glBindBuffer(GL_ARRAY_BUFFER, global_sqaure_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16, global_square_data, GL_STATIC_DRAW);

		glGenVertexArrays(1, &global_square_vao);
		glBindVertexArray(global_square_vao);
		glEnableVertexAttribArray(0); //position
		glEnableVertexAttribArray(1); //texture coordinates

		glBindVertexBuffer(0, global_sqaure_buffer, 0, sizeof(GLfloat) * 4);

		glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, 0); //position
		glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2); //texture coordinates
		glVertexAttribBinding(0, 0); //position -> to array zero
		glVertexAttribBinding(1, 0); //texture coordinates -> to array zero 


		glGenBuffers(1, &global_sqaure_left_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, global_sqaure_left_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16, global_square_left_data, GL_STATIC_DRAW);

		glGenBuffers(1, &global_sqaure_right_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, global_sqaure_right_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16, global_square_right_data, GL_STATIC_DRAW);


		glGenBuffers(64, sub_sqaure_buffers);
		for (uint32_t i = 0; i < 64; ++i) {
			glBindBuffer(GL_ARRAY_BUFFER, sub_sqaure_buffers[i]);

			const float cell_x = static_cast<float>(i & 7) / 8.0f;
			const float cell_y = static_cast<float>((i >> 3) & 7) / 8.0f;

			GLfloat global_sub_square_data[] = {
				0.0f, 0.0f, cell_x, cell_y,
				0.0f, 1.0f, cell_x, cell_y + 1.0f / 8.0f,
				1.0f, 1.0f, cell_x + 1.0f / 8.0f, cell_y + 1.0f / 8.0f,
				1.0f, 0.0f, cell_x + 1.0f / 8.0f, cell_y
			};

			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16, global_sub_square_data, GL_STATIC_DRAW);
		}
	}

	void create_shaders() {
		GLuint general_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		GLuint general_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

#ifdef _DEBUG
		if (general_vertex_shader == 0 || general_fragment_shader == 0) {
			MessageBox(nullptr, L"shader creation failed", L"OpenGL error", MB_OK);
			std::abort();
		}
#endif
		const GLchar* array_a[] = { tquad_vertex_shader };

		glShaderSource(general_vertex_shader, 1, array_a, nullptr);
		glCompileShader(general_vertex_shader);

#ifdef _DEBUG
		GLint result;
		glGetShaderiv(general_vertex_shader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			MessageBox(nullptr, L"vertex shader compilation failed", L"OpenGL error", MB_OK);

			GLint logLen;
			glGetShaderiv(general_vertex_shader, GL_INFO_LOG_LENGTH, &logLen);

			char * log = new char[logLen];
			GLsizei written;
			glGetShaderInfoLog(general_vertex_shader, logLen, &written, log);

			MessageBoxA(nullptr, log, "OpenGL error", MB_OK);
			delete[] log;

			std::abort();
		}
#endif


		const GLchar* array_b[] = { tquad_fragment_shader };

		glShaderSource(general_fragment_shader, 1, array_b, nullptr);
		glCompileShader(general_fragment_shader);

#ifdef _DEBUG
		glGetShaderiv(general_fragment_shader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			MessageBox(nullptr, L"fragment shader compilation failed", L"OpenGL error", MB_OK);

			GLint logLen;
			glGetShaderiv(general_fragment_shader, GL_INFO_LOG_LENGTH, &logLen);

			char * log = new char[logLen];
			GLsizei written;
			glGetShaderInfoLog(general_fragment_shader, logLen, &written, log);

			MessageBoxA(nullptr, log, "OpenGL error", MB_OK);
			delete[] log;

			std::abort();
		}
#endif

		general_shader = glCreateProgram();

#ifdef _DEBUG
		if (general_shader == 0) {
			MessageBox(nullptr, L"shader program creation failed", L"OpenGL error", MB_OK);
			std::abort();
		}
#endif

		glAttachShader(general_shader, general_vertex_shader);
		glAttachShader(general_shader, general_fragment_shader);
		glLinkProgram(general_shader);

#ifdef _DEBUG
		glGetProgramiv(general_shader, GL_LINK_STATUS, &result);
		if (result == GL_FALSE) {
			MessageBox(nullptr, L"shader program linking failed", L"OpenGL error", MB_OK);

			GLint logLen;
			glGetProgramiv(general_shader, GL_INFO_LOG_LENGTH, &logLen);

			char * log = new char[logLen];
			GLsizei written;
			glGetProgramInfoLog(general_shader, logLen, &written, log);
			MessageBoxA(nullptr, log, "OpenGL error", MB_OK);

			delete[] log;
			std::abort();
		}
#endif
		glDeleteShader(general_vertex_shader);
		glDeleteShader(general_fragment_shader);

		glUseProgram(general_shader);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	std::pair<HGLRC, HGLRC> setup_opengl_context(HWND hwnd, HDC window_dc) {

		PIXELFORMATDESCRIPTOR pfd;
		ZeroMemory(&pfd, sizeof(pfd));
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		pfd.iLayerType = PFD_MAIN_PLANE;

		const int pixel_format = ChoosePixelFormat(window_dc, &pfd);
		SetPixelFormat(window_dc, pixel_format, &pfd);

		auto handle_to_ogl_dc = wglCreateContext(window_dc);
		wglMakeCurrent(window_dc, handle_to_ogl_dc);


		glewExperimental = GL_TRUE;

		if (glewInit() != 0) {
			MessageBox(hwnd, L"GLEW failed to initialize", L"GLEW error", MB_OK);
		}

		int attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 4,
			WGL_CONTEXT_FLAGS_ARB,
	#ifdef _DEBUG
			WGL_CONTEXT_DEBUG_BIT_ARB |
	#endif
			0,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		if (wglewIsSupported("WGL_ARB_create_context") != 1) {
			MessageBox(hwnd, L"WGL_ARB_create_context not supported", L"OpenGL error", MB_OK);
			std::abort();
		} else {
			auto ui_context = wglCreateContextAttribsARB(window_dc, NULL, attribs);
			auto new_context = wglCreateContextAttribsARB(window_dc, NULL, attribs);

			if (wglShareLists(new_context, ui_context) == FALSE) {
				MessageBox(hwnd, L"Unable to share contexts", L"OpenGL error", MB_OK);
				std::abort();
			}


			wglMakeCurrent(window_dc, NULL);
			wglDeleteContext(handle_to_ogl_dc);
			wglMakeCurrent(window_dc, new_context);

#ifdef _DEBUG
			glDebugMessageCallback(debug_callback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_OTHER, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_FALSE);
#endif

			if (wglewIsSupported("WGL_EXT_swap_control_tear") == 1) {
				wglSwapIntervalEXT(-1);
			} else if (wglewIsSupported("WGL_EXT_swap_control") == 1) {
				wglSwapIntervalEXT(1);
			} else {
				MessageBox(hwnd, L"WGL_EXT_swap_control_tear and WGL_EXT_swap_control not supported", L"OpenGL error", MB_OK);
			}

			glEnable(GL_LINE_SMOOTH);
			glEnable(GL_SCISSOR_TEST);

			create_shaders();
			create_global_square();

			wglMakeCurrent(window_dc, NULL);

			return std::make_pair(new_context, ui_context);
		}
	}

	void release_opengl_context(void* _hwnd, HGLRC context) {
		HWND hwnd = (HWND)_hwnd;
		HDC window_dc = GetDC(hwnd);

		wglMakeCurrent(window_dc, NULL);
		wglDeleteContext(context);
	}

	class _open_gl_wrapper {
	public:
		HGLRC context;
		HGLRC ui_context;
		HDC window_dc;
		std::thread render_thread;

		uint32_t viewport_x = 0;
		uint32_t viewport_y = 0;
		std::atomic<bool> update_viewport = false;
		std::atomic<bool> active = true;
	};

	open_gl_wrapper::open_gl_wrapper() : impl(std::make_unique<_open_gl_wrapper>()) {}
	open_gl_wrapper::~open_gl_wrapper() {}

	void open_gl_wrapper::set_render_thread(const std::function<void()> &f) {
		impl->render_thread = std::thread(f);
	}

	bool open_gl_wrapper::is_running() {
		return impl->active.load(std::memory_order::memory_order_acquire);
	}

	void open_gl_wrapper::setup_context(void* hwnd) {
		impl->window_dc = GetDC((HWND)hwnd);
		std::tie(impl->context, impl->ui_context) = setup_opengl_context((HWND)hwnd, impl->window_dc);
	}

	void open_gl_wrapper::destory(void* hwnd) {
		impl->active.store(false, std::memory_order::memory_order_release);
		impl->render_thread.join();
		release_opengl_context((HWND)hwnd, impl->context);
		release_opengl_context((HWND)hwnd, impl->ui_context);
		ReleaseDC((HWND)hwnd, impl->window_dc);
	}

	void open_gl_wrapper::bind_to_thread() {
		wglMakeCurrent(impl->window_dc, impl->context);
	}

	void open_gl_wrapper::bind_to_ui_thread() {
		wglMakeCurrent(impl->window_dc, impl->ui_context);
	}

	void open_gl_wrapper::display() {
		SwapBuffers(impl->window_dc);
	}

	void open_gl_wrapper::clear() {
		bool expected = true;
		if(impl->update_viewport.compare_exchange_strong(expected, false, std::memory_order::memory_order_release, std::memory_order::memory_order_acquire)) {
			impl->update_viewport.store(false, std::memory_order::memory_order_release);
			glViewport(0, 0, impl->viewport_x, impl->viewport_y);
			glDepthRange(-1.0, 1.0);

			glUniform1f(parameters::screen_width, impl->viewport_x);
			glUniform1f(parameters::screen_height, impl->viewport_y);
		}
		glScissor(0, 0, impl->viewport_x, impl->viewport_y);

		glClearColor(0.5, 0.5, 0.5, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void open_gl_wrapper::render_textured_rect(bool enabled, float x, float y, float width, float height, texture& t, rotation r) {
		glBindVertexArray(global_square_vao);

		switch (r) {
			case rotation::upright:
				glBindVertexBuffer(0, global_sqaure_buffer, 0, sizeof(GLfloat) * 4); break;
			case rotation::left:
				glBindVertexBuffer(0, global_sqaure_left_buffer, 0, sizeof(GLfloat) * 4); break;
			case rotation::right:
				glBindVertexBuffer(0, global_sqaure_right_buffer, 0, sizeof(GLfloat) * 4); break;
		}


		glUniform4f(parameters::drawing_rectangle, x, y, width, height);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t.handle());

		GLuint subroutines[2] = { enabled ? parameters::enabled : parameters::disabled, parameters::no_filter };
		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 2, subroutines); // must set all subroutines in one call

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

	void open_gl_wrapper::render_linegraph(bool enabled, float x, float y, float width, float height, lines& l) {
		glBindVertexArray(global_square_vao);

		l.bind_buffer();

		glUniform4f(parameters::drawing_rectangle, x, y, width, height);
		glLineWidth(2.0f);

		GLuint subroutines[2] = { enabled ? parameters::enabled : parameters::disabled, parameters::linegraph };
		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 2, subroutines); // must set all subroutines in one call

		glDrawArrays(GL_LINE_STRIP, 0, l.count);
	}

	void open_gl_wrapper::render_barchart(bool enabled, float x, float y, float width, float height, data_texture& t, rotation r) {
		glBindVertexArray(global_square_vao);

		switch (r) {
			case rotation::upright:
				glBindVertexBuffer(0, global_sqaure_buffer, 0, sizeof(GLfloat) * 4); break;
			case rotation::left:
				glBindVertexBuffer(0, global_sqaure_left_buffer, 0, sizeof(GLfloat) * 4); break;
			case rotation::right:
				glBindVertexBuffer(0, global_sqaure_right_buffer, 0, sizeof(GLfloat) * 4); break;
		}


		glUniform4f(parameters::drawing_rectangle, x, y, width, height);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t.handle());

		GLuint subroutines[2] = { enabled ? parameters::enabled : parameters::disabled, parameters::barchart };
		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 2, subroutines); // must set all subroutines in one call

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

	void open_gl_wrapper::render_piechart(bool enabled, float x, float y, float size, data_texture& t) {
		glBindVertexArray(global_square_vao);

		glBindVertexBuffer(0, global_sqaure_buffer, 0, sizeof(GLfloat) * 4);

		glUniform4f(parameters::drawing_rectangle, x, y, size, size);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t.handle());

		GLuint subroutines[2] = { enabled ? parameters::enabled : parameters::disabled, parameters::piechart };
		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 2, subroutines); // must set all subroutines in one call

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

	void open_gl_wrapper::render_bordered_rect(bool enabled, float border_size, float x, float y, float width, float height, texture& t, rotation r) {
		glBindVertexArray(global_square_vao);

		switch (r) {
			case rotation::upright:
				glBindVertexBuffer(0, global_sqaure_buffer, 0, sizeof(GLfloat) * 4); break;
			case rotation::left:
				glBindVertexBuffer(0, global_sqaure_left_buffer, 0, sizeof(GLfloat) * 4); break;
			case rotation::right:
				glBindVertexBuffer(0, global_sqaure_right_buffer, 0, sizeof(GLfloat) * 4); break;
		}


		glUniform4f(parameters::drawing_rectangle, x, y, width, height);
		glUniform1f(parameters::border_size, border_size);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t.handle());

		GLuint subroutines[2] = { enabled ? parameters::enabled : parameters::disabled, parameters::frame_stretch };
		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 2, subroutines); // must set all subroutines in one call

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

	void open_gl_wrapper::render_masked_rect(bool enabled, float x, float y, float width, float height, texture& t, texture& mask, rotation r) {
		glBindVertexArray(global_square_vao);

		switch (r) {
			case rotation::upright:
				glBindVertexBuffer(0, global_sqaure_buffer, 0, sizeof(GLfloat) * 4); break;
			case rotation::left:
				glBindVertexBuffer(0, global_sqaure_left_buffer, 0, sizeof(GLfloat) * 4); break;
			case rotation::right:
				glBindVertexBuffer(0, global_sqaure_right_buffer, 0, sizeof(GLfloat) * 4); break;
		}


		glUniform4f(parameters::drawing_rectangle, x, y, width, height);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t.handle());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mask.handle());

		GLuint subroutines[2] = { enabled ? parameters::enabled : parameters::disabled, parameters::use_mask };
		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 2, subroutines); // must set all subroutines in one call

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

	void open_gl_wrapper::render_progress_bar(bool enabled, float progress, float x, float y, float width, float height, texture& left, texture& right, rotation r) {
		glBindVertexArray(global_square_vao);

		switch (r) {
			case rotation::upright:
				glBindVertexBuffer(0, global_sqaure_buffer, 0, sizeof(GLfloat) * 4); break;
			case rotation::left:
				glBindVertexBuffer(0, global_sqaure_left_buffer, 0, sizeof(GLfloat) * 4); break;
			case rotation::right:
				glBindVertexBuffer(0, global_sqaure_right_buffer, 0, sizeof(GLfloat) * 4); break;
		}


		glUniform4f(parameters::drawing_rectangle, x, y, width, height);
		glUniform1f(parameters::border_size, progress);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, left.handle());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, right.handle());

		GLuint subroutines[2] = { enabled ? parameters::enabled : parameters::disabled, parameters::progress_bar };
		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 2, subroutines); // must set all subroutines in one call

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

	void open_gl_wrapper::render_subsprite(bool enabled, int frame, int total_frames, float x, float y, float width, float height, texture& t, rotation r) {
		glBindVertexArray(global_square_vao);

		switch (r) {
			case rotation::upright:
				glBindVertexBuffer(0, global_sqaure_buffer, 0, sizeof(GLfloat) * 4); break;
			case rotation::left:
				glBindVertexBuffer(0, global_sqaure_left_buffer, 0, sizeof(GLfloat) * 4); break;
			case rotation::right:
				glBindVertexBuffer(0, global_sqaure_right_buffer, 0, sizeof(GLfloat) * 4); break;
		}

		const auto scale = 1.0f / static_cast<float>(total_frames);
		glUniform3f(parameters::inner_color, static_cast<float>(frame) * scale, scale, 0.0f);
		glUniform4f(parameters::drawing_rectangle, x, y, width, height);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t.handle());

		GLuint subroutines[2] = { enabled ? parameters::enabled : parameters::disabled, parameters::sub_sprite };
		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 2, subroutines); // must set all subroutines in one call

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

	void open_gl_wrapper::render_character(char16_t codepoint, bool enabled, float x, float y, float size, font& f) {
		const auto g = f.get_render_glyph(codepoint);

		glBindVertexBuffer(0, sub_sqaure_buffers[g.buffer], 0, sizeof(GLfloat) * 4);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g.texture);

		glUniform4f(parameters::drawing_rectangle, x, y, size, size);

		glUniform3f(parameters::inner_color, 0.0f, 0.0f, 0.0f);

		glUniform1f(parameters::border_size, 0.08f * 16.0f / size); // for normal outlines
		// glUniform1f(parameters::border_size, 0.16f * 16.0f / size); // for bold outlines

		//GLuint subroutines[2] = { enabled ? parameters::enabled : parameters::disabled, parameters::filter };
		GLuint subroutines[2] = { enabled ? parameters::enabled : parameters::disabled, parameters::border_filter };
		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 2, subroutines); // must set all subroutines in one call

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

	void internal_text_render(const char16_t* codepoints, uint32_t count, float x, float baseline_y, float size, font& f, float extra) {
		for (uint32_t i = 0; i < count; ++i) {
			const auto g = f.get_render_glyph(codepoints[i]);

			glBindVertexBuffer(0, sub_sqaure_buffers[g.buffer], 0, sizeof(GLfloat) * 4);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, g.texture);

			glUniform4f(parameters::drawing_rectangle, x + g.x_offset * size / 64.0f, baseline_y + g.y_offset * size / 64.0f, size, size);

			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			x += g.advance * size / 64.0f + extra + ((i != count - 1) ? f.render_kerning(codepoints[i], codepoints[i + 1]) * size / 64.0f : 0.0f);
		}
	}

	void open_gl_wrapper::render_outlined_text(const char16_t* codepoints, uint32_t count, bool enabled, float x, float y, float size, const color& c, font& f) {
		GLuint subroutines[2] = { enabled ? parameters::enabled : parameters::disabled, parameters::border_filter };
		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 2, subroutines);

		glUniform3f(parameters::inner_color, c.r, c.b, c.g);
		glUniform1f(parameters::border_size, 0.08f * 16.0f / size); // for normal outlines
		// glUniform1f(parameters::border_size, 0.16f * 16.0f / size); // for bold outlines

		internal_text_render(codepoints, count, x, y + size, size, f, 0.6);
	}

	void open_gl_wrapper::render_text(const char16_t* codepoints, uint32_t count, bool enabled, float x, float y, float size, const color& c, font& f) {
		GLuint subroutines[2] = { enabled ? parameters::enabled : parameters::disabled, parameters::filter };
		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 2, subroutines);

		glUniform3f(parameters::inner_color, c.r, c.b, c.g);
		glUniform1f(parameters::border_size, 0.08f * 16.0f / size);

		internal_text_render(codepoints, count, x, y + size, size, f, 0.0);
	}

	void open_gl_wrapper::set_viewport(uint32_t width, uint32_t height) {
		impl->viewport_x = width;
		impl->viewport_y = height;
		impl->update_viewport.store(true, std::memory_order::memory_order_release);
	}

	scissor_rect::scissor_rect(int32_t x, int32_t y, int32_t width, int32_t height) {
		glGetIntegerv(GL_SCISSOR_BOX, oldrect);

		const auto x_begin = std::max(x, oldrect[0]);
		const auto old_x_end = oldrect[0] + oldrect[2];
		const auto new_x_end = x + width;

		const auto y_begin = std::max(y, oldrect[1]);
		const auto old_y_end = oldrect[1] + oldrect[3];
		const auto new_y_end = y + height;

		
		glScissor(x_begin, y_begin, std::max(0, std::min(old_x_end, new_x_end) - x_begin), std::max(0, std::min(old_y_end, new_y_end) - y_begin));
	}

	scissor_rect::~scissor_rect() {
		glScissor(oldrect[0], oldrect[1], oldrect[2], oldrect[3]);
	}

}
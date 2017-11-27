#pragma once
#include <memory>
#include <functional>
#include "texture.h"
#include "text.h"

class _open_gl_wrapper;

class window_base;

struct color {
	float r = 0.0f;
	float b = 0.0f;
	float g = 0.0f;
};

class open_gl_wrapper {
private:
	void set_render_thread(const std::function<void()>&);
	bool is_running();
	void setup_context(void* hwnd);
public:
	std::unique_ptr<_open_gl_wrapper> impl;

	open_gl_wrapper();
	~open_gl_wrapper();

	template<typename T>
	void setup(void* hwnd, T& base);
	void destory(void* hwnd);
	void bind_to_thread();
	void clear();
	void display();
	void set_viewport(uint32_t width, uint32_t height);
	void render_textured_rect(bool enabled, float x, float y, float width, float height, texture& t);
	void render_character(char16_t codepoint, bool enabled, float x, float y, float size, font& f);
	void render_text(const char16_t* codepoints, uint32_t count, bool enabled, float x, float baseline_y, float size, const color& c, font& f);
	void render_outlined_text(const char16_t* codepoints, uint32_t count, bool enabled, float x, float baseline_y, float size, const color& c, font& f);
};
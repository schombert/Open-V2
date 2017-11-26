#pragma once
#include <memory>

class _font;

class glyph {
public:
	float advance = 0.0;
	float y_offset = 0.0;
	float x_offset = 0.0;
	uint32_t texture = 0;
	uint32_t buffer = 0;
};

class open_gl_wrapper;

class font {
public:
	const std::unique_ptr<_font> impl;

	font(const char* filename, font& p);
	font(const char* filename);
	~font();

	void load_font(open_gl_wrapper&);
	glyph get_glyph(char16_t codepoint);
	float line_height() const;
	float kerning(char16_t codepoint_first, char16_t codepoint_second) const;
};
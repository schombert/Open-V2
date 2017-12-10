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

class metrics_glyph {
public:
	float advance = 0.0;
};

class open_gl_wrapper;

class font {
public:
	const std::unique_ptr<_font> impl;

	font(const char* filename, font& p);
	font(const char* filename);
	~font();

	void load_font(open_gl_wrapper&);
	void load_metrics_font();
	glyph get_render_glyph(char16_t codepoint);
	metrics_glyph get_metrics_glyph(char16_t codepoint);
	float line_height() const;
	float render_kerning(char16_t codepoint_first, char16_t codepoint_second) const;
	float metrics_kerning(char16_t codepoint_first, char16_t codepoint_second) const;
	float metrics_text_extent(char16_t* codepoints, uint32_t count, float size, bool outlined = false) const;
};
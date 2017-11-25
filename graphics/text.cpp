#include "text.h"
#define GLEW_STATIC

#include "glew.h"
#include "wglew.h"
#include <Windows.h>
#include <wingdi.h>
#include <optional>
#include <algorithm>
#include <math.h>

#include "ft2build.h"
#include "freetype\\freetype.h"
#include "freetype\\ftglyph.h"
#include "boost\\container\\\flat_map.hpp"

#include "open_gl_setup.h"

#undef min
#undef max

class free_type_library {
public:
	FT_Library  library;

	free_type_library() {
		FT_Init_FreeType(&library);
	}
};

free_type_library global_freetype;

static constexpr int magnification_factor = 4;

int transform_offset(int x, int y, int xoffset, int yoffset, int btmap_x_off, int btmap_y_off, int width, int height, int pitch) {
	int bmp_x = x * magnification_factor + xoffset - btmap_x_off;
	int bmp_y = y * magnification_factor + yoffset - btmap_y_off;

	if ((bmp_x < 0) | (bmp_x >= width) | (bmp_y < 0) | (bmp_y >= height))
		return -1;
	else
		return bmp_x + bmp_y * pitch;
}

int32_t transform_offset_b(int32_t x, int32_t y, int32_t btmap_x_off, int32_t btmap_y_off, uint32_t width, uint32_t height, uint32_t pitch) {
	int bmp_x = x - btmap_x_off;
	int bmp_y = y - btmap_y_off;

	if ((bmp_x < 0) | (bmp_x >= width) | (bmp_y < 0) | (bmp_y >= height))
		return -1;
	else
		return bmp_x + bmp_y * pitch;
}

constexpr int dr_size = 64 * magnification_factor;
constexpr float rt_2 = 1.41421356237309504f;

void init_in_map(bool* in_map, uint8_t* bmp_data, int32_t btmap_x_off, int32_t btmap_y_off, uint32_t width, uint32_t height, uint32_t pitch) {
	for (uint32_t j = 1; j < dr_size - 1; ++j) {
		for (uint32_t i = 1; i < dr_size - 1; ++i) {
			const auto boff = transform_offset_b(i, j, btmap_x_off, btmap_y_off, width, height, pitch);
			in_map[i + dr_size * j] = (boff != -1) ? (bmp_data[boff] > 127) : false;
		}
	}
}

void dead_reckoning(float* distance_map, const bool* in_map) {
	int16_t yborder[dr_size * dr_size] = { -1 };
	int16_t xborder[dr_size * dr_size] = { -1 };

	for (uint32_t i = 0; i < dr_size*dr_size; ++i) {
		distance_map[i] = std::numeric_limits<float>::infinity();
	}
	for (uint32_t j = 1; j < dr_size-1; ++j) {
		for (uint32_t i = 1; i < dr_size-1; ++i) {
			if (in_map[i - 1 + dr_size * j] != in_map[i + dr_size * j] ||
				in_map[i + 1 + dr_size * j] != in_map[i + dr_size * j] ||
				in_map[i + dr_size * (j + 1)] != in_map[i + dr_size * j] ||
				in_map[i + dr_size * (j - 1)] != in_map[i + dr_size * j]) {
				distance_map[i + dr_size * j] = 0.0f;
				yborder[i + dr_size * j] = j;
				xborder[i + dr_size * j] = i;
			}
		}
	}
	for (uint32_t j = 1; j < dr_size - 1; ++j) {
		for (uint32_t i = 1; i < dr_size - 1; ++i) {
			if (distance_map[(i - 1) + dr_size * (j - 1)] + rt_2 < distance_map[(i) + dr_size * (j)]) {
				yborder[i + dr_size * j] = yborder[(i - 1) + dr_size * (j - 1)];
				xborder[i + dr_size * j] = xborder[(i - 1) + dr_size * (j - 1)];
				distance_map[(i)+dr_size * (j)] =
					std::sqrt((i - xborder[i + dr_size * j])*(i - xborder[i + dr_size * j]) +
					          (j - yborder[i + dr_size * j])*(j - yborder[i + dr_size * j]));
			}
			if (distance_map[(i) + dr_size * (j - 1)] + 1.0f < distance_map[(i) + dr_size * (j)]) {
				yborder[i + dr_size * j] = yborder[(i) + dr_size * (j - 1)];
				xborder[i + dr_size * j] = xborder[(i) + dr_size * (j - 1)];
				distance_map[(i)+dr_size * (j)] =
					std::sqrt((i - xborder[i + dr_size * j])*(i - xborder[i + dr_size * j]) +
					(j - yborder[i + dr_size * j])*(j - yborder[i + dr_size * j]));
			}
			if (distance_map[(i + 1) + dr_size * (j - 1)] + rt_2 < distance_map[(i)+dr_size * (j)]) {
				yborder[i + dr_size * j] = yborder[(i + 1) + dr_size * (j - 1)];
				xborder[i + dr_size * j] = xborder[(i + 1) + dr_size * (j - 1)];
				distance_map[(i)+dr_size * (j)] =
					std::sqrt((i - xborder[i + dr_size * j])*(i - xborder[i + dr_size * j]) +
					(j - yborder[i + dr_size * j])*(j - yborder[i + dr_size * j]));
			}
			if (distance_map[(i - 1) + dr_size * (j)] + 1.0f < distance_map[(i)+dr_size * (j)]) {
				yborder[i + dr_size * j] = yborder[(i - 1) + dr_size * (j)];
				xborder[i + dr_size * j] = xborder[(i - 1) + dr_size * (j)];
				distance_map[(i)+dr_size * (j)] =
					std::sqrt((i - xborder[i + dr_size * j])*(i - xborder[i + dr_size * j]) +
					(j - yborder[i + dr_size * j])*(j - yborder[i + dr_size * j]));
			}
		}
	}

	for (uint32_t j = dr_size - 1; j > 0; --j) {
		for (uint32_t i = dr_size - 1; i > 0; --i) {
			if (distance_map[(i + 1) + dr_size * (j)] + 1.0f < distance_map[(i)+dr_size * (j)]) {
				yborder[i + dr_size * j] = yborder[(i + 1) + dr_size * (j)];
				xborder[i + dr_size * j] = xborder[(i + 1) + dr_size * (j)];
				distance_map[(i)+dr_size * (j)] =
					std::sqrt((i - xborder[i + dr_size * j])*(i - xborder[i + dr_size * j]) +
					(j - yborder[i + dr_size * j])*(j - yborder[i + dr_size * j]));
			}
			if (distance_map[(i - 1) + dr_size * (j + 1)] + rt_2 < distance_map[(i)+dr_size * (j)]) {
				yborder[i + dr_size * j] = yborder[(i - 1) + dr_size * (j + 1)];
				xborder[i + dr_size * j] = xborder[(i - 1) + dr_size * (j + 1)];
				distance_map[(i)+dr_size * (j)] =
					std::sqrt((i - xborder[i + dr_size * j])*(i - xborder[i + dr_size * j]) +
					(j - yborder[i + dr_size * j])*(j - yborder[i + dr_size * j]));
			}
			if (distance_map[(i) + dr_size * (j + 1)] + 1.0f < distance_map[(i)+dr_size * (j)]) {
				yborder[i + dr_size * j] = yborder[(i) + dr_size * (j + 1)];
				xborder[i + dr_size * j] = xborder[(i) + dr_size * (j + 1)];
				distance_map[(i)+dr_size * (j)] =
					std::sqrt((i - xborder[i + dr_size * j])*(i - xborder[i + dr_size * j]) +
					(j - yborder[i + dr_size * j])*(j - yborder[i + dr_size * j]));
			}
			if (distance_map[(i + 1) + dr_size * (j + 1)] + rt_2 < distance_map[(i)+dr_size * (j)]) {
				yborder[i + dr_size * j] = yborder[(i + 1) + dr_size * (j + 1)];
				xborder[i + dr_size * j] = xborder[(i + 1) + dr_size * (j + 1)];
				distance_map[(i)+dr_size * (j)] =
					std::sqrt((i - xborder[i + dr_size * j])*(i - xborder[i + dr_size * j]) +
					(j - yborder[i + dr_size * j])*(j - yborder[i + dr_size * j]));
			}
		}
	}

	for (uint32_t i = 0; i < dr_size*dr_size; ++i) {
		if (in_map[i])
			distance_map[i] *= -1.0f;
	}
}

class _font {
public:

	_font* const parent;
	boost::container::flat_map<char16_t, glyph> glyph_mappings;
	std::vector<uint32_t> textures;

	FT_Face font_face;
	uint32_t last_in_texture = 0;

	_font(const char* filename, _font* p) : parent(p) {
		FT_New_Face(global_freetype.library, filename, 0, &font_face);
		FT_Select_Charmap(font_face, FT_ENCODING_UNICODE);

		FT_Set_Pixel_Sizes(font_face, 0, 64 * magnification_factor);
	}

	~_font() {
		FT_Done_Face(font_face);
	}

	bool glyph_stored(char16_t codepoint) {
		if (glyph_mappings.find(codepoint) != glyph_mappings.end() || (parent && (parent->glyph_stored(codepoint))))
			return true;
		return false;
	}

	std::optional<glyph> _getglyph(char16_t codepoint) {
		if (auto loc = glyph_mappings.find(codepoint);  loc != glyph_mappings.end()) {
			return std::optional<glyph>(loc->second);
		} else if (parent) {
			return parent->_getglyph(codepoint);
		} else {
			return std::optional<glyph>();
		}
	}

	glyph makeglyph(char16_t codepoint) {
		const auto index_in_this_font = FT_Get_Char_Index(font_face, codepoint);
		if (index_in_this_font) {
			FT_Load_Glyph(font_face, index_in_this_font, FT_LOAD_TARGET_NORMAL | FT_LOAD_RENDER);

			FT_Glyph g_result;
			FT_Get_Glyph(font_face->glyph, &g_result);

			glyph created;
			created.advance = static_cast<float>(font_face->glyph->metrics.horiAdvance) / static_cast<float>(1 << 6);

			if ((last_in_texture & (62 - 1)) == 0) {
				glGenTextures(1, &created.texture);
				glBindTexture(GL_TEXTURE_2D, created.texture);
				glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 64 * 8, 64 * 8);

				textures.push_back(created.texture);
			} else {
				created.texture = textures.back();
				glBindTexture(GL_TEXTURE_2D, created.texture);
			}

			// FTglyph_To_Bitmap(&g_result, FT_RENDER_MODE_NORMAL, nullptr, 1);

			FT_Bitmap& bitmap = ((FT_BitmapGlyphRec*)g_result)->bitmap;
			
			const float hb_x = static_cast<float>(font_face->glyph->metrics.horiBearingX) / static_cast<float>(1 << 6);
			const float hb_y = static_cast<float>(font_face->glyph->metrics.horiBearingY) / static_cast<float>(1 << 6);

			created.advance = static_cast<float>(font_face->glyph->metrics.horiAdvance) / static_cast<float>(1 << 6);

			created.buffer = (last_in_texture & 63);

			uint8_t pixel_buffer[4 * 64 * 64];;
			memset(pixel_buffer, 255, 4 * 64 * 64);

			const int btmap_x_off = 32 * magnification_factor - bitmap.width / 2;
			const int btmap_y_off = 32 * magnification_factor - bitmap.rows / 2;

			created.x_offset = (hb_x - static_cast<float>(btmap_x_off)) * 1.0f / static_cast<float>(magnification_factor);
			created.y_offset = (-hb_y - static_cast<float>(btmap_y_off)) * 1.0f / static_cast<float>(magnification_factor);

			bool in_map[dr_size * dr_size] = { false };
			float distance_map[dr_size * dr_size];

			init_in_map(in_map, bitmap.buffer, btmap_x_off, btmap_y_off, bitmap.width, bitmap.rows, bitmap.pitch);
			dead_reckoning(distance_map, in_map);


			for (int y = 0; y < 64; ++y) {
				for (int x = 0; x < 64; ++x) {
					
					const size_t index = (x + y * 64) * 4;
					const float distance_value = distance_map[
						(x * magnification_factor + magnification_factor / 2) + 
						(y * magnification_factor + magnification_factor / 2)* dr_size]
						/  static_cast<float>(magnification_factor * 64);
					const int int_value = static_cast<int>(distance_value * -255.0f + 128.0f);
					const uint8_t small_value = static_cast<uint8_t>(std::min(255, std::max(0, int_value)));

					pixel_buffer[index + 0] = small_value;
					pixel_buffer[index + 1] = small_value;
					pixel_buffer[index + 2] = small_value;
					pixel_buffer[index + 3] = 255;
				}
			}

			glTexSubImage2D(GL_TEXTURE_2D, 0,
				(last_in_texture & 7) * 64,
				((last_in_texture >> 3) & 7) * 64,
				64,
				64,
				GL_RGBA, GL_UNSIGNED_BYTE, pixel_buffer);

			++last_in_texture;
			glyph_mappings[codepoint] = created;

			return created;
		} else if(parent) {
			return parent->makeglyph(codepoint);
		} else {
			// impossible to display this codepoint
			return glyph();
		}
	}

	glyph getglyph(char16_t codepoint) {
		const auto found = _getglyph(codepoint);
		if (found)
			return *found;
		else
			return makeglyph(codepoint);
	}
};

glyph font::get_glyph(char16_t codepoint) {
	return impl->getglyph(codepoint);
}

font::font(const char* filename, font& p) : impl(std::make_unique<_font>(filename, p.impl.get())) {

}

font::font(const char* filename) : impl(std::make_unique<_font>(filename, nullptr)) {

}

font::~font() {

}


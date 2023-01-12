#include "text.h"
#define GLEW_STATIC

#include "glew.h"
#include "wglew.h"
#include <Windows.h>
#include <wingdi.h>
#include <optional>
#include <algorithm>
#include <math.h>
#include <ctype.h>

#include "common\\common.h"
#include "ft2build.h"
#include "freetype\\freetype.h"
#include "freetype\\ftglyph.h"
#include "simple_mpl\\simple_mpl.hpp"

#include "concurrency_tools\\concurrency_tools.hpp"
#include "open_gl_wrapper.h"
#include "Parsers\\parsers.hpp"

#undef min
#undef max

namespace graphics {
	constexpr int magnification_factor = 4;
	constexpr int dr_size = 64 * magnification_factor;

	int transform_offset(int x, int y, int xoffset, int yoffset, int btmap_x_off, int btmap_y_off, int width, int height, int pitch);
	int32_t transform_offset_b(int32_t x, int32_t y, int32_t btmap_x_off, int32_t btmap_y_off, uint32_t width, uint32_t height, uint32_t pitch);
	void init_in_map(bool in_map[dr_size * dr_size], uint8_t* bmp_data, int32_t btmap_x_off, int32_t btmap_y_off, uint32_t width, uint32_t height, uint32_t pitch);
	void dead_reckoning(float distance_map[dr_size * dr_size], const bool in_map[dr_size * dr_size]);

	class free_type_library {
	public:
		FT_Library render_library;
		FT_Library metrics_library;

		free_type_library() {
			FT_Init_FreeType(&render_library);
			FT_Init_FreeType(&metrics_library);
		}
	};

	static free_type_library global_freetype;

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

		if ((bmp_x < 0) | (bmp_x >= (int32_t)width) | (bmp_y < 0) | (bmp_y >= (int32_t)height))
			return -1;
		else
			return bmp_x + bmp_y * (int32_t)pitch;
	}

	
	constexpr float rt_2 = 1.41421356237309504f;

	void init_in_map(bool in_map[dr_size * dr_size], uint8_t* bmp_data, int32_t btmap_x_off, int32_t btmap_y_off, uint32_t width, uint32_t height, uint32_t pitch) {
		for (int32_t j = 0; j < dr_size; ++j) {
			for (int32_t i = 0; i < dr_size; ++i) {
				const auto boff = transform_offset_b(i, j, btmap_x_off, btmap_y_off, width, height, pitch);
				in_map[i + dr_size * j] = (boff != -1) ? (bmp_data[boff] > 127) : false;
			}
		}
	}

	void dead_reckoning(float distance_map[dr_size * dr_size], const bool in_map[dr_size * dr_size]) {
		int16_t yborder[dr_size * dr_size] = { 0 };
		int16_t xborder[dr_size * dr_size] = { 0 };

		for (uint32_t i = 0; i < dr_size*dr_size; ++i) {
			distance_map[i] = std::numeric_limits<float>::infinity();
		}
		for (int32_t j = 1; j < dr_size - 1; ++j) {
			for (int32_t i = 1; i < dr_size - 1; ++i) {
				if (in_map[i - 1 + dr_size * j] != in_map[i + dr_size * j] ||
					in_map[i + 1 + dr_size * j] != in_map[i + dr_size * j] ||
					in_map[i + dr_size * (j + 1)] != in_map[i + dr_size * j] ||
					in_map[i + dr_size * (j - 1)] != in_map[i + dr_size * j]) {
					distance_map[i + dr_size * j] = 0.0f;
					yborder[i + dr_size * j] = static_cast<int16_t>(j);
					xborder[i + dr_size * j] = static_cast<int16_t>(i);
				}
			}
		}
		for (int32_t j = 1; j < dr_size - 1; ++j) {
			for (int32_t i = 1; i < dr_size - 1; ++i) {
				if (distance_map[(i - 1) + dr_size * (j - 1)] + rt_2 < distance_map[(i)+dr_size * (j)]) {
					yborder[i + dr_size * j] = yborder[(i - 1) + dr_size * (j - 1)];
					xborder[i + dr_size * j] = xborder[(i - 1) + dr_size * (j - 1)];
					distance_map[(i)+dr_size * (j)] =
						(float)std::sqrt((i - xborder[i + dr_size * j])*(i - xborder[i + dr_size * j]) +
						(j - yborder[i + dr_size * j])*(j - yborder[i + dr_size * j]));
				}
				if (distance_map[(i)+dr_size * (j - 1)] + 1.0f < distance_map[(i)+dr_size * (j)]) {
					yborder[i + dr_size * j] = yborder[(i)+dr_size * (j - 1)];
					xborder[i + dr_size * j] = xborder[(i)+dr_size * (j - 1)];
					distance_map[(i)+dr_size * (j)] =
						(float)std::sqrt((i - xborder[i + dr_size * j])*(i - xborder[i + dr_size * j]) +
						(j - yborder[i + dr_size * j])*(j - yborder[i + dr_size * j]));
				}
				if (distance_map[(i + 1) + dr_size * (j - 1)] + rt_2 < distance_map[(i)+dr_size * (j)]) {
					yborder[i + dr_size * j] = yborder[(i + 1) + dr_size * (j - 1)];
					xborder[i + dr_size * j] = xborder[(i + 1) + dr_size * (j - 1)];
					distance_map[(i)+dr_size * (j)] =
						(float)std::sqrt((i - xborder[i + dr_size * j])*(i - xborder[i + dr_size * j]) +
						(j - yborder[i + dr_size * j])*(j - yborder[i + dr_size * j]));
				}
				if (distance_map[(i - 1) + dr_size * (j)] + 1.0f < distance_map[(i)+dr_size * (j)]) {
					yborder[i + dr_size * j] = yborder[(i - 1) + dr_size * (j)];
					xborder[i + dr_size * j] = xborder[(i - 1) + dr_size * (j)];
					distance_map[(i)+dr_size * (j)] =
						(float)std::sqrt((i - xborder[i + dr_size * j])*(i - xborder[i + dr_size * j]) +
						(j - yborder[i + dr_size * j])*(j - yborder[i + dr_size * j]));
				}
			}
		}

		for (int32_t j = dr_size - 2; j > 0; --j) {
			for (int32_t i = dr_size - 2; i > 0; --i) {
				if (distance_map[(i + 1) + dr_size * (j)] + 1.0f < distance_map[(i)+dr_size * (j)]) {
					yborder[i + dr_size * j] = yborder[(i + 1) + dr_size * (j)];
					xborder[i + dr_size * j] = xborder[(i + 1) + dr_size * (j)];
					distance_map[(i)+dr_size * (j)] =
						(float)std::sqrt((i - xborder[i + dr_size * j])*(i - xborder[i + dr_size * j]) +
						(j - yborder[i + dr_size * j])*(j - yborder[i + dr_size * j]));
				}
				if (distance_map[(i - 1) + dr_size * (j + 1)] + rt_2 < distance_map[(i)+dr_size * (j)]) {
					yborder[i + dr_size * j] = yborder[(i - 1) + dr_size * (j + 1)];
					xborder[i + dr_size * j] = xborder[(i - 1) + dr_size * (j + 1)];
					distance_map[(i)+dr_size * (j)] =
						(float)std::sqrt((i - xborder[i + dr_size * j])*(i - xborder[i + dr_size * j]) +
						(j - yborder[i + dr_size * j])*(j - yborder[i + dr_size * j]));
				}
				if (distance_map[(i)+dr_size * (j + 1)] + 1.0f < distance_map[(i)+dr_size * (j)]) {
					yborder[i + dr_size * j] = yborder[(i)+dr_size * (j + 1)];
					xborder[i + dr_size * j] = xborder[(i)+dr_size * (j + 1)];
					distance_map[(i)+dr_size * (j)] =
						(float)std::sqrt((i - xborder[i + dr_size * j])*(i - xborder[i + dr_size * j]) +
						(j - yborder[i + dr_size * j])*(j - yborder[i + dr_size * j]));
				}
				if (distance_map[(i + 1) + dr_size * (j + 1)] + rt_2 < distance_map[(i)+dr_size * (j)]) {
					yborder[i + dr_size * j] = yborder[(i + 1) + dr_size * (j + 1)];
					xborder[i + dr_size * j] = xborder[(i + 1) + dr_size * (j + 1)];
					distance_map[(i)+dr_size * (j)] =
						(float)std::sqrt((i - xborder[i + dr_size * j])*(i - xborder[i + dr_size * j]) +
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
		boost::container::flat_map<char16_t, metrics_glyph> metrics_glyph_mappings;
		std::vector<uint32_t> textures;

		const std::string font_file;

		FT_Face font_face;
		FT_Face metrics_font_face;
		uint32_t last_in_texture = 0;
		float line_height = 0.0f;
		bool ft_font_loaded = false;
		bool ft_metrics_font_loaded = false;


		void load() {
			FT_New_Face(global_freetype.render_library, font_file.c_str(), 0, &font_face);
			FT_Select_Charmap(font_face, FT_ENCODING_UNICODE);
			FT_Set_Pixel_Sizes(font_face, 0, 64 * magnification_factor);
			ft_font_loaded = true;

			line_height = static_cast<float>(font_face->size->metrics.height) / static_cast<float>((1 << 6) * magnification_factor);
		}

		void load_metrics() {
			FT_New_Face(global_freetype.metrics_library, font_file.c_str(), 0, &metrics_font_face);
			FT_Select_Charmap(metrics_font_face, FT_ENCODING_UNICODE);
			FT_Set_Pixel_Sizes(metrics_font_face, 0, 64 * magnification_factor);
			ft_metrics_font_loaded = true;

			line_height = static_cast<float>(metrics_font_face->size->metrics.height) / static_cast<float>((1 << 6) * magnification_factor);
		}

		_font(const char* filename, _font* p) : parent(p), font_file(filename) {

		}

		~_font() {
			if(ft_font_loaded)
				FT_Done_Face(font_face);
			if(ft_metrics_font_loaded)
				FT_Done_Face(metrics_font_face);
		}

		bool glyph_stored(char16_t codepoint) {
			if (glyph_mappings.find(codepoint) != glyph_mappings.end() || (parent && (parent->glyph_stored(codepoint))))
				return true;
			return false;
		}

		std::optional<glyph> _get_glyph(char16_t codepoint) {
			if (auto loc = glyph_mappings.find(codepoint);  loc != glyph_mappings.end()) {
				return std::optional<glyph>(loc->second);
			} else if (parent) {
				return parent->_get_glyph(codepoint);
			} else {
				return std::optional<glyph>();
			}
		}

		std::optional<metrics_glyph> _get_m_glyph(char16_t codepoint) {
			if (auto loc = metrics_glyph_mappings.find(codepoint);  loc != metrics_glyph_mappings.end()) {
				return std::optional<metrics_glyph>(loc->second);
			} else if (parent) {
				return parent->_get_m_glyph(codepoint);
			} else {
				return std::optional<metrics_glyph>();
			}
		}

		float kerning(char16_t codepoint_first, char16_t codepoint_second) const {
			const auto index_a = FT_Get_Char_Index(font_face, codepoint_first);
			const auto index_b = FT_Get_Char_Index(font_face, codepoint_second);

			if ((index_a == 0) | (index_b == 0)) {
				if (parent)
					return parent->kerning(codepoint_first, codepoint_second);
				else
					return 0.0f;
			}

			if (FT_HAS_KERNING(font_face)) {
				FT_Vector kerning;
				FT_Get_Kerning(font_face, index_a, index_b, FT_KERNING_DEFAULT, &kerning);
				return static_cast<float>(kerning.x) / static_cast<float>((1 << 6) * magnification_factor);
			} else {
				return 0.0f;
			}
		}

		float metrics_kerning(char16_t codepoint_first, char16_t codepoint_second) const {
			const auto index_a = FT_Get_Char_Index(metrics_font_face, codepoint_first);
			const auto index_b = FT_Get_Char_Index(metrics_font_face, codepoint_second);

			if ((index_a == 0) | (index_b == 0)) {
				if (parent)
					return parent->metrics_kerning(codepoint_first, codepoint_second);
				else
					return 0.0f;
			}

			if (FT_HAS_KERNING(metrics_font_face)) {
				FT_Vector kerning;
				FT_Get_Kerning(metrics_font_face, index_a, index_b, FT_KERNING_DEFAULT, &kerning);
				return static_cast<float>(kerning.x) / static_cast<float>((1 << 6) * magnification_factor);
			} else {
				return 0.0f;
			}
		}

		glyph make_glyph(char16_t codepoint) {
			const auto index_in_this_font = FT_Get_Char_Index(font_face, codepoint);
			if (index_in_this_font) {
				FT_Load_Glyph(font_face, index_in_this_font, FT_LOAD_TARGET_NORMAL | FT_LOAD_RENDER);

				FT_Glyph g_result;
				FT_Get_Glyph(font_face->glyph, &g_result);

				glyph created;

				if ((last_in_texture & 63) == 0) {
					glGenTextures(1, &created.texture);
					glBindTexture(GL_TEXTURE_2D, created.texture);
					glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, 64 * 8, 64 * 8);

					//glClearTexImage(created.texture, 0, GL_RED, GL_FLOAT, nullptr);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

					textures.push_back(created.texture);
				} else {
					created.texture = textures.back();
					glBindTexture(GL_TEXTURE_2D, created.texture);
				}

				FT_Bitmap& bitmap = ((FT_BitmapGlyphRec*)g_result)->bitmap;

				const float hb_x = static_cast<float>(font_face->glyph->metrics.horiBearingX) / static_cast<float>(1 << 6);
				const float hb_y = static_cast<float>(font_face->glyph->metrics.horiBearingY) / static_cast<float>(1 << 6);

				created.advance = static_cast<float>(font_face->glyph->metrics.horiAdvance) / static_cast<float>((1 << 6) * magnification_factor);

				created.buffer = (last_in_texture & 63);

				uint8_t pixel_buffer[64 * 64];

				const int btmap_x_off = 32 * magnification_factor - bitmap.width / 2;
				const int btmap_y_off = 32 * magnification_factor - bitmap.rows / 2;

				created.x_offset = (hb_x - static_cast<float>(btmap_x_off)) * 1.0f / static_cast<float>(magnification_factor);
				created.y_offset = (-hb_y - static_cast<float>(btmap_y_off)) * 1.0f / static_cast<float>(magnification_factor);

				bool in_map[dr_size * dr_size] = { false };
				float distance_map[dr_size * dr_size];

				init_in_map(in_map, bitmap.buffer, btmap_x_off, btmap_y_off, bitmap.width, bitmap.rows, (uint32_t)bitmap.pitch);
				dead_reckoning(distance_map, in_map);


				for (int y = 0; y < 64; ++y) {
					for (int x = 0; x < 64; ++x) {

						const size_t index = static_cast<size_t>(x + y * 64);
						const float distance_value = distance_map[
							(x * magnification_factor + magnification_factor / 2) +
								(y * magnification_factor + magnification_factor / 2)* dr_size]
							/ static_cast<float>(magnification_factor * 64);
							const int int_value = static_cast<int>(distance_value * -255.0f + 128.0f);
							const uint8_t small_value = static_cast<uint8_t>(std::min(255, std::max(0, int_value)));

							pixel_buffer[index] = small_value;
					}
				}

				glTexSubImage2D(GL_TEXTURE_2D, 0,
					(last_in_texture & 7) * 64,
					((last_in_texture >> 3) & 7) * 64,
					64,
					64,
					GL_RED, GL_UNSIGNED_BYTE, pixel_buffer);

				++last_in_texture;
				glyph_mappings[codepoint] = created;

				FT_Done_Glyph(g_result);

				return created;
			} else if (parent) {
				return parent->make_glyph(codepoint);
			} else {
				// impossible to display this codepoint
				return glyph();
			}
		}

		metrics_glyph make_metrics_glyph(char16_t codepoint) {
			const auto index_in_this_font = FT_Get_Char_Index(metrics_font_face, codepoint);
			if (index_in_this_font) {
				FT_Load_Glyph(metrics_font_face, index_in_this_font, FT_LOAD_TARGET_NORMAL);

				FT_Glyph g_result;
				FT_Get_Glyph(metrics_font_face->glyph, &g_result);

				metrics_glyph created;
				created.advance = static_cast<float>(metrics_font_face->glyph->metrics.horiAdvance) / static_cast<float>((1 << 6) * magnification_factor);
				metrics_glyph_mappings[codepoint] = created;

				FT_Done_Glyph(g_result);
				return created;
			} else if (parent) {
				return parent->make_metrics_glyph(codepoint);
			} else {
				return metrics_glyph();
			}
		}

		glyph get_glyph(char16_t codepoint) {
			const auto found = _get_glyph(codepoint);
			if (found)
				return *found;
			else
				return make_glyph(codepoint);
		}

		metrics_glyph get_metrics_glyph(char16_t codepoint) {
			const auto found = _get_m_glyph(codepoint);
			if (found)
				return *found;
			else
				return make_metrics_glyph(codepoint);
		}
	};

	glyph font::get_render_glyph(char16_t codepoint) {
		return impl->get_glyph(codepoint);
	}

	metrics_glyph font::get_metrics_glyph(char16_t codepoint) {
		return impl->get_metrics_glyph(codepoint);
	}

	float font::render_kerning(char16_t codepoint_first, char16_t codepoint_second) const {
		return impl->kerning(codepoint_first, codepoint_second);
	}

	float font::metrics_kerning(char16_t codepoint_first, char16_t codepoint_second) const {
		return impl->metrics_kerning(codepoint_first, codepoint_second);
	}

	void font::load_font(open_gl_wrapper&) {
		impl->load();
	}

	void font::load_metrics_font() {
		impl->load_metrics();
	}

	float font::metrics_text_extent(const char16_t* codepoints, uint32_t count, float size, bool outlined) const {
		float total = 0.0f;
		for (int32_t i = static_cast<int32_t>(count) - 1; i >= 0; --i) {
			total +=
				impl->get_metrics_glyph(codepoints[i]).advance * size / 64.0f +
				(outlined ? 0.6f : 0.0f) +
				((i != 0) ? metrics_kerning(codepoints[i - 1], codepoints[i]) * size / 64.0f : 0.0f);
		}
		return total;
	}

	float font::line_height(float size) const {
		return impl->line_height * size / 64.0f;
	}

	font::font(const char* filename, font& p) : impl(std::make_unique<_font>(filename, p.impl.get())) {
	}

	font::font(const char* filename) : impl(std::make_unique<_font>(filename, nullptr)) {
	}

	font::font(font&& other) noexcept : impl(std::move(other.impl)) {
	}

	font::~font() {
	}

	using font_map_type = type_list<
		CT_STRING_INT("arial", 1),
		CT_STRING_INT("fps", 1),
		CT_STRING_INT("main", 2),
		CT_STRING_INT("tooltip", 1),
		CT_STRING_INT("frangoth", 2),
		CT_STRING_INT("garmond", 2),
		CT_STRING_INT("impact", 2),
		CT_STRING_INT("old", 2),
		CT_STRING_INT("timefont", 1),
		CT_STRING_INT("vic", 2)>;

	using sorted_font_map_type = typename sorted<font_map_type>::type;

	using font_size_map_type = type_list<
		CT_STRING_INT("fps_font", 14),
		CT_STRING_INT("tooltip_font", 16),
		CT_STRING_INT("frangoth_bold", 18),
		CT_STRING_INT("impact_small", 24),
		CT_STRING_INT("old_english", 50),
		CT_STRING_INT("timefont", 24),
		CT_STRING_INT("vic_title", 42)>;

	using sorted_font_size_map_type = typename sorted<font_size_map_type>::type;

	font_manager::font_manager() {};
	font_manager::~font_manager() {};

	font_tag font_manager::find_font(const char* start, const char* end) {
		const char* effective_end = start;
		while (effective_end != end) {
			if (*effective_end == '_' || isdigit(*effective_end))
				break;
			++effective_end;
		}

		return font_tag(map_functions<sorted_font_map_type>::bt_scan_ci(start, effective_end, 0ui8));
	}

	font const& font_manager::at(font_tag t) const {
		return fonts[t];
	}
	font& font_manager::at(font_tag t) {
		return fonts[t];
	}

	namespace detail {
		uint32_t font_size_adjustment(uint32_t sz);

		uint32_t font_size_adjustment(uint32_t sz) {
			//const int32_t base = (sz * 2 + 2) / 3;
			//return base + (base & 1);
			return std::max(14ui32, sz + (sz & 1));
		}
	}

	uint32_t font_manager::find_font_size(const char* start, const char* end) {
		const uint32_t mapped_size = map_functions<sorted_font_size_map_type>::bt_scan_ci(start, end, 0ui32);
		if (mapped_size != 0)
			return detail::font_size_adjustment(mapped_size);

		const char* first_int = start;
		while (first_int != end) {
			if (isdigit(*first_int))
				break;
			++first_int;
		}
		const char* last_int = first_int;
		while (last_int != end) {
			if (!isdigit(*last_int))
				break;
			++last_int;
		}
		if (first_int == last_int)
			return 14;

		return detail::font_size_adjustment(parse_uint(first_int, last_int));
	}

	bool font_manager::is_black(const char* start, const char* end) {
		if (end - start <= 5)
			return false;
		return compile_time_str_compare_ci<CT_STRING("_bl")>(end - 3, end) == 0 || (compile_time_str_compare_ci<CT_STRING("black")>(end - 5, end) == 0 || ((end - start > 10) && compile_time_str_compare_ci<CT_STRING("black_bold")>(end - 10, end) == 0));
	}

	void font_manager::load_standard_fonts(const directory& root) {
		const char fallback[] = "unifont-9.0.02.ttf";
		const char sans_serif[] = "NotoSans-Bold.ttf";
		const char fancy_font[] = "AndadaSC-Regular.otf";

		const auto full_fn = root.peek_file(fallback, fallback + sizeof(fallback) - 1);
		if (full_fn) {
			const auto ufilename = full_fn->file_path() + u'\\' + full_fn->file_name();
			const std::string afilename(UTF16toUTF8(ufilename));
			fonts.emplace_back(afilename.c_str());
		} else {
#ifdef _DEBUG
			OutputDebugStringA("unable to open font unifont-9.0.02.ttf\n");
#endif
		}

		const auto full_fn_b = root.peek_file(sans_serif, sans_serif + sizeof(sans_serif) - 1);
		if (full_fn_b) {
			const auto ufilename = full_fn_b->file_path() + u'\\' + full_fn_b->file_name();
			const std::string afilename(UTF16toUTF8(ufilename));
			fonts.emplace_back(afilename.c_str(), at(font_tag(0)));
		} else {
#ifdef _DEBUG
			OutputDebugStringA("unable to open font NotoSans-Bold.ttf\n");
#endif
		}

		const auto full_fn_c = root.peek_file(fancy_font, fancy_font + sizeof(fancy_font) - 1);
		if (full_fn_c) {
			const auto ufilename = full_fn_c->file_path() + u'\\' + full_fn_c->file_name();
			const std::string afilename(UTF16toUTF8(ufilename));
			fonts.emplace_back(afilename.c_str(), at(font_tag(0)));
		} else {
#ifdef _DEBUG
			OutputDebugStringA("unable to open font CreteRound-Regular.otf\n");
#endif
		}
	}

	void font_manager::load_fonts(open_gl_wrapper& ogl) {
		for (auto& f : fonts) {
			f.load_font(ogl);
		}
	}
	void font_manager::load_metrics_fonts() {
		for (auto& f : fonts) {
			f.load_metrics_font();
		}
	}
}

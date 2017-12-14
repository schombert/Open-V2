#include "pch.h"
#include "graphics\\test_helpers.h"
#include "fake_fs\\fake_fs.h"

using namespace graphics;

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

TEST(texture_rendering, graphics_tests) {
	EXPECT_TRUE(test_rendering("F:\\VS2007Projects\\open_v2_test_data\\texture", 0, 0, 80, 80, [](open_gl_wrapper& ogl) {
		texture test_tex("F:\\VS2007Projects\\open_v2_test_data\\test_tx.bmp");

		ogl.render_textured_rect(true, 0.0f, 0.0f, 80.0f, 40.0f, test_tex);
		ogl.render_textured_rect(false, 0.0f, 40.0f, 80.0f, 40.0f, test_tex);
	}));
}

TEST(dds_texture_rendering, graphics_tests) {
	EXPECT_TRUE(test_rendering("F:\\VS2007Projects\\open_v2_test_data\\dds", 0, 0, 48, 48, [](open_gl_wrapper& ogl) {
		texture test_tex("F:\\VS2007Projects\\open_v2_test_data\\army_icon_2.dds");

		ogl.render_textured_rect(true, 0.0f, 0.0f, 48.0f, 24.0f, test_tex);
		ogl.render_textured_rect(false, 0.0f, 24.0f, 48.0f, 24.0f, test_tex);
	}));
}

TEST(text_rendering, graphics_tests) {
	EXPECT_TRUE(test_rendering("F:\\VS2007Projects\\open_v2_test_data\\text", 0, 0, 128, 210, [](open_gl_wrapper& ogl) {
		
		font test_fallback("F:\\VS2007Projects\\open_v2_test_data\\unifont-9.0.02.ttf");
		font test_font("F:\\VS2007Projects\\open_v2_test_data\\CreteRound-Regular.otf", test_fallback);

		test_fallback.load_font(ogl);
		test_font.load_font(ogl);
		
		ogl.render_outlined_text(u"明Test", 5, true, 0.0f, 0.0f,                                 16.0f, color{ 0.0f,0.0f,0.0f }, test_font);
		ogl.render_text(         u"明Test", 5, true, 0.0f, test_font.line_height(16.0f),         16.0f, color{ 1.0f,1.0f,1.0f }, test_font);
		ogl.render_outlined_text(u"明Test", 5, false, 0.0f, 2.0f * test_font.line_height(16.0f), 16.0f, color{ 0.0f,0.0f,0.0f }, test_font);
		ogl.render_text(         u"明Test", 5, false, 0.0f, 3.0f * test_font.line_height(16.0f), 16.0f, color{ 1.0f,1.0f,1.0f }, test_font);
		ogl.render_text(u"明Test", 5, true, 0.0f, 4.0f * test_font.line_height(16.0f), 16.0f, color{ 1.0f,0.0f,0.0f }, test_font);
		ogl.render_text(         u"Test",   4, true, 0.0f, 4.0f * test_font.line_height(16.0f), 128.0f, color{ 0.0f,1.0f,1.0f }, test_font);
	}));
}

TEST(progress_bar, graphics_tests) {
	EXPECT_TRUE(test_rendering("F:\\VS2007Projects\\open_v2_test_data\\t_progress", 0, 0, 80, 20, [](open_gl_wrapper& ogl) {
		texture prog1("F:\\VS2007Projects\\open_v2_test_data\\progress1.tga");
		texture prog2("F:\\VS2007Projects\\open_v2_test_data\\progress2.tga");

		ogl.render_progress_bar(true, 0.33f, 0.5f, 0.5f, 80.0f, 20.0f, prog1, prog2);
	}));
}

TEST(piechart, graphics_tests) {
	EXPECT_TRUE(test_rendering("F:\\VS2007Projects\\open_v2_test_data\\t_piechart", 0, 0, 50, 50, [](open_gl_wrapper& ogl) {

		data_texture dt(100, 3);
		const auto ptr = dt.data();
		for (int i = 0; i < 100; ++i) {
			if (i < 25) {
				ptr[i * 3] = 255; ptr[i * 3 + 1] = 0; ptr[i * 3 + 2] = 0;
			} else if (i < 50) {
				ptr[i * 3] = 0; ptr[i * 3 + 1] = 255; ptr[i * 3 + 2] = 0;
			} else {
				ptr[i * 3] = 0; ptr[i * 3 + 1] = 0; ptr[i * 3 + 2] = 255;
			}
		}
		dt.data_ready();

		ogl.render_piechart(true, 0.0f, 0.0f, 50.0f, dt);
	}));
}

TEST(barchart, graphics_tests) {
	EXPECT_TRUE(test_rendering("F:\\VS2007Projects\\open_v2_test_data\\t_barchart", 0, 0, 100, 75, [](open_gl_wrapper& ogl) {
		data_texture dt(100, 4);
		const auto ptr = dt.data();
		for (int i = 0; i < 100; ++i) {
			if (i < 25) {
				ptr[i * 4] = 255; ptr[i * 4 + 1] = 0; ptr[i * 4 + 2] = 0; ptr[i * 4 + 3] = 210;
			} else if (i < 50) {
				ptr[i * 4] = 0; ptr[i * 4 + 1] = 255; ptr[i * 4 + 2] = 0; ptr[i * 4 + 3] = 128;
			} else {
				ptr[i * 4] = 0; ptr[i * 4 + 1] = 0; ptr[i * 4 + 2] = 255; ptr[i * 4 + 3] = 64;
			}
		}
		dt.data_ready();

		ogl.render_barchart(true, 0.0f, 0.0f, 100.0f, 75.0f, dt);
	}));
}

TEST(linegraph, graphics_tests) {
	EXPECT_TRUE(test_rendering("F:\\VS2007Projects\\open_v2_test_data\\t_linegraph", 0, 0, 100, 75, [](open_gl_wrapper& ogl) {
		lines graph(10);
		float yval[] = { 0.3f, 0.6f, 0.5f, 1.0f, 0.4f, 0.5f, 0.0f, 0.3f, 0.2f, 0.6f };
		graph.set_y(yval);
		ogl.render_linegraph(true, 0.0f, 0.0f, 100.0f, 75.0f, graph);
	}));
}

TEST(rotated_mask, graphics_tests) {
	EXPECT_TRUE(test_rendering("F:\\VS2007Projects\\open_v2_test_data\\t_rotated_mask", 0, 0, 40, 80, [](open_gl_wrapper& ogl) {
		texture test_tex("F:\\VS2007Projects\\open_v2_test_data\\test_tx.bmp");
		texture mask_tex("F:\\VS2007Projects\\open_v2_test_data\\mask.tga");

		ogl.render_masked_rect(true, 0.0f, 0.0f, 40.0f, 80.0f, test_tex, mask_tex, rotation::right);
	}));
}

TEST(sprite_fragment, graphics_tests) {
	EXPECT_TRUE(test_rendering("F:\\VS2007Projects\\open_v2_test_data\\t_sprite_fragment", 0, 0, 36, 36, [](open_gl_wrapper& ogl) {
		texture strip_tex("F:\\VS2007Projects\\open_v2_test_data\\strip10.dds");

		ogl.render_subsprite(true, 7, 10, 0.0, 0.0, 36.0, 36.0, strip_tex);
	}));
}

TEST(border_stretch, graphics_tests) {
	EXPECT_TRUE(test_rendering("F:\\VS2007Projects\\open_v2_test_data\\t_border", 0, 0, 60, 50, [](open_gl_wrapper& ogl) {
		texture bord("F:\\VS2007Projects\\open_v2_test_data\\border.dds");

		ogl.render_bordered_rect(true, 32.0f, 0.0f, 0.0f, 350.0f, 150.0f, bord);
	}));
}

class test_file_structure {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation gfx_root = directory_representation(u"gfx", f_root);
	directory_representation mod = directory_representation(u"mod", f_root);
	directory_representation mod_gfx = directory_representation(u"gfx", mod);

	file_representation alt_a_file_b = file_representation(u"file_a.tga", gfx_root, "");
	file_representation a_file_a = file_representation(u"file_b.dds", gfx_root, "");

	file_representation b_file_a = file_representation(u"file_a.tga", mod_gfx, "");

	test_file_structure() {
		set_default_root(f_root);
	}
};


TEST(texture_manager_test, graphics_tests) {
	test_file_structure real_fs;
	file_system f;

	f.set_root(RANGE(u"F:"));

	texture_manager tm_a;
	const auto handle_a = tm_a.retrieve_by_name(f.get_root(), RANGE("gfx\\file_a.tga"));
	EXPECT_EQ(1, handle_a);
	const auto handle_b = tm_a.retrieve_by_name(f.get_root(), RANGE("gfx\\\\file_b.tga"));
	EXPECT_EQ(2, handle_b);

	EXPECT_EQ(std::string("F:\\gfx\\file_a.tga"), tm_a.retrieve_by_key(handle_a).filename);
	EXPECT_EQ(std::string("F:\\gfx\\file_b.dds"), tm_a.retrieve_by_key(handle_b).filename);

	f.add_root(u"F:\\mod");

	texture_manager tm_b;

	const auto handle_c = tm_b.retrieve_by_name(f.get_root(), RANGE("gfx\\\\file_a.tga"));
	EXPECT_EQ(1, handle_c);
	const auto handle_d = tm_b.retrieve_by_name(f.get_root(), RANGE("gfx\\file_b.tga"));
	EXPECT_EQ(2, handle_d);

	EXPECT_EQ(std::string("F:\\mod\\gfx\\file_a.tga"), tm_b.retrieve_by_key(handle_c).filename);
	EXPECT_EQ(std::string("F:\\gfx\\file_b.dds"), tm_b.retrieve_by_key(handle_d).filename);
}
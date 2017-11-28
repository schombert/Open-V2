#include "pch.h"
#include "graphics\\test_helpers.h"

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
	EXPECT_TRUE(test_rendering("F:\\VS2007Projects\\open_v2_test_data\\text", 0, 0, 128, 180, [](open_gl_wrapper& ogl) {
		
		font test_fallback("F:\\VS2007Projects\\open_v2_test_data\\unifont-9.0.02.ttf");
		font test_font("F:\\VS2007Projects\\open_v2_test_data\\CreteRound-Regular.otf", test_fallback);

		test_fallback.load_font(ogl);
		test_font.load_font(ogl);
		
		ogl.render_outlined_text(u"明Test", 5, true, 0.0f, 16.0f, 16.0f, color{ 0.0f,0.0f,0.0f }, test_font);
		ogl.render_text(         u"明Test", 5, true, 0.0f, 32.0f, 16.0f, color{ 1.0f,1.0f,1.0f }, test_font);
		ogl.render_outlined_text(u"明Test", 5, false, 0.0f, 48.0f, 16.0f, color{ 0.0f,0.0f,0.0f }, test_font);
		ogl.render_text(         u"明Test", 5, false, 0.0f, 64.0f, 16.0f, color{ 1.0f,1.0f,1.0f }, test_font);
		ogl.render_text(u"X", 1, true, 0.0f, 180.0f, 128.0f, color{ 0.0f,1.0f,1.0f }, test_font);
	}));
}
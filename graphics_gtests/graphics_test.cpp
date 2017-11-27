#include "pch.h"
#include "graphics\\test_helpers.h"

TEST(texture_rendering, graphics_tests) {
	EXPECT_TRUE(test_rendering("F:\\VS2007Projects\\open_v2_test_data\\texture_test", 0, 0, 80, 80, [](open_gl_wrapper& ogl) {
		texture test_tex("F:\\VS2007Projects\\open_v2_test_data\\test_tx.bmp");

		ogl.render_textured_rect(true, 0.0f, 0.0f, 80.0f, 40.0f, test_tex);
		ogl.render_textured_rect(false, 0.0f, 40.0f, 80.0f, 40.0f, test_tex);
	}));
}
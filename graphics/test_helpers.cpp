#include "test_helpers.h"
#include "v2_window.hpp"
#include "open_gl_wrapper.hpp"
#include "soil\\SOIL.h"

#include <string>
#include <Windows.h>

bool file_exists(const char* szPath) {
	DWORD dwAttrib = GetFileAttributesA(szPath);
	return dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

bool compare_image_files(const char* fna, const char* fnb) {
	int channels_a = 3;
	int width_a = 0;
	int height_a = 0;
	const auto data_a = SOIL_load_image(fna, &width_a, &height_a, &channels_a, SOIL_LOAD_RGB);

	int channels_b = 3;
	int width_b = 0;
	int height_b = 0;
	const auto data_b = SOIL_load_image(fnb, &width_b, &height_b, &channels_b, SOIL_LOAD_RGB);

	bool compare = true;

	if (width_a == width_b && height_a == height_b) {
		for (uint32_t i = 0; i < width_a * height_a * 3; ++i) {
			if (data_b[i] != data_a[i]) {
				compare = false;
				break;
			}
		}
	} else {
		compare = false;
	}

	SOIL_free_image_data(data_a);
	SOIL_free_image_data(data_b);

	return compare;
}

class render_once {
private:
	bool run = false;
	const std::function<void(open_gl_wrapper&)> function;
	const char* file_base = nullptr;
	const uint32_t x = 0;
	const uint32_t y = 0;
	const uint32_t w = 0;
	const uint32_t h = 0;
public:
	render_once(const char* fb, uint32_t xi, uint32_t yi, uint32_t wi, uint32_t hi, const std::function<void(open_gl_wrapper&)>& f) :
		function(f), file_base(fb), x(xi), y(yi), w(wi), h(hi)  {
	}
	template<typename T>
	void operator()(T&&, window_base& w) const {
		// do nothing;
	}
	void operator()(creation&&, window_base& w) const {
		w.close_window();
	}
	void initialize_graphics(open_gl_wrapper& ogl) {
	}

	void render(open_gl_wrapper& ogl) {
		if (!run) {
			function(ogl);

			ogl.display();
			ogl.display();

			std::string test_file(file_base);
			test_file += "_expected.bmp";

			std::string result_file(file_base);
			result_file += "_test.bmp";

			if (!file_exists(test_file.c_str())) {
				SOIL_save_screenshot(test_file.c_str(), SOIL_SAVE_TYPE_BMP, x, y, w, h);
			}
			SOIL_save_screenshot(result_file.c_str(), SOIL_SAVE_TYPE_BMP, x, y, w, h);

			run = true;
		}
	}
};

bool test_rendering(const char* base_file_name, uint32_t x, uint32_t y, uint32_t w, uint32_t h, const std::function<void(open_gl_wrapper&)>& f) {
	{
		window<render_once> test_window(x + w, y + h, base_file_name, x, y, w, h, f);
	}
	std::string test_file(base_file_name);
	test_file += "_expected.bmp";

	std::string result_file(base_file_name);
	result_file += "_test.bmp";

	return compare_image_files(test_file.c_str(), result_file.c_str());
}
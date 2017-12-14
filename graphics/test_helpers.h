#pragma once
#include "open_gl_wrapper.h"

namespace graphics{
	bool test_rendering(const char* base_file_name, uint32_t x, uint32_t y, uint32_t w, uint32_t h, const std::function<void(open_gl_wrapper&)>& f);
}

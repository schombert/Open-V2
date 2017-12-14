#pragma once
#include "open_gl_wrapper.h"
#include "v2_window.h"

namespace graphics {
	template<typename T>
	void open_gl_wrapper::setup(void* hwnd, T& base) {
		set_render_thread([hwnd, &base, _this = this]() {
			_this->setup_context(hwnd);

			base.initialize_graphics(*_this);

			do {
				//if we want to slow down to 30 fps ...
				//const auto start_time = std::chrono::high_resolution_clock::now();

				_this->clear();
				base.render(*_this);
				_this->display();

				//const auto end_time = std::chrono::high_resolution_clock::now();
				//const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
				//if (elapsed < (1000 / 30)) {
				//	Sleep((1000 / 30) - elapsed);
				//}
			} while (_this->is_running());
		});
	}
}

#include "v2_window.hpp"

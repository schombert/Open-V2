#pragma once
#include "v2_window.h"
#include <mutex>
#include <condition_variable>
#include <variant>

template<typename HANDLER>
long* __stdcall templated_win_proc(void* hwnd, unsigned int uMsg, unsigned int* wParam, long* lParam) {
	const auto message_result = yield_message(hwnd, uMsg, wParam, lParam);
	if (std::holds_alternative<int64_t>(message_result)) {
		return (long*)(std::get<int64_t>(message_result));
	} else {
		std::visit(HANDLER(), message_result);
		return 0;
	}
}

void generic_setup(long* (__stdcall *win_proc)(void*, unsigned int, unsigned int*, long*), uint32_t xsize, uint32_t ysize, window_base& container);

template<typename HANDLER>
window<HANDLER>::window() {
	generic_setup(templated_win_proc<HANDLER>, 0, 0, *this);
}

template<typename HANDLER>
window<HANDLER>::window(uint32_t x, uint32_t y) {
	generic_setup(templated_win_proc<HANDLER>, x, y, *this);
}

template<typename HANDLER>
window<HANDLER>::~window() {

}
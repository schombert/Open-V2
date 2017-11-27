#pragma once
#include "v2_window.h"
#include <mutex>
#include <condition_variable>
#include <variant>
#include "open_gl_wrapper.h"

void* get_handler(void* _hwnd);

template<typename HANDLER>
long* __stdcall templated_win_proc(void* hwnd, unsigned int uMsg, unsigned int* wParam, long* lParam) {
	const auto message_result = yield_message(hwnd, uMsg, wParam, lParam);
	if (std::holds_alternative<int64_t>(message_result)) {
		return (long*)(std::get<int64_t>(message_result));
	} else if (std::holds_alternative<creation>(message_result)) {
		window<HANDLER>* obj = (window<HANDLER>*)get_handler(hwnd);
		obj->gl_wrapper.setup(hwnd, obj->h);
		obj->h(creation{}, *obj);
		return 0;
	} else {
		window<HANDLER>* obj = (window<HANDLER>*)get_handler(hwnd);
		if(obj)
			std::visit([obj](auto&& o) { obj->h(std::forward<decltype(o)>(o), *obj); }, message_result);
		return 0;
	}
}

template<typename HANDLER>
template<typename ...T, typename>
window<HANDLER>::window(T&& ...args) : h(std::forward<T>(args)...) {
	generic_setup(templated_win_proc<HANDLER>, 0, 0);
}

template<typename HANDLER>
template<typename ...T>
window<HANDLER>::window(uint32_t x, uint32_t y, T&& ...args) :  h(std::forward<T>(args)...) {
	generic_setup(templated_win_proc<HANDLER>, x, y);
}

template<typename HANDLER>
window<HANDLER>::~window() {
	if (message_thread.joinable())
		message_thread.join();
}

#include "open_gl_wrapper.hpp"
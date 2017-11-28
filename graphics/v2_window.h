#pragma once
#include <cstdint>
#include <memory>
#include <variant>
#include <thread>

#include "common\\common.h"
#include "open_gl_wrapper.h"

enum modifiers {
	modifiers_none = 0x0,
	modifiers_alt = 0x4,
	modifiers_ctrl = 0x2,
	modifiers_shift = 0x1,
	modifiers_ctrl_shift = 0x3,
	modifiers_ctrl_alt = 0x6,
	modifiers_alt_shift = 0x5,
	modifiers_all = 0x7
};

struct rbutton_down {
	int x;
	int y;
	modifiers mod;
};

struct mouse_move {
	int x;
	int y;
	modifiers mod;
};

struct mouse_drag {
	int x;
	int y;
	modifiers mod;
};

struct rbutton_up {
	int x;
	int y;
	modifiers mod;
};

struct lbutton_down {
	int x;
	int y;
	modifiers mod;
};

struct resize {
	uint32_t width;
	uint32_t height;
};

struct lbutton_up {
	int x;
	int y;
	modifiers mod;
};

struct scroll {
	int x;
	int y;
	float amount;
	modifiers mod;
};

struct key_down {
	virtual_key keycode;
	modifiers mod;
};

struct key_up {
	virtual_key keycode;
	modifiers mod;
};

struct text_event {
	char16_t text;
};

struct creation {};

using message_variant = std::variant<int64_t, rbutton_down, rbutton_up, lbutton_down, lbutton_up, resize, scroll, key_down, key_up, text_event, mouse_move, mouse_drag, creation>;

message_variant yield_message(void* _hwnd, unsigned int uMsg, unsigned int* _wParam, long* _lParam);

class window_base;

template<typename HANDLER>
long* __stdcall templated_win_proc(void* hwnd, unsigned int uMsg, unsigned int* wParam, long* lParam);


class window_base {
protected:
	void* handle = nullptr;
	open_gl_wrapper gl_wrapper;
	std::thread message_thread;
public:
	window_base();
	~window_base();

	void generic_setup(long* (__stdcall *win_proc)(void*, unsigned int, unsigned int*, long*), uint32_t xsize, uint32_t ysize);
	void close_window();

	friend message_variant yield_message(void* _hwnd, unsigned int uMsg, unsigned int* _wParam, long* _lParam);
	template<typename T>
	friend long* __stdcall templated_win_proc(void* hwnd, unsigned int uMsg, unsigned int* wParam, long* lParam);
};


template<typename HANDLER>
class window : public window_base {
public:
	HANDLER h;

	template<typename ...T, typename = std::enable_if_t<std::is_constructible_v<HANDLER, T...>>>
	window(T&& ... args);
	template<typename ...T>
	window(uint32_t x, uint32_t y, T&& ... args);
	~window();
};
#pragma once
#include <cstdint>
#include <memory>
#include <variant>
#include <thread>
#include <type_traits>

#include "common\\common.h"
#include "open_gl_wrapper.h"

#define WM_GRAPHNOTIFY  (WM_APP + 1)

namespace ui {
	struct message_with_location {
		int32_t x;
		int32_t y;
	};

	struct rbutton_down : public message_with_location {
		key_modifiers mod;
	};

	struct mouse_move : public message_with_location {
		key_modifiers mod;
	};

	struct mouse_drag {
		int32_t x;
		int32_t y;
		key_modifiers mod;
	};

	struct rbutton_up : public message_with_location {
		key_modifiers mod;
	};

	struct lbutton_down : public message_with_location {
		key_modifiers mod;
	};

	struct resize {
		uint32_t width;
		uint32_t height;
	};

	struct lbutton_up : public message_with_location {
		key_modifiers mod;
	};

	struct scroll : public message_with_location {
		float amount;
		key_modifiers mod;
	};

	struct key_down {
		virtual_key keycode;
		key_modifiers mod;
	};

	struct key_up {
		virtual_key keycode;
		key_modifiers mod;
	};

	struct text_event {
		char16_t text;
	};

	struct music_finished {};
	struct creation {};
	struct idle {};

	template<typename T>
	T adjust_message_location(const T& message, int32_t delta_x, int32_t delta_y) {
		if constexpr(std::is_base_of_v<message_with_location, T>) {
			T new_message = message;
			new_message.x += delta_x;
			new_message.y += delta_y;
			return new_message;
		} else {
			return message;
		}
	}

	template<typename T>
	T rescale_message(const T& message, float scale) {
		if constexpr(std::is_base_of_v<message_with_location, T>) {
			T new_message = message;
			new_message.x  = static_cast<decltype(new_message.x)>(float(new_message.x) / scale);
			new_message.y = static_cast<decltype(new_message.y)>(float(new_message.y) / scale);
			return new_message;
		} else {
			return message;
		}
	}

	template<typename T>
	bool message_within_bounds(const T& message, int32_t x_max, int32_t y_max) {
		if constexpr(std::is_base_of_v<message_with_location, T>)
			return (message.x <= x_max) & (message.y <= y_max) & (message.x >= 0) & (message.y >= 0);
		else
			return true;
	}

	using message_variant = std::variant<int64_t, rbutton_down, rbutton_up, lbutton_down, lbutton_up, resize, scroll, key_down, key_up, text_event, mouse_move, mouse_drag, creation, idle, music_finished>;

	message_variant yield_message(void* _hwnd, unsigned int uMsg, unsigned int* _wParam, long* _lParam);

	class window_base;

	template<typename HANDLER>
	long* __stdcall templated_win_proc(void* hwnd, unsigned int uMsg, unsigned int* wParam, long* lParam);


	class window_base {
	protected:
		void* handle = nullptr;
		graphics::open_gl_wrapper gl_wrapper;
		std::thread message_thread;
		bool topmost;
	public:
		window_base(bool t);
		~window_base();

		void generic_setup(long* (__stdcall *win_proc)(void*, unsigned int, unsigned int*, long*), uint32_t xsize, uint32_t ysize);
		void close_window();
		void* get_handle() const { return handle; }
		void maximize();
		void make_fullscreen();
		void remove_fullscreen();

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
}

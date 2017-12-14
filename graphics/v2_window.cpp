#include "v2_window.hpp"
#include <Windows.h>
#include <Windowsx.h>

namespace ui {
	window_base::window_base() {}

	window_base::~window_base() {}

	void window_base::close_window() {
		const HWND hwnd = (HWND)handle;
		SendMessage(hwnd, WM_CLOSE, 0, 0);
	}

	modifiers get_current_modifiers() {
		return (modifiers)(((GetKeyState(VK_CONTROL) & 0x8000) ? modifiers_ctrl : modifiers_none) |
			((GetKeyState(VK_MENU) & 0x8000) ? modifiers_alt : modifiers_none) |
			((GetKeyState(VK_SHIFT) & 0x8000) ? modifiers_shift : modifiers_none));
	}

	void* get_handler(void* _hwnd) {
		return (void*)GetWindowLongPtr((HWND)_hwnd, GWLP_USERDATA);
	}

	message_variant yield_message(void* _hwnd, unsigned int uMsg, unsigned int* _wParam, long* _lParam) {
		const HWND hwnd = (HWND)_hwnd;
		const WPARAM wParam = (WPARAM)_wParam;
		const LPARAM lParam = (LPARAM)_lParam;

		static int drag_x_start = 0;
		static int drag_y_start = 0;

		window_base* winbase = (window_base*)GetWindowLongPtr((HWND)_hwnd, GWLP_USERDATA);

		switch (uMsg) {
			case WM_CLOSE:
				winbase->gl_wrapper.destory(_hwnd);
				DestroyWindow(hwnd);
				SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)nullptr);
				return int64_t(0);
			case WM_DESTROY:
				PostQuitMessage(0);
				return int64_t(0);
			case WM_CREATE:
			{
				CREATESTRUCT* cptr = (CREATESTRUCT*)lParam;
				window_base* base = (window_base*)(cptr->lpCreateParams);
				base->handle = (void*)(hwnd);

				//base->gl_wrapper.setup(_hwnd, base);

				RECT crect;
				GetClientRect(hwnd, &crect);
				base->gl_wrapper.set_viewport(uint32_t(crect.right), uint32_t(crect.bottom));

				SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)base);
				return creation{};
			}
			case WM_SETFOCUS:
			{
				HDC screenDC = GetDC(NULL);
				SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOREDRAW | SWP_NOSIZE | SWP_NOMOVE);
				ReleaseDC(NULL, screenDC);
			}
			return int64_t(0);
			case WM_KILLFOCUS:
				SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOREDRAW | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
				return int64_t(0);
			case WM_ERASEBKGND:
				return int64_t(1);
			case WM_LBUTTONDOWN:
				SetCapture(hwnd);
				drag_x_start = GET_X_LPARAM(lParam);
				drag_y_start = GET_Y_LPARAM(lParam);
				return lbutton_down{ { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }, get_current_modifiers() };
			case WM_LBUTTONUP:
				ReleaseCapture();
				return lbutton_up{ { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }, get_current_modifiers() };
			case WM_MOUSEMOVE:
				if (wParam & MK_LBUTTON)
					return mouse_drag{ GET_X_LPARAM(lParam) - drag_x_start, GET_Y_LPARAM(lParam) - drag_y_start, get_current_modifiers() };
				else
					return mouse_move{ { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }, get_current_modifiers() };
			case WM_RBUTTONDOWN:
				return rbutton_down{ { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }, get_current_modifiers() };
			case WM_RBUTTONUP:
				return rbutton_up{ { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }, get_current_modifiers() };
			case WM_SIZING:
			{
				RECT* window_rect = (RECT*)lParam;
				RECT zero_rect = { 0, 0, 0, 0 };
				AdjustWindowRectEx(&zero_rect, GetWindowLong(hwnd, GWL_STYLE), false, GetWindowLong(hwnd, GWL_EXSTYLE));

				if (winbase)
					winbase->gl_wrapper.set_viewport(
						uint32_t((window_rect->right - window_rect->left) - (zero_rect.right - zero_rect.left)),
						uint32_t((window_rect->bottom - window_rect->top) - (zero_rect.bottom - zero_rect.top)));

				return int64_t(0);
			}
			case WM_SIZE:
				if (winbase)
					winbase->gl_wrapper.set_viewport(uint32_t(LOWORD(lParam)), uint32_t(HIWORD(lParam)));
				return resize{ uint32_t(LOWORD(lParam)), uint32_t(HIWORD(lParam)) };
			case WM_MOUSEWHEEL:
				return scroll{ { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }, (float)(GET_WHEEL_DELTA_WPARAM(wParam)) / 120.0, get_current_modifiers() };
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				return key_down{ virtual_key(wParam), get_current_modifiers() };
			case WM_SYSKEYUP:
			case WM_KEYUP:
				return key_up{ virtual_key(wParam), get_current_modifiers() };
			case WM_CHAR:
				return text_event{ char16_t(wParam) };
			case WM_DPICHANGED:
				return int64_t(0);
		}
		return int64_t(DefWindowProc(hwnd, uMsg, wParam, lParam));
	}

	void windowing_thread(long* (__stdcall *win_proc)(void*, unsigned int, unsigned int*, long*), uint32_t xsize, uint32_t ysize, window_base & container) {

		HINSTANCE shCoreDll = LoadLibrary(L"Shcore.dll");
		bool SetProcessDpiAwareness_ok = false;

		enum ProcessDpiAwareness {
			ProcessDpiUnaware = 0,
			ProcessSystemDpiAware = 1,
			ProcessPerMonitorDpiAware = 2
		};

		typedef HRESULT(WINAPI* SetProcessDpiAwarenessFuncType)(ProcessDpiAwareness);

		if (shCoreDll) {
			SetProcessDpiAwarenessFuncType SetProcessDpiAwarenessFunc = reinterpret_cast<SetProcessDpiAwarenessFuncType>(GetProcAddress(shCoreDll, "SetProcessDpiAwareness"));
			if (SetProcessDpiAwarenessFunc) {
				if (SetProcessDpiAwarenessFunc(ProcessPerMonitorDpiAware) != E_INVALIDARG) {
					SetProcessDpiAwareness_ok = true;
				}
			}
			FreeLibrary(shCoreDll);
		}

		if (!SetProcessDpiAwareness_ok) {
			SetProcessDPIAware();
		}

		WNDCLASSEX windowClass;
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_OWNDC;
		windowClass.lpfnWndProc = (WNDPROC)win_proc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = sizeof(void*);
		windowClass.hInstance = GetModuleHandleW(NULL);
		windowClass.hIcon = NULL;
		windowClass.hCursor = 0;
		windowClass.hbrBackground = 0;
		windowClass.lpszMenuName = NULL;
		windowClass.hIconSm = NULL;
		windowClass.lpszClassName = L"Open V2 Window";
		RegisterClassEx(&windowClass);


		HDC screenDC = GetDC(NULL);
		int left = xsize != 0 ? GetDeviceCaps(screenDC, HORZRES) / 2 - static_cast<int>(xsize / 2) : 0;
		int top = ysize != 0 ? GetDeviceCaps(screenDC, VERTRES) / 2 - static_cast<int>(ysize / 2) : 0;
		int width = xsize != 0 ? xsize : GetDeviceCaps(screenDC, HORZRES);
		int height = ysize != 0 ? ysize : GetDeviceCaps(screenDC, VERTRES);
		ReleaseDC(NULL, screenDC);

		DWORD win32Style = xsize != 0 ?
			WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS :
			WS_VISIBLE | WS_BORDER | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

		if (xsize != 0) {
			RECT rectangle = { 0, 0, width, height };
			AdjustWindowRectEx(&rectangle, win32Style, false, xsize != 0 ? 0 : WS_EX_TOPMOST);
			width = rectangle.right - rectangle.left;
			height = rectangle.bottom - rectangle.top;
		}

		HWND handle = CreateWindowEx(
			xsize != 0 ? 0 : WS_EX_TOPMOST,
			L"Open V2 Window",
			L"Open V2",
			win32Style,
			left, top, width, height,
			NULL, NULL, GetModuleHandle(NULL), &container
		);

		ShowWindow(handle, SW_SHOW);
		UpdateWindow(handle);

		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


	void window_base::generic_setup(long* (__stdcall *win_proc)(void*, unsigned int, unsigned int*, long*), uint32_t xsize, uint32_t ysize) {
		message_thread = std::thread([win_proc, xsize, ysize, _this = this]() {
			windowing_thread(win_proc, xsize, ysize, *_this);
		});
	}
}

#include "v2_window.hpp"
#include <Windows.h>
#include <Windowsx.h>

window_base::window_base() {}

window_base::~window_base() {
	const HWND hwnd = (HWND)handle;
	SendMessage(hwnd, WM_CLOSE, 0, 0);
}

#ifdef VK_TAB
#endif

modifiers get_current_modifiers() {
	return (modifiers) ( ((GetKeyState(VK_CONTROL) & 0x8000) ? modifiers_ctrl : modifiers_none) |
		((GetKeyState(VK_MENU) & 0x8000) ? modifiers_alt : modifiers_none) |
		((GetKeyState(VK_SHIFT) & 0x8000) ? modifiers_shift : modifiers_none) );
}

message_variant yield_message(void* _hwnd, unsigned int uMsg, unsigned int* _wParam, long* _lParam) {
	const HWND hwnd = (HWND)_hwnd;
	const WPARAM wParam = (WPARAM)_wParam;
	const LPARAM lParam = (LPARAM)_lParam;

	switch (uMsg) {
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_CREATE:
		{
			CREATESTRUCT* cptr = (CREATESTRUCT*)lParam;
			window_base* base = (window_base*)(cptr->lpCreateParams);
			base->handle = (void*)(hwnd);
			return int64_t(0);
		}
		case WM_SETFOCUS:
			SetWindowPos(hwnd, HWND_TOPMOST, 0,0,0,0, SWP_NOREDRAW | SWP_NOMOVE | SWP_NOSIZE);
			return int64_t(0);
		case WM_KILLFOCUS:
			SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOREDRAW | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
			return int64_t(0);
		case WM_LBUTTONDOWN:
			SetCapture(hwnd);
			return lbutton_down{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), get_current_modifiers() };
		case WM_LBUTTONUP:
			ReleaseCapture();
			return lbutton_up{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), get_current_modifiers() };
		case WM_MOUSEMOVE:
			if (wParam & MK_LBUTTON)
				return mouse_drag{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), get_current_modifiers() };
			else
				return mouse_move{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), get_current_modifiers() };
		case WM_RBUTTONDOWN:
			return rbutton_down{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), get_current_modifiers() };
		case WM_RBUTTONUP:
			return rbutton_up{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), get_current_modifiers() };
		case WM_SIZE:
			return resize{ uint32_t(LOWORD(lParam)), uint32_t(HIWORD(lParam)) };
		case WM_MOUSEWHEEL:
			return scroll{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (float)(GET_WHEEL_DELTA_WPARAM(wParam)) / 120.0, get_current_modifiers() };
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			return key_down{ virtual_key(wParam), get_current_modifiers() };
		case WM_SYSKEYUP:
		case WM_KEYUP:
			return key_up{ virtual_key(wParam), get_current_modifiers() };
		case WM_CHAR:
			return text_event{char16_t(wParam)};
		case WM_DPICHANGED:
			return int64_t(0);
	}
	return int64_t(DefWindowProc(hwnd, uMsg, wParam, lParam));
}

inline void windowing_thread(long* (__stdcall *win_proc)(void*, unsigned int, unsigned int*, long*), uint32_t xsize, uint32_t ysize, window_base & container, bool & construction_completed, std::mutex & condition_mutex, std::condition_variable & wakeup) {

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

	WNDCLASSW windowClass;
	windowClass.style = 0;
	windowClass.lpfnWndProc = (WNDPROC)win_proc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleW(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = L"Open V2 Window";
	RegisterClassW(&windowClass);


	HDC screenDC = GetDC(NULL);
	int left = xsize != 0 ? GetDeviceCaps(screenDC, HORZRES) / 2 - static_cast<int>(xsize / 2) : 0;
	int top = ysize != 0 ? GetDeviceCaps(screenDC, VERTRES) / 2 - static_cast<int>(ysize / 2) : 0;
	int width = xsize != 0 ? xsize : GetDeviceCaps(screenDC, HORZRES);
	int height = ysize != 0 ? ysize : GetDeviceCaps(screenDC, VERTRES);
	ReleaseDC(NULL, screenDC);

	DWORD win32Style = xsize != 0 ?
		WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_SYSMENU :
		WS_VISIBLE | WS_POPUP | WS_BORDER;

	if (xsize != 0) {
		RECT rectangle = { 0, 0, width, height };
		AdjustWindowRect(&rectangle, win32Style, false);
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
	//CreateWindowW(L"Open V2 Window", L"Open V2", win32Style, left, top, width, height, NULL, NULL, GetModuleHandle(NULL), &container);

	{
		std::unique_lock<std::mutex> lock_to_condition(condition_mutex);
		construction_completed = true;
	}
	wakeup.notify_all();

	ShowWindow(handle, SW_SHOW);
	UpdateWindow(handle);

	// Start the message loop. 

	BOOL mreturn;
	MSG msg;

	while ((mreturn = GetMessage(&msg, handle, 0, 0)) != 0) {
		if (mreturn == -1) {
			// handle the error and possibly exit
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}


void generic_setup(long* (__stdcall *win_proc)(void*, unsigned int, unsigned int*, long*), uint32_t xsize, uint32_t ysize, window_base& container) {
	bool construction_completed = false;
	std::mutex condition_mutex;
	std::condition_variable wakeup;

	std::thread win_creation([win_proc, xsize, ysize, &container, &construction_completed, &condition_mutex, &wakeup]() {
		windowing_thread(win_proc, xsize, ysize, container, construction_completed, condition_mutex, wakeup);
	});

	std::unique_lock<std::mutex> lock_to_condition(condition_mutex);
	wakeup.wait(lock_to_condition, [&construction_completed]() { return construction_completed; });

	win_creation.detach();
}
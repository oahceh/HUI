/**
 * @file    native_window.cpp
 * @brief   基础的窗口类封装
 * @author  ksdjfdf <330773102@qq.com>
 * @date	2016-04-28
 */

#include "base.h"
#include <Windows.h>
#include <windowsx.h>
#include "MACROS.h"

namespace HUI {

UIWindowWnd::UIWindowWnd() : hwnd_(NULL),
	                         bMainWindow_(FALSE),
	                         old_window_proc_(::DefWindowProc) {} // DefWindowProc: Default window proc

BOOL UIWindowWnd::RegisterWindowClass() {
	WNDCLASSEX wcx = {0};
	wcx.cbSize = sizeof(WNDCLASSEX); 
	wcx.style = CS_HREDRAW | CS_VREDRAW;						// Redraw 
	wcx.lpfnWndProc = UIWindowWnd::_WndProc;		// specify window proc 
	wcx.cbClsExtra = 0;											// no extra class memory 
	wcx.cbWndExtra = 0;											// no extra window memory 
	wcx.hInstance = NULL;
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// predefined app. icon 
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);					// predefined arrow 
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// white background brush 
	wcx.lpszMenuName = NULL;									// name of menu resource 
	wcx.lpszClassName = GetWindowClassName();					// name of window class
	wcx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);				// small class icon
	return RegisterClassEx(&wcx) != 0 || 
		::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

BOOL UIWindowWnd::RegisterSuperClass() {
	WNDCLASSEX wc = {0};
	wc.cbSize = sizeof(WNDCLASSEX);
	if (!::GetClassInfoEx(NULL, GetSuperClassName(), &wc)) {
		if (!::GetClassInfoEx(NULL, GetSuperClassName(), &wc)) {
			return FALSE;
		}
	}
	old_window_proc_ = wc.lpfnWndProc;
	wc.lpfnWndProc = UIWindowWnd::_WndProc;
	wc.hInstance = NULL;
	wc.lpszClassName = GetWindowClassName();
	ATOM result = ::RegisterClassEx(&wc);
	return result != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

BOOL UIWindowWnd::Create(HWND hwndParent, LPCTSTR pStrName, DWORD dwStyle, DWORD dwExStyle, int x, int y, int cx, int cy, HMENU hMenu) {
	BOOL bCreated = FALSE;
	if (GetSuperClassName() != NULL) {
		if (!RegisterSuperClass()) {
			return FALSE;
		}
	} else {
		if (!RegisterWindowClass()) {
			return FALSE;
		}
	}
	hwnd_ = ::CreateWindowEx(dwExStyle, GetWindowClassName(), pStrName, dwStyle, x, y, cx, cy, hwndParent, hMenu, NULL, this);
	if (hwnd_ != NULL) { bCreated = AfterCreated(); }
	return bCreated;
}

// Invoke Create method above
BOOL UIWindowWnd::Create(HWND hwndParent, LPCTSTR pStrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu) {
	return Create(hwndParent, pStrName, dwStyle, dwExStyle, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hMenu);
}

void UIWindowWnd::ShowWindow(BOOL show, BOOL focused) {
	assert(::IsWindow(hwnd_));
	if (!::IsWindow(hwnd_)) { return; }
	::ShowWindow(hwnd_, show ? (focused ? SW_SHOWNORMAL : SW_SHOWNOACTIVATE) : SW_HIDE);
}

void UIWindowWnd::CenterWindow() {
	assert(::IsWindow(hwnd_));
	assert((GetWindowStyle(hwnd_) & WS_CHILD)==0);
	RECT window_rect = { 0 };
	::GetWindowRect(hwnd_, &window_rect);
	RECT area_rect = { 0 };
	RECT center_rect = { 0 };
	HWND hWndParent = ::GetParent(hwnd_);
	HWND hWndCenter = ::GetWindowOwner(hwnd_);
	::SystemParametersInfo(SPI_GETWORKAREA, NULL, &area_rect, NULL);
	if( hWndCenter == NULL ) { center_rect = area_rect; } 
	else { ::GetWindowRect(hWndCenter, &center_rect); }

	int width = window_rect.right - window_rect.left;
	int height = window_rect.bottom - window_rect.top;

	int left_x = (center_rect.left + center_rect.right) / 2 - width / 2;
	int top_y = (center_rect.top + center_rect.bottom) / 2 - height / 2;

	if( left_x < area_rect.left ) { 
		left_x = area_rect.left; 
	}
	else if( left_x + width > area_rect.right ) { 
		left_x = area_rect.right - width; 
	}
	if( top_y < area_rect.top ) { 
		top_y = area_rect.top; 
	}
	else if( top_y + height > area_rect.bottom ) { 
		top_y = area_rect.bottom - height; 
	}
	::SetWindowPos(hwnd_, NULL, left_x, top_y, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void UIWindowWnd::CloseWindow() {
	assert(::IsWindow(hwnd_));
	::PostMessage(hwnd_, WM_CLOSE, 0, 0);
}

void UIWindowWnd::SetIcon(const wchar_t *icon_path) {
	assert(::IsWindow(hwnd_));
	PTR_VOID(icon_path);
	HICON icon = (HICON)LoadImage(NULL, icon_path, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	SetIcon(icon);
}

void UIWindowWnd::SetIcon(int resource_id) {
	assert(::IsWindow(hwnd_));
	LPCWSTR A = MAKEINTRESOURCE(resource_id);
	HICON icon = (HICON)::LoadImage(NULL, MAKEINTRESOURCE(resource_id), IMAGE_ICON,
		::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(icon);
}

void UIWindowWnd::SetIcon(HICON icon) {
	assert(::IsWindow(hwnd_));
	PTR_VOID(icon);
	::SendMessage(hwnd_, WM_SETICON, (WPARAM)TRUE, (LPARAM)icon);
	::SendMessage(hwnd_, WM_SETICON, (WPARAM)FALSE, (LPARAM)icon);
}

void UIWindowWnd::GetWindowText(std::wstring &text) {
	assert(::IsWindow(hwnd_));
	text.empty();
	int length = ::GetWindowTextLength(hwnd_) + 1;
	wchar_t *temp = static_cast<wchar_t*>(_alloca(length * sizeof(wchar_t)));
	::GetWindowText(hwnd_, temp, length); // Invoke windows API
	text = temp;
}

void UIWindowWnd::SetTimer(UINT id, UINT elapse, TIMERPROC proc) {
	assert(::IsWindow(hwnd_));
	::SetTimer(hwnd_, id, elapse, proc);
}

void UIWindowWnd::KillTimer(UINT id) {
	assert(::IsWindow(hwnd_));
	::KillTimer(hwnd_, id);
}

BOOL UIWindowWnd::PostMessage(UINT message, WPARAM wParam, LPARAM lParam) {
	assert(::IsWindow(hwnd_));
	return ::PostMessage(hwnd_, message, wParam, lParam);
}

LRESULT UIWindowWnd::SendMessage(UINT message, WPARAM wParam, LPARAM lParam) {
	assert(::IsWindow(hwnd_));
	return ::SendMessage(hwnd_, message, wParam, lParam);
}

LRESULT UIWindowWnd::_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (message == WM_NCCREATE) {
		CREATESTRUCT *lpcs = (CREATESTRUCT*)lParam;
		UIWindowWnd *window = (UIWindowWnd*)lpcs->lpCreateParams;
		window->hwnd_ = hwnd;
		SetWindowLong(hwnd, GWL_USERDATA, (LPARAM)window);
	} else if (message == WM_NCDESTROY) {
		UIWindowWnd *window = (UIWindowWnd*)GetWindowLong(hwnd, GWL_USERDATA);
		if (window) {
			LRESULT result = ::CallWindowProc(window->old_window_proc_, hwnd, message, wParam, lParam);
			::SetWindowLongPtr(window->GetHWND(), GWLP_USERDATA, 0L);
			window->OnDestroy();
			return result;
		}
	}
	UIWindowWnd *window = (UIWindowWnd*)GetWindowLong(hwnd, GWL_USERDATA);
	if (window) { 
		return window->HandleMessages(message, wParam, lParam); 
	} else { 
		return ::DefWindowProc(hwnd, message, wParam, lParam);
	}
}

LRESULT UIWindowWnd::HandleMessages(UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_ERASEBKGND:
		return OnEraseBkgnd();
	case WM_TIMER:
		{
			OnTimer((UINT)wParam);
		}
		break;
	case WM_SIZE:
		{
			int cx = LOWORD(lParam);
			int cy = HIWORD(lParam);
			OnSize((UINT)wParam, cx, cy);
		}
		break;
	case WM_GETMINMAXINFO:
		{
			OnGetMinMaxInfo((MINMAXINFO*)lParam);
		}
		break;
	default:
		break;
	}
	return ::CallWindowProc(old_window_proc_, hwnd_, message, wParam, lParam);
}

BOOL UIWindowWnd::AfterCreated() { return TRUE; }

BOOL UIWindowWnd::OnEraseBkgnd() { return TRUE; }

void UIWindowWnd::OnTimer(UINT id) {}

void UIWindowWnd::OnSize(UINT type,int cx,int cy) {
	if (::GetFocus() != hwnd_) {
		::SetFocus(hwnd_);
	}
}

void UIWindowWnd::OnGetMinMaxInfo(MINMAXINFO* lpMMI) {}

void UIWindowWnd::OnDestroy() {
	if (bMainWindow_) {
		::PostQuitMessage(0);
	}
}

} // namespace HUI
#include "window.h"
#include "MACROS.h"
#include "render.h"
using namespace HUI::Utils;
namespace HUI {

UIWindow::UIWindow() : mb_closing(FALSE),
	                   mb_resize_needed(TRUE),
	                   mb_mouse_in(FALSE),
	                   mp_focused_control(NULL),
	                   mp_root_control(NULL) {}

UIWindow::~UIWindow() { delete mp_root_control; }

LRESULT UIWindow::HandleMessages(UINT message, WPARAM wParam, LPARAM lParam) {
	if (mb_closing) {
		return __super::HandleMessages(message, wParam, lParam);
	}
	LRESULT result = 0L;
	switch (message) {
	case WM_PAINT:
		HandleWMPaintMessage();
		result = TRUE;
		break;
	case WM_CLOSE:
		mb_closing = TRUE;
		break;
	// mouse message
	case WM_MOUSELEAVE:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_MOUSEWHEEL:
		{
			HandleMouseMessage(message, wParam, lParam);
		}
		break;
	default:
		break;
	}
	// If message has not been handled, then invoke HandleMessages of base
	return result ? result : __super::HandleMessages(message, wParam, lParam);
}

void UIWindow::SetRootControl(UIControl *pControl) {
	PTR_VOID(pControl);
	if (mp_root_control != NULL) { delete mp_root_control; }
	mp_root_control = pControl;
	mp_root_control->SetParentWnd(this);
}

void UIWindow::UpdateLayout() {
	mb_resize_needed = TRUE;
	::InvalidateRect(hwnd_, NULL, FALSE); // redraw without erasing background????
}

void UIWindow::Invalidate(const UIRect *rect) {
	::InvalidateRect(hwnd_, rect, FALSE);
}

void UIWindow::SetWindowMinSize(const UISize &size) { min_size = size; }
void UIWindow::SetWindowMinSize(int width,int height) { SetWindowMinSize(UISize(width,height)); }

void UIWindow::SetWindowMaxSize(const UISize &size) { max_size = size; }
void UIWindow::SetWindowMaxSize(int width,int height) { SetWindowMaxSize(UISize(width,height)); }

void UIWindow::AddNotifier(IUINotify *notifier) { m_notifiers.push_back(notifier); }

void UIWindow::RaiseEvent(UINotifyEvent event, UIControl *pControl) {
	for (auto var : m_notifiers) {
		var->Notify(event, pControl);
	}
}

void UIWindow::OnSize(UINT type, int cx, int cy) {
	__super::OnSize(type, cx, cy);
	DoLayout();
}

void UIWindow::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	HMONITOR hmonitor = MonitorFromWindow(hwnd_, MONITOR_DEFAULTTONEAREST);
	MONITORINFO minfo;
	minfo.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(hmonitor, &minfo);
	UIPoint offset(1,1);

	lpMMI->ptMaxPosition.x = minfo.rcWork.left - minfo.rcMonitor.left - offset.x;
	lpMMI->ptMaxPosition.y = minfo.rcWork.top - minfo.rcMonitor.top - offset.y;

	if (max_size.cx != 0 && max_size.cy != 0) {
		lpMMI->ptMaxSize.x = max_size.cx + offset.x * 2;
		lpMMI->ptMaxSize.y = max_size.cy + offset.y * 2;
		lpMMI->ptMaxTrackSize.x = lpMMI->ptMaxSize.x;
		lpMMI->ptMaxTrackSize.y = lpMMI->ptMaxSize.y;
	}

	if (min_size.cx != 0 && min_size.cy != 0) {
		lpMMI->ptMinTrackSize.x = min_size.cx + offset.x * 2;
		lpMMI->ptMinTrackSize.y = min_size.cy + offset.y * 2;
	}
}

/********************** private methods ****************************/
void UIWindow::Render(const UIRect &rect) {
	if (mp_root_control) {
		mp_root_control->Render(&rect);
	}
}

void UIWindow::DoLayout() {
	if (mp_root_control) {
		RECT rect;
		::GetClientRect(hwnd_, &rect);
		mp_root_control->DoLayout(UIRect(rect));
	}
}

// Process WM_PAINT message
void UIWindow::HandleWMPaintMessage() {
	if (mb_resize_needed) {
		DoLayout(); // 计算控件布局
		mb_resize_needed = FALSE;
	}
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint(hwnd_, &ps);
	{
		RECT rect;
		::GetClientRect(hwnd_, &rect);
		// Create GDI render engine based on hdc and client rect
		UIGDIRenderEngine engine(hdc, rect.right - rect.left, rect.bottom - rect.top);
		SetCurrentRenderEngine(&engine); 
		Render(UIRect(rect));
		// Dispose render engine
		SetCurrentRenderEngine(NULL);
	}
	::EndPaint(hwnd_, &ps);
}

// 处理窗口的鼠标消息
void UIWindow::HandleMouseMessage(UINT message, WPARAM wParam, LPARAM lParam) {
	if (WM_MOUSELEAVE == message) { // 鼠标离开
		SetCurrentMouseControl(NULL);
		mb_mouse_in = FALSE;
		return;
	}

	if (WM_MOUSEMOVE == message) { // 鼠标移动
		if (! mb_mouse_in) {
			TRACKMOUSEEVENT track_mouse_event;
			track_mouse_event.cbSize = sizeof(TRACKMOUSEEVENT);
			track_mouse_event.dwFlags = TME_LEAVE;
			track_mouse_event.hwndTrack = GetHWND();
			mb_mouse_in = ::TrackMouseEvent(&track_mouse_event);
		}
	}

	if (WM_LBUTTONDOWN == message) { ::SetFocus(hwnd_); } // 如果鼠标左键按下，设置此窗口活动

	// 关键之处，如果根控件存在，则由根控件来寻找目前鼠标在哪个控件上，在由找到的控件处理此消息
	if (mp_root_control) {
		UIPoint pt((short)LOWORD(lParam), (short)HIWORD(lParam));
		UIControl *pMouseControl = mp_root_control->LookupMouseFocusedControl(pt);
		if (WM_MOUSEMOVE == message) { SetCurrentMouseControl(pMouseControl); }
		pMouseControl->DispatchMouseMessage(message, wParam, lParam);
	}
}

void UIWindow::SetCurrentMouseControl(UIControl *pControl) {
	if (mp_focused_control == pControl) { return; }
	if (mp_focused_control) { mp_focused_control->DispatchMouseMessage(WM_MOUSELEAVE, 0, 0); }
	if (pControl) { pControl->DispatchMouseMessage(WM_MOUSEHOVER, 0, 0); }
	mp_focused_control = pControl;
}

} // namespace HUI
/**
 * @file    window.h
 * @brief   通用窗口类
 * @author  ksdjfdf <330773102@qq.com>
 * @date	2016-04-27
 */

#pragma once

#include <vector>
#include "base.h"
#include "utils.h"
#include "control.h"

using namespace HUI::Utils;

namespace HUI {

class UIWindow : public UIWindowWnd
{
public:
	UIWindow();
	~UIWindow();

	// Override base method
	LRESULT HandleMessages(UINT message, WPARAM wParam, LPARAM lParam);

	void SetRootControl(UIControl *pControl);

	// update layout
	void UpdateLayout();

	// Refresh display
	void Invalidate(const UIRect *rect);

	void SetWindowMinSize(const UISize &size);
	void SetWindowMinSize(int width, int height);
	void SetWindowMaxSize(const UISize &size);
	void SetWindowMaxSize(int width, int height);

	void AddNotifier(IUINotify *notifier);

	void RaiseEvent(UINotifyEvent event, UIControl *control);

	// Override base methods
	void OnSize(UINT type, int cx, int cy);
	void OnGetMinMaxInfo(MINMAXINFO *lpMMI);

private:
	void Render(const UIRect &rect);

	// 计算控件布局
	void DoLayout();

	// 处理WM_PAINT消息
	void HandleWMPaintMessage();

	// 处理鼠标消息
	void HandleMouseMessage(UINT message, WPARAM wParam, LPARAM lParam);

	void SetCurrentMouseControl(UIControl *pControl);

private:
	BOOL mb_closing;
	BOOL mb_resize_needed;
	BOOL mb_mouse_in;
	UIControl *mp_focused_control;
	UIControl *mp_root_control;
	UISize min_size;
	UISize max_size;
	std::vector<IUINotify*> m_notifiers;
}; // class UIWindow


} // namespace HUI
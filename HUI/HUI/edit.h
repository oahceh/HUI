#pragma once

/**
 * @file    ui_edit.h
 * @brief   UIEdit 编辑框控件
 * @author  ksdjfdf <330773102@qq.com>
 * @date	2016-04-27
 */

#include "control.h"
#include "base.h"
#include "utils.h"
#include "event.h"

namespace HUI {
namespace Control {

struct UIEditConfig
{
	UIEditConfig();
	float corner_radius;  // 边框弧度
	UIColor border_color; 
	UIColor text_color;
	bool is_password;
	std::wstring text;
	Font font;
}; // struct UIEditConfig

class UIEdit;
class UIEditWindow : public UIWindowWnd
{
public:
	UIEditWindow();
	// 重写基类方法
	const wchar_t* GetWindowClassName() const { return L"HUIEditWnd"; }
	const wchar_t* GetSuperClassName() const { return L"Edit"; }
	LRESULT HandleMessages(UINT message, WPARAM wParam, LPARAM lParam);
	void OnDestroy();
public:
	void Init(UIEdit *edit);
private:
	UIEdit *m_pOwner;
}; // class UIEditWindow


class UIEdit : public UIControl
{
public:
	UIEdit();

	const wchar_t* GetEditText() const;
	void SetEditText(const wchar_t *text, bool redraw = false);
	void SetFontSize(int size);
	void SetFontFamily(LPCTSTR family);
	void SetCornerRadius(float radius);
	void SetBorderColor(UIColor color);
	void SetTextColor(UIColor color);
	void SetAsPasswordEdit(bool isPassword);
	void LoadConfig(const UIEditConfig &config);

public:
	// 重写自绘方法
	void RenderSelf(const UIRect *rect);
	// 重写事件响应
	void OnMouseEntered(UINT flag,const UIPoint &point);
	void OnMouseExited(UINT flag,const UIPoint &point);
	BOOL OnMouseMove(UINT flag,const UIPoint &point);
	BOOL OnLButtonDown(UINT flag,const UIPoint &point);
public:
	// 事件声明
	MouseEvent MouseEnter;
	MouseEvent MouseExit;
	MouseEvent MouseMove;
	MouseEvent MouseLButtonDown;
	MouseEvent TextChange;

private:
	void OnTextChanged();
private:
	UIEditConfig m_config;
	UIEditWindow *m_pWindowHolder;

	friend class UIEditWindow;
}; // namespace UIEdit

} // namesapce Control
} // namespace HUI
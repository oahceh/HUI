#pragma once

/**
 * @file    ui_label.h
 * @brief   UILabel
 * @author  ksdjfdf <330773102@qq.com>
 * @date	2016-04-27
 */
#include "control.h"
#include "event.h"

namespace HUI {
namespace Control {

struct UILabelConfig
{
	UILabelConfig();
	std::wstring text;
	std::wstring bk_img;
	UIColor text_color;
	UIColor link_color;
	bool is_hyperlink;
	bool is_singleline;
	bool show_underline;
	std::wstring url;
	TextAlignment horizontal_alignment;
	TextAlignment vertical_alignment;
	Font font;
};


class UILabel : public UIControl
{
public:
	UILabel();

	void SetText(const wchar_t *text);
	const wchar_t* GetText() const;
	void SetBkImg(const wchar_t *path);
	void SetTextColor(UIColor color);
	void SetLinkColor(UIColor color);
	void SetURL(const wchar_t *url);
	const wchar_t* GetURL() const;
	void SetAsHyperlink(bool bHyperlink);
	void SetSingleline(bool bSingleline);
	void SetShowUnderline(bool bShowUnderline);
	void SetTextAlignment(TextAlignment horAlign, TextAlignment verAlign);
	void SetTextFont(const Font &font);
	void SetTextFontSize(int size);
	void SetTextFontFamily(LPCTSTR family);
	void LoadConfig(const UILabelConfig &config);
public:
	// 重写绘制方法
	void RenderSelf(const UIRect *rect);

	// 重写消息响应方法
	void OnMouseEntered(UINT flag, const UIPoint &point);
	void OnMouseExited(UINT flag, const UIPoint &point);
	BOOL OnMouseMove(UINT flag, const UIPoint &point);
	BOOL OnLButtonDown(UINT flag, const UIPoint &point);
	BOOL OnLButtonUp(UINT flag, const UIPoint &point);
private:
	void OnClicked();
public:
	// 事件声明
	MouseEvent MouseClick;
	MouseEvent MouseEnter;
	MouseEvent MouseExit;
	MouseEvent MouseMove;
	MouseEvent MouseLButtonDown;
	MouseEvent MouseLButtonUp;

private:
	UILabelConfig m_config;
	bool mb_mouse_in;
	bool mb_clicked;
}; // class UILabel

} // namespace Control
} // namespace HUI
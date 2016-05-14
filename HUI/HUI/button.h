#pragma once

#include <string>
#include "utils.h"
#include "control.h"
#include "event.h"

using namespace HUI::Utils;

namespace HUI {
namespace Control {

enum UIButtonState {
	UIButtonNormal,
	UIButtonHover,
	UIButtonPressed,
	UIButtonDisabled,
};

struct UIButtonConfig {
	UIButtonConfig();
	std::wstring button_text;
	std::wstring normal_img;
	std::wstring hover_img;
	std::wstring pressed_img;
	std::wstring disabled_img;
	UIColor text_normal_color;
	UIColor text_focus_color;
	UIColor text_disabled_color;
	UIPoint text_offset;
	UIFont font;
	TextAlignment text_alignment;
};

class UIButton : public UIControl
{
public:
	UIButton();

	void SetText(const wchar_t *text);
	const wchar_t* GetText() const;
	void SetNormalImg(const wchar_t *path);
	void SetHoverImg(const wchar_t *path);
	void SetPressedImg(const wchar_t *path);
	void SetDisabledImg(const wchar_t *path);
	void SetTextColor(UIColor color);
	void SetFocusTextColor(UIColor color);
	void SetDisabledTextColor(UIColor color);
	void SetTextOffset(const UIPoint &offset);
	void SetTextAlignment(TextAlignment alignment);
	void SetTextFont(const UIFont& font);
	void SetTextFontSize(int size);
	void SetTextFontFamily(LPCTSTR family);
	void LoadButtonConfig(const UIButtonConfig &config);

public:
	void RenderSelf(const UIRect *rect);

	void OnMouseEntered(UINT flag, const UIPoint &point);
	void OnMouseExited(UINT flag, const UIPoint &point);
	BOOL OnMouseMove(UINT flag, const UIPoint &point);
	BOOL OnLButtonDown(UINT flag, const UIPoint &point);
	BOOL OnLButtonUp(UINT flag, const UIPoint &point);

public:
	// ÊÂ¼þÉùÃ÷£¬²âÊÔ
	MouseEvent MouseClick;
	MouseEvent MouseEnter;
	MouseEvent MouseExit;
	MouseEvent MouseLButtonDown;
	MouseEvent MouseLButtonUp;

private:
	void SetButtonState(UIButtonState state);
	void DrawText();
	void OnClicked();

private:
	UIButtonState m_button_state;
	BOOL mb_self_clicked;
	UIButtonConfig m_config;
};

} // namespace Control
} // namespace HUI
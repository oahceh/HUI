#include "button.h"
#include "render.h"
using namespace HUI::Utils;
namespace HUI {
namespace Control {

UIButtonConfig::UIButtonConfig() : text_normal_color(RGB(0, 0, 0)),
		                           text_focus_color(RGB(0, 0, 0)),
		                           text_disabled_color(RGB(163, 163, 163)),
		                           text_alignment(TextAlignmentCenter)
{ 
	GetDefaultUIFont(font);
}

UIButton::UIButton() : m_button_state(UIButtonNormal),
	                   mb_self_clicked(FALSE)
{
	name_ = "Button";
}

void UIButton::SetText(const wchar_t *text) {
	m_config.button_text = text;
	SetNeedDisplay(); // 设置重绘
}

const wchar_t* UIButton::GetText() const { return m_config.button_text.c_str(); }

void UIButton::SetNormalImg(const wchar_t *path) { m_config.normal_img = path; }
void UIButton::SetHoverImg(const wchar_t *path) { m_config.hover_img = path; }
void UIButton::SetPressedImg(const wchar_t *path) { m_config.pressed_img = path; }
void UIButton::SetDisabledImg(const wchar_t *path) { m_config.disabled_img = path; }
void UIButton::SetTextColor(UIColor color) { m_config.text_normal_color = color; }
void UIButton::SetFocusTextColor(UIColor color) { m_config.text_focus_color = color; }
void UIButton::SetDisabledTextColor(UIColor color) { m_config.text_disabled_color = color; }
void UIButton::SetTextOffset(const UIPoint &offset) { m_config.text_offset = offset; }
void UIButton::SetTextAlignment(TextAlignment alignment) { m_config.text_alignment = alignment; }
void UIButton::SetTextFont(const UIFont &font) { m_config.font = font; }
void UIButton::SetTextFontSize(int size) {
	m_config.font.lfHeight = size;
}
void UIButton::SetTextFontFamily(LPCTSTR family) {
	wcscpy_s(m_config.font.lfFaceName, family);
}
void UIButton::LoadButtonConfig(const UIButtonConfig &config) {
	m_config = config;
	SetNeedDisplay(); // 设置重绘
}

void UIButton::OnMouseEntered(UINT flag, const UIPoint &point) {
	SetButtonState(UIButtonHover);
	//SetCursor(IDC_HAND);
	MouseEnter.RaiseEvent(this);
}

void UIButton::OnMouseExited(UINT flag, const UIPoint &point) {
	SetButtonState(UIButtonNormal);
	mb_self_clicked = FALSE;
	MouseExit.RaiseEvent(this);
}

BOOL UIButton::OnMouseMove(UINT flag, const UIPoint &point) {
	SetCursor(IDC_HAND);
	return TRUE;
}

BOOL UIButton::OnLButtonDown(UINT flag, const UIPoint &point) {
	SetButtonState(UIButtonPressed);
	mb_self_clicked = TRUE;
	MouseLButtonDown.RaiseEvent(this);
	return TRUE;
}

BOOL UIButton::OnLButtonUp(UINT flag, const UIPoint &point) {
	SetButtonState(UIButtonHover);
	MouseLButtonUp.RaiseEvent(this);
	if (mb_self_clicked) {
		OnClicked();
		mb_self_clicked = FALSE;
	}
	return TRUE;
}

// 绘制控件
void UIButton::RenderSelf(const UIRect *rect) {
	UIRenderEngine *engine = CurrentRenderEngine();
	BOOL enabled = IsEnabled();
	if (!enabled) { m_button_state = UIButtonDisabled; }
	switch (m_button_state) { // 绘制背景图
	case UIButtonNormal:
		if (!m_config.normal_img.empty()) {
			engine->DrawImage(m_config.normal_img, &frame_);
		}
		break;
	case UIButtonHover:
		if (!m_config.hover_img.empty()) {
			engine->DrawImage(m_config.hover_img, &frame_);
		}
		break;
	case UIButtonPressed:
		if (!m_config.pressed_img.empty()) {
			engine->DrawImage(m_config.pressed_img, &frame_);
		}
		break;
	case UIButtonDisabled:
		if (!m_config.disabled_img.empty()) {
			engine->DrawImage(m_config.disabled_img, &frame_);
		}
		break;
	default:
		break;
	}
	DrawText(); // 绘制文字
}

/******************** Private Methods **********************/
void UIButton::SetButtonState(UIButtonState state) {
	m_button_state = state;
	SetNeedDisplay(); // 重绘
}

void UIButton::DrawText() {
	if (!m_config.button_text.empty()) {
		UIColor color = m_config.text_normal_color;
		if (m_button_state == UIButtonHover || m_button_state == UIButtonPressed) {
			color = m_config.text_focus_color;
		} else if (m_button_state == UIButtonDisabled) {
			color = m_config.text_disabled_color;
		}

		UIPoint &offset = m_config.text_offset;
		int width = frame_.GetWidth();
		int height = frame_.GetHeight();
		int x = frame_.GetX() + offset.x;
		int y = frame_.GetY() + offset.y;
		UIRect text_rect(x, y, width, height);

		std::wstring &button_text = m_config.button_text;

		UIRenderEngine *engine = CurrentRenderEngine();
		engine->DrawText(button_text.c_str(), button_text.length(), &text_rect, color,
			m_config.text_alignment | DT_VCENTER | DT_SINGLELINE, &m_config.font);
	}
}

void UIButton::OnClicked() {
	MouseClick.RaiseEvent(this);
}

} // namespace Control
} // namespace HUI
#include "label.h"
#include "render.h"

namespace HUI {
namespace Control {

UILabelConfig :: UILabelConfig() : text_color(RGB(0, 0, 0)),
		                           link_color(RGB(0, 91, 202)),
		                           is_hyperlink(false),
		                           is_singleline(true),
		                           show_underline(false),
		                           horizontal_alignment(TextAlignmentVCenter),
		                           vertical_alignment(TextAlignmentCenter)
{
	Utils::GetDefaultUIFont(font);
}

UILabel::UILabel() : mb_mouse_in(false),
	                 mb_clicked(false) {}

void UILabel::SetText(const wchar_t *text) {
	m_config.text = text;
	SetNeedDisplay();
}

const wchar_t* UILabel::GetText() const { return m_config.text.c_str(); }

void UILabel::SetBkImg(const wchar_t *path) { m_config.bk_img = path; }
void UILabel::SetTextColor(UIColor color) { m_config.text_color = color; }
void UILabel::SetLinkColor(UIColor color) { m_config.link_color = color; }
void UILabel::SetURL(const wchar_t *url) {
	m_config.url = url;
	m_config.is_hyperlink = TRUE;
	SetNeedDisplay();
}
const wchar_t* UILabel::GetURL() const { return m_config.url.c_str(); }
void UILabel::SetAsHyperlink(bool bHyperlink) { m_config.is_hyperlink = bHyperlink; }
void UILabel::SetSingleline(bool bSingleline) { m_config.is_singleline = bSingleline; }
void UILabel::SetShowUnderline(bool bShowUnderline) { m_config.show_underline = bShowUnderline; }
void UILabel::SetTextAlignment(TextAlignment horAlign,TextAlignment verAlign) {
	m_config.horizontal_alignment = horAlign;
	m_config.vertical_alignment = verAlign;
}
void UILabel::SetTextFont(const Font &font) { m_config.font = font; }
void UILabel::SetTextFontSize(int size) { m_config.font.SetFontSize(size); }
void UILabel::SetTextFontFamily(LPCTSTR family) { m_config.font.SetFontFamily(family); }
void UILabel::LoadConfig(const UILabelConfig &config) {
	m_config = config;
	SetNeedDisplay();
}

// 重写自绘方法
void UILabel::RenderSelf(const UIRect *rect) {
	//MessageBox(NULL, L"UILabel.RederSelf has been called", L"", 0);
	UIRenderEngine *engine = CurrentRenderEngine();
	// 如果有背景图片则绘制背景
	if (!m_config.bk_img.empty()) {
		engine->DrawImage(m_config.bk_img, &frame_);
	}
	UIColor color = m_config.is_hyperlink ? m_config.link_color : m_config.text_color;
	UINT format = m_config.horizontal_alignment | m_config.vertical_alignment;
	if (m_config.is_singleline) {
		format |= DT_SINGLELINE;
	}
	else {
		format |= DT_WORDBREAK;
	}
	m_config.font.lfUnderline = m_config.show_underline || (m_config.is_hyperlink && mb_mouse_in);
	std::wstring text = m_config.text.empty() ? m_config.url : m_config.text;
	engine->DrawText(text.c_str(), text.length(), &frame_, color, format, &m_config.font);
}

void UILabel::OnMouseEntered(UINT flag, const UIPoint &point) {
	mb_mouse_in = true;
	if (m_config.is_hyperlink) {
		SetCursor(IDC_HAND);
		if (!m_config.show_underline) {
			SetNeedDisplay();
		}
	}
	MouseEnter.RaiseEvent(this);
}

void UILabel::OnMouseExited(UINT flag, const UIPoint &point) {
	mb_mouse_in = false;
	mb_clicked = false;
	SetCursor(IDC_ARROW);
	if (m_config.is_hyperlink && !m_config.show_underline) {
		SetNeedDisplay();
	}
	MouseExit.RaiseEvent(this);
}

BOOL UILabel::OnMouseMove(UINT flag, const UIPoint &point) {
	mb_mouse_in = true;
	if (m_config.is_hyperlink) {
		SetCursor(IDC_HAND);
	}
	MouseMove.RaiseEvent(this);
	return TRUE;
}

BOOL UILabel::OnLButtonDown(UINT flag, const UIPoint &point) {
	mb_clicked = true;
	MouseLButtonDown.RaiseEvent(this);
	return true;
}

BOOL UILabel::OnLButtonUp(UINT flag, const UIPoint &point) {
	if (mb_clicked && m_config.is_hyperlink) {
		OnClicked();
		mb_clicked = false;
	}
	MouseLButtonUp.RaiseEvent(this);
	return TRUE;
}

void UILabel::OnClicked() {
	Utils::OpenURL(m_config.url.c_str());
	MouseClick.RaiseEvent(this);
}

} // namespace Control
} // namespace HUI
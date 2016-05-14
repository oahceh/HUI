#include "edit.h"
#include "render.h"
#include <windowsx.h>
#include <OleCtl.h>

namespace HUI {
namespace Control {

UIEditConfig::UIEditConfig() : corner_radius(0.0f),
		                       border_color(RGB(0, 0, 0)),
		                       text_color(RGB(0, 0, 0)),
		                       is_password(false)
{
	Utils::GetDefaultUIFont(font);
}


UIEditWindow::UIEditWindow() : m_pOwner(NULL) {}

void UIEditWindow::Init(UIEdit *pEdit) {
	UIRect rect = pEdit->GetFrame();
	rect.Deflate(1, 3);
	DWORD style = WS_CHILD | ES_AUTOHSCROLL;
	if (pEdit->m_config.is_password) { style |= ES_PASSWORD; }
	Create(pEdit->GetParentWnd()->GetHWND(), NULL, style, 0, rect);
	HWND hwnd = GetHWND();
	HFONT font = CreateFontIndirect(&pEdit->m_config.font);
	SetWindowFont(hwnd, font, TRUE);
	::SetWindowText(hwnd, pEdit->m_config.text.c_str());
	Edit_SetModify(hwnd, TRUE);
	SendMessage(EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(2, 2));
	Edit_SetSel(hwnd, 0, -1);
	::ShowWindow(hwnd, SW_SHOWNOACTIVATE);
	::SetFocus(hwnd);
	m_pOwner = pEdit;
}

LRESULT UIEditWindow::HandleMessages(UINT message, WPARAM wParam, LPARAM lParam) {
	if (WM_KILLFOCUS == message) {
		std::wstring text;
		GetWindowText(text);
		m_pOwner->SetEditText(text.c_str(), TRUE);
		PostMessage(WM_CLOSE);
	} else if (WM_KEYUP == message) {
		std::wstring text;
		GetWindowText(text);
		if (wcscmp(m_pOwner->GetEditText(), text.c_str()) != 0) {
			m_pOwner->SetEditText(text.c_str());
			m_pOwner->OnTextChanged();
		}
	}
	return __super::HandleMessages(message, wParam, lParam);
}

void UIEditWindow::OnDestroy() {
	__super::OnDestroy();
	m_pOwner->m_pWindowHolder = NULL;
	delete this;
}

/********************** class UIEdit ***************************/
UIEdit::UIEdit() {}

const wchar_t *UIEdit::GetEditText() const { return m_config.text.c_str(); }

void UIEdit::SetEditText(const wchar_t *text, bool redraw) {
	m_config.text = text;
	if (redraw) { SetNeedDisplay(); }
}

void UIEdit::SetFontSize(int size) { m_config.font.SetFontSize(size); }

void UIEdit::SetFontFamily(LPCTSTR family) { m_config.font.SetFontFamily(family); }

void UIEdit::SetCornerRadius(float radius) { m_config.corner_radius = radius; }

void UIEdit::SetBorderColor(UIColor color) { m_config.border_color = color; }

void UIEdit::SetTextColor(UIColor color) { m_config.text_color = color; }

void UIEdit::SetAsPasswordEdit(bool isPassword) { m_config.is_password = isPassword; }

void UIEdit::LoadConfig(const UIEditConfig &config) {
	m_config = config;
	SetNeedDisplay();
}

void UIEdit::RenderSelf(const UIRect *rect) {
	UIRenderEngine *engine = CurrentRenderEngine();
	engine->Rectangle(&frame_, m_config.border_color); // »æÖÆ±ß¿ò
	UIRect text_rect = frame_;
	text_rect.Inflate(-3, 0);
	const wchar_t *text = NULL;
	std::wstring unicode_text;
	if (m_config.is_password) {
		for (size_t i = 0; i < m_config.text.length(); i++) {
			unicode_text += L"*";//unicode_text = wstring(L"*", 5);
		}
		text = unicode_text.c_str();
	} else {
		text = m_config.text.c_str();
	}
	engine->DrawText(text, m_config.text.length(), &text_rect, m_config.text_color,
		TextAlignmentVCenter | DT_SINGLELINE | DT_NOPREFIX | DT_EDITCONTROL, &m_config.font);
}

void UIEdit::OnMouseEntered(UINT flag, const UIPoint &point) { 
	SetCursor(IDC_IBEAM);
	MouseEnter.RaiseEvent(this);
}

void UIEdit::OnMouseExited(UINT flag, const UIPoint &point) { 
	SetCursor(IDC_ARROW);
	MouseExit.RaiseEvent(this);
}

BOOL UIEdit::OnMouseMove(UINT flag, const UIPoint &point) { 
	SetCursor(IDC_IBEAM);
	MouseMove.RaiseEvent(this);
	return TRUE;
}

BOOL UIEdit::OnLButtonDown(UINT flag, const UIPoint &point) {
	UIEditWindow *window = new UIEditWindow();
	window->Init(this);
	m_pWindowHolder = window;
	return TRUE;
}

void UIEdit::OnTextChanged() {
	TextChange.RaiseEvent(this);
}

} // namespace Control
} // namespace HUI
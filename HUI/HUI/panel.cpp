#include "panel.h"
#include "render.h"

namespace HUI {
namespace Layout {

UIStackPanel::UIStackPanel() : m_orientation(OrientationVertical),
		                   m_padding(UIPadding()) 
{}

void UIStackPanel::SetOrientation(Orientation orientation) {
	m_orientation = orientation;
}

void UIStackPanel::SetPadding(int left, int top, int right, int bottom) {
	m_padding = UIPadding(left, top, right, bottom);
}

void UIStackPanel::SetPadding(const UIPadding &padding) {
	m_padding = padding;
}

void UIStackPanel::DoLayout(const UIRect &rect) {
	UIControl::DoLayout(rect);
	CaculateChildrenRect();
	for (size_t i = 0; i < m_children.size(); i++) {
		m_children[i]->DoLayout(m_vChildrenRect[i]);
	}
}

void UIStackPanel::RenderSelf(const UIRect *rect) {
	DrawGridLine();
}

void UIStackPanel::CaculateChildrenRect() {
	m_vChildrenRect.clear();
	if (OrientationHorizontal == m_orientation) {
		// 水平布局
		for (size_t i = 0; i < m_children.size(); i++) {
			int left = frame_.left + m_padding.left;

		}
	} else {
		int left = frame_.left + m_padding.left;
		int right = frame_.right - m_padding.right;
		int top = frame_.top + m_padding.top;
		UISize size;
		// 垂直布局
		for (size_t i = 0; i < m_children.size(); i++) {
			size = m_children[i]->GetSize();
			m_vChildrenRect.push_back(UIRect(left, top, right - left, size.cy));
			top += size.cy;
		}
	}
}

void UIStackPanel::DrawGridLine() {
	UIRenderEngine *engine = CurrentRenderEngine();
	for (size_t i = 0; i < m_vChildrenRect.size(); i++) {
		engine->Rectangle(&m_vChildrenRect[i], PS_DASH, 0);
	}
}

} // namespace Layout
} // namespace HUI
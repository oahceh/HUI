#include "container.h"
#include "render.h"
#include "MACROS.h"

namespace HUI {

UIContainer::UIContainer() : UIControl() {
	name_ = "container control";
}

UIControl* UIContainer::LookupMouseFocusedControl(const UIPoint &point) {
	UIControl *pFoundControl = this;
	for (size_t i = m_children.size(); i > 0; i--) {
		UIControl *control = m_children[i - 1];
		if (control->GetVisibility() && control->IsEnabled() && control->IsMouseInControl(point)) {
			pFoundControl = control->LookupMouseFocusedControl(point);
			break;
		}
	}
	return pFoundControl;
}

void UIContainer::DoLayout(const UIRect &rect) {
	UIControl::DoLayout(rect);
	for (size_t i = 0; i < m_children.size(); i++) {
		m_children[i]->DoLayout(rect);
	}
}


void UIContainer::Render(const UIRect *rect) {
	//生成裁剪区域
	UIRect rc = UIRect::IntersectRect(&frame_, rect);
	CurrentRenderEngine()->SelectClipedRect(&rc);
	RenderSelf(&rc);
	
	//绘制子控件
	for (size_t i = 0; i <m_children.size(); i++)
	{
		UIControl *child = m_children[i];
		if (child->GetVisibility())
		{
			child->Render(&rc);
		}
	}
}

UIControl* UIContainer::GetChild(int iIndex) const {
	if (iIndex < 0 || iIndex >= m_children.size()) {
		return NULL;
	}
	return m_children[iIndex]; 
}

int UIContainer::GetCount() const { return m_children.size(); } 

bool UIContainer::Add(UIControl* pControl) { 
	m_children.push_back(pControl);
	pControl->SetParent(this);
	return true;
}

bool UIContainer::Remove(UIControl* pControl) {
	if (!pControl) { return false; }
	std::vector<UIControl*>::iterator iter = std::find(m_children.begin(), m_children.end(), pControl);
	if (iter != m_children.end()) { m_children.erase(iter); }
	return true;
}

void UIContainer::RemoveAll() { return m_children.clear(); }

void UIContainer::CaculateChildrenRect() {}

void UIContainer::DrawGridLine() {}

} // namespace HUI
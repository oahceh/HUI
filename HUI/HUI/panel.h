#pragma once
#include "container.h"

namespace HUI {
namespace Layout {

class UIStackPanel : public UIContainer
{
public:
	UIStackPanel();

	void SetOrientation(Orientation orientation);

	// 设置内边距
	void SetPadding(int left, int top, int right, int bottom);
	void SetPadding(const UIPadding &padding);

	// 重写计算布局
	void DoLayout(const UIRect &rect);

	// 重写自绘方法
	void RenderSelf(const UIRect *rect);

protected:
	void CaculateChildrenRect();
	void DrawGridLine();
private:
	Orientation m_orientation;
	UIPadding m_padding;
	vector<UIRect> m_vChildrenRect;
}; // class StackPanel

} // namespace Layout
} // namespace HUI
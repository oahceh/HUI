#pragma once

#include "container.h"
#include <cstdarg>

namespace HUI {

namespace Layout {
	
class UIGrid : public UIContainer
{
public:
	UIGrid();

	void SetRowsAndCols(int rows, int cols);
	int GetRows() const;
	int GetCols() const;

	void SetHeight(int rows, ...);
	void SetHeightScale(int rows, ...);

	void SetWidth(int cols, ...);
	void SetWidthScale(int cols, ...);

	void SetPos(UIControl *pControl, int row, int col);

	void ShowGridLine(bool showGridLine);

	// 重写计算布局方法
	void DoLayout(const UIRect &rect);

	// 重写自绘
	void RenderSelf(const UIRect *rect);

protected:
	// 计算子矩形
	void CaculateChildrenRect();
	// 绘制网格线
	void DrawGridLine();
private:
	int m_iRows; // 行数
	int m_iCols; // 列数
	bool m_bShowGridLine;

	// Grid行列数据
	vector<float> m_vHeight;
	vector<float> m_vWidth;
	vector<float> m_vHeightScale;
	vector<float> m_vWidthScale;

	vector<vector<UIControl*>> m_pvChildren;
	vector<vector<UIRect*>> m_pvChildrenRect;
}; // class Grid

}// namespace Layout

} // namespace HUI
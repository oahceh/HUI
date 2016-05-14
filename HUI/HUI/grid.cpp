#include "grid.h"
#include "render.h"
#include <numeric>


namespace HUI {

namespace Layout {

UIGrid::UIGrid() : m_iRows(1), 
	               m_iCols(1), 
				   m_bShowGridLine(false) {}

void UIGrid::SetRowsAndCols(int rows, int cols) {
	m_iRows = rows;
	m_iCols = cols;
	m_pvChildrenRect.clear();
	m_pvChildren.clear();
	vector<UIControl*> pTempControl;
	vector<UIRect*> pTempRect;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			pTempControl.push_back(NULL);
			pTempRect.push_back(NULL);
		}
		m_pvChildren.push_back(pTempControl);
		m_pvChildrenRect.push_back(pTempRect);
		pTempControl.clear();
		pTempRect.clear();
	}
}

int UIGrid::GetRows() const {
	return m_iRows;
}

int UIGrid::GetCols() const {
	return m_iCols;
}

void UIGrid::SetHeight(int rows, ...) {
	m_vHeight.clear();
	va_list args;
	va_start(args, rows);
	for (int i = 0; i < rows; i++) {
		m_vHeight.push_back(va_arg(args, int));
	}
	m_iRows = rows;
	va_end(args);
}

void UIGrid::SetHeightScale(int rows, ...) {
	m_vHeightScale.clear();
	va_list args;
	va_start(args, rows);
	for (int i = 0; i < rows; i++) {
		m_vHeightScale.push_back(va_arg(args, int));
	}
	m_iRows = rows;
	va_end(args);
}

void UIGrid::SetWidth(int cols, ...) {
	m_vWidth.clear();
	va_list args;
	va_start(args, cols);
	for (int i = 0; i < cols; i++) {
		m_vWidth.push_back(va_arg(args, int));
	}
	m_iCols = cols;
	va_end(args);
}

void UIGrid::SetWidthScale(int cols, ...) {
	m_vWidthScale.clear();
	va_list args;
	va_start(args, cols);
	for (int i = 0; i < cols; i++) {
		m_vWidthScale.push_back(va_arg(args, int));
	}
	m_iCols = cols;
	va_end(args);
}

void UIGrid::SetPos(UIControl *pControl, int row, int col) {
	m_pvChildren[row][col] = pControl;
}

void UIGrid::ShowGridLine(bool showGridLine) {
	m_bShowGridLine = showGridLine;
}

void UIGrid::DoLayout(const UIRect &rect) {
	UIControl::DoLayout(rect);
	CaculateChildrenRect();
	for (int i = 0; i < m_iRows; i++) {
		for (int j = 0; j < m_iCols; j++) {
			if (m_pvChildren[i][j] != NULL) {
				m_pvChildren[i][j]->DoLayout(*m_pvChildrenRect[i][j]);
			}
		}
	}
}

void UIGrid::RenderSelf(const UIRect *rect) {
	if (m_bShowGridLine) {
		DrawGridLine();
	}
}

void UIGrid::CaculateChildrenRect() {
	if (!m_vHeightScale.empty()) {
		float sum = std::accumulate(m_vHeightScale.begin(), m_vHeightScale.end(), 0);
		m_vHeight.clear();
		for (int i = 0; i < m_iRows; i++) {
			m_vHeight.push_back(frame_.GetHeight() * m_vHeightScale[i] / sum);
		}
	}
	if (!m_vWidthScale.empty()) {
		float sum = std::accumulate(m_vWidthScale.begin(), m_vWidthScale.end(), 0);
		m_vWidth.clear();
		for (int i = 0; i < m_iCols; i++) {
			m_vWidth.push_back(frame_.GetWidth() * m_vWidthScale[i] / sum);
		}
	}
	int xStart = 0, yStart = 0;
	for (int i = 0; i < m_iRows; i++) {
		xStart = 0;
		for (int j = 0; j < m_iCols; j++) {
			m_pvChildrenRect[i][j] = new UIRect(frame_.left + xStart, frame_.top + yStart, 
				m_vWidth[j], m_vHeight[i]);
			xStart = m_pvChildrenRect[i][j]->right;
		}
		yStart = m_pvChildrenRect[i][0]->bottom;
	}
}

void UIGrid::DrawGridLine() {
	UIRenderEngine *engine = CurrentRenderEngine();
	for (int i = 0; i < m_iRows; i++) {
		for (int j = 0; j < m_iCols; j++) {
			engine->Rectangle(m_pvChildrenRect[i][j], PS_DASH, 0);
		}
	}
}

} // namespace Layout

} // namespace HUI
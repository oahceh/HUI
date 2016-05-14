#pragma once

#include "control.h"

namespace HUI {
namespace Control {

class UIImage : public UIControl
{
public:
	UIImage();

	void SetImg(const wchar_t *path);

	void ShowBorder(bool isShowBorder);

	void RenderSelf(const UIRect *rect);

private:
	std::wstring m_wsImgPath;
	bool m_bShowBorder;
}; // class UIImage

} // namespace Control
} // namespace HUI
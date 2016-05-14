#include "image.h"
#include "render.h"

namespace HUI {
namespace Control {

UIImage::UIImage() : m_bShowBorder(false) {}

void UIImage::SetImg(const wchar_t *path) {
	m_wsImgPath = path;
	SetNeedDisplay();
}

void UIImage::ShowBorder(bool isShowBorder) {
	m_bShowBorder = isShowBorder;
}

void UIImage::RenderSelf(const UIRect *rect) {
	UIRenderEngine *engine = CurrentRenderEngine();
	if (m_bShowBorder) {
		engine->Rectangle(&frame_, 0);
	}
	if (!m_wsImgPath.empty()) {
		engine->DrawImage(m_wsImgPath, &frame_);
	} else {
		engine->FillRect(&frame_, RGB(0, 0, 0));
	}
}

} // namespace Control
} // namespace HUI
/**
 * @file    render.cpp
 * @brief   界面渲染引擎
 * @author  ksdjfdf <330773102@qq.com>
 * @date	2016-04-28
 */

#include "render.h"
#include "MACROS.h"
#include "resource.h"
#include "CxImage\ximage.h"

// Import cximage library
#pragma comment(lib, "CxImage\\cximaged.lib")
#pragma comment(lib, "CxImage\\jasper.lib")
#pragma comment(lib, "CxImage\\jbig.lib ")
#pragma comment(lib, "CxImage\\Jpeg.lib ")
#pragma comment(lib, "CxImage\\libdcr.lib ")
#pragma comment(lib, "CxImage\\libpsd.lib ")
#pragma comment(lib, "CxImage\\mng.lib ")
#pragma comment(lib, "CxImage\\png.lib ")
#pragma comment(lib, "CxImage\\Tiff.lib ")
#pragma comment(lib, "CxImage\\zlib.lib ")

namespace HUI {

UIGDIRenderEngine::UIGDIRenderEngine(HDC hdc, int width, int height) : 
	hdc_(hdc), 
	width_(width),
	height_(height) {
		assert(hdc);
		mem_hdc_ = CreateCompatibleDC(hdc_);
		bitmap_ = CreateCompatibleBitmap(hdc, width, height);
		// select the bitmap object into DC mem_hdc_
		old_bitmap_ = (HBITMAP)SelectObject(mem_hdc_, bitmap_);
		// 从原设备中复制位图到目标设备
		BitBlt(mem_hdc_, 0, 0, width_, height_, 0, 0, 0, WHITENESS);
}

UIGDIRenderEngine::~UIGDIRenderEngine() {
	BitBlt(hdc_, 0, 0, width_, height_, mem_hdc_, 0, 0, SRCCOPY);
	SelectObject(mem_hdc_, old_bitmap_);
	DeleteDC(mem_hdc_);
	DeleteObject(bitmap_);
}

void UIGDIRenderEngine::FillRect(const UIRect *rect, UIColor color) {
	PTR_VOID(rect);
	HBRUSH brush = CreateSolidBrush(color);
	::FillRect(mem_hdc_, rect, brush);
	DeleteObject(brush);
}

void UIGDIRenderEngine::Rectangle(const UIRect *rect, UIColor color) {
	PTR_VOID(rect);
	LOGBRUSH lb = {BS_NULL};
	HBRUSH brush = CreateBrushIndirect(&lb);
	HBRUSH old_brush = (HBRUSH)::SelectObject(mem_hdc_, brush);
	HPEN pen = CreatePen(PS_SOLID, 1, color);
	HPEN old_pen = (HPEN)::SelectObject(mem_hdc_, pen);
	::Rectangle(mem_hdc_, rect->left, rect->top, rect->right, rect->bottom);
	::SelectObject(mem_hdc_, old_brush);
	::SelectObject(mem_hdc_, old_pen);
	::DeleteObject(brush);
	::DeleteObject(pen);
}

void UIGDIRenderEngine::Rectangle(const UIRect *rect, int style, UIColor color) {
	PTR_VOID(rect);
	LOGBRUSH lb = {BS_NULL};
	HBRUSH brush = CreateBrushIndirect(&lb);
	HBRUSH old_brush = (HBRUSH)::SelectObject(mem_hdc_, brush);
	HPEN pen = CreatePen(style, 1, color);
	HPEN old_pen = (HPEN)::SelectObject(mem_hdc_, pen);
	::Rectangle(mem_hdc_, rect->left, rect->top, rect->right, rect->bottom);
	::SelectObject(mem_hdc_, old_brush);
	::SelectObject(mem_hdc_, old_pen);
	::DeleteObject(brush);
	::DeleteObject(pen);
}

void UIGDIRenderEngine::SelectClipedRect(const UIRect *rect) {
	PTR_VOID(rect);
	HRGN hrgn = ::CreateRectRgnIndirect(rect);
	::SelectClipRgn(mem_hdc_, hrgn);
	DeleteObject(hrgn);
}

void UIGDIRenderEngine::DrawImage(const std::wstring &path, const UIRect *src, const UIRect *dst) {
	CxImage *image = NULL;
	if (UIResourceManager::GetInstance()->GetImageByFilePath(path, &image)) {
		image->Draw(mem_hdc_, dst->ToRect());
	}
}

void UIGDIRenderEngine::DrawImage(const std::wstring &path, const UIRect *dst) {
	CxImage *image = NULL;
	if (UIResourceManager::GetInstance()->GetImageByFilePath(path, &image)) {
		image->Draw(mem_hdc_, dst->ToRect());
	}
}

void UIGDIRenderEngine::DrawText(const wchar_t *text,size_t text_length,const UIRect *rect,UIColor text_color,UINT format,const UIFont *font /* = NULL */) {
	HFONT old_font = NULL;
	HFONT current_font = NULL;
	if (font) {
		current_font = CreateFontIndirect(font);
		old_font = (HFONT)SelectObject(mem_hdc_, current_font);
	}
	SetBkMode(mem_hdc_, TRANSPARENT);
	UIColor old_text_color = SetTextColor(mem_hdc_, text_color);
	::DrawText(mem_hdc_, text, (int)text_length, &rect->ToRect(), format);
	if (old_font && current_font) {
		SelectObject(mem_hdc_, old_font);
		DeleteObject(current_font);
	}
	SetTextColor(mem_hdc_, old_text_color);
}

UIRenderEngine *gEngine = NULL;
UIRenderEngine* CurrentRenderEngine() { return gEngine; }
void SetCurrentRenderEngine(UIRenderEngine *engine) { gEngine = engine; }

} // namespace HUI
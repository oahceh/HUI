#pragma once

#include <string>
#include "utils.h"

using namespace std;
using namespace HUI::Utils;

namespace HUI {

class UIRenderEngine
{
public:
	virtual void FillRect(const UIRect *rect,UIColor color) = 0;
	virtual	void Rectangle(const UIRect *rect,UIColor color) = 0;
	virtual void Rectangle(const UIRect *rect, int style, UIColor color) = 0;
	virtual void SelectClipedRect(const UIRect *rect) = 0;
	virtual void DrawImage(const std::wstring &path,const UIRect *src,const UIRect *dst) = 0;
	virtual void DrawImage(const std::wstring &path,const UIRect *dst) = 0;
	virtual	void DrawText(const wchar_t *text,size_t text_length,const UIRect *rect,UIColor text_color,UINT format,const UIFont *font = NULL) = 0;
};

class UIGDIRenderEngine : public UIRenderEngine
{
public:
	UIGDIRenderEngine(HDC hdc, int width, int height);
	~UIGDIRenderEngine();
public:
	void FillRect(const UIRect *rect,UIColor color);
	void Rectangle(const UIRect *rect,UIColor color);
	void Rectangle(const UIRect *rect, int style, UIColor color);
	void SelectClipedRect(const UIRect *rect);
	void DrawImage(const std::wstring &path,const UIRect *src,const UIRect *dst);
	void DrawImage(const std::wstring &path,const UIRect *dst);
	void DrawText(const wchar_t *text,size_t text_length,const UIRect *rect,UIColor text_color,UINT format,const UIFont *font = NULL);
private:
	HDC hdc_;
	HDC mem_hdc_;
	HBITMAP bitmap_;
	HBITMAP old_bitmap_;
	int width_;
	int height_;
};

UIRenderEngine* CurrentRenderEngine();
void SetCurrentRenderEngine(UIRenderEngine *engine);

} // namespace HUI
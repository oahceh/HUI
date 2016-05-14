/**
 * @file    utils.cpp
 * @brief   UI基础设施实现
 * @author  ksdjfdf <330773102@qq.com>
 * @date	2016-04-27
 */
#include "utils.h"

#define GET_X_LPARAM(lp)	((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)	((int)(short)HIWORD(lp))

namespace HUI {
namespace Utils {

/////////////////////////////////////UIRect//////////////////////////////////////////
UIRect::UIRect() {
	left = top = right = bottom = 0;
}

UIRect::UIRect(const RECT& src) {
	left = src.left;
	top = src.top;
	right = src.right;
	bottom = src.bottom;
}

UIRect::UIRect(int x,int y,int width,int height) {
	left	= x;
	top		= y;
	right	= x + width;
	bottom	= y + height;
}

RECT UIRect::ToRect() const {
	RECT rect = *this;
	return rect;
}

int UIRect::GetX() const { return left; }

int UIRect::GetY() const { return top; }

int UIRect::GetWidth() const { return right - left; }

int UIRect::GetHeight() const { return bottom - top; }

void UIRect::Empty() { left = top = right = bottom = 0; }

void UIRect::Join(const RECT& rc) {
	if( rc.left < left ) left = rc.left;
	if( rc.top < top ) top = rc.top;
	if( rc.right > right ) right = rc.right;
	if( rc.bottom > bottom ) bottom = rc.bottom;
}

void UIRect::ResetOffset() { ::OffsetRect(this, -left, -top); }

void UIRect::Normalize()
{
	if( left > right ) { 
		int iTemp = left;
		left = right;
		right = iTemp;
	}
	if( top > bottom ) { 
		int iTemp = top;
		top = bottom;
		bottom = iTemp;
	}
}

void UIRect::Offset(int cx, int cy) { ::OffsetRect(this, cx, cy); }

void UIRect::Inflate(int cx, int cy) { ::InflateRect(this, cx, cy); }

void UIRect::Deflate(int cx, int cy) { ::InflateRect(this, -cx, -cy); }

void UIRect::Union(UIRect& rc) { ::UnionRect(this, this, &rc); }

UIRect	UIRect::IntersectRect(const UIRect *first,const UIRect *second) {
	UIRect rect;
	::IntersectRect(&rect,first,second);
	return rect;
}


////////////////////////UISize///////////////////////////////
UISize::UISize() { cx = cy = 0; }

UISize::UISize(const SIZE &src) {
   cx = src.cx;
   cy = src.cy;
}

UISize::UISize(const RECT &rc) {
   cx = rc.right - rc.left;
   cy = rc.bottom - rc.top;
}

UISize::UISize(int _cx, int _cy) {
   cx = _cx;
   cy = _cy;
}

//////////////////////////////UIPoint///////////////////////////////////
UIPoint::UIPoint() { x = y = 0; }

UIPoint::UIPoint(const POINT &src) {
   x = src.x;
   y = src.y;
}

UIPoint::UIPoint(int _x, int _y) {
   x = _x;
   y = _y;
}

UIPoint::UIPoint(LPARAM lParam) {
   x = GET_X_LPARAM(lParam);
   y = GET_Y_LPARAM(lParam);
}


/********************** UIMargin ********************************/
UIMargin::UIMargin() { left = top = right = bottom = 0; }

UIMargin::UIMargin(int left_param, int top_param, int right_param, int bottom_param) {
	left = left_param;
	top = top_param;
	right = right_param;
	bottom = bottom_param;
}

/********************** UIPadding *******************************/
UIPadding::UIPadding() : left(0), top(0), right(0), bottom(0) {}

UIPadding::UIPadding(int leftPara, int topPara, int rightPara, int bottomPara) :
	left(leftPara), top(topPara), right(rightPara), bottom(bottomPara) {}

/******************* 工具函数 *************************/
std::wstring GetAppPath()
{
	static std::wstring app_path;
	if (app_path.empty())
	{
		wchar_t	buffer[MAX_PATH];
		ZeroMemory(buffer, sizeof(wchar_t) * MAX_PATH);	
		::GetModuleFileName(NULL, buffer, MAX_PATH);
		std::wstring temp(buffer);
		size_t index = temp.rfind('\\');
		if (index == std::wstring::npos)
		{
			index = temp.rfind(';');
		}
		app_path = temp.substr(0,index + 1);
	}
	return app_path;
}

std::wstring UTF8toUTF16(const char *src) {
	int length = ::MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
	wchar_t *dst = new wchar_t[length];
	::MultiByteToWideChar(CP_UTF8, 0, src, -1, dst, length);
	std::wstring output(dst,length);
	delete []dst;
	return output;
}

void GetDefaultUIFont(UIFont &font) {
	memset(&font, 0, sizeof(UIFont));
	font.lfHeight = -11;
	font.lfWeight = FW_NORMAL;
	font.lfOutPrecision = OUT_STROKE_PRECIS;
	font.lfClipPrecision = CLIP_STROKE_PRECIS;
	font.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
	wcscpy_s(font.lfFaceName, L"SimSum");
}

UIFont *GetGlobalEditFont() {
	static UIFont *global_font = NULL;
	if (global_font == NULL) {
		global_font = new UIFont();
		memset(global_font, 0, sizeof(UIFont));
		global_font->lfHeight				= -15;
		global_font->lfWeight				= FW_NORMAL;
		global_font->lfOutPrecision			= OUT_STROKE_PRECIS;
		global_font->lfClipPrecision		= CLIP_STROKE_PRECIS;
		global_font->lfQuality				= DEFAULT_QUALITY;
		global_font->lfPitchAndFamily		= VARIABLE_PITCH | FF_SWISS;
		wcscpy_s(global_font->lfFaceName, L"Consolas");/*SimSum*/
	}
	return global_font;
}

void OpenURL(const wchar_t *url, int flag, BOOL use_default_browser) {
	if (use_default_browser) {
		::ShellExecute(NULL, L"open", url, NULL, NULL, flag);
	} else {
		::ShellExecute(NULL, L"open", L"IEEXPLORE.EXE", url, NULL, flag);
	}
}

} // namespace Utils
} // namespace HUI
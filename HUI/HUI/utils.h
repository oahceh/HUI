/**
 * @file    utils.h
 * @brief   UI基础设施, 封装一些基础数据结构类，以及一些工具类，并不是字面的意义
 * @author  ksdjfdf <330773102@qq.com>
 * @date	2016-04-22
 */

#pragma once
#include<Windows.h>
#include<string>

namespace HUI {

class UIControl;
namespace Utils {

// 把系统自带的tagRect封装为自己的Rect
class UIRect : public tagRECT
{
public:
	UIRect();
	UIRect(const RECT& src);
	UIRect(int x,int y,int width,int height);

	RECT ToRect() const;
	int GetX() const;
	int GetY() const;
	int GetWidth() const;
	int GetHeight() const;
	void Empty();
	void Join(const RECT& rc);
	void ResetOffset();
	void Normalize();
	void Offset(int cx, int cy);
	void Inflate(int cx, int cy);
	void Deflate(int cx, int cy);
	void Union(UIRect& rc);

	// 计算两个源矩形相交的矩形，即first和second所指的两个矩形相交的矩形
	static UIRect IntersectRect(const UIRect *first,const UIRect *second);
};


// 把系统自带的tagSIZE封装为自己的SIZE类
class UISize : public tagSIZE
{
public:
   UISize();
   UISize(const SIZE &src);
   UISize(const RECT &rc);
   UISize(int cx, int cy);
};


// 同上UIPoint
class  UIPoint : public tagPOINT
{
public:
   UIPoint();
   UIPoint(const POINT &src);
   UIPoint(int x, int y);
   UIPoint(LPARAM lParam);
};


typedef COLORREF UIColor;
typedef LOGFONT	 UIFont;

class Font : public LOGFONT 
{
public:
	void SetFontSize(int size) {
		lfHeight = size;
	}

	void SetFontFamily(LPCTSTR family) {
		wcscpy_s(lfFaceName, family);
	}
};

// 外边距
struct UIMargin
{
	UIMargin();
	UIMargin(int left,int top = 0,int right = 0,int bottom = 0);

	int left;
	int top;
	int right;
	int bottom;
};

// 内边距
struct UIPadding
{
	UIPadding();
	UIPadding(int leftPara, int topPara, int rightPara, int bottomPara);

	int left;
	int top;
	int right;
	int bottom;
};

// 文本定位方式
enum TextAlignment
{
	TextAlignmentLeft = DT_LEFT,
	TextAlignmentCenter	= DT_CENTER,
	TextAlignmentRight = DT_RIGHT,

	TextAlignmentTop = DT_TOP,
	TextAlignmentVCenter = DT_VCENTER,
	TextAlignmentBottom	= DT_BOTTOM,
};

//定位方式
enum Alignment
{
//绝对定位
	AlignmentLeftTop,
	AlignmentLeftCenter,
	AlignmentLeftBottom,
	AlignmentCenterTop,
	AlignmentCenter,
	AlignmentCenterBottom,
	AlignmentRightTop,
	AlignmentRightCenter,
	AlignmentRightBottom,
//相对定位
	AlignmentLeft,
	AlignmentRight,
	AlignmentTop,
	AlignmentBottom,
	AlignmentFill,
};

// 朝向
enum Orientation {
	OrientationHorizontal, // 水平
	OrientationVertical,   // 垂直
};

// class UIControl;
enum UINotifyEvent
{
	EventClick,		//点击事件
	EventTextChanged,	//文本变化事件
};

// 事件通知接口
class IUINotify
{
public:
	virtual	void Notify(UINotifyEvent event,UIControl *control) = 0;
};


// 工具函数
std::wstring GetAppPath();
std::wstring UTF8toUTF16(const char *src);
void GetDefaultUIFont(UIFont &font);
UIFont* GetGlobalEditFont();
void OpenURL(const wchar_t *url, int flag = SW_SHOWMAXIMIZED, BOOL use_default_browser = TRUE);

} // namespace Utils
} // namespace HUI
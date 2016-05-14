/**
 * @file    control.cpp
 * @brief   UI控件基类
 * @author  ksdjfdf <330773102@qq.com>
 * @date	2016-04-28
 */

#include "control.h"
#include "MACROS.h"
#include "render.h"
#include <vector>

namespace HUI {

UIControl::UIControl() : name_(""),
	                     tag_(0),
	                     alignment_(AlignmentLeftTop),
						 mp_parent_wnd(NULL),
						 parent_(NULL),
						 visibility_(TRUE),
						 enabled_(TRUE) {}

void UIControl::SetAttribute(const char *name,const char *value)
{
	if (strcmp(name,"name") == 0) { SetName(value); }
	else if (strcmp(name,"tag") == 0) { SetTag(atoi(value)); }
	else if (strcmp(name,"alignment") == 0) {
		Alignment alignment = GetAlignmentByString(value);
		SetAlignment(alignment);
	}
	else if (strcmp(name,"size") == 0) {
		UISize size;
		sscanf(value,"%d,%d",&size.cx,&size.cy);
		SetSize(size);
	}
	else if (strcmp(name,"margin") == 0) {
		UIMargin margin;
		sscanf(value,"%d,%d,%d,%d",
			&margin.left,&margin.top,&margin.right,&margin.bottom);
		SetMargin(margin);
	}
	else if (strcmp(name,"visible") == 0) { SetVisibility(stricmp(value,"true") == 0); }
	else if (strcmp(name,"enable") == 0) { SetEnabled(stricmp(value,"true") == 0); }
}

void UIControl::SetName(const char *name) { name_ = name; }
std::string	UIControl::GetName() const { return name_; }

void UIControl::SetTag(int tag) { tag_ = tag; }
int UIControl::GetTag() const { return tag_; }

void UIControl::SetMargin(const UIMargin &margin) {
	margin_ = margin;
	SetNeedLayout();
}
void	UIControl::SetMargin(int left, int top, int right, int bottom) {
	SetMargin(UIMargin(left,top,right,bottom));
}

void UIControl::SetSize(const UISize &size) {
	size_ = size;
	SetNeedLayout();
}
void UIControl::SetSize(int width,int height) { SetSize(UISize(width,height)); }
UISize UIControl::GetSize() const { return size_; }

void UIControl::SetAlignment(Alignment alignment) {
	alignment_ = alignment;
	SetNeedLayout();
}
Alignment UIControl::GetAlignment() const { return alignment_; }

void UIControl::SetVisibility(BOOL visible) { visibility_ = visible; }
BOOL UIControl::GetVisibility() const { return parent_->visibility_ && visibility_; }

void UIControl::SetEnabled(BOOL enabled) { enabled_ = enabled; }
BOOL UIControl::IsEnabled() const { return parent_->enabled_ && enabled_; }

void UIControl::SetParent(UIControl *control) {
	PTR_VOID(control);
	parent_	= control;
}
UIControl*	UIControl::GetParent() const {  return parent_; }

void UIControl::Render(const UIRect *rect) {
	UIRect rc = UIRect::IntersectRect(&frame_, rect);
	CurrentRenderEngine()->SelectClipedRect(&rc);
	RenderSelf(&rc);
}

void UIControl::RenderSelf(const UIRect *rect) {
	// Do this by sub-class
}

UIControl* UIControl::LookupMouseFocusedControl(const UIPoint &point) { return this; }

void UIControl::SetFrame(const UIRect *rect) {
	PTR_VOID(rect);
	frame_ = *rect;
}
UIRect UIControl::GetFrame() const { return frame_; }

void UIControl::SetParentWnd(UIWindow *pWnd) {
	PTR_VOID(pWnd);
	mp_parent_wnd = pWnd;
}
UIWindow* UIControl::GetParentWnd() {
	// Get parent window recursively
	if (NULL == mp_parent_wnd && parent_) {
		mp_parent_wnd = parent_->GetParentWnd();
	}
	return mp_parent_wnd;
}

void UIControl::DoLayout(const UIRect &rect) {
	UIRect parent_frame = rect;
	int x = 0;
	int y = 0;
	int width  = size_.cx;
	int height = size_.cy;
	switch(alignment_)
	{
	//绝对定位
	case AlignmentLeftTop:
		{
			x = parent_frame.GetX() + margin_.left;
			y = parent_frame.GetY() + margin_.top;
			break;
		}
	case AlignmentLeftCenter:
		{
			x = parent_frame.GetX() + margin_.left;
			y = parent_frame.GetY() + ((parent_frame.GetHeight() - size_.cy) >> 1);
			break;
		}
	case  AlignmentLeftBottom:
		{
			x = parent_frame.GetX() + margin_.left;
			y = parent_frame.GetY() + parent_frame.GetHeight() - size_.cy - margin_.bottom;
			break;
		}
	case AlignmentCenterTop:
		{
			x = parent_frame.GetX() + ((parent_frame.GetWidth() - size_.cx) >> 1) ;
			y = parent_frame.GetY() + margin_.top;
			break;
		}
	case  AlignmentCenter:
		{
			x = parent_frame.GetX() + ((parent_frame.GetWidth() - size_.cx) >> 1);
			y = parent_frame.GetY() + ((parent_frame.GetHeight() - size_.cy) >> 1);
			break;
		}
	case  AlignmentCenterBottom:
		{
			x = parent_frame.GetX() + ((parent_frame.GetWidth() - size_.cx) >> 1);
			y = parent_frame.GetY() + parent_frame.GetHeight() - size_.cy - margin_.bottom;
			break;
		}
	case AlignmentRightTop:
		{
			x = parent_frame.GetX() +  parent_frame.GetWidth() - size_.cx - margin_.right;
			y = parent_frame.GetY() + margin_.top;
			break;
		}
	case AlignmentRightCenter:
		{
			x = parent_frame.GetX() +  parent_frame.GetWidth() - size_.cx - margin_.right;
			y = parent_frame.GetY() + ((parent_frame.GetHeight() - size_.cy) >> 1);
			break;
		}
	case AlignmentRightBottom:
		{
			x = parent_frame.GetX() +  parent_frame.GetWidth() - size_.cx - margin_.right;
			y = parent_frame.GetY() + parent_frame.GetHeight() - size_.cy - margin_.bottom;
			break;
		}
		//相对定位
	case AlignmentLeft:
		{
			x = parent_frame.GetX() + margin_.left;
			y = parent_frame.GetY() + margin_.top;
			height = parent_frame.GetHeight() - margin_.top - margin_.bottom;
			break;
		}
	case  AlignmentRight:
		{
			x = parent_frame.GetX() + parent_frame.GetWidth() - size_.cx - margin_.right;
			y = parent_frame.GetY() + margin_.top;
			height = parent_frame.GetHeight() - margin_.top - margin_.bottom;
			break;
		}
	case AlignmentTop:
		{
			x = parent_frame.GetX() + margin_.left;
			y = parent_frame.GetY() + margin_.top;
			width = parent_frame.GetWidth() - margin_.left - margin_.right;
			break;
		}
	case AlignmentBottom:
		{
			x = parent_frame.GetX() + margin_.left;
			y = parent_frame.GetY() + parent_frame.GetHeight() - size_.cy - margin_.bottom;
			width = parent_frame.GetWidth() - margin_.left - margin_.right;
			break;
		}
	case AlignmentFill:
		{

			x = parent_frame.GetX() + margin_.left;
			y = parent_frame.GetY() + margin_.top;
			width = parent_frame.GetWidth() - margin_.left - margin_.right ;
			height = parent_frame.GetHeight() - margin_.top - margin_.bottom;
			break;
		}
	default:
		assert(0);
		break;
	}
	frame_ = UIRect(x,y,width,height);
}

// 重新布局
void UIControl::SetNeedLayout() {
	UIWindow *pParentWnd = GetParentWnd();
	if (pParentWnd) { 
		pParentWnd->UpdateLayout();
	}
}

void UIControl::SetNeedDisplay() {
	UIWindow *pParentWnd = GetParentWnd();
	if (pParentWnd) { pParentWnd->Invalidate(NULL); }
}

void UIControl::SetCursor(const wchar_t *cursor) {
	ShowCursor(FALSE);
	::SetCursor(LoadCursor(NULL, cursor));
	ShowCursor(TRUE);
}


void UIControl::Notify(UINotifyEvent event,UIControl *control) {
}


// Key: Message Handler
BOOL UIControl::OnMouseMessage(UINT message, WPARAM wParam, LPARAM lParam) {
	// Get the current mouse point
	UIPoint pt((short)LOWORD(lParam), (short)HIWORD(lParam));
	UINT flag = (UINT)wParam;
	// bHandled: if it is true, then this message will no be passed to parent control
	// , it is kind of similar to WPF event route.
	// About bHandled of WM_MOUSEHOVER, there are three way to process this event
	// 1. Add return value to OnMouseEntered, check the returned value in overrided method
	// 2. The default value is TRUE, if mouse moved into child control from parent, it is of HOVER
	// 3. The default value is FALSE, it is of always HOVER state
	BOOL bMessageHandled = FALSE;
	switch (message) {
	case WM_MOUSEHOVER:
		OnMouseEntered(flag, pt);
		bMessageHandled = TRUE;
		break;
	case WM_MOUSELEAVE:
		OnMouseExited(flag, pt);
		bMessageHandled = TRUE;
		break;
	case WM_MOUSEWHEEL:
		OnMouseWheel(GET_KEYSTATE_WPARAM(wParam), GET_WHEEL_DELTA_WPARAM(wParam), pt);
		bMessageHandled = TRUE;
		break;
	case WM_MOUSEMOVE:
		bMessageHandled = OnMouseMove(flag, pt);
		break;
	case WM_LBUTTONDOWN:
		bMessageHandled = OnLButtonDown(flag, pt);
		break;
	case WM_LBUTTONUP:
		bMessageHandled = OnLButtonUp(flag, pt);
		break;
	case WM_LBUTTONDBLCLK:
		bMessageHandled = OnLButtonDoubleClicked(flag, pt);
		break;
	case WM_RBUTTONDOWN:
		bMessageHandled = OnRButtonDown(flag, pt);
		break;
	case WM_RBUTTONUP:
		bMessageHandled = OnRButtonUp(flag, pt);
		break;
	case WM_RBUTTONDBLCLK:
		bMessageHandled = OnRButtonDoubleClicked(flag, pt);
		break;
	case WM_MBUTTONDOWN:
		bMessageHandled = OnMButtonDown(flag, pt);
		break;
	case WM_MBUTTONUP:
		bMessageHandled = OnMButtonUp(flag, pt);
		break;
	case WM_MBUTTONDBLCLK:
		bMessageHandled = OnMButtonDoubleClicked(flag, pt);
		break;
	}
	return bMessageHandled;
}

void UIControl::OnMouseEntered(UINT flag,const UIPoint &point) {}
void UIControl::OnMouseExited(UINT flag,const UIPoint &point) {}
BOOL UIControl::OnLButtonDown(UINT flag,const UIPoint &point) { return FALSE; }
BOOL UIControl::OnLButtonUp(UINT flag,const UIPoint &point) { return FALSE; }
BOOL UIControl::OnLButtonDoubleClicked(UINT flag,const UIPoint &point) { return FALSE; }
BOOL UIControl::OnRButtonDown(UINT flag,const UIPoint &point) { return FALSE; }
BOOL UIControl::OnRButtonUp(UINT flag,const UIPoint &point) { return FALSE; }
BOOL UIControl::OnRButtonDoubleClicked(UINT flag,const UIPoint &point) { return FALSE; }
BOOL UIControl::OnMButtonDown(UINT flag,const UIPoint &point) { return FALSE; }
BOOL UIControl::OnMButtonUp(UINT flag,const UIPoint &point) { return FALSE; }
BOOL UIControl::OnMButtonDoubleClicked(UINT flag,const UIPoint &point) { return FALSE; }
void UIControl::OnMouseWheel(UINT flag, short delta,const UIPoint &point) {}
BOOL UIControl::OnMouseMove(UINT flag,const UIPoint &point) { return TRUE; }

/************************ private methods *************************/
BOOL UIControl::IsMouseInControl(const UIPoint &pt) { return PtInRect(&frame_, pt); }

void UIControl::DispatchMouseMessage(UINT message, WPARAM wParam, LPARAM lParam) {
	// The bMessageHandled is FALSE, which means this message still needs to be passed
	// to its parent control
	if (FALSE == OnMouseMessage(message, wParam, lParam)) {
		UIControl *control = GetParent();
		if (control) {
			control->DispatchMouseMessage(message, wParam, lParam);
		}
	}
}

Alignment UIControl::GetAlignmentByString(const char *value) {
	Alignment alignment = AlignmentLeftTop;

	static BOOL init_alignment_list = FALSE;
	static std::vector<std::pair<const char *,Alignment>> alignments;

	if (! init_alignment_list) {
		alignments.push_back(std::make_pair("left_top",AlignmentLeftTop));
		alignments.push_back(std::make_pair("left_center",AlignmentLeftCenter));
		alignments.push_back(std::make_pair("left_bottom",AlignmentLeftBottom));
		alignments.push_back(std::make_pair("center_top",AlignmentCenterTop));
		alignments.push_back(std::make_pair("center",AlignmentCenter));
		alignments.push_back(std::make_pair("center_bottom",AlignmentCenterBottom));
		alignments.push_back(std::make_pair("right_top",AlignmentRightTop));
		alignments.push_back(std::make_pair("right_center",AlignmentRightCenter));
		alignments.push_back(std::make_pair("right_bottom",AlignmentRightBottom));
		alignments.push_back(std::make_pair("left",AlignmentLeft));
		alignments.push_back(std::make_pair("right",AlignmentRight));
		alignments.push_back(std::make_pair("top",AlignmentTop));
		alignments.push_back(std::make_pair("bottom",AlignmentBottom));
		alignments.push_back(std::make_pair("fill",AlignmentFill));
		init_alignment_list = TRUE;
	}

	for (size_t i = 0; i < alignments.size(); i++) {
		std::pair<const char *,Alignment> &pair = alignments[i];
		if(strcmp(pair.first,value) == 0) {
			alignment = pair.second;
			break;
		}
	}
	return alignment;
}

void UIControl::SendNotify(UINotifyEvent event) {
}

} // namespace HUI
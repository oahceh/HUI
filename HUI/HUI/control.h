#pragma once

#include <string>
#include "utils.h"
#include "window.h"
using namespace std;
using namespace HUI::Utils;

namespace HUI {

class UIWindow;
class UIControl : public IUINotify
{
public:
	UIControl();
	virtual ~UIControl() {};
	void SetAttribute(const char *name, const char *value);
public:
	/********************** Properties ************************/
	// Name property
	void SetName(const char *name);
	string GetName() const;

	// Tag property
	void SetTag(int tag);
	int GetTag() const;

	// Margin property
	void SetMargin(const UIMargin &margin);
	void SetMargin(int left, int top = 0, int right = 0, int bottom = 0);

	// Size property
	void SetSize(const UISize &size);
	void SetSize(int width, int height);
	UISize GetSize() const;

	// Alignment property
	void SetAlignment(Alignment alignment);
	Alignment GetAlignment() const;

	// Visibility property
	void SetVisibility(BOOL visibility);
	BOOL GetVisibility() const;

	// Enabled property
	void SetEnabled(BOOL enabled);
	BOOL IsEnabled() const;

	// Parent property
	void SetParent(UIControl *control);
	UIControl* GetParent() const;

	// Frame property，如果是容器控件，需要计算其子控件的布局，所以为需方法
	void SetFrame(const UIRect *rect);
	UIRect GetFrame() const;

	// Parent window property
	void SetParentWnd(UIWindow *pWnd);
	UIWindow* GetParentWnd();

	// 根据父控件的矩形区，计算控件的布局（即此控件的矩形区)
	virtual void DoLayout(const UIRect &rect);

	// Set to re-layout and re-render
	void SetNeedLayout();
	void SetNeedDisplay();

	// Set cursor type
	void SetCursor(const wchar_t *cursor);

	virtual void Render(const UIRect *rect);
	// Render, Needs to be override by sub-class
	virtual void RenderSelf(const UIRect *rect);
	// Search the focused element
	virtual UIControl* LookupMouseFocusedControl(const UIPoint &point);

	// Realization of IUINotify
	void Notify(UINotifyEvent event,UIControl *control);

	BOOL IsMouseInControl(const UIPoint &pt);

	/*********************** Message Process ****************************/
	void DispatchMouseMessage(UINT message, WPARAM wParam, LPARAM lParam);

	virtual	void	OnMouseEntered(UINT flag,const UIPoint &point);
	virtual	void	OnMouseExited(UINT flag,const UIPoint &point);

	virtual	BOOL	OnLButtonDown(UINT flag,const UIPoint &point);
	virtual	BOOL	OnLButtonUp(UINT flag,const UIPoint &point);
	virtual	BOOL	OnLButtonDoubleClicked(UINT flag,const UIPoint &point);

	virtual	BOOL	OnRButtonDown(UINT flag,const UIPoint &point);
	virtual	BOOL	OnRButtonUp(UINT flag,const UIPoint &point);
	virtual	BOOL	OnRButtonDoubleClicked(UINT flag,const UIPoint &point);

	virtual	BOOL	OnMButtonDown(UINT flag,const UIPoint &point);
	virtual	BOOL	OnMButtonUp(UINT flag,const UIPoint &point);
	virtual	BOOL	OnMButtonDoubleClicked(UINT flag,const UIPoint &point);

	virtual void	OnMouseWheel(UINT flag, short delta,const UIPoint &point);

	virtual	BOOL	OnMouseMove(UINT flag,const UIPoint &point);
protected:
	void SendNotify(UINotifyEvent event);
private:
	BOOL OnMouseMessage(UINT message,WPARAM wParam,LPARAM lParam);
	Alignment GetAlignmentByString(const char *value);
protected:
	string name_;
	int tag_;
	UIMargin margin_;
	UISize size_;
	Alignment alignment_;
	UIRect frame_;
	UIWindow *mp_parent_wnd;
	UIControl *parent_;
	BOOL visibility_;
	BOOL enabled_;
};

} // namespace HUI
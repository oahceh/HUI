/**
 * @file    base.h
 * @brief   基础的窗口类封装
 * @author  ksdjfdf <330773102@qq.com>
 * @date	2016-04-27
 */

#pragma once

#include <Windows.h>
#include <string>
using namespace std;

namespace HUI {


class UIWindowWnd
{
public:
	UIWindowWnd();
	virtual ~UIWindowWnd() {}
	// Get window handle
	HWND GetHWND() const { return hwnd_; }
	operator HWND() const { return hwnd_; }

	virtual const wchar_t *GetWindowClassName() const { return L"HUIWindowWnd"; }
	virtual const wchar_t *GetSuperClassName() const { return NULL; }

	// Regist window, invoke windows API
	BOOL RegisterWindowClass();
	BOOL RegisterSuperClass();

	// Create window, invoke windows API
	BOOL Create(HWND hwndParent, LPCTSTR pStrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu = NULL);
	BOOL Create(HWND hwndParent, LPCTSTR pStrName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);

	void ShowWindow(BOOL show = TRUE, BOOL focused = TRUE);
	void CenterWindow();
	void CloseWindow();

	void SetIcon(const wchar_t *icon_path);
	void SetIcon(int resource_id);
	void SetIcon(HICON icon);

	void SetWindowText(const wchar_t *text);
	void GetWindowText(std::wstring &text);

	void SetTimer(UINT id, UINT elapse, TIMERPROC proc = NULL);
	void KillTimer(UINT id);

	// Put the messages into the message queue, and this message will be sent to specific
	// window by meesage cycle<Asynchronous>
	BOOL PostMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
	// Invoke meesage handler of the specific window<Synchronous>
	LRESULT SendMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);

	void SetAsMainWindow() { bMainWindow_ = TRUE; }

	virtual BOOL AfterCreated();
	virtual BOOL OnEraseBkgnd();
	virtual void OnTimer(UINT id);
	virtual void OnSize(UINT type, int cx, int cy);
	virtual void OnGetMinMaxInfo(MINMAXINFO *lpMMI);
	virtual void OnDestroy();
public:
	// Message handle proc, it needs to be given during window registing
	static LRESULT CALLBACK _WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	// This base class simply process some basic window event, more window event proc is realized
	// in sub class
	virtual LRESULT HandleMessages(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	HWND hwnd_;
	BOOL bMainWindow_;
	WNDPROC old_window_proc_;
}; // class UIWindowWnd


} // namespace HUI
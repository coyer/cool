#pragma once

#include "../base/kstring.h"
#include "kuidef.h"
#include <windef.h>

class KCanvas;

class KUI_API KRealWndBase
{
	friend class KWinApp;
public:
	KRealWndBase();
	virtual ~KRealWndBase();

	virtual BOOL	CreateWnd(HINSTANCE hInst, LPCTSTR szCaption, HWND hParent, DWORD dwStyle, DWORD dwExStyle = 0, HICON hIcon = 0);
	virtual void	DestroyWindow();

	virtual void	OnRegisterWndClass(WNDCLASSEX & wcs);

	virtual void	OnCommand(int cmdID, int cmdEvent);
	virtual void	OnDraw(HDC hDC);
	virtual void	OnCreate();
	virtual void	OnDestroyWindow();

	virtual LRESULT	WndProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL	PreTranslateMsg(UINT message, WPARAM wParam, LPARAM lParam);
	virtual KCanvas*	CreateMainCanvas();

	virtual BOOL	SubclassWindow(HWND hWnd);
	virtual HWND	UnsubclassWindow();

	virtual BOOL	ShowWindow(int nCmdShow);
	virtual BOOL	UpdateWindow();

	virtual void	SetWindowCaption(KString caption);

	virtual ATOM	RegisterWndClass(HINSTANCE hInstance, LPCTSTR szWindowClass, HICON hIcon);

	virtual void		SetCanvas(KCanvas * pCanvas);
	virtual KCanvas*	GetCanvas();

	virtual void	OnSize(int cx, int cy);
	virtual void	OnWindowPosChanged();

	virtual void	OnSetFocus();
	virtual void	OnKillFocus();

	virtual void	OnMouseHover();
	virtual void	OnMouseLeave();

	virtual void	OnLButtonDown(UINT nFlags, KPoint point);
	virtual void	OnLButtonUp(UINT nFlags, KPoint point);
	virtual void	OnRButtonDown(UINT nFlags, KPoint point);
	virtual void	OnRButtonUp(UINT nFlags, KPoint point);
	virtual void	OnMouseMove(UINT nFlags, KPoint point);
	virtual void	OnMouseHeel(UINT nFlags, short zDelta, KPoint pt);
	virtual void	OnMouseHWheel(UINT nFlags, short zDelta, KPoint pt);
	virtual void	OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void	OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

	void	SetWindowPos(int x, int y, int cx, int cy, unsigned int flag);
	void	SetWindowPos(KRect & rcPos, unsigned int flag);
	KRect	GetWindowPos();
	void	CenterWindow();
	void	CenterWindow(KSize & size);

	void	SetCapture();
	void	ReleaseCapture();
	BOOL	HasCapture();

	void	SetCursor(HCURSOR cursor);
	KPoint	GetCursorPos();
	void	SetCursorPos(KPoint& pt);

	KPoint  CanvasPointToScreen(KPoint& pt);
	KRect	CanvasRectToScreen(KRect& rc);
	KPoint	ScreenPointToCanvas(KPoint& pt);
	KRect   ScreenRectToCanvas(KRect& rc);

	void	PostMessage(UINT msg, WPARAM wParam, LPARAM lParam);
	void	SendMessage(UINT msg, WPARAM wParam, LPARAM lParam);
protected:
	HWND		m_hWnd;
	KCanvas*	m_pCanvas;

	LONG	m_userSuper;
	WNDPROC	m_pfnSuper;	//for subclass.
};
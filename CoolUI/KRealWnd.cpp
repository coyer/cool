#include "stdafx.h"

#define KREALVIEW_CLASS_NAME		_T("KRealWnd")

LRESULT CALLBACK KRealWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	KRealWndBase* pView = NULL;

	if (message == WM_NCCREATE)
	{
		LPCREATESTRUCT pCD = (LPCREATESTRUCT)lParam;
		pView = (KRealWndBase*)pCD->lpCreateParams;
		::SetWindowLong(hWnd, GWL_USERDATA, (LONG)(pView));
		pView->WndProc(message, wParam, lParam);
		return DefWindowProc(hWnd, message, wParam, lParam); 
	}
	else
	{
		pView = (KRealWndBase*)::GetWindowLong(hWnd, GWL_USERDATA);
	}

	if (message == WM_NCDESTROY)
	{
		::SetWindowLong(hWnd, GWL_USERDATA, (LONG)0);
	}

	if (pView)
		return pView->WndProc(message, wParam, lParam);

	return DefWindowProc(hWnd, message, wParam, lParam);	
}

KRealWndBase::KRealWndBase()
{
	m_hWnd = 0;
	m_pfnSuper = 0;
	m_userSuper = 0;

	m_pCanvas = 0;
}

KRealWndBase::~KRealWndBase()
{
}

void KRealWndBase::SetCanvas(KCanvas * pCanvas)
{
	if (m_pCanvas == pCanvas)
		return;

	if (m_pCanvas)
		m_pCanvas->SetSafeHwnd(0);

	m_pCanvas = pCanvas;

	if (m_pCanvas)
		m_pCanvas->SetSafeHwnd(m_hWnd);
}

KCanvas* KRealWndBase::GetCanvas()
{
	return m_pCanvas;
}

KCanvas* KRealWndBase::CreateMainCanvas()
{
	return new KCanvas();
}

BOOL KRealWndBase::CreateWnd(HINSTANCE hInst, LPCTSTR szCaption, HWND hParent, DWORD dwStyle, DWORD dwExStyle, HICON hIcon)
{
	if(m_hWnd)
		return FALSE;

	WNDCLASS wc = {0};

	if (!GetClassInfo(hInst, KREALVIEW_CLASS_NAME, &wc))
	{
		if (0 == RegisterWndClass(hInst, KREALVIEW_CLASS_NAME, hIcon))
			return FALSE;
	}

	m_hWnd = ::CreateWindowEx(dwExStyle, KREALVIEW_CLASS_NAME, szCaption, dwStyle,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, hParent, NULL, hInst, this);

	if (0 == m_hWnd)
		return FALSE;

	if (m_pCanvas)
		m_pCanvas->SetSafeHwnd(m_hWnd);

	return TRUE;
}


void KRealWndBase::DestroyWindow()
{
	if (m_pCanvas)
	{
		m_pCanvas->DestroyView();
		m_pCanvas = 0;
	}

	if (m_hWnd)
	{
		::DestroyWindow(m_hWnd);
		m_hWnd = 0;
	}
}

void KRealWndBase::SetWindowCaption(KString caption)
{
	::SetWindowText(m_hWnd, caption);
}

BOOL KRealWndBase::ShowWindow(int nCmdShow)
{
	return ::ShowWindow(m_hWnd, nCmdShow);
}

BOOL KRealWndBase::UpdateWindow()
{
	return ::UpdateWindow(m_hWnd);
}

void KRealWndBase::OnRegisterWndClass(WNDCLASSEX& wcs)
{

}

ATOM KRealWndBase::RegisterWndClass(HINSTANCE hInstance, LPCTSTR szWindowClass, HICON hIcon)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= KRealWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= hIcon;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;//MAKEINTRESOURCE(IDC_TESTUI);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= 0;

	OnRegisterWndClass(wcex);

	return ::RegisterClassEx(&wcex);
}

void KRealWndBase::OnCommand(int cmdID, int cmdEvent)
{

		int wmId = cmdID;
		// ·ÖÎö²Ëµ¥Ñ¡Ôñ:
		switch (wmId)
		{
		case IDM_ABOUT:
			//::DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			::DestroyWindow(m_hWnd);
			break;
		}


}

void KRealWndBase::OnDraw(HDC hDC)
{
	if (m_pCanvas)
	{
		m_pCanvas->OnDraw(hDC);
	}
}

void KRealWndBase::OnCreate()
{
	SetCanvas(CreateMainCanvas());
}

void KRealWndBase::OnDestroyWindow()
{
	PostQuitMessage(0);
}

BOOL KRealWndBase::SubclassWindow(HWND hWnd)
{
	if (m_hWnd || m_pfnSuper)
		return FALSE;

	m_hWnd = hWnd;
	m_pfnSuper = (WNDPROC)::SetWindowLong(hWnd, GWLP_WNDPROC, (INT_PTR)KRealWndProc);
	if (0 == m_pfnSuper)
		return FALSE;

	m_userSuper = ::GetWindowLong(hWnd, GWL_USERDATA);
	::SetWindowLong(hWnd, GWL_USERDATA, (LONG)(this));

	if (m_pCanvas)
		m_pCanvas->SetSafeHwnd(m_hWnd);

	return TRUE;
}

HWND KRealWndBase::UnsubclassWindow()
{
	if (0 == m_pfnSuper)
		return 0;

	if (m_pCanvas)
		m_pCanvas->SetSafeHwnd(0);

	HWND hWnd = m_hWnd;
	m_hWnd = 0;

	SetWindowLong(hWnd, GWLP_WNDPROC, (INT_PTR)m_pfnSuper);
	SetWindowLong(hWnd, GWL_USERDATA, m_userSuper);
	m_pfnSuper = 0;
	m_userSuper = 0;

	return hWnd;
}

BOOL KRealWndBase::PreTranslateMsg(UINT message, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

KPoint LParam2Point(LPARAM lp)
{
	return KPoint((int)(short)LOWORD(lp), (int)(short)HIWORD(lp));
}

LRESULT	KRealWndBase::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (PreTranslateMsg(message, wParam, lParam))
		return TRUE;

	switch (message)
	{
	case WM_COMMAND:
		{
			int wmId, wmEvent;
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			OnCommand(wmId, wmEvent);
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint(m_hWnd, &ps);
			OnDraw(hdc);
			EndPaint(m_hWnd, &ps);
		}
		break;
	case WM_CREATE:
		OnCreate();
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, LParam2Point(lParam));
		break;
	case WM_DESTROY:
		OnDestroyWindow();		
		break;
	case WM_LBUTTONDOWN:
		OnLButtonDown(wParam, LParam2Point(lParam));
		break;
	case WM_LBUTTONUP:
		OnLButtonUp(wParam, LParam2Point(lParam));
		break;
	case WM_RBUTTONDOWN:
		OnRButtonDown(wParam, LParam2Point(lParam));
		break;
	case WM_RBUTTONUP:
		OnRButtonUp(wParam, LParam2Point(lParam));
		break;
	case WM_MOUSEWHEEL:
		OnMouseHeel(GET_KEYSTATE_WPARAM(wParam), GET_WHEEL_DELTA_WPARAM(wParam), LParam2Point(lParam));
		break;
#if (_WIN32_WINNT >= 0x0600)
	case WM_MOUSEHWHEEL:
		OnMouseHWheel(GET_KEYSTATE_WPARAM(wParam), GET_WHEEL_DELTA_WPARAM(wParam), LParam2Point(lParam));
		break;
#endif
	case WM_KEYDOWN:
		OnKeyDown(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_KEYUP:
		OnKeyUp(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_SIZE:
		OnSize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_WINDOWPOSCHANGED:
		OnWindowPosChanged();
		break;
	case WM_SETFOCUS:
		OnSetFocus();
		break;
	case WM_KILLFOCUS:
		OnKillFocus();
		break;
	case WM_MOUSEHOVER:
		OnMouseHover();
		break;
	case WM_MOUSELEAVE:
		OnMouseLeave();
		break;
	default:
		return DefWindowProc(m_hWnd, message, wParam, lParam);
	}
	return 0;
}

void KRealWndBase::OnLButtonDown(UINT nFlags, KPoint point)
{
	if (m_pCanvas)	m_pCanvas->OnLButtonDown(nFlags, point);
}

void KRealWndBase::OnLButtonUp(UINT nFlags, KPoint point)
{
	if (m_pCanvas)	m_pCanvas->OnLButtonUp(nFlags, point);
}

void KRealWndBase::OnRButtonDown(UINT nFlags, KPoint point)
{
	if (m_pCanvas)	m_pCanvas->OnRButtonDown(nFlags, point);
}

void KRealWndBase::OnRButtonUp(UINT nFlags, KPoint point)
{
	if (m_pCanvas)	m_pCanvas->OnRButtonUp(nFlags, point);
}

void KRealWndBase::OnMouseMove(UINT nFlags, KPoint point)
{
	if (m_pCanvas)	m_pCanvas->OnMouseMove(nFlags, point);
}

void KRealWndBase::OnMouseHeel(UINT nFlags, short zDelta, KPoint pt)
{
	if (m_pCanvas)	m_pCanvas->OnMouseWheel(nFlags, zDelta, pt);
}

void KRealWndBase::OnMouseHWheel(UINT nFlags, short zDelta, KPoint pt)
{
	if (m_pCanvas)	m_pCanvas->OnMouseHWheel(nFlags, zDelta, pt);
}

void KRealWndBase::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pCanvas)	m_pCanvas->OnKeyDown(nChar, nRepCnt, nFlags);
}

void KRealWndBase::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pCanvas)	m_pCanvas->OnKeyUp(nChar, nRepCnt, nFlags);
}

void KRealWndBase::OnSize(int cx, int cy)
{
	if (0 == m_pCanvas)
		return;

	m_pCanvas->SetPosition(0, 0, cx, cy);
}

void KRealWndBase::OnWindowPosChanged() {
	//if (m_pCanvas)		m_pCanvas->OnWindowPosChanged();
}

void KRealWndBase::OnSetFocus()
{
	if (m_pCanvas)	m_pCanvas->OnSetFocus();
}

void KRealWndBase::OnKillFocus()
{
	if (m_pCanvas)	m_pCanvas->OnKillFocus();
}

void KRealWndBase::OnMouseHover()
{
	if (m_pCanvas)	m_pCanvas->OnMouseHover();
}

void KRealWndBase::OnMouseLeave()
{
	if (m_pCanvas)	m_pCanvas->OnMouseMoveOut();
}

void KRealWndBase::SetWindowPos(int x, int y, int cx, int cy, unsigned int flag = SWP_NOZORDER | SWP_NOACTIVATE)
{
	::SetWindowPos(m_hWnd, NULL, x, y, cx, cy, flag);
}

void KRealWndBase::SetWindowPos(KRect & rcPos, unsigned int flag = SWP_NOZORDER | SWP_NOACTIVATE)
{
	::SetWindowPos(m_hWnd, NULL, rcPos.left, rcPos.top, rcPos.Width(), rcPos.Height(), flag);
}

KRect KRealWndBase::GetWindowPos()
{
	KRect rc;
	::GetWindowRect(m_hWnd, &rc);

	return rc;
}

void KRealWndBase::CenterWindow()
{
	KRect rcScreen;
	KRect rcWnd;
	::GetWindowRect(m_hWnd, &rcWnd);

	CenterWindow(rcWnd.Size());
}

void KRealWndBase::CenterWindow(KSize & size)
{
	HMONITOR hMonitor = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
	if(NULL == hMonitor)
		return;

	KRect rcScreen;
	MONITORINFO mi = {0};
	mi.cbSize = sizeof(mi);
	::GetMonitorInfo(hMonitor, &mi);
	rcScreen = mi.rcWork;

	KRect rcBound;
	rcBound.left = rcScreen.left + (rcScreen.Width() - size.cx) / 2;
	rcBound.right = rcBound.left + size.cx;

	rcBound.top = rcScreen.top + (rcScreen.Height() - size.cy) / 2;
	rcBound.bottom = rcBound.top + size.cy;

	rcBound.MakeInBound(rcScreen);

	SetWindowPos(rcBound);
}

void KRealWndBase::SetCapture()
{
	::SetCapture(m_hWnd);
}

void KRealWndBase::ReleaseCapture()
{
	::ReleaseCapture();
}

BOOL KRealWndBase::HasCapture()
{
	return ::GetCapture() == m_hWnd;
}

void KRealWndBase::SetCursor(HCURSOR cursor)
{
	::SetCursor(cursor);
}

KPoint KRealWndBase::GetCursorPos()
{
	KPoint pt;
	::GetCursorPos(&pt);

	return pt;
}

void KRealWndBase::SetCursorPos(KPoint& pt)
{
	::SetCursorPos(pt.x, pt.y);
}

KPoint KRealWndBase::CanvasPointToScreen(KPoint& pt)
{
	KPoint ret = pt;
	::ClientToScreen(m_hWnd, &ret);
	return ret;
}

KRect KRealWndBase::CanvasRectToScreen(KRect& rc)
{
	KPoint pt1 = rc.LeftTop();
	KPoint pt2 = rc.RightBottom();

	::ClientToScreen(m_hWnd, &pt1);
	::ClientToScreen(m_hWnd, &pt2);

	KRect ret;
	ret.SetRect(pt1, pt2);

	return ret;
}

KPoint KRealWndBase::ScreenPointToCanvas(KPoint& pt)
{
	KPoint ret = pt;
	::ScreenToClient(m_hWnd, &ret);
	return ret;
}

KRect KRealWndBase::ScreenRectToCanvas(KRect& rc)
{
	KPoint pt1 = rc.LeftTop();
	KPoint pt2 = rc.RightBottom();

	::ScreenToClient(m_hWnd, &pt1);
	::ScreenToClient(m_hWnd, &pt2);

	KRect ret;
	ret.SetRect(pt1, pt2);

	return ret;
}

void KRealWndBase::PostMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	::PostMessage(m_hWnd, msg, wParam, lParam);
}

void KRealWndBase::SendMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	::SendMessage(m_hWnd, msg, wParam, lParam);
}

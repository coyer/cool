#include "stdafx.h"

KCanvas::KCanvas()
{
	m_bModified = FALSE;
	m_hWnd = 0;
	m_skin = KCanvasSkinDefault::Instance();
}

KCanvas::~KCanvas()
{
	if (m_rgnClip)
		::DeleteObject(m_rgnClip);
}

KString	KCanvas::GetViewTypeName()
{
	return g_sKCanvas;
}

void KCanvas::SetSafeHwnd(HWND hWnd)
{
	m_hWnd = hWnd;
}

HWND KCanvas::GetSafeHwnd()
{
	return m_hWnd;
}

KCanvas* KCanvas::GetCanvas()
{
	return this;
}

void KCanvas::Invalidate()
{
	RECT rc;
	::GetClientRect(m_hWnd, &rc);
	::InvalidateRect(m_hWnd, &rc, TRUE);
	m_bModified = TRUE;
}

void KCanvas::InvalidataRect(KRect &rcPos)
{
	if (0 == m_rgnClip)
	{
		m_rgnClip = ::CreateRectRgnIndirect(&rcPos);
	}
	else
	{
		HRGN hDst, hTmp;
		hTmp = ::CreateRectRgnIndirect(&rcPos);
		hDst = ::CreateRectRgn(0, 0, 1, 1);
		::CombineRgn(hDst, m_rgnClip, hTmp, RGN_OR);
		::DeleteObject(m_rgnClip);
		::DeleteObject(hTmp);
		m_rgnClip = hDst;
	}
	m_bModified = TRUE;
}

void KCanvas::SetModified()
{
	m_bModified = TRUE;
}

BOOL KCanvas::IsModified()
{
	return m_bModified;
}

void KCanvas::OnDraw(HDC hDC)
{
	if (!m_memDC.GetSafeHdc())
	{
		m_memDC.CreateMemDC(hDC, m_rcPos.Width(), m_rcPos.Height());
		CleanRgn();
		m_bModified = TRUE;
	}
	else
	{
		KSize sz = m_memDC.GetDCSize();
		if (sz.cx != m_rcPos.Width() || sz.cy != m_rcPos.Height())
		{
			m_memDC.ChangeMemDCSize(m_rcPos.Width(), m_rcPos.Height());
			CleanRgn();
			m_bModified = TRUE;
		}
	}

	if (m_bModified)
	{
		if (m_rgnClip)
			::SelectClipRgn(m_memDC, m_rgnClip);

		m_skin->DrawView(m_memDC, this);
		m_bModified = FALSE;

		KViewGroup::OnDraw(m_memDC);

		if (m_rgnClip)
			::SelectClipRgn(m_memDC, 0);

		CleanRgn();
	}

	m_memDC.BitbltToDC(hDC, 0, 0, m_rcPos.Width(), m_rcPos.Height());
}

void KCanvas::CleanRgn()
{
	if (m_rgnClip)
	{
		::DeleteObject(m_rgnClip);
		m_rgnClip = 0;
	}
}
#include "stdafx.h"

KMemDC::KMemDC()
{
	m_bitmap = 0;
	m_memDC  = 0;
}

KMemDC::~KMemDC()
{
	DestroyMemDC();
}

BOOL KMemDC::CreateMemDC(HDC hDC, int cx, int cy)
{
	DestroyMemDC();

	m_dcSize.cx = cx;
	m_dcSize.cy = cy;
	m_bitmap = ::CreateCompatibleBitmap(hDC, cx, cy);
	if (0 == m_bitmap)
		return FALSE;

	m_memDC = ::CreateCompatibleDC(hDC);
	::SelectObject(m_memDC, m_bitmap);

	return TRUE;
}

void KMemDC::DestroyMemDC()
{
	if (m_memDC)
	{
		::DeleteDC(m_memDC);
		m_memDC = 0;
	}

	if (m_bitmap)
	{
		::DeleteObject(m_bitmap);
		m_bitmap = 0;
	}

	m_dcSize.cx = 0;
	m_dcSize.cy = 0;
}

HDC	KMemDC::GetSafeHdc()
{
	return m_memDC;
}

KSize KMemDC::GetDCSize()
{
	return m_dcSize;
}

KMemDC::operator HDC()
{
	return m_memDC;
}

BOOL KMemDC::ChangeMemDCSize(int cx, int cy)
{
	if (0 == m_memDC)
		return FALSE;

	if (m_dcSize.cx == cx && m_dcSize.cy == cy)
		return TRUE;

	HBITMAP hbmp = ::CreateCompatibleBitmap(m_memDC, cx, cy);
	if (0 == hbmp)
		return FALSE;

	::SelectObject(m_memDC, hbmp);
	::DeleteObject(m_bitmap);
	m_bitmap = hbmp;

	m_dcSize.cx = cx;
	m_dcSize.cy = cy;

	return TRUE;
}

void KMemDC::BitbltToDC(HDC hDst, int left, int top, int width, int height)
{
	::BitBlt(hDst, left, top, m_dcSize.cx, m_dcSize.cy, m_memDC, 0, 0, SRCCOPY);
}

void KMemDC::BitbltFromDC(HDC hSrc, int left, int top, int width, int height)
{
	::BitBlt(m_memDC, 0, 0, m_dcSize.cx, m_dcSize.cy, hSrc, left, top, SRCCOPY);
}
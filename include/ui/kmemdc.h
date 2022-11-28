#pragma once

#include "kuidef.h"

class KUI_API KMemDC
{
public:
	KMemDC();
	~KMemDC();

	operator HDC();

	BOOL	CreateMemDC(HDC hDC, int cx, int cy);
	void	DestroyMemDC();
	HDC		GetSafeHdc();
	BOOL	ChangeMemDCSize(int cx, int cy);
	KSize	GetDCSize();

	void	BitbltToDC(HDC hDst, int left, int top, int width, int height);
	void	BitbltFromDC(HDC hDst, int left, int top, int width, int height);
	
protected:
	HDC			m_memDC;
	HBITMAP		m_bitmap;
	KSize		m_dcSize;
};
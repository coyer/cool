#pragma once

#include "kuidef.h"

#include "kmemdc.h"


class KUI_API KCanvas : public KViewGroup
{
public:
	KCanvas();
	virtual ~KCanvas();

	virtual		KString	GetViewTypeName();

	void		SetSafeHwnd(HWND hWnd);
	virtual		HWND	GetSafeHwnd();
	virtual		KCanvas*	GetCanvas();

	virtual		void	Invalidate();
	virtual		void	InvalidataRect(KRect &rcPos);

	virtual		void	OnDraw(HDC hDC);

	void		SetModified();
	BOOL		IsModified();

	void		CleanRgn();

protected:
	HWND		m_hWnd;
	KMemDC		m_memDC;
	BOOL		m_bModified;

	HRGN		m_rgnClip;
};
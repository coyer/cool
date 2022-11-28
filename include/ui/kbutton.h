#pragma once

#include "kuidef.h"
#include "klabel.h"

typedef void (*ButtonExecProc)(BOOL lbutton, void* pVoid);

class KUI_API KButtonBase : public KLabel
{
public:
	KButtonBase();
	virtual ~KButtonBase();

	virtual		void	OnLButtonUp(UINT nFlags, KPoint pt);
	virtual		void	OnRButtonUp(UINT nFlags, KPoint pt);

	virtual void	OnLClick();
	virtual void	OnRClick();

	void	SetClickEvent(ButtonExecProc proc, void* pData);

protected:
	ButtonExecProc	m_ClickProc;
	void*			m_ClickProcData;
	BOOL			m_bMouseDown;
};

class KUI_API KButton : public KButtonBase
{
public:
	KButton();
	virtual ~KButton();

	virtual		KString		GetViewTypeName();
};
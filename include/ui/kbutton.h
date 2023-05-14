#pragma once

#include "kuidef.h"
#include "klabel.h"

typedef void (*ButtonExecProc)(BOOL lbutton, void* pVoid);

class KUI_API KButtonBase : public KLabel
{
public:
	KButtonBase();
	virtual ~KButtonBase();

	virtual void	OnLClick(UINT nFlags, KPoint pt);
	virtual void	OnRClick(UINT nFlags, KPoint pt);

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
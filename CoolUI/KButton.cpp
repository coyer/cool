#include "stdafx.h"


KButtonBase::KButtonBase()
{
	m_ClickProc = 0;
	m_bMouseDown = FALSE;
	m_skin = KButtonSkinDefault::Instance();
}

KButtonBase::~KButtonBase()
{
}

void KButtonBase::OnLClick()
{
	if (m_ClickProc)
		m_ClickProc(TRUE, m_ClickProcData);
}

void KButtonBase::OnRClick()
{
	if (m_ClickProc)
		m_ClickProc(FALSE, m_ClickProcData);
}

void KButtonBase::SetClickEvent(ButtonExecProc proc, void* pData)
{
	m_ClickProc  = proc;
	m_ClickProcData = pData;
}

void KButtonBase::OnLButtonUp(UINT nFlags, KPoint pt)
{
	BOOL bClick = FALSE;
	if (IsMouseLDown())
	{
		bClick = TRUE;
	}

	KView::OnLButtonUp(nFlags, pt);

	if (bClick)
		OnLClick();
}

void KButtonBase::OnRButtonUp(UINT nFlags, KPoint pt)
{
	BOOL bClick = FALSE;
	if (IsMouseRDown())
	{
		bClick = TRUE;		
	}

	KView::OnLButtonUp(nFlags, pt);

	if (bClick)
		OnRClick();
}

////////////////////////////////////////////////////////////////////////////////////
KButton::KButton()
{
}

KButton::~KButton()
{
}


KString	KButton::GetViewTypeName()
{
	return g_sKButton;
}
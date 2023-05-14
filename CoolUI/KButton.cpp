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

void KButtonBase::OnLClick(UINT nFlags, KPoint pt)
{
	if (m_ClickProc) {
		m_ClickProc(TRUE, m_ClickProcData);
	}
	else {
		KView::OnLClick(nFlags, pt);
	}
}

void KButtonBase::OnRClick(UINT nFlags, KPoint pt)
{
	if (m_ClickProc) {
		m_ClickProc(FALSE, m_ClickProcData);
	}
	else {
		KView::OnRClick(nFlags, pt);
	}
}

void KButtonBase::SetClickEvent(ButtonExecProc proc, void* pData)
{
	m_ClickProc  = proc;
	m_ClickProcData = pData;
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
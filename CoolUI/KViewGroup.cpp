#include "stdafx.h"

KViewGroup::KViewGroup()
{
	m_pLastOn = 0;
	m_pLayout = 0;
	m_skin = KWndGroupSkinDefault::Instance();

	m_pLayout = new KLayoutSimple();
}

KViewGroup::~KViewGroup()
{

}

KString	KViewGroup::GetViewTypeName()
{
	return g_sKViewGroup;
}

void KViewGroup::DestroyView()
{
	RemoveAll(TRUE);

	__super::DestroyView();
}

int	KViewGroup::AddChild(KView* pView)
{
	pView->SetParent(this);
	return m_childs.Add(pView);
}

int	KViewGroup::GetVisibleCount()
{
	int ret = 0;
	for(int i = m_childs.GetCount() - 1; i >= 0; i--)
	{
		KView* pView = (KView*)m_childs.GetAt(i);
		if (pView->GetVisibility() != KView::GONE)
			ret++;
	}

	return ret;
}

int KViewGroup::GetChildCount()
{
	return m_childs.GetCount();
}

KView* KViewGroup::GetChildAt(int index)
{
	return (KView*)m_childs.GetAt(index);
}

int	KViewGroup::FindChildIndex(KView* pView)
{
	int n = m_childs.GetCount();
	for(int i = 0 ;i < n; i++)
	{
		if (pView == m_childs.GetAt(i))
			return i;
	}
	return -1;
}

void KViewGroup::RemoveChild(int index, BOOL destroy)
{
	KView* pView = (KView*)m_childs.GetAt(index);
	if (0 == pView)
		return;

	m_childs.RemoveAt(index);
	if (destroy)
	{
		pView->DestroyView();
	}

	Invalidate();
}

void KViewGroup::RemoveAll(BOOL destroy)
{
	KArrayPtr tmp;
	m_childs.Switch(tmp);

	if (destroy)
	{
		int n = tmp.GetCount();
		for(int i = 0 ;i < n; i++)
		{
			KView* pView = (KView*)tmp.GetAt(i);
			pView->DestroyView();
		}
	}

	Invalidate();
}

void KViewGroup::OnSize(int cx, int cy)
{
	Layout();
}

void KViewGroup::OnPositionChanged()
{
	Layout();
}

KSize KViewGroup::OnMeasure(KRect rcMax)
{
	if (m_pLayout)
		return m_pLayout->Measure(this, rcMax);

	return KView::OnMeasure(rcMax);
}

void KViewGroup::OnSkinSystemChanged(KSkinSystem* pSkinSystem)
{
	int n = m_childs.GetCount();
	for(int i = 0 ;i < n; i++)
	{
		KView* pView = (KView*)m_childs.GetAt(i);
		pView->OnSkinSystemChanged(pSkinSystem);
	}
	KView::OnSkinSystemChanged(pSkinSystem);

	Invalidate();
}

BOOL KViewGroup::OnCreate()
{
	if (!KView::OnCreate())
		return FALSE;

	int n = m_childs.GetCount();
	for(int i = 0 ;i < n; i++)
	{
		KView* pView = (KView*)m_childs.GetAt(i);
		pView->OnCreate();
	}

	Invalidate();
	return TRUE;
}

void KViewGroup::OnDestroy()
{
	m_pLastOn = 0;
	RemoveAll(TRUE);

	KView::OnDestroy();
}

void KViewGroup::OnLButtonDown(UINT nFlags, KPoint pt)
{
	KView* pView = HitChildTest(pt);
	if (pView)
		pView->OnLButtonDown(nFlags, pt);
	else
		__super::OnLButtonDown(nFlags, pt);
}

void KViewGroup::OnLButtonUp(UINT nFlags, KPoint pt)
{
	KView* pView = HitChildTest(pt);
	if (pView)
		pView->OnLButtonUp(nFlags, pt);
	else
		__super::OnLButtonUp(nFlags, pt);
}

void KViewGroup::OnRButtonDown(UINT nFlags, KPoint pt)
{
	KView* pView = HitChildTest(pt);
	if (pView)
		pView->OnRButtonDown(nFlags, pt);
	else
		__super::OnRButtonDown(nFlags, pt);
}

void KViewGroup::OnRButtonUp(UINT nFlags, KPoint pt)
{
	KView* pView = HitChildTest(pt);
	if (pView)
		pView->OnRButtonUp(nFlags, pt);
	else
		__super::OnRButtonUp(nFlags, pt);
}

void KViewGroup::OnMouseMove(UINT nFlags, KPoint pt)
{
	KView* pView = HitChildTest(pt);
	if (pView)
	{
		if (pView == m_pLastOn)
		{
			pView->OnMouseMove(nFlags, pt);
		}
		else
		{ 
			if (m_pLastOn)
				m_pLastOn->OnMouseMoveOut();

			m_pLastOn = pView;
			m_pLastOn->OnMouseMoveIn(nFlags, pt);
		}	
	}
	else
	{
		if (m_pLastOn)
		{
			m_pLastOn->OnMouseMoveOut();
			m_pLastOn = 0;
		}

		__super::OnMouseMove(nFlags, pt);
	}
}

void KViewGroup::OnMouseMoveIn(UINT nFlags, KPoint pt)
{
	KView* pView = HitChildTest(pt);
	if (pView)
	{
		m_pLastOn = pView;
		m_pLastOn->OnMouseMoveIn(nFlags, pt);
	}
	else
	{
		__super::OnMouseMoveIn(nFlags, pt);
	}
}

void KViewGroup::OnMouseMoveOut()
{
	if (m_pLastOn)
	{
		m_pLastOn->OnMouseMoveOut();
		m_pLastOn = 0;
	}

	__super::OnMouseMoveOut();
}

void KViewGroup::OnMouseHover()
{
	if (m_pLastOn)
	{
		m_pLastOn->OnMouseHover();
	}

	__super::OnMouseHover();
}

void KViewGroup::OnMouseHWheel(UINT nFlags, short zDelta, KPoint pt)
{
	if (m_pLastOn)
	{
		m_pLastOn->OnMouseHWheel(nFlags, zDelta, pt);
		m_pLastOn = 0;
	}

	__super::OnMouseHWheel(nFlags, zDelta, pt);
}

void KViewGroup::OnMouseWheel(UINT nFlags, short zDelta, KPoint pt)
{
	if (m_pLastOn)
	{
		m_pLastOn->OnMouseWheel(nFlags, zDelta, pt);
		m_pLastOn = 0;
	}

	__super::OnMouseWheel(nFlags, zDelta, pt);
}

void KViewGroup::Layout()
{
	if (m_pLayout)
		m_pLayout->Layout(this);

	Invalidate();
}

KView* KViewGroup::HitChildTest(KPoint& pt)
{
	int n = m_childs.GetCount();
	for(int i=0; i<n; i++)
	{
		KView* pView = (KView*)m_childs.GetAt(i);
		if (pView->HitTest(pt))
			return pView;
	}

	return NULL;
}

void KViewGroup::OnDraw(HDC hDC)
{
	__super::OnDraw(hDC);

	int n = m_childs.GetCount();
	for(int i=0; i<n; i++)
	{
		KView* pView = (KView*)m_childs.GetAt(i);
		pView->OnDraw(hDC);
	}
}

void KViewGroup::SetLayout(IKViewLayout* pLayout)
{
	if (m_pLayout)
		m_pLayout->Release();
	m_pLayout = pLayout;

	Invalidate();
}

IKViewLayout* KViewGroup::GetLayout()
{
	return m_pLayout;
}

KView* KViewGroup::GetChildByID(KString id)
{
	int n = m_childs.GetCount();
	for(int i = 0 ;i < n; i++)
	{
		KView* pView = (KView*)(m_childs.GetAt(i));
		
		KView* pRet = pView->GetChildByID(id);
		if (pRet)
			return pRet;
	}
	return __super::GetChildByID(id);
}

//////////////////////////////////////////////////////////////////////////////
KLayoutSimple::KLayoutSimple(BOOL vert)
{
	m_VertMode = vert;
}

void KLayoutSimple::Release()
{
	delete this;
}

void KLayoutSimple::SetLayoutMode(BOOL vertmode)
{
	m_VertMode = vertmode;
}

BOOL KLayoutSimple::GetLayoutMode()
{
	return m_VertMode;
}

KSize KLayoutSimple::Measure(KViewGroup* pGroup, KRect rcMax)
{
	return rcMax.Size();
}

void KLayoutSimple::Layout(KViewGroup* pGroup)
{
	if (m_VertMode)
		_LayoutV(pGroup);
	else
		_LayoutH(pGroup);
}

void KLayoutSimple::_LayoutH(KViewGroup* pGroup)
{
	KRect rc = pGroup->GetPosition();
	if (rc.IsEmpty())
		return;

	int visibleCount = pGroup->GetVisibleCount();
	if (visibleCount == 0)
		return;

	int nCount = pGroup->GetChildCount();

	int w = rc.Width() / visibleCount;
	KRect rcPos;
	rcPos.top = rc.top;
	rcPos.bottom = rc.bottom;
	rcPos.left = rc.left;
	rcPos.right = rcPos.left + w;
	for(int i=0; i<nCount - 1; i++)
	{
		KView* pView = pGroup->GetChildAt(i);
		if (pView->GetVisibility() == KView::GONE)
			continue;

		pView->SetPosition(rcPos);

		rcPos.left = rcPos.right;
		rcPos.right = rcPos.left + w;
	}

	rcPos.right = rcPos.right;
	KView* pView = pGroup->GetChildAt(nCount - 1);
	pView->SetPosition(rcPos);
}

void KLayoutSimple::_LayoutV(KViewGroup* pGroup)
{
	KRect rc = pGroup->GetPosition();
	if (rc.IsEmpty())
		return;

	int visibleCount = pGroup->GetVisibleCount();
	if (visibleCount == 0)
		return;

	int nCount = pGroup->GetChildCount();
	int h = rc.Width() / visibleCount;
	KRect rcPos;
	rcPos.top = rc.top;
	rcPos.bottom = rc.top + h;
	rcPos.left = rc.left;
	rcPos.right = rc.right;
	for(int i=0; i<nCount - 1; i++)
	{
		KView* pView = pGroup->GetChildAt(i);
		if (pView->GetVisibility() == KView::GONE)
			continue;
		
		pView->SetPosition(rcPos);

		rcPos.top = rcPos.bottom;
		rcPos.bottom = rcPos.top + h;
	}

	rcPos.bottom = rcPos.top + h;
	KView* pView = pGroup->GetChildAt(nCount - 1);
	pView->SetPosition(rcPos);
}
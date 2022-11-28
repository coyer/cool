#include "stdafx.h"

#define		VIEW_ATTR_DISABLE		0x0001
#define		VIEW_ATTR_CLICK_DISABLE	0x0002

#define		VIEW_ATTR_FOCUSED		0x0008

#define		VIEW_ATTR_MOUSEOVER		0x0100
#define		VIEW_ATTR_MOUSE_LDOWN	0x0200
#define		VIEW_ATTR_MOUSE_RDOWN	0x0400




KViewlayoutParam::KViewlayoutParam()
{
	marginLeft = 0;
	marginRight = 0;
	marginTop = 0;
	marginBottom = 0;

	paddingLeft = 0;
	paddingRight = 0;
	paddingTop = 0;
	paddingBottom = 0;

	Original = 0;
	Weight = 0;
}

BOOL KViewlayoutParam::fromString(KString style)
{
	return FALSE;
}

KString KViewlayoutParam::toString()
{
	KString s;
	return s;
}

//////////////////////////////////////////////////////////////////////////

KView::KView()
{
	m_AutoDelete = TRUE;
	m_Parent = 0;
	m_dwAttr = 0;
	m_pFont = 0;
	memset(&m_EventListener, 0, sizeof(ViewEventListener));
	m_LayoutParam = NULL;

	m_skin = KWndSkinDefault::Instance();
}

KView::~KView()
{
}

KString	KView::GetViewTypeName()
{
	return g_sKView;
}

void KView::SetAutoDelete(BOOL autoDelete)
{
	m_AutoDelete = autoDelete;
}

BOOL KView::IsAutoDelete()
{
	return m_AutoDelete;
}

void KView::DestroyView()
{
	OnDestroy();

	if (m_AutoDelete)
	{
		delete this;
	}
}

BOOL KView::OnCreate()
{
	return TRUE;
}

void KView::OnDestroy()
{
}

void KView::OnLButtonDown(UINT nFlags, KPoint pt)
{
	SetViewAttr(VIEW_ATTR_MOUSE_LDOWN, TRUE);
	if (m_EventListener.OnMouseLDown)
	{
		m_EventListener.OnMouseLDown(this, nFlags, pt);
	}
}

void KView::OnLButtonUp(UINT nFlags, KPoint pt)
{
	BOOL bClick = FALSE;
	if (IsMouseLDown())
	{
		bClick = TRUE;
	}

	SetViewAttr(VIEW_ATTR_MOUSE_LDOWN, FALSE);
	if (m_EventListener.OnMouseLUp)
	{
		m_EventListener.OnMouseLUp(this, nFlags, pt);
	}

	if (bClick)
		OnLClick(nFlags, pt);
}


void KView::OnLClick(UINT nFlags, KPoint pt)
{
	if (m_EventListener.OnMouseLClick)
	{
		m_EventListener.OnMouseLClick(this, nFlags, pt);
	}
}

void KView::OnRClick(UINT nFlags, KPoint pt)
{
	if (m_EventListener.OnMouseRClick)
	{
		m_EventListener.OnMouseRClick(this, nFlags, pt);
	}
}

void KView::OnRButtonDown(UINT nFlags, KPoint pt)
{
	SetViewAttr(VIEW_ATTR_MOUSE_RDOWN, TRUE);

	if (m_EventListener.OnMouseRDown)
	{
		m_EventListener.OnMouseRDown(this, nFlags, pt);
	}
}

void KView::OnRButtonUp(UINT nFlags, KPoint pt)
{
	BOOL bClick = FALSE;
	if (IsMouseRDown())
	{
		bClick = TRUE;
	}

	SetViewAttr(VIEW_ATTR_MOUSE_RDOWN, FALSE);
	if (m_EventListener.OnMouseRUp)
	{
		m_EventListener.OnMouseRUp(this, nFlags, pt);
	}

	if (bClick)
		OnRClick(nFlags, pt);
}

void KView::OnMouseMove(UINT nFlags, KPoint pt)
{
	if (m_EventListener.OnMouseMove)
	{
		m_EventListener.OnMouseMove(this, nFlags, pt);
	}
}

void KView::OnMouseMoveIn(UINT nFlags, KPoint pt)
{
	SetViewAttr(VIEW_ATTR_MOUSEOVER, TRUE);

	if (m_EventListener.OnMouseIn)
	{
		m_EventListener.OnMouseIn(this, nFlags, pt);
	}
}

void KView::OnMouseMoveOut()
{
	SetViewAttr(VIEW_ATTR_MOUSEOVER | VIEW_ATTR_MOUSE_LDOWN | VIEW_ATTR_MOUSE_RDOWN, FALSE);

	if (m_EventListener.OnMouseIn)
	{
		KPoint pt(0, 0);
		m_EventListener.OnMouseOut(this, 0, pt);
	}
}

void KView::OnMouseHover()
{
}

void KView::OnMouseHWheel(UINT nFlags, short zDelta, KPoint pt)
{
}

void KView::OnMouseWheel(UINT nFlags, short zDelta, KPoint pt)
{
	
}

void KView::OnLButtonDblClk(UINT nFlags, KPoint point)
{
}

void KView::OnRButtonDblClk(UINT nFlags, KPoint point)
{
}


void KView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
}

void KView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
}

void KView::OnSize(int cx, int cy)
{
}

void KView::OnPositionChanged()
{
}

void KView::OnSetFocus()
{
	
}

void KView::OnKillFocus()
{
	SetViewAttr(VIEW_ATTR_FOCUSED, FALSE);
}

void KView::SetParent(KView* pView)
{
	m_Parent = pView;
}

KView* KView::GetParent()
{
	return m_Parent;
}

HWND KView::GetSafeHwnd()
{
	if (m_Parent)
		return m_Parent->GetSafeHwnd();

	return 0;
}

int KView::GetChildCount()
{
	return 0;
}

KView* KView::GetChildAt(int index)
{
	return 0;
}

KCanvas* KView::GetCanvas()
{
	if (m_Parent)
		return m_Parent->GetCanvas();

	return 0;
}

BOOL KView::IsEnable()
{
	return !(m_dwAttr & VIEW_ATTR_DISABLE);
}

BOOL KView::IsMouseOver()
{
	return (m_dwAttr & VIEW_ATTR_MOUSEOVER);
}

BOOL KView::IsMouseLDown()
{
	return (m_dwAttr & VIEW_ATTR_MOUSE_LDOWN);
}

BOOL KView::IsMouseRDown()
{
	return (m_dwAttr & VIEW_ATTR_MOUSE_RDOWN);
}

void KView::SetFocus(BOOL focused)
{
	if (focused)
	{
		if (IsFocused())
			return;

		SetViewAttr(VIEW_ATTR_FOCUSED, focused);
		OnSetFocus();
	}
	else
	{
		if (!IsFocused())
			return;

		SetViewAttr(VIEW_ATTR_FOCUSED, focused);
		OnKillFocus();
	}
}

BOOL KView::IsFocused()
{
	return (m_dwAttr & VIEW_ATTR_FOCUSED);
}

void KView::SetClickEnable(BOOL enable)
{
	SetViewAttr(VIEW_ATTR_CLICK_DISABLE, !enable);
}

BOOL KView::IsClickEnable()
{
	return !(m_dwAttr & VIEW_ATTR_CLICK_DISABLE);
}

KView::Visibility KView::GetVisibility()
{
	return m_visibility;
}

void KView::SetVisibility(Visibility show)
{
	if (m_visibility == show)
		return;

	m_visibility = show;

	OnStatusChanged();

	KView* parent = GetParent();
	if (parent)
		parent->Relayout();
}

void KView::SetEnable(BOOL enable)
{
	SetViewAttr(VIEW_ATTR_DISABLE, !enable);
}


void KView::SetViewAttr(DWORD dwAttr, BOOL enable)
{
	BOOL t = m_dwAttr & dwAttr;
	if (t && enable)
		return;
	if (!t && !enable)
		return;

	if (enable)
		m_dwAttr |= dwAttr;
	else
		m_dwAttr &= ~dwAttr;

	OnStatusChanged();
}

KRect KView::GetPosition()
{
	return m_rcPos;
}

void KView::SetPosition(int l, int t, int r, int b, BOOL relayout)
{
	KRect rc(l, t, r, b);
	SetPosition(rc, relayout);
}

void KView::SetPosition(KRect rect, BOOL relayout)
{
	if (m_rcPos == rect)
		return;

	m_rcPos = rect;
	
	OnLayout();

	OnPositionChanged();

	if (relayout)
		Redraw();
}

KSize KView::OnMeasure(KRect rcMax)
{
	return rcMax.Size();
}

void KView::OnLayout()
{

}

void KView::Redraw()
{
	KCanvas* pCanvas = GetCanvas();
	if (pCanvas)
		pCanvas->InvalidataRect(m_rcPos);
}

void KView::Relayout()
{
	
	OnLayout();
}

void KView::SetText(KString strText)
{
	m_strText = strText;
}

KString KView::GetText()
{
	return m_strText;
}

void KView::SetFont(KFont* ft)
{
	m_pFont = ft;
}

KFont* KView::GetFont()
{
	return m_pFont;
}

BOOL KView::HitTest(KPoint& pt)
{
	return m_rcPos.IsPtInRect(pt);
}

void KView::SetSkin(KViewSkin* pSkin)
{
	m_skin = pSkin;
}

KViewSkin* KView::GetSkin()
{
	return m_skin;
}

void KView::OnDraw(HDC hDC)
{
	m_skin->DrawView(hDC, this);
}

void KView::OnSkinSystemChanged(KSkinSystem* pSkinSystem)
{
	KViewSkin* pSkin = pSkinSystem->GetSkin(GetViewTypeName());

	if (pSkin)
		m_skin = pSkin;
}

void KView::OnStatusChanged()
{

}

void KView::Invalidate()
{
	KCanvas* pCanvas = GetCanvas();
	
	GetCanvas()->Invalidate();
}

KString KView::GetID()
{
	return m_id;
}

KView* KView::GetChildByID(KString id)
{
	if (id == m_id)
		return this;

	return NULL;
}

void KView::SetEventListener(ViewEventListener listener)
{
	memset(&m_EventListener, 0, sizeof(ViewEventListener));
	m_EventListener.pData = listener.pData;
	m_EventListener.OnMouseLDown = listener.OnMouseLDown;
	m_EventListener.OnMouseLUp = listener.OnMouseLUp;
	m_EventListener.OnMouseLClick = listener.OnMouseLClick;
	m_EventListener.OnMouseRDown = listener.OnMouseRDown;
	m_EventListener.OnMouseRUp = listener.OnMouseRUp;
	m_EventListener.OnMouseRClick = listener.OnMouseRClick;
	m_EventListener.OnMouseMove = listener.OnMouseMove;
	m_EventListener.OnMouseIn = listener.OnMouseIn;
	m_EventListener.OnMouseOut = listener.OnMouseOut;
	m_EventListener.OnWheelVert = listener.OnWheelVert;
	m_EventListener.OnWheelHorz = listener.OnWheelHorz;
	m_EventListener.OnKeyDown = listener.OnKeyDown;
	m_EventListener.OnKeyUp = listener.OnKeyUp;
}

ViewEventListener* KView::GetEventListener()
{
	return &m_EventListener;
}

KViewlayoutParam* KView::GetLayoutParam()
{
	if (NULL == m_LayoutParam)
		m_LayoutParam = OnCreateLayoutParam();

	return m_LayoutParam;
}

KViewlayoutParam* KView::OnCreateLayoutParam()
{
	return new KViewlayoutParam();
}


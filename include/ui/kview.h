#pragma once

#include "kuidef.h"
#include "KFont.h"
#include "KSkin.h"
#include "../base/kstring.h"


typedef void (*ViewMouseEventProc)(KView* view, UINT nFlags, KPoint pt);
typedef void (*ViewWheelEventProc)(KView* view, int data, UINT nFlags, KPoint pt);
typedef void (*ViewKeyEventProc)(KView* view, UINT nChar, UINT nRepCnt, UINT nFlags);

typedef struct KUI_API tagViewEventListener {
	void* pData;

	ViewMouseEventProc	OnMouseLDown;
	ViewMouseEventProc	OnMouseLUp;
	ViewMouseEventProc	OnMouseLClick;
	ViewMouseEventProc	OnMouseLDblClick;
	ViewMouseEventProc	OnMouseRDown;
	ViewMouseEventProc	OnMouseRUp;
	ViewMouseEventProc	OnMouseRClick;
	ViewMouseEventProc	OnMouseRDblClick;
	ViewMouseEventProc	OnMouseMove;
	ViewMouseEventProc	OnMouseIn;
	ViewMouseEventProc	OnMouseOut;
	ViewWheelEventProc  OnWheelVert;
	ViewWheelEventProc	OnWheelHorz;

	ViewKeyEventProc	OnKeyDown;
	ViewKeyEventProc	OnKeyUp;

	tagViewEventListener()
	{
		memset(this, 0, sizeof(ViewEventListener));
	}
}ViewEventListener;

class KUI_API KViewlayoutParam
{
public:
	int marginLeft;
	int marginRight;
	int marginTop;
	int marginBottom;

	int paddingLeft;
	int paddingRight;
	int paddingTop;
	int paddingBottom;

	int Original;
	int Weight;

	BOOL absMode;	//
	int left;
	int top;
	int right;
	int bottom;

	KViewlayoutParam();

	BOOL fromString(KString style);
	KString toString();
};

class KUI_API KView
{
	friend class KCanvas;
	friend class KViewGroup;
	
public:

	typedef enum tagVisibility {
		VISABLE = 0,
		INVISABLE,
		GONE
	}Visibility;

	KView();
	virtual ~KView();

	virtual		KString	GetViewTypeName();

	virtual		void	DestroyView();

	virtual		KView*	GetParent();
	virtual		void	SetParent(KView* pView);
	virtual		HWND	GetSafeHwnd();

	virtual		int		GetChildCount();
	virtual		KView*	GetChildAt(int index);
	virtual		KView*	GetChildByID(KString id);

	virtual		KCanvas*	GetCanvas();


	virtual		void	Redraw();
	virtual		void	Relayout();
	virtual		BOOL	HitTest(KPoint& pt);

	KString GetID();

	KRect	GetPosition();	
	void	SetPosition(int l, int t, int r, int b, BOOL relayout = TRUE);
	void	SetPosition(KRect rect, BOOL relayout = TRUE);

	BOOL	IsEnable();
	BOOL	IsMouseOver();
	BOOL	IsMouseLDown();
	BOOL	IsMouseRDown();
	void	SetEnable(BOOL enable);
	void	SetFocus(BOOL focused);
	BOOL	IsFocused();
	void	SetClickEnable(BOOL enable);
	BOOL	IsClickEnable();

	Visibility	GetVisibility();
	void	SetVisibility(Visibility show);	


	void	SetViewAttr(DWORD dwAttr, BOOL enable);

	void	SetText(KString strText);
	KString	GetText();

	void	SetFont(KFont* pft);
	KFont*	GetFont();

	void	SetAutoDelete(BOOL autoDelete = FALSE);
	BOOL	IsAutoDelete();

	void		SetSkin(KViewSkin* pSkin);
	KViewSkin*	GetSkin();

	void		Invalidate();

	void		SetEventListener(ViewEventListener listener);
	ViewEventListener*	GetEventListener();

	KViewlayoutParam*	GetLayoutParam();

public:
	virtual		BOOL	OnCreate();
	virtual		void	OnDestroy();
	virtual		void	OnLButtonDown(UINT nFlags, KPoint pt);
	virtual		void	OnLButtonUp(UINT nFlags, KPoint pt);
	virtual		void	OnRButtonDown(UINT nFlags, KPoint pt);
	virtual		void	OnRButtonUp(UINT nFlags, KPoint pt);
	virtual		void	OnMouseMove(UINT nFlags, KPoint pt);
	virtual		void	OnMouseMoveIn(UINT nFlags, KPoint pt);
	virtual		void	OnMouseMoveOut();
	virtual		void	OnMouseHover();
	virtual		void	OnMouseHWheel(UINT nFlags, short zDelta, KPoint pt);
	virtual		void	OnMouseWheel(UINT nFlags, short zDelta, KPoint pt);
	virtual		void	OnLButtonDblClk(UINT nFlags, KPoint point);
	virtual		void	OnRButtonDblClk(UINT nFlags, KPoint point);
	virtual		void	OnLClick(UINT nFlags, KPoint pt);
	virtual		void	OnRClick(UINT nFlags, KPoint pt);

	virtual		void	OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual		void	OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual		void	OnSize(int cx, int cy);
	virtual		void	OnPositionChanged();

	virtual		void	OnSkinSystemChanged(KSkinSystem* pSkinSystem);
	virtual		void    OnStatusChanged();

	virtual		KSize	OnMeasure(KRect rcMax);
	virtual		void	OnLayout();

	virtual		void	OnDraw(HDC hDC);

	virtual		void	OnSetFocus();
	virtual		void	OnKillFocus();

	virtual		KViewlayoutParam*	OnCreateLayoutParam();
protected:
	KString		m_id;
	KView*		m_Parent;
	DWORD		m_dwAttr;
	Visibility	m_visibility;
	KRect		m_rcPos;
	KString		m_strText;	
	BOOL		m_AutoDelete;
	KFont*		m_pFont;
	KViewSkin*	m_skin;

	ViewEventListener	m_EventListener;
	KViewlayoutParam*	m_LayoutParam;
};
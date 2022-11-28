#pragma once

#include "kuidef.h"



class KUI_API KViewGroup : public KView
{
	friend struct IKViewLayout;

public:
	KViewGroup();
	virtual		~KViewGroup();
	virtual		KString	GetViewTypeName();
	virtual		void	DestroyView();

	virtual		int		AddChild(KView* pView);
	virtual		int		GetChildCount();
	virtual		int		GetVisibleCount();

	virtual		KView*	GetChildAt(int index);
	virtual		KView*	GetChildByID(KString id);
	virtual		KView*  HitChildTest(KPoint& pt);

	virtual		int		FindChildIndex(KView* pView);
	virtual		void	RemoveChild(int index, BOOL destroy);
	virtual		void	RemoveAll(BOOL destroy);

	virtual		void	OnSize(int cx, int cy);
	virtual		void	OnPositionChanged();

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
	virtual		void	Layout();
	virtual		KSize	OnMeasure(KRect rcMax);
	virtual		void	OnSkinSystemChanged(KSkinSystem* pSkinSystem);

	virtual		void	OnDraw(HDC hDC);

	void			SetLayout(IKViewLayout* pLayout);
	IKViewLayout*	GetLayout();


protected:
	KView*			m_pLastOn;
	KArrayPtr		m_childs;
	IKViewLayout*	m_pLayout;
};

struct IKViewLayout
{
	virtual		void	Release() = 0;
	virtual		KSize	Measure(KViewGroup* pGroup, KRect rcMax) = 0;
	virtual		void	Layout(KViewGroup* pGroup) = 0;
};

class KUI_API KLayoutSimple : public IKViewLayout
{
public:
	KLayoutSimple(BOOL vert = FALSE);

	virtual		void	Release();
	virtual		KSize	Measure(KViewGroup* pGroup, KRect rcMax);
	virtual		void	Layout(KViewGroup* pGroup);

	void	SetLayoutMode(BOOL vertmode);
	BOOL	GetLayoutMode();

protected:
	virtual		void	_LayoutH(KViewGroup* pGroup);
	virtual		void	_LayoutV(KViewGroup* pGroup);

protected:
	BOOL	m_VertMode;
};

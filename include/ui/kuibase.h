#pragma once

#include "kuidef.h"

class KUI_API KPoint : public tagPOINT
{
public:
	KPoint();
	KPoint(int tx, int ty);
	KPoint(KPoint& pt);
	~KPoint();

	KPoint& operator = (KPoint& pt);
	BOOL operator == (KPoint pt);
};

class KUI_API KSize : public tagSIZE
{
public:
	KSize();
	KSize(int w, int h);
	KSize(KSize& sz);
	~KSize();

	KSize& operator = (KSize& sz);
	BOOL operator == (KSize sz);

	int		Width();
	int		Height();
};

class KUI_API KRect : public tagRECT
{
public:
	KRect();
	KRect(KRect& rc);
	KRect(int l, int t, int r, int b);

	~KRect();
	KRect&	operator = (KRect& rc);
	KRect&  operator = (RECT& rc);
	BOOL	operator == (KRect rect);

	void	Empty();
	BOOL	IsEmpty();

	KPoint	LeftTop();
	KPoint	RightBottom();
	KPoint	Center();

	KSize	Size();

	int		Width();
	int		Height();

	void	SetRect(int l, int t, int r, int b);
	void	SetRect(KPoint& pt, KSize& size);
	void	SetRect(KPoint& lt, KPoint& rb);

	BOOL	IsPtInRect(KPoint& pt);

	void	MakeInBound(KRect& bound);
	void	MakeOutBound(KRect& bound);
};

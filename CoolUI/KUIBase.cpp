#include "stdafx.h"

KPoint::KPoint()
{
	this->x = 0;
	this->y = 0;
}

KPoint::KPoint(int tx, int ty)
{
	this->x = tx;
	this->y = ty;
}

KPoint::KPoint(KPoint& pt)
{
	x = pt.x;
	y = pt.y;
}

KPoint::~KPoint()
{
}

KPoint& KPoint::operator = (KPoint& pt)
{
	x = pt.x;
	y = pt.y;

	return *this;
}

BOOL KPoint::operator == (KPoint pt)
{
	return this->x == pt.x && this->y == pt.y;
}

///////////////////////////////////////////////////////////////////
KSize::KSize()
{
	this->cx = 0;
	this->cy = 0;
}

KSize::KSize(int w, int h)
{
	cx = w;
	cy = h;
}

KSize::KSize(KSize& sz)
{
	cx = sz.cx;
	cy = sz.cy;
}

KSize::~KSize()
{
}

KSize& KSize::operator = (KSize& sz)
{
	cx = sz.cx;
	cy = sz.cy;

	return *this;
}

BOOL KSize::operator == (KSize sz)
{
	return this->cx == sz.cx && this->cy == sz.cy;
}

int KSize::Width()
{
	return cx;
}

int KSize::Height()
{
	return cy;
}

///////////////////////////////////////////////////////////////////////////
KRect::KRect()
{
	Empty();
}

KRect::KRect(int l, int t, int r, int b)
{
	SetRect(l, t, r, b);
}

KRect::KRect(KRect& rc)
{
	left = rc.left;
	top = rc.top;
	right = rc.right;
	bottom = rc.bottom;
}

KRect::~KRect()
{
}

KRect&	KRect::operator = (KRect& rc)
{
	left = rc.left;
	top = rc.top;
	right = rc.right;
	bottom = rc.bottom;

	return *this;
}

KRect& KRect::operator=(RECT& rc)
{
	left = rc.left;
	top = rc.top;
	right = rc.right;
	bottom = rc.bottom;

	return *this;
}

BOOL KRect::operator == (KRect rect)
{
	return (this->left == rect.left
			&& this->top == rect.top
			&& this->right == rect.right
			&& this->bottom == rect.bottom);
}

void KRect::Empty()
{
	left = 0;
	top = 0;
	right = 0;
	bottom = 0;
}

BOOL KRect::IsEmpty()
{
	if (top == bottom && left == right)
		return TRUE;

	return FALSE;
}

KPoint KRect::LeftTop()
{
	return KPoint(left, top);
}

KPoint KRect::RightBottom()
{
	return KPoint(right, bottom);
}

KPoint KRect::Center()
{
	return KPoint((left + right) / 2, (top + bottom) / 2);
}

KSize KRect::Size()
{
	return KSize(right - left, bottom - top);
}

int	KRect::Width()
{
	return right - left;
}

int	KRect::Height()
{
	return bottom - top;
}

void KRect::SetRect(int l, int t, int r, int b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}

void KRect::SetRect(KPoint& pt, KSize& size)
{
	left = pt.x;
	top = pt.y;
	right = left + size.cx;
	bottom = top + size.cy;
}

void KRect::SetRect(KPoint& lt, KPoint& rb)
{
	left = lt.x;
	top = lt.y;
	right = rb.x;
	bottom = rb.y;
}

BOOL KRect::IsPtInRect(KPoint& pt)
{
	return (pt.x >= left && pt.x < right && pt.y >= top && pt.y < bottom);
}

void KRect::MakeInBound(KRect& bound)
{
	if (left < bound.left)
		left = bound.left;

	if (right > bound.right)
		right = bound.right;

	if (top < bound.top)
		top = bound.top;

	if (bottom > bound.bottom)
		bottom = bound.bottom;
}

void KRect::MakeOutBound(KRect& bound)
{
	if (left > bound.left)
		left = bound.left;

	if (right < bound.right)
		right = bound.right;

	if (top > bound.top)
		top = bound.top;

	if (bottom < bound.bottom)
		bottom = bound.bottom;
}


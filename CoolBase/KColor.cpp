#include "StdAfx.h"
#include <math.h>
#include "../include/base/kcolor.h"
#include "../include/base/kdeft.h"

#include <tchar.h>

///////////////////////////////////////////////////////////////////////
//public functions.

double HuetoRGB(double m1, double m2, double h) 
{ 
	if (h < 0)   
		h += 1.0; 
	if (h > 1)   
		h -= 1.0; 
	if (6.0*h < 1) 
		return (m1+(m2-m1)*h*6.0); 
	if (2.0*h < 1) 
		return m2; 
	if (3.0*h < 2.0) 
		return (m1+(m2-m1)*((2.0/3.0)-h)*6.0); 
	return m1; 
}



//////////////////////////////////////////////////////////////////////
KColorRGB::KColorRGB()
{
	 r = g = b = 0;
}

KColorRGB::KColorRGB(int _r, int _g, int _b)
{
	r = _r;
	g = _g;
	b = _b;
}

KColorRGB::KColorRGB(const COLORREF& rgb)
{
	r = (const char)(rgb && 0xFF);
	g = (const char)((rgb >> 8) && 0xFF);
	b = (const char)((rgb >> 16) && 0xFF);
}

KColorRGB::KColorRGB(const KRGB& rgb)
{
	r = rgb.r;
	g = rgb.g;
	b = rgb.b;
}

KColorRGB::KColorRGB(const KColorHSL& hsl)
{
	double m1, m2;
	double dr, dg, db;

	if (hsl.s == 0)   
	{ 
		dr = dg = db = hsl.l; 
	}   
	else   
	{ 
		if (hsl.l <= 0.5) 
			m2 = hsl.l * (1.0 + hsl.s); 
		else 
			m2 = hsl.l + hsl.s - hsl.l * hsl.s; 
		m1 = 2.0 * hsl.l -m2; 
		dr = HuetoRGB(m1, m2, hsl.h + 1.0/3.0); 
		dg = HuetoRGB(m1, m2, hsl.h); 
		db = HuetoRGB(m1, m2, hsl.h - 1.0/3.0); 
	} 
	r = (const char)(dr * 255);
	g = (const char)(dg * 255);
	b = (const char)(db * 255); 
}

KColorRGB::operator COLORREF()
{
	return ToNumber();
}

COLORREF KColorRGB::GetColor()
{
	return ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)));
}

unsigned long KColorRGB::ToNumber()
{
	return ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)));
}

KColorHSL KColorRGB::ToColorHSL()
{
	return KColorHSL(*this);
}

KString	KColorRGB::ToString(RGB_Print_Type type)
{
	KString str;
	switch(type)
	{
	case HEX_VALUE:
		str.Format(_T("0x%X"), ToNumber());
		break;
	case DEC_VALUE:
		str.Format(_T("%l"), ToNumber());
		break;
	case VAL_COMMA_VAL:
		str.Format(_T("%d, %d, %d"), r, g, b);
		break;
	case BARCKET_RBG:
		str.Format(_T("(%d, %d, %d)"), r, b, g);
		break;
	case VAL_RBG:
		str.Format(_T("%d, %d, %d"), r, b, g);
		break;
	case INFOR_DETAIL:
		str.Format(_T("(R, G, B) =(%d, %d, %d) "), r, g, b);
	case BARCKET_COMMA_BARCKET:
	default:
		str.Format(_T("(%d, %d, %d)"), r, g, b);
		break;
	}
	return str;
}


///////////////////////////////////////////////////////////////////
KColorHSL::KColorHSL()
{
	h = 1.0f;
	s = 0.0f;
	l = 0.0f;
}

KColorHSL::KColorHSL(double _h, double _s, double _l)
{
	h = _h;
	s = _s;
	l = _l;
}

KColorHSL::KColorHSL(int H, int S, int L, int K)
{
	h = 1.0 * H / K;
	s = 1.0 * S / K;
	l = 1.0 * L / K;
}

KColorHSL::KColorHSL(const KHSL& hsl)
{
	h = hsl.h;
	s = hsl.s;
	l = hsl.l;
}

KColorHSL::KColorHSL(const KRGB& rgb)
{
	unsigned char nmin, nmax;
	nmin = K_Min3(rgb.r, rgb.g, rgb.b);
	nmax = K_Max3(rgb.r, rgb.g, rgb.b);

	double  ddif = (double)(nmax - nmin);
	double  dsum = (double)(nmax + nmin);

	l = dsum / 510.0f; 
	s = 0.0f; 
	h = 1.0f;

	if (nmax != nmin) 
	{ 
		double rnorm = (nmax - rgb.r) / ddif;   
		double gnorm = (nmax - rgb.g) / ddif; 
		double bnorm = (nmax - rgb.b) / ddif; 

		s = (l <= 0.5f) ? (ddif / dsum) : (ddif / (510.0f - dsum)); 

		if (rgb.r == nmax) h = 60.0f * (6.0f + bnorm - gnorm); 
		if (rgb.g == nmax) h = 60.0f * (2.0f + rnorm - bnorm); 
		if (rgb.b == nmax) h = 60.0f * (4.0f + gnorm - rnorm); 
		if (h > 360.0f) h = h - 360.0f;
		h /= 360.0;
	} 
}

KColorHSL::KColorHSL(const COLORREF& rgb)
{
	KColorRGB nrgb(rgb);
	this->KColorHSL::KColorHSL(nrgb);
}

KColorRGB KColorHSL::ToColorRGB()
{
	return KColorRGB(*this);
}

COLORREF KColorHSL::GetColor()
{
	KColorRGB rgb(*this);
	return rgb.GetColor();
}

COLORREF KColorHSL::GetColor(double _s, double _l)
{
	KColorHSL hsl(h, _s, _l);
	KColorRGB rgb(hsl);
	return rgb.GetColor();
}

KString KColorHSL::ToString(int type, int K)
{
	KString str;
	str.Format(_T("HUE = %d, Saturation = %d, Luminance= %d "), (int)(h * K), (int)(s * K), (int)(l * K));

	return str;
}

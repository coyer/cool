#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : kcolor.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : RGB or HSL are both color type. this class give some common function.

*******************************************************************************************/

#include "kdef.h"
#include "KString.h"

#define		KColorHSL_K_WINDOWS		240
#define		KColorHSL_K_NORMAL		255

typedef struct tagKRGB
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}KRGB;

typedef struct tagKHSL
{
	double h;
	double s;
	double l;
}KHSL;

class KColorHSL;
class KAPI KColorRGB : public tagKRGB
{
public:
	enum RGB_Print_Type
	{
		BARCKET_COMMA_BARCKET	= 0,	//(R, G, B)
		VAL_COMMA_VAL,					//R, G, B
		HEX_VALUE,						//0xFFFFFF
		DEC_VALUE,						//323423
		BARCKET_RBG,					//(R, B, G)
		VAL_RBG,						//R, B, G
		INFOR_DETAIL,					//R = , G= , B=
		UNKNOW
	};
public:
	KColorRGB();
	KColorRGB(int _r, int _g, int _b);
	KColorRGB(const COLORREF& rgb);
	KColorRGB(const KRGB& KRGB);
	KColorRGB(const KColorHSL& hsl);

	operator COLORREF();
	unsigned long	ToNumber();
	KColorHSL		ToColorHSL();
	KString			ToString(RGB_Print_Type type = BARCKET_COMMA_BARCKET);

	COLORREF		GetColor();
};

class KAPI KColorHSL : public tagKHSL
{
public:
	KColorHSL();
	KColorHSL(double _h, double _s, double _l);
	KColorHSL(int H, int S, int L, int K);
	KColorHSL(const KHSL& hsl);
	KColorHSL(const KRGB& rgb);
	KColorHSL(const COLORREF& rgb);

	KColorRGB	ToColorRGB();
	COLORREF	GetColor();
	COLORREF	GetColor(double _s, double _l);

	KString		ToString(int type, int K);
};
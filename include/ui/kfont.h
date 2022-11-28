#pragma once

#include "kuidef.h"

class KUI_API KFont
{
public:
	KFont();
	~KFont();

	int		size;
	TCHAR	name[64];
	COLORREF color;
	BOOL	bold;
	BOOL	itilic;
};
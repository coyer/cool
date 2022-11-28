#include "stdafx.h"

KFont::KFont()
{
	size = 14;
	memcpy(name, "tohoma", 6);
	color = RGB(0, 0, 0);
	bold = FALSE;
	itilic = FALSE;

}

KFont::~KFont()
{
}
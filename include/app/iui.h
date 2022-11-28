#pragma once

#include "../base/kinc.h"


struct IKButton
{
	virtual HWND		GetMainWnd() = 0;
	virtual KString		GetTitle() = 0;
	virtual KString		GetAppPath(int pathType) = 0;
};
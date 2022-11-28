#pragma once

#include "../base/kinc.h"


struct IKFrameWnd;

struct IKApp
{
	virtual HWND		GetMainWnd() = 0;
	virtual KString		GetTitle() = 0;
	virtual KString		GetAppPath(int pathType) = 0;
	virtual IKFrameWnd*	GetFrameWnd() = 0;
};

struct IKFrameWnd
{
	virtual HWND		GetMainWnd() = 0;
};
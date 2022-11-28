// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

static HMODULE g_hInstance = 0;
ULONG_PTR	g_gdiplusToken;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

			g_hInstance = hModule;

			Gdiplus::GdiplusStartupInput gdiplusStartupInput;
			Gdiplus::GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);
		}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		{
			Gdiplus::GdiplusShutdown(g_gdiplusToken);
			g_hInstance = 0;
		}
		break;
	}
	return TRUE;
}


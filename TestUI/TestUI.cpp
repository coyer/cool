// TestUI.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TestUI.h"
#include "TestApp.h"

CTestApp theApp;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	//UNREFERENCED_PARAMETER(hPrevInstance);
	//UNREFERENCED_PARAMETER(lpCmdLine);

	theApp.SetAppTitle(_T("MyTest app."));
	if (!theApp.InitInstance(hInstance, nCmdShow))
		return FALSE;

	theApp.ExecuteCmdLine(lpCmdLine);
	theApp.Run();

	theApp.ExitInstance();

	return 0;
}

//
//
//// “关于”框的消息处理程序。
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	UNREFERENCED_PARAMETER(lParam);
//	switch (message)
//	{
//	case WM_INITDIALOG:
//		return (INT_PTR)TRUE;
//
//	case WM_COMMAND:
//		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//		{
//			EndDialog(hDlg, LOWORD(wParam));
//			return (INT_PTR)TRUE;
//		}
//		break;
//	}
//	return (INT_PTR)FALSE;
//}

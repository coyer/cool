// dllmain.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include <afxwin.h>
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE KPluginDemo2DLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// ���ʹ�� lpReserved���뽫���Ƴ�
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("KPluginDemo2.DLL ���ڳ�ʼ��!\n");
		
		// ��չ DLL һ���Գ�ʼ��
		if (!AfxInitExtensionModule(KPluginDemo2DLL, hInstance))
			return 0;

		// ���� DLL ���뵽��Դ����
		// ע��: �������չ DLL ��
		//  MFC ���� DLL (�� ActiveX �ؼ�)��ʽ���ӵ���
		//  �������� MFC Ӧ�ó������ӵ�������Ҫ
		//  �����д� DllMain ���Ƴ������������һ��
		//  �Ӵ���չ DLL �����ĵ����ĺ����С�ʹ�ô���չ DLL ��
		//  ���� DLL Ȼ��Ӧ��ʽ
		//  ���øú����Գ�ʼ������չ DLL������
		//  CDynLinkLibrary ���󲻻ḽ�ӵ�
		//  ���� DLL ����Դ���������������ص�
		//  ���⡣

		new CDynLinkLibrary(KPluginDemo2DLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("KPluginDemo2.DLL ������ֹ!\n");

		// �ڵ�����������֮ǰ��ֹ�ÿ�
		AfxTermExtensionModule(KPluginDemo2DLL);
	}
	return 1;   // ȷ��
}

CPlugin	theApp;

//;	must return KAPP_VERSION, otherwise this dll will not be loaded.
int	DllGetInfo(KDllInfo* pInfo)
{
	pInfo->auther = _T("coyer@126.com");
	pInfo->buildtime = __TIME__;
	pInfo->desc = _T("Image view plugin.");
	pInfo->help = _T("");

	pInfo->mainVer = 1;
	pInfo->subVer = 1;
	pInfo->type = 0;

	return KAPP_VERSION;
}

int DllInit(IKApp* pApp, int appVer)
{
	theApp.m_pKApp = pApp;
	pApp->RegisterPlugin(&theApp);

	return 1;
}

void DllUninit()
{
	theApp.m_pKApp->UnregisterPlugin(&theApp);
}

// KImageView.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "KImageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CKImageViewApp



// Ψһ��һ�� CKImageViewApp ����

CKImageViewApp theApp;


// CKImageViewApp ��ʼ��

BOOL CKImageViewApp::InitInstance()
{
	CWinApp::InitInstance();

	
	return TRUE;
}

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

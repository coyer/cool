#include "stdafx.h"

KWinApp::KWinApp()
{
	m_pMainWnd = 0;
	m_hInstance = 0;
	m_hAccelTable = 0;

	m_pRender = 0;
}

KWinApp::~KWinApp()
{
}

BOOL KWinApp::InitInstance (HINSTANCE hInstance, int nCmdShow)
{
	if (m_hInstance)
		return FALSE;

	m_hInstance = hInstance;

	LoadKResource();
	m_pMainWnd = CreateMainWnd(nCmdShow);

	if (m_pMainWnd)
	{
		m_pMainWnd->ShowWindow(nCmdShow);
		m_pMainWnd->UpdateWindow();
	}

	return TRUE;
}

void KWinApp::Run()
{
	int idleCount = 0;
	MSG msg = {0};
	m_hAccelTable = CreateAccelTable();

    while( msg.message != WM_QUIT )         //ʹ��whileѭ��   
    {
		if (!PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (m_pRender)
			{
				ReanderWnd();   //������Ⱦ
			}

			if (!ProcessTask())
			{
				idleCount++;
				if (idleCount > 10)
				{
					idleCount = 0;
					OnIdle();
				}
			}
			continue;
		}
		//GetMessage(&msg, NULL, 0, 0);

		if (m_hAccelTable && (TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg)))
			continue;

		TranslateMessage( &msg );       //���������Ϣת��Ϊ�ַ���Ϣ   
		DispatchMessage( &msg );        //�ú����ַ�һ����Ϣ�����ڳ���   
    }
}

void KWinApp::ExitInstance()
{
	if (m_pMainWnd)
	{
		m_pMainWnd->DestroyWindow();
		DeleteMainWnd(m_pMainWnd);
		m_pMainWnd = 0;
	}
}

void KWinApp::CloseApp()
{
	m_pMainWnd->PostMessage(WM_COMMAND, MAKEWPARAM(0, IDM_EXIT, 0), 0);
}

KRealWndBase* KWinApp::CreateMainWnd(int nCmdShow)
{
	KRealWndBase* pWnd = new KRealWndBase();
	if (!pWnd->CreateWnd(GetInstance(), GetAppTitle().c_str(), 0, WS_OVERLAPPEDWINDOW, 0, GetIcon(0)))
	{
		delete pWnd;
		pWnd = 0;
	}

	return pWnd;
}

void KWinApp::DeleteMainWnd(KRealWndBase* pWnd)
{
	if (pWnd)
		delete pWnd;
}

BOOL KWinApp::ExecuteCmdLine(KString strCmdLine)
{
	return TRUE;
}

void KWinApp::OnIdle()
{
}

void KWinApp::ReanderWnd()
{
}

HACCEL KWinApp::CreateAccelTable()
{
	return 0;
}

HINSTANCE KWinApp::GetInstance()
{
	return m_hInstance;
}

KString	KWinApp::GetAppTitle()
{
	return m_AppTitle;
}

void KWinApp::SetAppTitle(KString title)
{
	m_AppTitle = title;
	if (m_pMainWnd)
		m_pMainWnd->SetWindowCaption(m_AppTitle);
}

void KWinApp::SetRenderObject(IRenderObject* pRender)
{
	m_pRender = pRender;
}

HICON KWinApp::GetIcon(int mode)
{
	return NULL;
}

BOOL KWinApp::ProcessTask()
{

	return FALSE;
}

BOOL KWinApp::LoadKResource()
{
	return TRUE;
}
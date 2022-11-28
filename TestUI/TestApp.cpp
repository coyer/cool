#include "StdAfx.h"
#include "TestApp.h"


void ButtonClickExecProc(KView* view, UINT nFlags, KPoint pt)
{
	KButton* pBtn = (KButton*)view;
	KString strMsg;
	strMsg.Format(_T("You click me, button text = %s"), pBtn->GetText().GetBuffer());
	MessageBox(0, strMsg.GetBuffer(), _T("haha"), MB_ICONINFORMATION);
}

void CloseWindowProc(BOOL lbutton, void* pVoid)
{
	CTestApp* pApp = (CTestApp*)pVoid;
	pApp->CloseApp();
}

CTestApp::CTestApp(void)
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

}

CTestApp::~CTestApp(void)
{
}

HICON CTestApp::GetIcon(int mode)
{
	return LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_TESTUI));
}

BOOL CTestApp::InitInstance (HINSTANCE hInstance, int nCmdShow)
{
	if (!KWinApp::InitInstance(hInstance, nCmdShow))
		return FALSE;

	KCanvas* pCanvas = m_pMainWnd->GetCanvas();
	pCanvas->SetLayout(new KLayoutSimple(TRUE));

	KViewFactory* pFactory = KGetViewFactory();
	KViewGroup* pGroup = (KViewGroup*)pFactory->CreateView(L"KViewGroup");

	KButton* pBtnClose = (KButton*)pFactory->CreateView(L"KButton");
	
	pBtnClose->SetText("Close");
	pBtnClose->SetClickEvent(CloseWindowProc, this);
	pGroup->AddChild(pBtnClose);
	pCanvas->AddChild(pGroup);

	pGroup = (KViewGroup*)pFactory->CreateView(L"KViewGroup");
	for(int i=0; i<10; i++)
	{
		KButton* pBtn = (KButton*)pFactory->CreateView(L"KButton");
		KString strText;
		strText.Format(_T("Button %d"), i + 1);

		pBtn->SetText(strText);

		ViewEventListener listener;
		listener.pData = pBtn;
		listener.OnMouseLClick = ButtonClickExecProc;

		pBtn->SetEventListener(listener);

		pGroup->AddChild(pBtn);
	}
	pCanvas->AddChild(pGroup);
	pCanvas->Layout();

	return TRUE;
}
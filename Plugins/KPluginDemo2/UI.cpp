#include "StdAfx.h"
#include "UI.h"


extern CPlugin	theApp;


void Cmd1Exec(int cmdid, void* pVoid)
{
	CUI* pUI = (CUI*)pVoid;
	
	AfxMessageBox(_T("demo plugin : btn1 exec."));
}

bool Cmd1Enable(int cmdid, void* pVoid)
{
	CUI* pUI = (CUI*)pVoid;
	return true;
}

void Cmd2Exec(int cmdid, void* pVoid)
{
	CUI* pUI = (CUI*)pVoid;
	
	AfxMessageBox(_T("demo plugin : btn2 exec."));
}

bool Cmd2Enable(int cmdid, void* pVoid)
{
	CUI* pUI = (CUI*)pVoid;
	return true;
}

void Cmd3Exec(int cmdid, void* pVoid)
{
	CUI* pUI = (CUI*)pVoid;
	
	AfxMessageBox(_T("demo plugin : btn3 exec."));
}

bool Cmd3Enable(int cmdid, void* pVoid)
{
	CUI* pUI = (CUI*)pVoid;
	return true;
}

void Cmd4Exec(int cmdid, void* pVoid)
{
	CUI* pUI = (CUI*)pVoid;
	
	AfxMessageBox(_T("demo plugin : btn4 exec."));
}

bool Cmd4Enable(int cmdid, void* pVoid)
{
	CUI* pUI = (CUI*)pVoid;
	return true;
}


CUI::CUI(void)
{
	m_client = 0;
}


CUI::~CUI(void)
{
}

int	CUI::CreateMainWnd(HWND hParent)
{
	if(0 == m_client)
	{
		RECT rc;
		::GetClientRect(hParent, &rc);
		m_client = (CDemoView*)RUNTIME_CLASS(CDemoView)->CreateObject();
		m_client->CreateView(CWnd::FromHandle(hParent));

	//	m_client->PostMessage(WM_INITIALUPDATE, 0, 0);
	}
	return 1;
}

HWND CUI::GetMainWnd()
{
	if (m_client)
		return m_client->GetSafeHwnd();
	return 0;
}

int	CUI::UpdateMainWnd()
{
	return 0;
}

int	CUI::InitUI(IKFrameWnd* pWnd)
{
	IKToolbar* toolbar = pWnd->CreateToolbar(&theApp, L"Test2", L"aa", L"", L"");

	int cmdid = pWnd->RegisterCmd(Cmd1Exec, Cmd1Enable, 0, this);
	KToolbarGroup group1 = toolbar->AddGroup(0, L"group1", L"ga");
	toolbar->AddButton(group1, cmdid, KBTN_TYPE_BUTTON, L"1btn1", -1, -1, 0);
	
	cmdid = pWnd->RegisterCmd(Cmd2Exec, Cmd2Enable, 0, this);
	toolbar->AddButton(group1, cmdid, KBTN_TYPE_BUTTON, L"1btn2", -1, -1, 0);

	KToolbarGroup group2 = toolbar->AddGroup(0, L"group2", L"gb");
	cmdid = pWnd->RegisterCmd(Cmd3Exec, Cmd3Enable, 0, this);
	toolbar->AddButton(group2, cmdid, KBTN_TYPE_BUTTON, L"2btn1", -1, -1, 0);

	cmdid = pWnd->RegisterCmd(Cmd4Exec, Cmd4Enable, 0, this);
	toolbar->AddButton(group2, cmdid, KBTN_TYPE_BUTTON, L"2btn2", -1, -1, 0);
	
	return 0;
}

void CUI::UninitUI(IKFrameWnd* pWnd)
{
	//return 0;
	m_client = 0;
}

int	CUI::OpenConfig(KXmlNode* pRoot)
{
	return 0;
}

int	CUI::SaveConfig(KXmlNode* pRoot)
{
	return 0;
}

int	CUI::IsSystemBtnEnable(int btntype)
{
	return 1;
}

int	CUI::OpenDoc(KString filename)
{
	return 0;
}

int	CUI::SaveDoc(KString filename)
{
	return 0;
}

int	CUI::CloseDoc(IKPluginDoc* pDoc)
{
	return 0;
}

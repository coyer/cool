#include "stdafx.h"
#include "ClientView.h"


BEGIN_MESSAGE_MAP(CKImageViewApp, CWinApp)
END_MESSAGE_MAP()


// CKImageViewApp ππ‘Ï

CKImageViewApp::CKImageViewApp()
{
	m_pKApp = 0;
}

KString	CKImageViewApp::GetPluginName()
{
	return _T("ImageView");
}

int	CKImageViewApp::GetPluginType()
{
	return 0;
}

//no init function, the init function is export name: DllInit
void CKImageViewApp::Uninit(IKApp* pApp)
{
}

IKPluginDoc* CKImageViewApp::GetCurDoc()
{
	return 0;
}

IKPluginDoc* CKImageViewApp::CreateDoc()
{
	return 0;
}


int	CKImageViewApp::CanExitNow()
{
	return 0;
}

int	CKImageViewApp::OnCommand(KStringArray argv)
{
	return 0;
}


//if have not ui, return 0;
IKPluginUI*	CKImageViewApp::GetUI()
{
	return &m_ui;
}

//if havn't action, return 0;
IKPluginAction*	 CKImageViewApp::GetAction()
{
	return 0;
}


#include "StdAfx.h"
#include "Plugin.h"


CPlugin::CPlugin(void)
{
	m_pKApp = 0;
}


CPlugin::~CPlugin(void)
{
}

KString	CPlugin::GetPluginName()
{
	return _T("PlugDemo2");
}

int	CPlugin::GetPluginType()
{
	return 0;
}

//no init function, the init function is export name: DllInit
void CPlugin::Uninit(IKApp* pApp)
{
}

IKPluginDoc* CPlugin::GetCurDoc()
{
	return 0;
}

IKPluginDoc* CPlugin::CreateDoc()
{
	return 0;
}


int	CPlugin::CanExitNow()
{
	return 0;
}

int	CPlugin::OnCommand(KStringArray argv)
{
	return 0;
}


//if have not ui, return 0;
IKPluginUI*	CPlugin::GetUI()
{
	return &m_ui;
}

//if havn't action, return 0;
IKPluginAction*	 CPlugin::GetAction()
{
	return 0;
}


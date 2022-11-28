#pragma once
#include "UI.h"

class CPlugin : public IKPlugin
{
public:
	CPlugin(void);
	~CPlugin(void);

	virtual KString		GetPluginName();
	virtual int			GetPluginType();

	//no init function, the init function is export name: DllInit
	virtual void		Uninit(IKApp* pApp);
	
	virtual IKPluginDoc*	GetCurDoc();
	virtual IKPluginDoc*	CreateDoc();

	virtual int			CanExitNow();
	virtual int			OnCommand(KStringArray argv);

	//if have not ui, return 0;
	virtual IKPluginUI*			GetUI();
	//if havn't action, return 0;
	virtual IKPluginAction*		GetAction();

protected:
	CUI		m_ui;
public:
	IKApp*		m_pKApp;
};


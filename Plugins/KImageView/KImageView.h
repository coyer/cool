// KImageView.h : KImageView DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#include "UI.h"

// CKImageViewApp
// 有关此类实现的信息，请参阅 KImageView.cpp
//

class CKImageViewApp : public CWinApp,
					   public IKPlugin
{
public:
	CKImageViewApp();

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


public:

protected:
	CUI		m_ui;
public:
	IKApp*		m_pKApp;
// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

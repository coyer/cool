// KImageView.h : KImageView DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include "UI.h"

// CKImageViewApp
// �йش���ʵ�ֵ���Ϣ������� KImageView.cpp
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
// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

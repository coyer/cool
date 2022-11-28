// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// CoolApp.h : CoolApp Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "AppLogControl.h"
#include "../include/core/coredef.h"

// CCoolAppApp:
// �йش����ʵ�֣������ CoolApp.cpp
//

class CCoolAppApp : public CWinAppEx,
					public IKApp
{
public:
	CCoolAppApp();
	~CCoolAppApp();


	virtual const KDllInfo*	GetAppInfo();

	//0: app.exe path.	as K_PATH_EXE
	//1: register path. as K_PATH_REG
	//2: log path.
	//3: config path.
	virtual KString		GetAppPath(int pathType);
	virtual IKDllMgr*	GetDllMgr();
	virtual IKExtMgr*	GetExtMgr();
	virtual IKLog*		GetLog();

	virtual int			RegisterPlugin(IKPlugin* plugin);
	virtual int			UnregisterPlugin(IKPlugin* plugin);
	virtual int			GetPluginCount();
	virtual IKPlugin*	GetPluginAt(int index);
	virtual IKPlugin*	GetPlugin(KString name);

	virtual IKFrameWnd*		GetFrame();

public:
	void	InitAppInfo();
	void	InitAppPath();
	void	LoadPlugin();
	void	UnloadPlugin();
// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void OnFileNew();
	virtual void OnFileOpen();

// ʵ��

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	UINT		m_nAppLook;
	BOOL		m_bHiColorIcons;

protected:
	KDllInfo	m_appInfo;
	KString		m_strPath[K_PATH_MAX];

	IKDllMgr*	m_pDllMgr;
	IKExtMgr*	m_pExtMgr;

	CAppLogControl	m_logControl;
	CAppLogDealer*	m_pLogDealer;

	KLog		m_logSystem;
	KArrayT<IKPlugin*>	m_plugins;
};

extern CCoolAppApp theApp;

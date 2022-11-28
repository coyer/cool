// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// CoolApp.h : CoolApp 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "AppLogControl.h"
#include "../include/core/coredef.h"

// CCoolAppApp:
// 有关此类的实现，请参阅 CoolApp.cpp
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
// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void OnFileNew();
	virtual void OnFileOpen();

// 实现

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

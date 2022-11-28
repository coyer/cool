#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : iapp.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : 插件平台接口，插件中可以调用此接口中的函数，达到与平台交互的功能。

*******************************************************************************************/

#include "iext.h"
#include "idll.h"
#include "iplugin.h"
#include "ilog.h"
#include "iframe.h"

struct IKApp
{
	virtual const KDllInfo*	GetAppInfo() = 0;

	//0: app.exe path.	as K_PATH_EXE
	//1: register path. as K_PATH_REG
	//2: log path.
	//3: config path.
	virtual KString		GetAppPath(int pathType) = 0;
	virtual IKDllMgr*	GetDllMgr() = 0;
	virtual IKExtMgr*	GetExtMgr() = 0;
	virtual IKLog*		GetLog() = 0;

	virtual int			RegisterPlugin(IKPlugin* plugin) = 0;
	virtual int			UnregisterPlugin(IKPlugin* plugin) = 0;
	virtual int			GetPluginCount() = 0;
	virtual IKPlugin*	GetPluginAt(int index) = 0;
	virtual IKPlugin*	GetPlugin(KString name) = 0;

	virtual IKFrameWnd*		GetFrame() = 0;
};




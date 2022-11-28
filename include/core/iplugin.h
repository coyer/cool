#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : iplugin.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : 插件的接口定义，插件需要实现IPlugin主接口。详细信息请参看readme.exe文件

*******************************************************************************************/

#include "coredef.h"


struct IKApp;
struct IKFrameWnd;

struct IKPluginDoc
{
	virtual KString		GetFileExt() = 0;
	virtual int			Open(KString filename) = 0;
	virtual int			Save(KString filename) = 0;
	virtual int			IsModified() = 0;

	//if len == 0, only calculate buffer len. return buffer len.
	virtual int			ToBuffer(void* buffer, int len) = 0;
	virtual int			FromBuffer(void* buffer, int len) = 0;
};

struct IKPluginUI
{
	virtual int			CreateMainWnd(HWND hParent) = 0;
	virtual HWND		GetMainWnd() = 0;
	virtual int			UpdateMainWnd() = 0;

	virtual int			InitUI(IKFrameWnd* pWnd) = 0;
	virtual void		UninitUI(IKFrameWnd* pWnd) = 0;

	virtual int			OpenConfig(KXmlNode* pRoot) = 0;
	virtual int			SaveConfig(KXmlNode* pRoot) = 0;

	virtual int			IsSystemBtnEnable(int btntype) = 0;

	virtual int			OpenDoc(KString filename) = 0;
	virtual int			SaveDoc(KString filename) = 0;
	virtual int			CloseDoc(IKPluginDoc* pDoc) = 0;
};

struct IKPluginAction
{
	virtual int			Start() = 0;
	virtual int			Stop() = 0;
};

struct IKPlugin
{
	void*				privateData;			//Don't use these variant, it is used by KApp !!!

	virtual KString		GetPluginName() = 0;
	virtual int			GetPluginType() = 0;

	//no init function, the init function is export name: DllInit
	virtual void		Uninit(IKApp* pApp) = 0;
	
	virtual IKPluginDoc*	GetCurDoc() = 0;
	virtual IKPluginDoc*	CreateDoc() = 0;

	virtual int			CanExitNow() = 0;
	virtual int			OnCommand(KStringArray argv) = 0;

	//if have not ui, return 0;
	virtual IKPluginUI*			GetUI() = 0;
	//if havn't action, return 0;
	virtual IKPluginAction*		GetAction() = 0;
};


//all KApp Dll dll must export fellow functions:
//int   DllInit(IKApp* pApp, int appVer);

typedef int   (*DllInitProc)(IKApp* pApp, int appVer);

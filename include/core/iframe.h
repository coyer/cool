#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : iframe.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : 平台中提供的界面接口，插件通过这些接口函数达到增加工具栏之类的功能。详细信息请参看readme.exe文件

*******************************************************************************************/


typedef void* KToolbarGroup;
typedef void* KToolbarEle;

typedef void (*CmdExecProc)(int cmdid, void* pVoid);
typedef bool (*CmdEnableProc)(int cmdid, void* pVoid);
typedef bool (*CmdCheckableProc)(int cmdid, void* pVoid);


#define KBTN_TYPE_BUTTON		1
#define KBTN_TYPE_CHECKBOX		2

#define KBTN_TYPE_FONT_SELECT	6
#define KBTN_TYPE_DROPLIST		7
#define KBTN_TYPE_COMBOBOX		8

struct IKToolbar
{
	virtual KToolbarGroup	AddGroup(int cmdid, const wchar_t* strText, const wchar_t* strTip) = 0;
	virtual KToolbarEle	    AddButton(KToolbarGroup group, int cmdid, int btnType, const wchar_t* szText, int bigIcon, int smallIcon, const wchar_t* szKeys) = 0;

	virtual KToolbarEle		AddLabel(KToolbarGroup group, const wchar_t* szText, int multiline, const wchar_t* szKeys) = 0;
	virtual KToolbarEle		AddLink(KToolbarGroup group, int cmdid, const wchar_t* szText, const wchar_t* strUrl, const wchar_t* szKeys) = 0;
	virtual KToolbarEle		AddEdit(KToolbarGroup group, int cmdid, const wchar_t* szText, int width, const wchar_t* szKeys) = 0;

	virtual int				GetGroupCount() = 0;
	virtual KToolbarGroup	GetGroupAt(int index) = 0;
	virtual void			RemoveGroup(int index) = 0;

	virtual int				GetEleCount(KToolbarGroup group) = 0;
	virtual KToolbarEle		GetEleByIndex(KToolbarGroup group, int index) = 0;
	virtual void			RemoveEle(KToolbarGroup group, int index) = 0;

};

//not support now.
struct IKDockbar
{
};

struct IKFrameWnd
{
	virtual HWND		GetHWND() = 0;
	virtual IKToolbar*	CreateToolbar(IKPlugin* plugin, const wchar_t* szCaption, const wchar_t* szKeys, const wchar_t* imageBig, const wchar_t* imagesmall) = 0;
	virtual IKDockbar*	CreateDockbar(IKPlugin* plugin, const wchar_t* szCaption, const wchar_t* szKeys) = 0;

	virtual int			RegisterCmd(CmdExecProc execProc, CmdEnableProc enableProc, CmdCheckableProc checkProc, void* procClient) = 0;
	virtual void		UnregisterCmd(int cmdid) = 0;

	virtual void		GetRealClient(RECT* pRect) = 0;
	virtual void		SetStatusText(int index, const wchar_t* szText) = 0;
};

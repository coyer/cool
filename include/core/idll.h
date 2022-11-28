#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : idll.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : 平台中加载的dll信息，可以通过这个接口获得。详细信息请参看readme.exe文件

*******************************************************************************************/

#include "coredef.h"

typedef struct tagDllInfo
{
	KString		idname;
	KString		type;
	int			mainVer;
	int			subVer;
	KString		desc;
	KString		auther;
	KString		help;
	KString		buildtime;
	KString		filepath;
}KDllInfo;

//all KApp Dll dll must export fellow functions:
//int	DllGetInfo(KDllInfo* pInfo);	must return KAPP_VERSION, otherwise this dll will not be loaded.
//int   DllInit(IKApp* pApp, int appVer);
//void	DllUninit();

typedef int	 (*DllGetInfoProc)(KDllInfo* pInfo);
typedef void (*DllUninitProc)();

struct IKDllMgr
{
	virtual int			GetCount() = 0;
	virtual KDllInfo*	GetInfoAt(int index) = 0;
	virtual KDll*		GetDllAt(int index) = 0;
	
	//Dllext is file filter, like ".dll"
	virtual int			LoadPath(const wchar_t* DllFolder, const wchar_t* Dllext) = 0;
	//return 0 means ok, else is error code.
	virtual int			LoadDll(const wchar_t* fullpath) = 0;

	virtual void		UnloadAll() = 0;
	virtual void		UnloadDll(KString idname) = 0;
};

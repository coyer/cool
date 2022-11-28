#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : iext.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : 平台中的扩展接口定义。详细信息请参看readme.exe文件

*******************************************************************************************/

#include "coredef.h"

struct IKExt// : public KObj
{
	virtual KString		GetFuncName() = 0;
	virtual int			GetFuncType() = 0;
	virtual int			DoAction(int argc, wchar_t** argv) = 0;
};

struct IKExtMgr
{
	virtual int		GetFuncCount() = 0;
	virtual IKExt*	GetFuncAt(int index) = 0;
	virtual IKExt*	GetFunc(KString name) = 0;
	virtual IKExt*	GetFunc(int type) = 0;

	virtual int		FindFunc(KString name) = 0;
	virtual int		FindFunc(int type) = 0;

	virtual int		AddFunc(IKExt* pFunc) = 0;
	virtual void	RemoveFunc(int index) = 0;
	virtual void	RemoveFuncAll() = 0;
};

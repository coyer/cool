#pragma once

#include "../base/kinc.h"


struct IKFunc : public KObj
{
	virtual KString		GetFuncName() = 0;
	virtual int			GetFuncType() = 0;
	//virtual int			DoAction(int argc, wchar_t** argv) = 0;
};

struct IKFuncMgr : public IKFunc
{
	virtual int		GetFuncNumber() = 0;
	virtual IKFunc*	GetFuncAt(int index) = 0;
	virtual IKFunc*	GetFunc(KString name) = 0;
	virtual IKFunc*	GetFunc(int type) = 0;

	virtual int		FindFunc(KString name) = 0;
	virtual int		FindFunc(int type) = 0;

	virtual int		AddFunc(IKFunc* pFunc) = 0;
	virtual int		RemoveFunc(int index) = 0;
	virtual void	RemoveFuncAll() = 0;
};
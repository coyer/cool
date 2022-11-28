#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : kbase.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : KRefBase is a base Obj class. some other class is derived from this class.

*******************************************************************************************/

#include "kdef.h"

class KAPI KObj
{
public:
	KObj();
	virtual~ KObj();
};

class KAPI KRefBase : public KObj
{
	volatile long	m_ref;
public:
	KRefBase();
	virtual ~KRefBase();

	virtual int		GetRef();
	virtual void	AddRef();
	virtual void	Release();
};


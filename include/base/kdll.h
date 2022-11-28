#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : kdll.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : support linux or windows dll functions.

*******************************************************************************************/

#include "kdef.h"
#include "kstring.h"
#include <stdio.h>


class KAPI KDll
{
protected:
	void*	m_dll;

#ifdef WIN32
	KString	m_filename;
#else
	KStringA	m_filename;
#endif

public:
	KDll();
	~KDll();

	bool	Load(const char* filename, unsigned int openflag = 0);
	bool	Load(const wchar_t* filename, unsigned int openflag = 0);
	bool	Load(KString& filename, unsigned int openflag = 0);	

	KString	GetFilename();

	void*	Detach();
	void	Attach(void* hDll);

	void	Close();
	bool	IsOpened();

	void*	GetProc(const char* procname);
};
#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : kdef.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : 

*******************************************************************************************/

#ifndef KAPI
#ifdef _USRDLL
#ifdef COOLBASE_EXPORTS
#define KAPI __declspec(dllexport)
#else
#define KAPI __declspec(dllimport)
#endif
#else
#define KAPI	 
#endif
#endif

typedef void* KPosition;
typedef void* PVOID;

#ifdef WIN32
	#include <windows.h>
#else
#endif

typedef int (*KCompareDataProc)(const void* data1, const void* data2);
typedef int (*KCompareByKeyProc)(const void* pKey, const void* data);
typedef void (*KDestroyDataProc)(void* pdata);

typedef void (*KJobExecuteProc)(void* pJob, void* pVoid);

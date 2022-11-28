#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : kthead.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : hide linux, windows difference.

*******************************************************************************************/

#include "kdef.h"
#include "karrayptr.h"

class KAPI KThreadLock
{
#ifdef WIN32
	CRITICAL_SECTION	m_lock;
#else
  pthread_mutex   m_lock;
#endif
public:
	KThreadLock();
	~KThreadLock();

	void Lock();
	void Unlock();
};

class KAPI KThreadLockAuto
{
	KThreadLock*  m_plock;
public:
	KThreadLockAuto(KThreadLock* plock);
	~KThreadLockAuto();
};

typedef void (*KThreadProc)(void* pVoid);
typedef void (*KThreadProcEx)(int threadindex, void* pVoid);

class KAPI KThread
{
	friend static unsigned int __stdcall KThreadDoingProc(void* pClient);
	void	OnThread(int index);

	void*	m_procCallback;
	void*	m_procCallbackData;

	int		m_procType;
	long	m_threadCount;
	
	KArrayPtr	m_Handles;
public:
	KThread();
	~KThread();
  
	void	SetThreadProc(KThreadProc procDoing, void* pClientDoing);
	void	SetThreadProc(KThreadProcEx procDoing, void* pClientDoing);

	bool	Start(int threadCount = 1);

	int		GetThreadCount();
	
	bool	IsRunning();

	//if you want wait forever, ms = -1.
	void	WaitThreadEnd(int ms = -1);
};

KAPI void KSleepMs(int ms);
KAPI void KSleepUs(int us);

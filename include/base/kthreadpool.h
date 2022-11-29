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
#include "kthread.h"


class KAPI KThreadPool
{
protected:
	KThread		m_thread;
	KListPtr	m_jobs;
	bool		m_running;
	KThreadLock	m_lock;
	int			m_sleepMs;
	int			m_maxThreadCount;

protected:
	void*		GetJob();

public:
	KThreadPool();
	~KThreadPool();

	void	AddJob(KJobExecuteProc jobProc, void* jobParam, void* pVoid);
	void	ClearJob();

	virtual int		Start();
	virtual void	Stop(int waitms = -1);

	virtual void	OnThreadDoing();
	virtual void	OnDoingJob(void* pJob);

	void	SetNoJobWaitTime(int ms);
	int		GetNoJobWaitTime();

	void	SetThreadMaxCount(int maxCount);
	int		GetThreadMaxCount();
};

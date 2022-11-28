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


class KAPI KLooper
{
protected:
	KThread		m_thread;
	KListPtr	m_jobs;
	bool		m_running;
	KThreadLock	m_lock;

protected:
	void*		GetJob();

public:
	KLooper();
	~KLooper();

	void	AddJob(KJobExecuteProc jobProc, void* jobParam, void* pVoid);
	void	ClearJob();

	virtual int		Start();
	virtual void	Stop(int waitms);

	virtual void	OnThreadDoing();
	virtual void	OnDoingJob(void* pJob);
};

class KAPI KThreadPool : public KLooper
{
protected:
	int			m_maxThreadCount;
	int			m_sleepMs;

public:
	KThreadPool();
	~KThreadPool();

	void	SetNoJobWaitTime(int ms);
	int		GetNoJobWaitTime();

	void	SetThreadMaxCount(int maxCount);
	int		GetThreadMaxCount();

	int		Start();

	void	OnThreadDoing();
};
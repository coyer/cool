#include "stdafx.h"
#include "../include/base/kthreadpool.h"

typedef struct tagJobDataInfo
{
	KJobExecuteProc jobProc;
	void* jobParam;
	void* jobVoid;
}JobDataInfo;

void s_KThreadPoolProc(void* pVoid)
{
	KThreadPool* pool = (KThreadPool*)pVoid;
	pool->OnThreadDoing();
}

KThreadPool::KThreadPool()
{
	m_running = false;
	m_sleepMs = 100;
	m_maxThreadCount = 2;

	m_thread.SetThreadProc(s_KThreadPoolProc, this);
}

KThreadPool::~KThreadPool()
{
	Stop();
	ClearJob();
}

void KThreadPool::AddJob(KJobExecuteProc jobProc, void* jobParam, void* pVoid)
{
	m_lock.Lock();
	JobDataInfo* jobInfo = new JobDataInfo();
	jobInfo->jobVoid = jobProc;
	jobInfo->jobParam = jobParam;
	jobInfo->jobVoid = pVoid;
	m_jobs.AddTail(jobInfo);
	m_lock.Unlock();
}

void KThreadPool::ClearJob()
{
	m_lock.Lock();
	KPosition pos = m_jobs.GetHeadPos();

	void* data = NULL;
	while(m_jobs.GetNext(pos, data))
	{
		JobDataInfo* d = (JobDataInfo*)data;
		delete d;
	}
	m_jobs.RemoveAll();
	m_lock.Unlock();
}

void KThreadPool::SetNoJobWaitTime(int ms)
{
	m_sleepMs = ms;
}

int	KThreadPool::GetNoJobWaitTime()
{
	return m_sleepMs;
}

int	KThreadPool::Start()
{
	if (m_running)
		return 0;

	m_running = true;
	m_thread.Start(m_maxThreadCount);

	return 1;
}

void KThreadPool::Stop(int waitms)
{
	m_running = false;
	m_thread.WaitThreadEnd(waitms);
}

void* KThreadPool::GetJob()
{
	void* pJob = 0;
	m_lock.Lock();
	pJob = m_jobs.GetHead(true);
	m_lock.Unlock();

	return pJob;
}

void KThreadPool::OnThreadDoing()
{
	while(m_running)
	{
		void* pJob = GetJob();
		if (0 == pJob)
		{
			Sleep(m_sleepMs);
		}
		else
		{
			OnDoingJob(pJob);
		}
	}
}

void KThreadPool::OnDoingJob(void* pJob)
{
	JobDataInfo* d = (JobDataInfo*)pJob;
	if (d->jobProc)
	{
		d->jobProc(d->jobParam, d->jobVoid);
	}

	delete d;
}

void KThreadPool::SetThreadMaxCount(int maxCount)
{
	m_maxThreadCount = maxCount;
}

int	KThreadPool::GetThreadMaxCount()
{
	return m_maxThreadCount;
}

#include "stdafx.h"
#include "../include/base/klooper.h"

typedef struct tagJobDataInfo
{
	KJobExecuteProc jobProc;
	void* jobParam;
	void* jobVoid;
}JobDataInfo;

void s_KLooperProc(void* pVoid)
{
	KLooper* pool = (KLooper*)pVoid;
	pool->OnThreadDoing();
}

KLooper::KLooper()
{
	m_running = false;
	m_thread.SetThreadProc(s_KLooperProc, this);
}

KLooper::~KLooper()
{
	ClearJob();
}

void KLooper::AddJob(KJobExecuteProc jobProc, void* jobParam, void* pVoid)
{
	m_lock.Lock();
	JobDataInfo* jobInfo = new JobDataInfo();
	jobInfo->jobVoid = jobProc;
	jobInfo->jobParam = jobParam;
	jobInfo->jobVoid = pVoid;
	m_jobs.AddTail(jobInfo);
	m_lock.Unlock();
}

void KLooper::ClearJob()
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

int	KLooper::Start()
{
	if (m_running)
		return 0;

	m_running = true;
	m_thread.Start(1);

	return 1;
}

void KLooper::Stop(int waitms)
{
	m_running = false;
	m_thread.WaitThreadEnd(waitms);
}

void* KLooper::GetJob()
{
	void* pJob = 0;
	m_lock.Lock();
	pJob = m_jobs.GetHead(true);
	m_lock.Unlock();

	return pJob;
}

void KLooper::OnThreadDoing()
{
	while(m_running)
	{
		void* pJob = GetJob();
		if (0 == pJob)
		{
			Sleep(0);			
		}
		else
		{
			OnDoingJob(pJob);
		}
	}
}

void KLooper::OnDoingJob(void* pJob)
{
	JobDataInfo* d = (JobDataInfo*)pJob;
	if (d->jobProc)
	{
		d->jobProc(d->jobParam, d->jobVoid);
	}

	delete d;
}


//////////////////////////////////////////////////////////////////////////
KThreadPool::KThreadPool()
{
	m_running = false;
	m_maxThreadCount = 2;
	m_sleepMs = 100;
}

KThreadPool::~KThreadPool()
{
}

void KThreadPool::SetNoJobWaitTime(int ms)
{
	m_sleepMs = ms;
}

int	KThreadPool::GetNoJobWaitTime()
{
	return m_sleepMs;
}

void KThreadPool::SetThreadMaxCount(int maxCount)
{
	m_maxThreadCount = maxCount;
}

int	KThreadPool::GetThreadMaxCount()
{
	return m_maxThreadCount;
}

int	KThreadPool::Start()
{
	if (m_running)
		return 0;

	m_running = true;
	m_thread.Start(m_maxThreadCount);

	return 1;
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

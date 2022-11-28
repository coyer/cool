#include "stdafx.h"
#include "../include/base/kthread.h"

#include <process.h>


#ifdef WIN32

KThreadLock::KThreadLock()
{
	::InitializeCriticalSection(&m_lock);
}

KThreadLock::~KThreadLock()
{
	::DeleteCriticalSection(&m_lock);
}

void KThreadLock::Lock()
{
	::EnterCriticalSection(&m_lock);
}

void KThreadLock::Unlock()
{
	::LeaveCriticalSection(&m_lock);
}

#else

KThreadLock::KThreadLock()
{
}

KThreadLock::~KThreadLock()
{
}

void KThreadLock::Lock()
{
}

void KThreadLock::Unlock()
{
}
#endif

KThreadLockAuto::KThreadLockAuto(KThreadLock* plock)
{
	m_plock = plock;
	m_plock->Lock();
}

KThreadLockAuto::~KThreadLockAuto()
{
	m_plock->Unlock();
}

typedef struct tagKThreadData
{
	KThread* pThis;
	int index;
}KThreadData;

KThread::KThread()
{
	m_procType = 0;
	m_procCallback = 0;
	m_procCallbackData = 0;

	m_threadCount = 0;
}

KThread::~KThread()
{
	for(int i=0; i<m_Handles.GetCount(); i++)
	{
		CloseHandle(m_Handles[i]);
	}
	m_Handles.RemoveAll();
}

static unsigned int __stdcall KThreadDoingProc(void* pClient)
{
	KThreadData* p = (KThreadData*)pClient;

	KThread* pThis = p->pThis;
	int index = p->index;

	delete p;

	pThis->OnThread(index);

	_endthreadex(0);
	return 0;
}

void KThread::OnThread(int index)
{
	::InterlockedIncrement(&m_threadCount);

	if (m_procType == 1)
	{
		KThreadProc proc = (KThreadProc)m_procCallback;
		proc(m_procCallbackData);
	}
	else if (m_procType == 2)
	{
		KThreadProcEx proc = (KThreadProcEx)m_procCallback;
		proc(index, m_procCallbackData);
	}

	::InterlockedDecrement(&m_threadCount);
}


void KThread::SetThreadProc(KThreadProc procDoing, void* pClientDoing)
{
	if (procDoing)
		m_procType = 1;
	else
		m_procType = 0;

	m_procCallback = procDoing;
	m_procCallbackData = pClientDoing;
}

void KThread::SetThreadProc(KThreadProcEx procDoing, void* pClientDoing)
{
	if (procDoing)
		m_procType = 2;
	else
		m_procType = 0;

	m_procCallback = pClientDoing;
	m_procCallbackData = pClientDoing;
}

bool KThread::Start(int threadCount)
{
	if (0 == m_procType)
		return false;

	int t = threadCount - m_threadCount;
	for(int i=0; i<t; i++)
	{
		KThreadData* p = new KThreadData;
		p->pThis = this;
		p->index = i;
		void* handle = (void*)_beginthreadex(0, 0, KThreadDoingProc, p, 0, 0);
		m_Handles.Add(handle);
	}

	return t > 0 ? true : false;
}

int	KThread::GetThreadCount()
{
	return m_threadCount;
}

bool KThread::IsRunning()
{
	return m_threadCount > 0 ? true : false;
}

void KThread::WaitThreadEnd(int ms)
{
#ifdef WIN32
	int handleCount = m_Handles.GetCount();
	if (handleCount > 0)
	{
		::WaitForMultipleObjects(handleCount, (HANDLE*)m_Handles.GetData(), TRUE, ms);
		for(int i=0; i<handleCount; i++)
		{
			CloseHandle(m_Handles[i]);
		}
		m_Handles.RemoveAll();
	}
#else
	pthread_join();
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////
KAPI void KSleepMs(int ms)
{
#ifdef WIN32
	Sleep(ms);
#else
	sleep(ms / 1000);
#endif
}

KAPI void KSleepUs(int us)
{
#ifdef WIN32
    HANDLE hTimer = NULL;
    LARGE_INTEGER liDueTime;

    liDueTime.QuadPart = -10 * us;

    // Create an unnamed waitable timer.
    hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
    if (NULL == hTimer)
    {
        //printf("CreateWaitableTimer failed (%d)\n", GetLastError());
        return ;
    }

    if (!SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, 0))
    {
        //printf("SetWaitableTimer failed (%d)\n", GetLastError());
		CloseHandle(hTimer);
        return;
    }

    // Wait for the timer.

    WaitForSingleObject(hTimer, INFINITE);
	CloseHandle(hTimer);
#else
	sleep(us);
#endif
}

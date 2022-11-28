#include "StdAfx.h"
#include "../include/base/klog.h"
#include <time.h>
#include <tchar.h>
#include <stdarg.h>

const char* logstrA[] = 
{
	"Debug",
	"Infor",
	"Warn",
	"Error",
	"",
};

const wchar_t* logstrW[] = 
{
	L"Debug",
	L"Infor",
	L"Warn",
	L"Error",
	L"",
};

KLogBaseDealer::KLogBaseDealer()
{
	m_OutputMode = KLOG_TO_SCREEN;
}

KLogBaseDealer::~KLogBaseDealer()
{
}

void KLogBaseDealer::DealLog(int logtype, int logowner, time_t time, const wchar_t* logstr)
{
	struct tm * lt = localtime(&time);

	KStringW strMsg;
	strMsg.Format(L"[%d][%d%02d%02d %02d%02d%02d][%s]:%s", logowner, 
					lt->tm_year, lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec,
					logstrW[logtype - 1], logstr);

	if (IsToScreenEnable())
	{
		wprintf(strMsg.c_str());
	}
	if (IsToDebugTrace())
	{
#ifdef WIN32
		OutputDebugStringW(strMsg.c_str());
#else
#endif
	}
	if (IsToFileEnable())
	{
		if (_PrepareLogFile())
			m_file.WriteString(strMsg);
	}
}

void KLogBaseDealer::DealLog(int logtype, int logowner, time_t time, const char* logstr)
{
	struct tm * lt = localtime(&time);

	KStringA strMsg;
	strMsg.Format("[%d][%d%02d%02d %02d%02d%02d][%s]:%s", logowner, 
					lt->tm_year, lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec,
					logstrA[logtype - 1], logstr);

	if (IsToScreenEnable())
	{
		printf(strMsg.c_str());
	}
	if (IsToDebugTrace())
	{
#ifdef WIN32
		OutputDebugStringA(strMsg.c_str());
#else
#endif
	}
	if (IsToFileEnable())
	{
		if (_PrepareLogFile())
			m_file.WriteString(strMsg);
	}
}

bool KLogBaseDealer::_PrepareLogFile()
{
	if (m_file.IsOpened())
		return true;


	if (m_logpath.GetLength() == 0)
		m_logpath = KFile::GetFolderPath(KFile::fnDocument);
#ifdef WIN32
	if (m_logpath.GetAt(m_logpath.GetLength() - 1) != '\\')
		m_logpath += '\\';
#else
	if (m_logpath.GetAt(m_logpath.GetLength() - 1) != '/')
		m_logpath += '/';
#endif
	TCHAR buff[32];
	__time32_t curtm;
	_time32(&curtm);

	struct tm * pt = _localtime32(&curtm);
	_tasctime_s(buff, sizeof(buff), pt);

	KString strFile;
	strFile.Format(_T("%s%s.log"), m_logpath.c_str(), buff);

	if (!m_file.Open(strFile, KFile::modeCreate | KFile::modeWrite))
		return false;

#ifdef WIN32
	m_file.SetCodeFormat(KFile::cf_utf16);
#endif

	return true;
}

void KLogBaseDealer::EnableToFile(int enable, const wchar_t* filepath)
{
	if (enable)
	{
		m_OutputMode |= KLOG_TO_FILE;
		m_logpath = filepath;
	}
	else
	{
		m_OutputMode &= ~KLOG_TO_FILE;
	}
}

void KLogBaseDealer::EnableToScreen(int enable)
{
	if (enable)
		m_OutputMode |= KLOG_TO_SCREEN;
	else
		m_OutputMode &= ~KLOG_TO_SCREEN;
}

void KLogBaseDealer::EnableToDebugTrace(int enable)
{
	if (enable)
		m_OutputMode |= KLOG_TO_TRACE;
	else
		m_OutputMode &= ~KLOG_TO_TRACE;
}

int	KLogBaseDealer::IsToFileEnable()
{
	return (m_OutputMode & KLOG_TO_FILE) > 0;
}

int	KLogBaseDealer::IsToScreenEnable()
{
	return (m_OutputMode & KLOG_TO_SCREEN) > 0;
}

int	KLogBaseDealer::IsToDebugTrace()
{
	return (m_OutputMode & KLOG_TO_TRACE) > 0;
}

KString KLogBaseDealer::GetLogFilePath()
{
	return m_logpath;
}

//////////////////////////////////////////////////////////////////////////

typedef struct tagKLogItem
{
	int		logtype;
	int		logowner;
	time_t	time;
	int		buftype;
	char	buffer[2];
}KLogItem;

KLog::KLog()
{
	m_pDealer = 0;

	m_viewLogType = KLOG_OUTPUT_ERROR;
	m_nCacheCount  = 10000;
}

KLog::~KLog()
{
	_RemoveAllItem();

	if (m_pDealer)
		m_pDealer->Release();
}

static void KLogThreadProc(void* pVoid)
{
	KLog* pThis = (KLog*)pVoid;
	pThis->DoThread();
}

void KLog::Init(KLogDealer* pDealer)
{
	if (m_pDealer == pDealer)
		return;

	if (m_pDealer)
		m_pDealer->Release();

	if (0 == pDealer)
		m_pDealer = new KLogBaseDealer();
	else
		m_pDealer = pDealer;

	if (m_nCacheCount)
	{
		m_bRunning = true;
		m_thread.SetThreadProc(KLogThreadProc, this);
		m_thread.Start(1);
	}
}

void KLog::Uninit()
{
	m_bRunning = false;
	m_thread.WaitThreadEnd(1000);
}

KLogDealer*	KLog::GetDealer()
{
	return m_pDealer;
}

void KLog::SetOutputLogType(int outputmode)
{
	m_viewLogType = outputmode;
}

int	KLog::GetOutputLogType()
{
	return m_viewLogType;
}

bool KLog::CalLogout(int logtype)
{
	if (KLOG_OUTPUT_NONE == m_viewLogType)
		return false;

	if (KLOGTYPE_MUST == logtype)
		return true;
	if (KLOGTYPE_WARN == logtype)
		return (m_viewLogType & KLOG_OUTPUT_WARN) > 0 ? true : false;
	if (KLOGTYPE_ERROR == logtype)
		return (m_viewLogType & KLOG_OUTPUT_ERROR) > 0 ? true : false;
	if (KLOGTYPE_INFO == logtype)
		return (m_viewLogType & KLOG_OUTPUT_INFO) > 0 ? true : false;
	if (KLOGTYPE_DEBUG == logtype)
		return (m_viewLogType & KLOG_OUTPUT_DEBUG) > 0 ? true : false;
	
	return false;
}

void KLog::SetCacheCount(long cacheCount)
{
#ifdef WIN32
	InterlockedExchange(&m_nCacheCount, cacheCount);
#else
	m_nCacheCount = cacheCount;
#endif
}

long KLog::GetCacheCount()
{
	return m_nCacheCount;
}

void KLog::LogMsg(int logtype, int logowner, const char* logstr, ...)
{
	if (0 == logstr || 0 == logstr[0])
		return;

	if (!CalLogout(logtype))
		return;

	int len = strlen(logstr);
	char *szBuffer = static_cast<char*>(malloc(sizeof(char) * (len + 10240)));
	memset(szBuffer, 0, sizeof(char) * (len + 1024));

	va_list argList;
	va_start(argList, logstr);
	int iRet = vsprintf_s(szBuffer, len + 10240, logstr, argList);
	va_end(argList);

	LogStr(logtype, logowner, szBuffer);
	free(szBuffer);
}

void KLog::LogMsg(int logtype, int logowner, const wchar_t* logstr, ...)
{
	if (0 == logstr || 0 == logstr[0])
		return;

	if (!CalLogout(logtype))
		return;

	int len = wcslen(logstr);
	wchar_t *szBuffer = static_cast<wchar_t*>(malloc(sizeof(wchar_t) * (len + 10240)));
	memset(szBuffer, 0, sizeof(wchar_t) * (len + 1024));

	va_list argList;
	va_start(argList, logstr);
	int iRet = vswprintf_s(szBuffer, len + 10240, logstr, argList);
	va_end(argList);

	LogStr(logtype, logowner, szBuffer);
	free(szBuffer);
}

void KLog::LogStr(int logtype, int logowner, const char* logstr)
{
	if (!CalLogout(logtype))
		return;

	if (0 == logstr || 0 == logstr[0])
		return;

	time_t t;
	time(&t);
	if (m_nCacheCount)
	{
		if (m_cacheItem.GetCount() > m_nCacheCount)
			return;

		int len = strlen(logstr);
		KLogItem* pItem = (KLogItem*)malloc(sizeof(KLogItem) + len);
		memset(pItem, 0, sizeof(KLogItem) + len);
		pItem->time = t;
		pItem->logowner = logowner;
		pItem->logtype = logtype;
		pItem->buftype = 1;
		memcpy(pItem->buffer, logstr, len);

		_AddLogItem(pItem);
	}
	else
	{
		m_pDealer->DealLog(logtype, logowner, t, logstr);
	}
}

void KLog::LogStr(int logtype, int logowner, const wchar_t* logstr)
{
	if (!CalLogout(logtype))
		return;

	if (0 == logstr || 0 == logstr[0])
		return;

	time_t t;
	time(&t);

	if (m_nCacheCount)
	{
		if (m_cacheItem.GetCount() > m_nCacheCount)
			return;
				
		int len = wcslen(logstr);
		KLogItem* pItem = (KLogItem*)malloc(sizeof(KLogItem) + len * sizeof(wchar_t));
		memset(pItem, 0, sizeof(KLogItem) + len * sizeof(wchar_t));
		pItem->time = t;
		pItem->logowner = logowner;
		pItem->logtype = logtype;
		pItem->buftype = 1;
		memcpy(pItem->buffer, logstr, len * sizeof(wchar_t));

		_AddLogItem(pItem);
	}
	else
	{
		m_pDealer->DealLog(logtype, logowner, t, logstr);
	}
}

void KLog::LogStr(int logtype, int logowner, KString logstr)
{
	if (!CalLogout(logtype))
		return;

	if (logstr.GetLength() == 0)
		return;

	time_t t;
	time(&t);

	if (m_nCacheCount)
	{
		if (m_cacheItem.GetCount() > m_nCacheCount)
			return;
				
		int len = logstr.GetLength();
		KLogItem* pItem = (KLogItem*)malloc(sizeof(KLogItem) + len * sizeof(TCHAR));
		memset(pItem, 0, sizeof(KLogItem) + len * sizeof(TCHAR));
		pItem->time = t;
		pItem->logowner = logowner;
		pItem->logtype = logtype;
		pItem->buftype = 1;
		memcpy(pItem->buffer, logstr.GetBuffer(), len * sizeof(TCHAR));

		_AddLogItem(pItem);
	}
	else
	{
		m_pDealer->DealLog(logtype, logowner, t, logstr);
	}
}

void KLog::Dump(int logowner, unsigned char* buffer, int buflen)
{
	if (!CalLogout(KLOGTYPE_DEBUG))
		return;

	KString strHex = KBuffer2HexString(buffer, buflen, 1, 0, 0);
	LogStr(KLOGTYPE_DEBUG, logowner, strHex);
}

void KLog::DoThread()
{
	while(m_bRunning && m_nCacheCount)
	{
		KLogItem* pItem = (KLogItem*)_GetLogItem();
		if (0 == pItem)
		{
			Sleep(100);
			continue;
		}

		if (pItem->buftype == 1)
			m_pDealer->DealLog(pItem->logtype, pItem->logowner, pItem->time, pItem->buffer);
		else
			m_pDealer->DealLog(pItem->logtype, pItem->logowner, pItem->time, (wchar_t*)pItem->buffer);

		free(pItem);
	}
}

void* KLog::_GetLogItem()
{
	void* pLogItem = 0;

	m_cacheLock.Lock();
	pLogItem = m_cacheItem.GetHead(true);
	m_cacheLock.Unlock();

	return pLogItem;
}

void KLog::_AddLogItem(void* pLogItem)
{
	m_cacheLock.Lock();
	m_cacheItem.AddTail(pLogItem);
	m_cacheLock.Unlock();
}

void KLog::_RemoveAllItem()
{
	while(1)
	{
		KLogItem* pItem = (KLogItem*)_GetLogItem();
		if (0 == pItem)
		{
			break;
		}

		free(pItem);
	}
}
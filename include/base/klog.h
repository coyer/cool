#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : klog.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : All application need log system, this is a good log, you need give KLogDealer implement.

*******************************************************************************************/

#include "kdef.h"
#include "kthread.h"
#include "kstring.h"
#include "kfile.h"
#include "klistptr.h"

#define KLOGTYPE_DEBUG		1
#define KLOGTYPE_INFO		2
#define KLOGTYPE_WARN		3
#define KLOGTYPE_ERROR		4
#define KLOGTYPE_MUST		5

#define KLOG_OUTPUT_NONE	0x0000
#define KLOG_OUTPUT_DEBUG	0x0001
#define KLOG_OUTPUT_INFO	0x0002
#define KLOG_OUTPUT_WARN	0x0004
#define KLOG_OUTPUT_ERROR	0x0008


class KAPI KLogDealer : public KRefBase
{
public:	
	virtual ~KLogDealer()	{;}
	virtual	void DealLog(int logtype, int logowner, time_t tm, const wchar_t* logstr) = 0;
	virtual	void DealLog(int logtype, int logowner, time_t tm, const char* logstr) = 0;	
};

class KAPI KLogBaseDealer : public KLogDealer
{
	friend class KLog;
protected:
	KTextFile	m_file;
	KString	m_logpath;
	int		m_OutputMode;

public:
	typedef enum
	{
		KLOG_TO_SCREEN = 0x01,
		KLOG_TO_TRACE  = 0x02,
		KLOG_TO_FILE   = 0x04,
	}OUTPUT_MODE;
	
	KLogBaseDealer();
	virtual ~KLogBaseDealer();

	virtual	 void	DealLog(int logtype, int logowner, time_t time, const wchar_t* logstr);
	virtual	 void	DealLog(int logtype, int logowner, time_t time, const char* logstr);

	void	EnableToFile(int enable, const wchar_t* filepath = 0);
	void	EnableToScreen(int enable);
	void	EnableToDebugTrace(int enable);

	int		IsToFileEnable();
	int		IsToScreenEnable();
	int		IsToDebugTrace();

	KString	GetLogFilePath();

protected:
	bool	_PrepareLogFile();
};

class KAPI KLog
{
public:
	KLog();
	~KLog();
	void	Init(KLogDealer* pDealer);
	void	Uninit();
	KLogDealer*	GetDealer();

	virtual void	LogMsg(int logtype, int logowner, const char* logstr, ...);
	virtual void	LogMsg(int logtype, int logowner, const wchar_t* logstr, ...);

	virtual void	LogStr(int logtype, int logowner, const char* logstr);
	virtual void	LogStr(int logtype, int logowner, const wchar_t* logstr);
	virtual void	LogStr(int logtype, int logowner, KString logstr);

	virtual void	Dump(int logowner, unsigned char* buffer, int buflen);

	void	SetOutputLogType(int outputmode);
	int		GetOutputLogType();

	void	SetCacheCount(long cacheCount);
	long	GetCacheCount();

	virtual void	DoThread();
	virtual bool	CalLogout(int logtype);

protected:
	void*		_GetLogItem();
	void		_AddLogItem(void* pLogItem);
	void		_RemoveAllItem();

protected:
	KLogDealer*		m_pDealer;
	int				m_viewLogType;
	volatile long	m_nCacheCount;
	KThread			m_thread;
	bool			m_bRunning;

	KListPtr		m_cacheItem;
	KThreadLock		m_cacheLock;
	//KListPtr		m_cacheFree;
};
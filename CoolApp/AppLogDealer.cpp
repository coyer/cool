#include "StdAfx.h"
#include "AppLogDealer.h"


CAppLogDealer::CAppLogDealer(void)
{
}


CAppLogDealer::~CAppLogDealer(void)
{
}

void CAppLogDealer::DealLog(int logtype, int logowner, time_t tm, const wchar_t* logstr)
{
	__super::DealLog(logtype, logowner, tm, logstr);
}

void CAppLogDealer::DealLog(int logtype, int logowner, time_t tm, const char* logstr)
{
	__super::DealLog(logtype, logowner, tm, logstr);
}

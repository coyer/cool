#include "StdAfx.h"
#include "AppLogControl.h"


CAppLogControl::CAppLogControl(void)
{
}


CAppLogControl::~CAppLogControl(void)
{
}

void CAppLogControl::LogStr(int pluginType, int loglevel, const char* strLog)
{
}

void CAppLogControl::LogStr(int pluginType, int loglevel, const wchar_t* strLog)
{
}

void CAppLogControl::LogStr(int pluginType, int loglevel, KString strLog)
{
}

int	CAppLogControl::GetOutputLevel()
{
	return 0;
}

int	CAppLogControl::SetOutputLevel(int level)
{
	return 0;
}

void CAppLogControl::EnableToFile(bool enable)
{
}

bool CAppLogControl::IsEnableToFile()
{
	return false;
}

//if maxcount == 0, close cache mode.
void CAppLogControl::SetCacheCount(int maxcount)
{
}

int	CAppLogControl::GetCacheCount()
{
	return 0;
}

void CAppLogControl::SetLogDealer(KLogDealer* pDealer)
{
	m_pLogDealer = pDealer;
}


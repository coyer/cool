#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : ilog.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : 平台中的日志系统接口，整个系统的日志需要统一处理。详细信息请参看readme.exe文件

*******************************************************************************************/


#include "coredef.h"

struct IKLog
{
	virtual void	LogStr(int pluginType, int loglevel, const char* strLog) = 0;
	virtual void	LogStr(int pluginType, int loglevel, const wchar_t* strLog) = 0;
	virtual void	LogStr(int pluginType, int loglevel, KString strLog) = 0;

	virtual int		GetOutputLevel() = 0;
	virtual int		SetOutputLevel(int level) = 0;

	virtual void	EnableToFile(bool enable) = 0;
	virtual bool	IsEnableToFile() = 0;

	//if maxcount == 0, close cache mode.
	virtual void	SetCacheCount(int maxcount) = 0;
	virtual int		GetCacheCount() = 0;
};

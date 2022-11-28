#pragma once

class CAppLogControl : public IKLog
{
public:
	CAppLogControl(void);
	~CAppLogControl(void);

	virtual void	LogStr(int pluginType, int loglevel, const char* strLog);
	virtual void	LogStr(int pluginType, int loglevel, const wchar_t* strLog);
	virtual void	LogStr(int pluginType, int loglevel, KString strLog);

	virtual int		GetOutputLevel();
	virtual int		SetOutputLevel(int level);

	virtual void	EnableToFile(bool enable);
	virtual bool	IsEnableToFile();

	//if maxcount == 0, close cache mode.
	virtual void	SetCacheCount(int maxcount);
	virtual int		GetCacheCount();

	void	SetLogDealer(KLogDealer* pDealer);

protected:
	KLogDealer*	m_pLogDealer;
};


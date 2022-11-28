#pragma once

class CAppLogDealer : public KLogBaseDealer
{
public:
	CAppLogDealer(void);
	~CAppLogDealer(void);

	virtual	 void DealLog(int logtype, int logowner, time_t tm, const wchar_t* logstr);
	virtual	 void DealLog(int logtype, int logowner, time_t tm, const char* logstr);

};


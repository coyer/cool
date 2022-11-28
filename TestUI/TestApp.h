#pragma once

class CTestApp : public KWinApp
{
public:
	CTestApp(void);
	virtual ~CTestApp(void);

	virtual HICON	GetIcon(int mode);
	virtual BOOL	InitInstance (HINSTANCE hInstance, int nCmdShow);

};


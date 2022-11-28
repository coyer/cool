#pragma once

#include "kuidef.h"
#include "krealwnd.h"
#include "../base/kinc.h"

struct IRenderObject
{
	virtual void	DoRender() = 0;
};

class KUI_API KWinApp
{
public:
	KWinApp();
	virtual ~KWinApp();

	virtual BOOL	InitInstance (HINSTANCE hInstance, int nCmdShow);
	virtual void	Run();
	virtual void	ExitInstance();

	virtual HICON			GetIcon(int mode);
	virtual KRealWndBase*	CreateMainWnd(int nCmdShow);
	virtual void			DeleteMainWnd(KRealWndBase* pView);
	virtual BOOL			ExecuteCmdLine(KString strCmdLine);

	virtual BOOL			LoadKResource();

	virtual HACCEL	CreateAccelTable();
	virtual void	OnIdle();
	virtual void	ReanderWnd();
	virtual BOOL	ProcessTask();
	
	HINSTANCE		GetInstance();
	KString			GetAppTitle();
	
	void			SetAppTitle(KString title);
	void			SetRenderObject(IRenderObject* pRender);

	void			CloseApp();
protected:
	KRealWndBase*	m_pMainWnd;
	HINSTANCE		m_hInstance;
	HACCEL			m_hAccelTable;

	KString			m_AppTitle;
	IRenderObject*	m_pRender;
};
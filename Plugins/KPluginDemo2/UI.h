#pragma once
#include "../../include/core/iplugin.h"


class CUI :	public IKPluginUI
{
public:
	CUI(void);
	~CUI(void);

	virtual int			CreateMainWnd(HWND hParent);
	virtual HWND		GetMainWnd();
	virtual int			UpdateMainWnd();

	virtual int			InitUI(IKFrameWnd* pWnd);
	virtual void		UninitUI(IKFrameWnd* pWnd);

	virtual int			OpenConfig(KXmlNode* pRoot);
	virtual int			SaveConfig(KXmlNode* pRoot);

	virtual int			IsSystemBtnEnable(int btntype);

	virtual int			OpenDoc(KString filename);
	virtual int			SaveDoc(KString filename);
	virtual int			CloseDoc(IKPluginDoc* pDoc);

public:
	CDemoView*		m_client;
};


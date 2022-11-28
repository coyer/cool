#pragma once
#include "afxcmn.h"



// CDemoView 窗体视图

class CDemoView : public CFormView
{
	DECLARE_DYNCREATE(CDemoView)

protected:
	CDemoView();           // 动态创建所使用的受保护的构造函数
	virtual ~CDemoView();
	virtual void OnInitialUpdate(); // 构造后第一次调用
public:

	BOOL	CreateView(CWnd* parent);

public:
	enum { IDD = IDD_DEMOVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_wndProgress;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};



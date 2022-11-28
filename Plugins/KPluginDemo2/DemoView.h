#pragma once
#include "afxcmn.h"



// CDemoView ������ͼ

class CDemoView : public CFormView
{
	DECLARE_DYNCREATE(CDemoView)

protected:
	CDemoView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDemoView();
	virtual void OnInitialUpdate(); // ������һ�ε���
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_wndProgress;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};



#pragma once


// CClientView 视图

class CClientView : public CView
{
	DECLARE_DYNCREATE(CClientView)

public:
	CClientView();           // 动态创建所使用的受保护的构造函数
	virtual ~CClientView();

	COLORREF	m_clrBackground;

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};



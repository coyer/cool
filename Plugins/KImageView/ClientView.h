#pragma once


// CClientView ��ͼ

class CClientView : public CView
{
	DECLARE_DYNCREATE(CClientView)

public:
	CClientView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CClientView();

	COLORREF	m_clrBackground;

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
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



#pragma once


// CMyClientView ��ͼ

class CMyClientView : public CView
{
	DECLARE_DYNCREATE(CMyClientView)

protected:
	CMyClientView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CMyClientView();

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
};



// MyClientView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CoolApp.h"
#include "MyClientView.h"


// CMyClientView

IMPLEMENT_DYNCREATE(CMyClientView, CView)

CMyClientView::CMyClientView()
{

}

CMyClientView::~CMyClientView()
{
}

BEGIN_MESSAGE_MAP(CMyClientView, CView)
END_MESSAGE_MAP()


// CMyClientView ��ͼ

void CMyClientView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	
	CRect rcClient;
	GetClientRect(&rcClient);

	pDC->FillSolidRect(rcClient, RGB(200, 200, 200));

	CString strText;
	strText = _T("Welcome use my plugin application demo.");
	pDC->DrawText(strText, rcClient, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
}

// CMyClientView ���

#ifdef _DEBUG
void CMyClientView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyClientView ��Ϣ�������

// ClientView.cpp : 实现文件
//

#include "stdafx.h"
#include "KImageView.h"
#include "ClientView.h"

// CClientView

IMPLEMENT_DYNCREATE(CClientView, CView)

CClientView::CClientView()
{
	m_clrBackground = RGB(200, 200, 220);
}

CClientView::~CClientView()
{
}

BEGIN_MESSAGE_MAP(CClientView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CClientView 绘图

void CClientView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码

	CRect rc;
	GetClientRect(&rc);

	pDC->FillSolidRect(rc, m_clrBackground);
	pDC->Draw3dRect(&rc, RGB(100,120, 130), RGB(200, 120, 180));

	int w = rc.Width() - 30;
	int h = rc.Width() - 30;

	for(int i=0; i<20; i++)
	{
		int x = rand() % w;
		int y = rand() % h;
		int t = rand() % 30 + 20;

		pDC->Ellipse(x, y, x + t, y + t);
	}
}


// CClientView 诊断

#ifdef _DEBUG
void CClientView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CClientView 消息处理程序


int CClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	srand(time(0));
	SetTimer(1, 400, 0);

	return 0;
}


void CClientView::OnDestroy()
{
	CView::OnDestroy();

	KillTimer(1);
}


void CClientView::OnTimer(UINT_PTR nIDEvent)
{
	int r = 200;
	int g = 200;
	int b = 200 + rand() % 50;

	m_clrBackground = RGB(r, g, b);


	Invalidate();

	CView::OnTimer(nIDEvent);
}

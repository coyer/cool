// DemoView.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "DemoView.h"


// CDemoView

IMPLEMENT_DYNCREATE(CDemoView, CFormView)

CDemoView::CDemoView()
	: CFormView(CDemoView::IDD)
{

}

CDemoView::~CDemoView()
{
}

void CDemoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_wndProgress);
}

BEGIN_MESSAGE_MAP(CDemoView, CFormView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDemoView 诊断

#ifdef _DEBUG
void CDemoView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDemoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDemoView 消息处理程序
BOOL CDemoView::CreateView(CWnd* parent)
{
	CRect rc(0, 0, 100, 100);
	return Create(AfxRegisterWndClass(0), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER, rc, parent, 104, 0);
}

void CDemoView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	m_wndProgress.SetRange(0, 100);
}

int CDemoView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(1, 100, 0);

	return 0;
}


void CDemoView::OnDestroy()
{
	KillTimer(1);

	CFormView::OnDestroy();
}


void CDemoView::OnTimer(UINT_PTR nIDEvent)
{
	int pos = m_wndProgress.GetPos();
	pos += 5;

	if (pos > 100)
		pos = 0;
	m_wndProgress.SetPos(pos);

	CFormView::OnTimer(nIDEvent);
}

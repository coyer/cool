// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "CoolApp.h"

#include "MainFrm.h"
#include "MyClientView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define USER_CMD_START		0x6000
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_FILE_PRINT, &CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnFilePrintPreview)
	ON_COMMAND_RANGE(USER_CMD_START, (USER_CMD_START + 10000), OnRegisterCmd)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnUpdateFilePrintPreview)
	ON_WM_SETTINGCHANGE()
	ON_WM_DESTROY()
	ON_REGISTERED_MESSAGE(AFX_WM_ON_CHANGE_RIBBON_CATEGORY, OnCategoryChanged)
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLACK);

	m_curWnd = 0;
}

CMainFrame::~CMainFrame()
{
}

HWND CMainFrame::GetHWND()
{
	return GetSafeHwnd();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	
	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 加载菜单项图像(不在任何标准工具栏上):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// 创建停靠窗口
	if (!CreateDockingWindows())
	{
		TRACE0("未能创建停靠窗口\n");
		return -1;
	}

	//m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndFileView);
	//CDockablePane* pTabbedBar = NULL;
	//m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	//m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndOutput);
	//m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndProperties);

	CRect rcClient;
	GetRealClient(rcClient);

	CMyClientView* pView = (CMyClientView*)RUNTIME_CLASS(CMyClientView)->CreateObject();
	pView->Create(AfxRegisterWndClass(0), _T(""), WS_CHILD | WS_VISIBLE | WS_DISABLED, rcClient, this, 100);
	
	m_wndFirst = pView;
	m_curWnd = pView;

	AdjustClientArea();

	_LoadPluginUI();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	//BOOL bNameValid;

	// 创建类视图
	//CString strClassView;
	//bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	//ASSERT(bNameValid);
	//if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("未能创建“类视图”窗口\n");
	//	return FALSE; // 未能创建
	//}

	// 创建文件视图
	//CString strFileView;
	//bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	//ASSERT(bNameValid);
	//if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("未能创建“文件视图”窗口\n");
	//	return FALSE; // 未能创建
	//}

	// 创建输出窗口
	//CString strOutputWnd;
	//bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	//ASSERT(bNameValid);
	//if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("未能创建输出窗口\n");
	//	return FALSE; // 未能创建
	//}

	// 创建属性窗口
	//CString strPropertiesWnd;
	//bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	//ASSERT(bNameValid);
	//if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("未能创建“属性”窗口\n");
	//	return FALSE; // 未能创建
	//}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	//HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	//HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	//HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	//HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnFilePrint()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CMainFrame::OnFilePrintPreview()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // 强制关闭“打印预览”模式
	}
}

void CMainFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsPrintPreview());
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
	//m_wndOutput.UpdateFonts();
}

void CMainFrame::OnDestroy()
{
	int count = theApp.GetPluginCount();
	for(int i=0; i<count; i++)
	{
		IKPlugin* plg = theApp.GetPluginAt(i);
		IKPluginUI* pUI = plg->GetUI();
		if (pUI)
			pUI->UninitUI(this);
	}

	__super::OnDestroy();
}

void CMainFrame::_LoadPluginUI()
{
	int count = theApp.GetPluginCount();
	for(int i=0; i<count; i++)
	{
		IKPlugin* plg = theApp.GetPluginAt(i);
		IKPluginUI* pUI = plg->GetUI();
		if (pUI)
			pUI->InitUI(this);
	}
}

IKToolbar* CMainFrame::CreateToolbar(IKPlugin* plugin, const wchar_t* szCaption, const wchar_t* szHint, const wchar_t* imageBig, const wchar_t* imagesmall)
{
	CKToolBar* ctg = (CKToolBar*)m_wndRibbonBar.AddCategory(szCaption, 0, 0, CSize(16, 16), CSize(32, 32), -1, RUNTIME_CLASS(CKToolBar));	
	ctg->SetData((DWORD_PTR)plugin);
	
	return ctg;
}

IKDockbar* CMainFrame::CreateDockbar(IKPlugin* plugin, const wchar_t* strCaption, const wchar_t* strHint)
{
	return 0;
}

int	CMainFrame::RegisterCmd(CmdExecProc execProc, CmdEnableProc enableProc, CmdCheckableProc checkProc, void* procClient)
{
	static int s_cmdid = USER_CMD_START;

	MsgCmdInfo info = {0};

	if (m_cmdFree.GetCount() > 0)
	{
		info.cmdid = m_cmdFree.GetAt(0);
		m_cmdFree.RemoveAt(0);
	}
	else
	{
		info.cmdid = s_cmdid++;
	}

	info.procClient = procClient;
	info.procCheck = checkProc;
	info.procEnable = enableProc;
	info.procExec = execProc;

	m_cmds[info.cmdid] = info;	

	return info.cmdid;
}

void CMainFrame::UnregisterCmd(int cmdid)
{
	MsgCmdInfo info;
	if (!m_cmds.Lookup(cmdid, info))
		return;

	m_cmdFree.Add(cmdid);
	m_cmds.RemoveKey(cmdid);
}

void CMainFrame::OnRegisterCmd(UINT id)
{
	MsgCmdInfo info;
	if (!m_cmds.Lookup(id, info))
		return;

	if (info.procExec)
		info.procExec(id, info.procClient);
}

LRESULT	CMainFrame::OnCategoryChanged(WPARAM wParam, LPARAM lParam)
{
	CMFCRibbonCategory* pCategory = m_wndRibbonBar.GetActiveCategory();
	IKPlugin* plg = (IKPlugin*)pCategory->GetData();

	if (m_curWnd)
	{
		m_curWnd->ShowWindow(SW_HIDE);
		m_curWnd = 0;
	}

	if (plg)
	{
		IKPluginUI* pUI = plg->GetUI();
		if (pUI)
		{	
			HWND hWnd = pUI->GetMainWnd();
			if (0 == hWnd)
			{
				PluginAttr* pAttr = (PluginAttr*)plg->privateData;
				if (pAttr && (!pAttr->ClientCreated))
				{
					pAttr->ClientCreated = true;
					pUI->CreateMainWnd(GetSafeHwnd());
				}

				hWnd = pUI->GetMainWnd();
				if (0 == hWnd)
				{
					m_curWnd = m_wndFirst;
					m_curWnd->ShowWindow(SW_SHOW);
					return 0;
				}
				m_curWnd = CWnd::FromHandle(hWnd);
				m_curWnd->SendMessage(WM_INITIALUPDATE, 0, 0);
				//m_curWnd ->SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);
			}
			else
			{
				m_curWnd = CWnd::FromHandle(hWnd);
			}
		}
	}
	else
	{
		m_curWnd = m_wndFirst;
	}

	if (m_curWnd)
	{
		m_curWnd->ShowWindow(SW_SHOW);
		AdjustClientArea();
	}

	return 0;
}

void CMainFrame::AdjustClientArea()
{
	if (m_curWnd != NULL)
	{
		CRect rectClientAreaBounds;
		GetRealClient(rectClientAreaBounds);
		//= m_dockManager.GetClientAreaBounds();

		//rectClientAreaBounds.left += m_rectBorder.left;
		//rectClientAreaBounds.top  += m_rectBorder.top;
		//rectClientAreaBounds.right -= m_rectBorder.right;
		//rectClientAreaBounds.bottom -= m_rectBorder.bottom;

		m_curWnd->CalcWindowRect(rectClientAreaBounds);

		m_curWnd->SetWindowPos(&wndBottom, rectClientAreaBounds.left, rectClientAreaBounds.top, rectClientAreaBounds.Width(), rectClientAreaBounds.Height(), SWP_NOACTIVATE);
	}
}

void CMainFrame::GetRealClient(RECT* pRect)
{
	CRect rectClientAreaBounds = m_dockManager.GetClientAreaBounds();

	rectClientAreaBounds.left += m_rectBorder.left;
	rectClientAreaBounds.top  += m_rectBorder.top;
	rectClientAreaBounds.right -= m_rectBorder.right;
	rectClientAreaBounds.bottom -= m_rectBorder.bottom;

	memcpy(pRect, &rectClientAreaBounds, sizeof(RECT));	
}

void CMainFrame::SetStatusText(int index, const wchar_t* szText)
{
	CMFCRibbonBaseElement* pEle = m_wndStatusBar.GetElement(index);
	if (pEle)
		pEle->SetText(szText);
}
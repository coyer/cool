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

// CoolApp.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "CoolApp.h"
#include "MainFrm.h"

#include "CoolAppDoc.h"
#include "CoolAppView.h"
#include <ShlObj.h>
#include "aboutDlg.h"

#include "../CoolCore/CoolCore.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCoolAppApp

BEGIN_MESSAGE_MAP(CCoolAppApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CCoolAppApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CCoolAppApp 构造

CCoolAppApp::CCoolAppApp()
{
	m_bHiColorIcons = TRUE;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("CoolApp.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中

	m_pDllMgr = KDllMgrCreate();
	m_pExtMgr = KExtMgrCreate();

	m_pLogDealer = new CAppLogDealer();
	
	m_logControl.SetLogDealer(m_pLogDealer);
	m_logSystem.Init(m_pLogDealer);
}

CCoolAppApp::~CCoolAppApp()
{

}

// 唯一的一个 CCoolAppApp 对象

CCoolAppApp theApp;


// CCoolAppApp 初始化

BOOL CCoolAppApp::InitInstance()
{
	InitAppPath();
	InitAppInfo();

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("Coyer"));
	LoadStdProfileSettings(16);  // 加载标准 INI 文件选项(包括 MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	LoadPlugin();

	CMainFrame* pFrame = new CMainFrame();
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pFrame;
		return FALSE;
	}

	m_pMainWnd = pFrame;


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生

	return TRUE;
}

int CCoolAppApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	UnloadPlugin();

	int ret = CWinAppEx::ExitInstance();
	
	m_logSystem.Uninit();

	KExtMgrDestroy(m_pExtMgr);
	m_pExtMgr = 0;

	KDllMgrDestroy(m_pDllMgr);
	m_pDllMgr = 0;
	
	return ret;
}

// CCoolAppApp 消息处理程序
void CCoolAppApp::OnFileNew()
{
}

void CCoolAppApp::OnFileOpen()
{
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

// 用于运行对话框的应用程序命令
void CCoolAppApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CCoolAppApp 自定义加载/保存方法

void CCoolAppApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CCoolAppApp::LoadCustomState()
{
}

void CCoolAppApp::SaveCustomState()
{
}

// CCoolAppApp 消息处理程序



const KDllInfo* CCoolAppApp::GetAppInfo()
{
	return &m_appInfo;
}
	
KString	CCoolAppApp::GetAppPath(int pathType)
{
	if (pathType >= 0 && pathType < K_PATH_MAX)
		return m_strPath[pathType];

	return _T("");
}

IKDllMgr* CCoolAppApp::GetDllMgr()
{
	return m_pDllMgr;
}

IKExtMgr* CCoolAppApp::GetExtMgr()
{
	return m_pExtMgr;
}

IKLog* CCoolAppApp::GetLog()
{
	return &m_logControl;
}

int	CCoolAppApp::RegisterPlugin(IKPlugin* plugin)
{
	PluginAttr * attr = new PluginAttr;
	attr->ClientCreated = false;
	plugin->privateData = attr;
	return m_plugins.Add(plugin);
}

int	CCoolAppApp::UnregisterPlugin(IKPlugin* plugin)
{
	if (plugin)
	{
		PluginAttr * attr = (PluginAttr *)plugin->privateData;
		plugin->privateData = 0;
		if (attr)
			delete attr;
	}

	int index = m_plugins.Find(plugin);
	m_plugins.RemoveAt(index);
	return 0;
}

int	CCoolAppApp::GetPluginCount()
{
	return m_plugins.GetCount();
}

IKPlugin* CCoolAppApp::GetPluginAt(int index)
{
	return m_plugins.GetAt(index);
}

IKPlugin* CCoolAppApp::GetPlugin(KString name)
{
	int n = m_plugins.GetCount();
	for(int i=0; i<n; i++)
	{
		if (m_plugins.GetAt(i)->GetPluginName().CompareNoCase(name) == 0)
			return m_plugins.GetAt(i);
	}
	return 0;
}

IKFrameWnd*	CCoolAppApp::GetFrame()
{
	CMainFrame* pFrame = (CMainFrame*)m_pMainWnd;
	return pFrame;
}

void CCoolAppApp::InitAppInfo()
{
	m_appInfo.auther = _T("coyer@126.com");
	m_appInfo.buildtime = __FILE__;
	m_appInfo.desc = _T("Plugin application example.");

	m_appInfo.filepath = m_strPath[K_PATH_EXE];
	m_appInfo.help = _T("coolapp.chm");
	m_appInfo.idname = _T("CoolApp");
	m_appInfo.mainVer = KAPP_VERSION;
	m_appInfo.subVer = 100;
	m_appInfo.type = 0;
}

void CCoolAppApp::InitAppPath()
{
	TCHAR szFilePath[MAX_PATH] = {0};
	GetModuleFileName(0, szFilePath, MAX_PATH);

	m_strPath[K_PATH_EXE] = szFilePath;

	TCHAR* pos = _tcsrchr(szFilePath, _T('\\'));
	*pos = 0;

	KString	strPath = szFilePath;

	m_strPath[K_PATH_CONFIG] = strPath;
	m_strPath[K_PATH_CONFIG] += _T("\\config\\");

	m_strPath[K_PATH_LOG] = strPath;
	m_strPath[K_PATH_LOG] += _T("\\log\\");

	m_strPath[K_PATH_PLUGIN] = strPath;
	m_strPath[K_PATH_PLUGIN] += _T("\\plugins\\");

	m_strPath[K_PATH_COMMON] = strPath;
	m_strPath[K_PATH_COMMON] += _T("\\Common\\");

	m_strPath[K_PATH_IMAGE] = strPath;
	m_strPath[K_PATH_IMAGE] += _T("\\images\\");

	m_strPath[K_PATH_REG] = _T("Software\\coyer\\CoolApp");

	szFilePath[0] = 0;
	SHGetSpecialFolderPath(0, szFilePath, CSIDL_APPDATA, 0);
	
	strPath = szFilePath;

	m_strPath[K_PATH_APPDATA] = strPath;
	m_strPath[K_PATH_APPDATA] += _T("\\CoolApp");

	m_strPath[K_PATH_COMMON_APPDATA] = strPath;
	m_strPath[K_PATH_COMMON_APPDATA] += _T("\\CoolApp");
}

void CCoolAppApp::LoadPlugin()
{
	int n = m_pDllMgr->LoadPath(m_strPath[K_PATH_PLUGIN], _T("*.plg"));

	for(int i=0; i<n; i++)
	{
		KDll* pDll = m_pDllMgr->GetDllAt(i);
		DllInitProc procInit = (DllInitProc)(pDll->GetProc("DllInit"));
		if (0 != procInit)
		{
			procInit(this, KAPP_VERSION);
		}
	}
}

void CCoolAppApp::UnloadPlugin()
{
	m_pDllMgr->UnloadAll();
}
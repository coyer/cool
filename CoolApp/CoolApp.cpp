// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// CoolApp.cpp : ����Ӧ�ó��������Ϊ��
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
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CCoolAppApp ����

CCoolAppApp::CCoolAppApp()
{
	m_bHiColorIcons = TRUE;

	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("CoolApp.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��

	m_pDllMgr = KDllMgrCreate();
	m_pExtMgr = KExtMgrCreate();

	m_pLogDealer = new CAppLogDealer();
	
	m_logControl.SetLogDealer(m_pLogDealer);
	m_logSystem.Init(m_pLogDealer);
}

CCoolAppApp::~CCoolAppApp()
{

}

// Ψһ��һ�� CCoolAppApp ����

CCoolAppApp theApp;


// CCoolAppApp ��ʼ��

BOOL CCoolAppApp::InitInstance()
{
	InitAppPath();
	InitAppInfo();

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Coyer"));
	LoadStdProfileSettings(16);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


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


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����

	return TRUE;
}

int CCoolAppApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
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

// CCoolAppApp ��Ϣ�������
void CCoolAppApp::OnFileNew()
{
}

void CCoolAppApp::OnFileOpen()
{
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

// �������жԻ����Ӧ�ó�������
void CCoolAppApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CCoolAppApp �Զ������/���淽��

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

// CCoolAppApp ��Ϣ�������



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
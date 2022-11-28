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

// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "KToolBar.h"
#include <afxcoll.h>

typedef struct tagMsgCmdInfo
{
	int cmdid;
	void*			procClient;
	CmdExecProc		procExec;
	CmdEnableProc	procEnable;
	CmdCheckableProc	procCheck;
}MsgCmdInfo;


class CMainFrame : public CFrameWndEx,
				   public IKFrameWnd
{
	
protected: // �������л�����
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:
	CMainFrame();

// ����
public:
	virtual HWND		GetHWND();
	virtual IKToolbar*	CreateToolbar(IKPlugin* plugin, const wchar_t* szCaption, const wchar_t* szHint, const wchar_t* imageBig, const wchar_t* imagesmall);
	virtual IKDockbar*	CreateDockbar(IKPlugin* plugin, const wchar_t* strCaption, const wchar_t* strHint);
	virtual int			RegisterCmd(CmdExecProc execProc, CmdEnableProc enableProc, CmdCheckableProc checkProc, void* procClient);
	virtual void		UnregisterCmd(int cmdid);

	virtual void		GetRealClient(RECT* pRect);
	virtual void		SetStatusText(int index, const wchar_t* szText);
// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void	_LoadPluginUI();

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	virtual void AdjustClientArea();

protected:  // �ؼ���Ƕ���Ա
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	//CFileView         m_wndFileView;
	//CClassView        m_wndClassView;
	//COutputWnd        m_wndOutput;
	//CPropertiesWnd    m_wndProperties;

	CMap<int, int, MsgCmdInfo, MsgCmdInfo>	m_cmds;
	CArray<int>			m_cmdFree;
	CWnd*				m_curWnd;
	CWnd*				m_wndFirst;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnRegisterCmd(UINT id);
	afx_msg LRESULT	OnCategoryChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	afx_msg void OnDestroy();
//	afx_msg void OnSize(UINT nType, int cx, int cy);
};



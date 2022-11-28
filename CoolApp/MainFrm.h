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

// MainFrm.h : CMainFrame 类的接口
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
	
protected: // 仅从序列化创建
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:
	CMainFrame();

// 操作
public:
	virtual HWND		GetHWND();
	virtual IKToolbar*	CreateToolbar(IKPlugin* plugin, const wchar_t* szCaption, const wchar_t* szHint, const wchar_t* imageBig, const wchar_t* imagesmall);
	virtual IKDockbar*	CreateDockbar(IKPlugin* plugin, const wchar_t* strCaption, const wchar_t* strHint);
	virtual int			RegisterCmd(CmdExecProc execProc, CmdEnableProc enableProc, CmdCheckableProc checkProc, void* procClient);
	virtual void		UnregisterCmd(int cmdid);

	virtual void		GetRealClient(RECT* pRect);
	virtual void		SetStatusText(int index, const wchar_t* szText);
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void	_LoadPluginUI();

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	virtual void AdjustClientArea();

protected:  // 控件条嵌入成员
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

// 生成的消息映射函数
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



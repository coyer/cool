// KToolBar.cpp : 实现文件
//

#include "stdafx.h"
#include "CoolApp.h"
#include "KToolBar.h"


// CKToolBar
IMPLEMENT_DYNCREATE(CKToolBar, CMFCRibbonCategory)


CKToolBar::CKToolBar()
{
}

CKToolBar::~CKToolBar()
{
}


IKPlugin* CKToolBar::GetPlugin()
{
	IKPlugin* plugin = (IKPlugin*)GetData();
	return plugin;
}

// CKToolBar 成员函数
KToolbarGroup CKToolBar::AddGroup(int cmdid, const wchar_t* strText, const wchar_t* strTip)
{
	CMFCRibbonPanel* panel = this->AddPanel(strText);
	
	return panel;
}

KToolbarEle	CKToolBar::AddButton(KToolbarGroup group, int cmdid, int btnType, const wchar_t* szText, int bigIcon, int smallIcon, const wchar_t* szKeys)
{
	CMFCRibbonPanel* panel = (CMFCRibbonPanel*)group;
	CMFCRibbonBaseElement* pEle = 0;

	switch (btnType)
		{
	case KBTN_TYPE_BUTTON:
		pEle = new CMFCRibbonButton(cmdid, szText, bigIcon, smallIcon);
		break;
	case KBTN_TYPE_CHECKBOX:
		pEle = new CMFCRibbonCheckBox(cmdid, szText);
		break;
	case KBTN_TYPE_COMBOBOX:
		pEle = new CMFCRibbonComboBox(cmdid, 1);
		break;
	case KBTN_TYPE_DROPLIST:
		pEle = new CMFCRibbonComboBox(cmdid, 0);
		break;
	case KBTN_TYPE_FONT_SELECT:
		pEle = new CMFCRibbonFontComboBox(cmdid);
		break;
	default:
		break;
	}
	panel->Add(pEle);

	if (szKeys)
		pEle->SetKeys(szKeys);
	
	return pEle;
}

KToolbarEle	CKToolBar::AddLabel(KToolbarGroup group, const wchar_t* szText, int multiline, const wchar_t* szKeys)
{
	CMFCRibbonPanel* panel = (CMFCRibbonPanel*)group;
	CMFCRibbonBaseElement* pEle = new CMFCRibbonLabel(szText, multiline);
	panel->Add(pEle);
	pEle->SetKeys(szKeys);

	return pEle;
}

KToolbarEle	CKToolBar::AddLink(KToolbarGroup group, int cmdid, const wchar_t* szText, const wchar_t* strUrl, const wchar_t* szKeys)
{
	CMFCRibbonPanel* panel = (CMFCRibbonPanel*)group;
	CMFCRibbonBaseElement* pEle = new CMFCRibbonLinkCtrl(cmdid, szText, strUrl);
	panel->Add(pEle);
	pEle->SetKeys(szKeys);

	return pEle;
}

KToolbarEle	CKToolBar::AddEdit(KToolbarGroup group, int cmdid, const wchar_t* szText, int width, const wchar_t* szKeys)
{
	CMFCRibbonPanel* panel = (CMFCRibbonPanel*)group;
	CMFCRibbonBaseElement* pEle = new CMFCRibbonEdit(cmdid, width, szText);
	panel->Add(pEle);
	pEle->SetKeys(szKeys);

	return pEle;
}

int	CKToolBar::GetEleCount(KToolbarGroup group)
{
	CMFCRibbonPanel* panel = (CMFCRibbonPanel*)group;
	if (panel)
		return panel->GetCount();

	return 0;
}

KToolbarEle	CKToolBar::GetEleByIndex(KToolbarGroup group, int index)
{
	CMFCRibbonPanel* panel = (CMFCRibbonPanel*)group;
	if (panel)
		return panel->GetElement(index);

	return 0;
}

void CKToolBar::RemoveEle(KToolbarGroup group, int index)
{
	CMFCRibbonPanel* panel = (CMFCRibbonPanel*)group;
	if (panel)
		panel->Remove(index);
}

int	CKToolBar::GetGroupCount()
{
	return GetPanelCount();
}

KToolbarGroup CKToolBar::GetGroupAt(int index)
{
	return GetPanel(index);
}

void CKToolBar::RemoveGroup(int index)
{
	RemovePanel(index);
}

////////////////////////////////////////////////////////////////////////////////

#pragma once

// CKToolBar ÃüÁîÄ¿±ê

class CKToolBar : public CMFCRibbonCategory,
				  public IKToolbar
{
	DECLARE_DYNCREATE(CKToolBar)
public:
	CKToolBar();
	virtual ~CKToolBar();

	IKPlugin*	GetPlugin();
	
	virtual KToolbarGroup	AddGroup(int cmdid, const wchar_t* strText, const wchar_t* strTip);
	virtual KToolbarEle	    AddButton(KToolbarGroup group, int cmdid, int btnType, const wchar_t* szText, int bigIcon, int smallIcon, const wchar_t* szKeys);
	virtual KToolbarEle		AddLabel(KToolbarGroup group, const wchar_t* szText, int multiline, const wchar_t* szKeys);
	virtual KToolbarEle		AddLink(KToolbarGroup group, int cmdid, const wchar_t* szText, const wchar_t* strUrl, const wchar_t* szKeys);
	virtual KToolbarEle		AddEdit(KToolbarGroup group, int cmdid, const wchar_t* szText, int width, const wchar_t* szKeys);

	virtual int				GetGroupCount();
	virtual KToolbarGroup	GetGroupAt(int index);
	virtual void			RemoveGroup(int index);

	virtual int				GetEleCount(KToolbarGroup group);
	virtual KToolbarEle		GetEleByIndex(KToolbarGroup group, int index);
	virtual void			RemoveEle(KToolbarGroup group, int index);
	
	
protected:

};



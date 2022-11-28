#include "StdAfx.h"
#include "KSkinDefault.h"

KWndSkinDefault::KWndSkinDefault()
{
}

void KWndSkinDefault::Release()
{
}

void KWndSkinDefault::DrawView(HDC hDC, KView* pView)
{
	KRect rc = pView->GetPosition();
	::Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
	KString strCaption = pView->GetText();
	if (strCaption.GetLength() != 0)
		::DrawText(hDC, strCaption, strCaption.GetLength(), &rc, DT_SINGLELINE);

	//RECT rcPos = pView->GetWindowPos();
	//::Rectangle(hDC, 0, 0, rcPos.right, rcPos.bottom);

	//HPEN pen = ::CreatePen(PS_SOLID, 2, RGB(200, 0, 0));
	//HGDIOBJ oldpen = ::SelectObject(hDC, pen);
	//::RoundRect(hDC, 20, 20, rcPos.right - 20, rcPos.bottom - 20, 15, 15);
	//
	//::DrawText(hDC, _T("Hello, my directUI demo."), 20, &rcPos, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	//::SelectObject(hDC, oldpen);
	//::DeleteObject(pen);
}

KWndSkinDefault* KWndSkinDefault::Instance()
{
	static KWndSkinDefault defaultKWndSkin;
	return &defaultKWndSkin;
}

KUI_API KViewSkin* KGetViewDefaultSkin()
{
	return KWndSkinDefault::Instance();	
}

/////////////////////////////////////////////////////////////////////////////////
KSkinSystem*	g_SkinSystem = 0;

KUI_API KSkinSystem*	KGetSkinSystem()
{
	if (0 == g_SkinSystem)
		return KSkinSystemDefault::Instance();

	return g_SkinSystem;
}

KUI_API void KSetSkinSystem(KSkinSystem* pSkin)
{
	g_SkinSystem = pSkin;
}

///////////////////////////////////////////////////////////////
KSkinSystemDefault::KSkinSystemDefault(void)
{
}

KSkinSystemDefault::~KSkinSystemDefault(void)
{
}

KSkinSystemDefault*	KSkinSystemDefault::Instance()
{
	static KSkinSystemDefault g_SkinSystemDefault;
	return &g_SkinSystemDefault;
}

KViewSkin* KSkinSystemDefault::CreateSkin(KString skinname)
{
	if (skinname == g_sKView)
		return KWndSkinDefault::Instance();

	if (skinname == g_sKButton)
		return KButtonSkinDefault::Instance();

	if (skinname == g_sKCanvas)
		return KCanvasSkinDefault::Instance();

	if (skinname == g_sKEdit)
		return KEditSkinDefault::Instance();

	if (skinname == g_sKLabel)
		return KLabelSkinDefault::Instance();

	if (skinname == g_sKLink)
		return KLinkSkinDefault::Instance();

	if (skinname == g_sList)
		return KListSkinDefault::Instance();

	if (skinname == g_sComboBox)
		return KComboBoxSkinDefault::Instance();

	if (skinname == g_sTree)
		return KTreeSkinDefault::Instance();

	if (skinname == g_sToolbar)
		return KToolbarSkinDefault::Instance();

	if (skinname == g_sKViewGroup)
		return KWndGroupSkinDefault::Instance();

	return 0;
}





///////////////////////////////////////////////////////////////
void KWndSkinDefaultBase::Release()
{
}

KWndGroupSkinDefault* KWndGroupSkinDefault::Instance()
{
	static KWndGroupSkinDefault s_GroupSkinDefault;
	return &s_GroupSkinDefault;
}

///////////////////////////////////////////////////////////////
KCanvasSkinDefault::KCanvasSkinDefault()
{
}

KCanvasSkinDefault::~KCanvasSkinDefault()
{
}

void KCanvasSkinDefault::DrawView(HDC hDC, KView* pView)
{
	KRect rc = pView->GetPosition();
	HGDIOBJ hbru = ::GetStockObject(WHITE_BRUSH);
	::SelectObject(hDC, hbru);

	::Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
	//KRect rc = pView->GetWindowPos();

	//int w = rc.Width() / 11;
	//int h = rc.Height() / 10;

	//HBRUSH bru = 0;
	//for(int i=0; i <10; i++)
	//{
	//	int top = i * h;
	//	for(int j=0; j<11; j++)
	//	{
	//		KColorHSL hsl( i * 0.1, 1, j * 0.1);

	//		bru = ::CreateSolidBrush(hsl.GetColor());
	//		::SelectObject(hDC, bru);
	//		::Rectangle(hDC, j * w, top, j * w + w, top + h);
	//		::DeleteObject(bru);
	//	}
	//}
}

KCanvasSkinDefault*	KCanvasSkinDefault::Instance()
{
	static KCanvasSkinDefault s_CanvasSkinDefault;
	return &s_CanvasSkinDefault;
}

///////////////////////////////////////////////////////////////
KButtonSkinDefault::KButtonSkinDefault()
{
}

KButtonSkinDefault::~KButtonSkinDefault()
{
}

void KButtonSkinDefault::DrawView(HDC hDC, KView* pView)
{
	HGDIOBJ hpen = ::GetStockObject(BLACK_PEN);
	::SelectObject(hDC, hpen);

	KRect rcPos = pView->GetPosition();
	::Rectangle(hDC, rcPos.left, rcPos.top, rcPos.right, rcPos.bottom);

	KString strText = pView->GetText();
	if (strText.GetLength() > 0)
		::DrawText(hDC, strText, strText.GetLength(), &rcPos, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

KButtonSkinDefault*	KButtonSkinDefault::Instance()
{
	static KButtonSkinDefault s_BtnSkinDefault;
	return &s_BtnSkinDefault;
}

//2
///////////////////////////////////////////////////////////////
KLinkSkinDefault::KLinkSkinDefault()
{
}

KLinkSkinDefault::~KLinkSkinDefault()
{
}

void KLinkSkinDefault::DrawView(HDC hDC, KView* pView)
{
}

KLinkSkinDefault*	KLinkSkinDefault::Instance()
{
	static KLinkSkinDefault s_LinkSkinDefault;
	return &s_LinkSkinDefault;
}

//3
///////////////////////////////////////////////////////////////
KListSkinDefault::KListSkinDefault()
{
}

KListSkinDefault::~KListSkinDefault()
{
}

void KListSkinDefault::DrawView(HDC hDC, KView* pView)
{
}

KListSkinDefault*	KListSkinDefault::Instance()
{
	static KListSkinDefault s_ListSkinDefault;
	return &s_ListSkinDefault;
}

//4
///////////////////////////////////////////////////////////////
KEditSkinDefault::KEditSkinDefault()
{
}

KEditSkinDefault::~KEditSkinDefault()
{
}

void KEditSkinDefault::DrawView(HDC hDC, KView* pView)
{
}

KEditSkinDefault*	KEditSkinDefault::Instance()
{
	static KEditSkinDefault s_EditSkinDefault;
	return &s_EditSkinDefault;
}

//KLabelSkinDefault
///////////////////////////////////////////////////////////////
KLabelSkinDefault::KLabelSkinDefault()
{
}

KLabelSkinDefault::~KLabelSkinDefault()
{
}

void KLabelSkinDefault::DrawView(HDC hDC, KView* pView)
{
}

KLabelSkinDefault*	KLabelSkinDefault::Instance()
{
	static KLabelSkinDefault s_LabelSkinDefault;
	return &s_LabelSkinDefault;
}

//KTreeSkinDefault
///////////////////////////////////////////////////////////////
KTreeSkinDefault::KTreeSkinDefault()
{
}

KTreeSkinDefault::~KTreeSkinDefault()
{
}


void KTreeSkinDefault::DrawView(HDC hDC, KView* pView)
{
}

KTreeSkinDefault*	KTreeSkinDefault::Instance()
{
	static KTreeSkinDefault s_TreeSkinDefault;
	return &s_TreeSkinDefault;
}

//KToolbarSkinDefault
///////////////////////////////////////////////////////////////
KToolbarSkinDefault::KToolbarSkinDefault()
{
}

KToolbarSkinDefault::~KToolbarSkinDefault()
{
}

void KToolbarSkinDefault::DrawView(HDC hDC, KView* pView)
{
}

KToolbarSkinDefault*	KToolbarSkinDefault::Instance()
{
	static KToolbarSkinDefault s_ToolbarSkinDefault;
	return &s_ToolbarSkinDefault;
}

//KComboBoxSkinDefault
///////////////////////////////////////////////////////////////
KComboBoxSkinDefault::KComboBoxSkinDefault()
{
}

KComboBoxSkinDefault::~KComboBoxSkinDefault()
{
}

void KComboBoxSkinDefault::DrawView(HDC hDC, KView* pView)
{
}

KComboBoxSkinDefault*	KComboBoxSkinDefault::Instance()
{
	static KComboBoxSkinDefault s_ComboBoxSkinDefault;
	return &s_ComboBoxSkinDefault;
}
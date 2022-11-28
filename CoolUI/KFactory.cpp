#include "stdafx.h"

static KViewFactory g_wndFactoryDefault;

static KViewFactory* g_wndFactory = &g_wndFactoryDefault;

KUI_API	KViewFactory* KGetViewFactory()
{
	return g_wndFactory;
}

KUI_API void KSetViewFactory(KViewFactory* pFactory)
{
	g_wndFactory = pFactory;
}

KView* KViewFactory::CreateView(KString wndname)
{
	if (wndname == g_sKView)
		return new KView();

	if (wndname == g_sKButton)
		return new KButton();

	if (wndname == g_sKCanvas)
		return new KCanvas();

	if (wndname == g_sKEdit)
		return new KEdit();

	if (wndname == g_sKLabel)
		return new KLabel();

	if (wndname == g_sKLink)
		return new KLink();

	if (wndname == g_sList)
		return 0;

	if (wndname == g_sComboBox)
		return 0;

	if (wndname == g_sTree)
		return 0;

	if (wndname == g_sToolbar)
		return 0;

	if (wndname == g_sKViewGroup)
		return new KViewGroup;

	return 0;
}

KView* KViewFactory::CreateView(KString wndname, KXmlNode* pAttr)
{
	return KViewFactory::CreateView(wndname);
}
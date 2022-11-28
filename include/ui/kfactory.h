#pragma once

#include "kuidef.h"


typedef KView* (*KViewCreaterFunc)(KXmlNode* pAttr, void* pfuncData);

class KUI_API KViewFactory
{
public:
	virtual	KView*	CreateView(KString wndname);
	virtual KView* CreateView(KString wndname, KXmlNode* pAttr);

	void	RegisterCreater(KString name, KViewCreaterFunc func, void* pfuncData);
	void	UnregisterCreater(KString name);

	KArrayPtr	m_datas;
};


KUI_API	KViewFactory*		KGetViewFactory();
KUI_API void				KSetViewFactory(KViewFactory* pFactory);
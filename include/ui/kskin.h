#pragma once

#include "kuidef.h"
#include "../base/kxml.h"
#include "../base/kcolor.h"

class KUI_API KViewSkin : public KRefBase
{
public:
	KViewSkin();
	virtual ~KViewSkin();

	virtual	void	DrawView(HDC hDC, KView* pView);

	virtual BOOL	SaveSkin(KXmlNode*	pRoot);
	virtual BOOL	LoadSkin(KXmlNode*  pRoot);
};

KUI_API KViewSkin*	KGetViewDefaultSkin();

class KUI_API KSkinSystem
{
public:
	KSkinSystem();
	virtual ~KSkinSystem();

	virtual KViewSkin*	GetSkin(KString skinname);
	virtual	int			GetSkinCount();
	virtual KViewSkin*	GetSkinAt(int index);

	//If you add to my list, please not call pSkin->Release again.
	virtual int			AddSkin(KString skinname, KViewSkin* pSkin);

	virtual void		RemoveSkin(int index);
	virtual void		RemoveSkin(KString skinname);
	virtual int			FindSkinIndex(KString skinname);
	virtual void		RemoveAll();

	virtual BOOL		LoadSkin(KString filename);
	virtual BOOL		SaveSkin(KString filename);

	virtual KViewSkin*	CreateSkin(KString skinname);

	COLORREF	GetColor(double Saturation, double Luminance); 

protected:
	KArrayPtr	m_skins;
	KColorHSL	m_hslBase;
};

KUI_API KSkinSystem*	KGetSkinSystem();
KUI_API void			KSetSkinSystem(KSkinSystem* pSkin);

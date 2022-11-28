#pragma once
#include "kdef.h"
#include "kstring.h"

class KAPI KResource
{
public:
	KResource();
	~KResource();

	virtual BOOL Init(HINSTANCE resInst, KString resPath);
	virtual void Uninit();

	Font	GetFont(KString fontName);
	void	SetFont(KString fontName, Font ft);

	Image	LoadImageFile(KString filename, BOOL cache);
	HCURSOR LoadCursorFile(KString filename);
	BOOL	LoadXmlFile(KString filename, KXmlDoc& xmlDoc, BOOL cache);
	BOOL	LoadLanguageXml(KString filename);

	KString	GetString(KXmlNode* node, KString id);

};
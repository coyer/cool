#pragma once

class KWndSkinDefault : public KViewSkin
{
public:
	KWndSkinDefault();

	//不需要外面释放了
	void Release();

	void DrawView(HDC hDC, KView* pView);

	static KWndSkinDefault*		Instance();
};

class KSkinSystemDefault : public KSkinSystem
{
public:
	KSkinSystemDefault(void);
	~KSkinSystemDefault(void);

	static KSkinSystemDefault*	Instance();

	virtual KViewSkin*	CreateSkin(KString skinname);
};

/////////////////////////////////////////////////////////////////////
class KWndSkinDefaultBase : public KViewSkin
{
public:
	virtual void Release();
};

class KWndGroupSkinDefault : public KWndSkinDefaultBase
{
public:
	static KWndGroupSkinDefault*		Instance();
};

class KCanvasSkinDefault : public KWndSkinDefaultBase
{
public:
	KCanvasSkinDefault();
	~KCanvasSkinDefault();

	void DrawView(HDC hDC, KView* pView);

	static KCanvasSkinDefault*		Instance();
};

class KButtonSkinDefault : public KWndSkinDefaultBase
{
public:
	KButtonSkinDefault();
	~KButtonSkinDefault();

	void DrawView(HDC hDC, KView* pView);

	static KButtonSkinDefault*		Instance();
};

class KLinkSkinDefault : public KWndSkinDefaultBase
{
public:
	KLinkSkinDefault();
	~KLinkSkinDefault();

	void DrawView(HDC hDC, KView* pView);

	static KLinkSkinDefault*		Instance();
};

class KListSkinDefault : public KWndSkinDefaultBase
{
public:
	KListSkinDefault();
	~KListSkinDefault();

	void DrawView(HDC hDC, KView* pView);

	static KListSkinDefault*		Instance();
};

class KEditSkinDefault : public KWndSkinDefaultBase
{
public:
	KEditSkinDefault();
	~KEditSkinDefault();

	void DrawView(HDC hDC, KView* pView);

	static KEditSkinDefault*		Instance();
};

class KLabelSkinDefault : public KWndSkinDefaultBase
{
public:
	KLabelSkinDefault();
	~KLabelSkinDefault();

	void DrawView(HDC hDC, KView* pView);

	static KLabelSkinDefault*		Instance();
};

class KTreeSkinDefault : public KWndSkinDefaultBase
{
public:
	KTreeSkinDefault();
	~KTreeSkinDefault();

	void DrawView(HDC hDC, KView* pView);

	static KTreeSkinDefault*		Instance();
};

class KToolbarSkinDefault : public KWndSkinDefaultBase
{
public:
	KToolbarSkinDefault();
	~KToolbarSkinDefault();

	void DrawView(HDC hDC, KView* pView);

	static KToolbarSkinDefault*		Instance();
};

class KComboBoxSkinDefault : public KWndSkinDefaultBase
{
public:
	KComboBoxSkinDefault();
	~KComboBoxSkinDefault();

	void DrawView(HDC hDC, KView* pView);

	static KComboBoxSkinDefault*		Instance();
};








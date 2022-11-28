#include "stdafx.h"


KViewSkin::KViewSkin()
{
}

KViewSkin::~KViewSkin()
{
}

void KViewSkin::DrawView(HDC hDC, KView* pView)
{
}

BOOL KViewSkin::SaveSkin(KXmlNode* pRoot)
{
	return TRUE;
}

BOOL KViewSkin::LoadSkin(KXmlNode* pRoot)
{
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
typedef struct tagKViewSkinItem
{
	KString		name;
	KViewSkin*	pSkin;
}KViewSkinItem;

static int s_CompareSkinItem(const void* data1, const void* data2)
{
	KViewSkinItem* p1 = (KViewSkinItem*)data1;
	KViewSkinItem* p2 = (KViewSkinItem*)data2;

	return p1->name.Compare(p2->name);
}

static int s_CompareSkinName(const void* pKey, const void* data)
{
	KViewSkinItem* pItem = (KViewSkinItem*)data;
	KString* name = (KString*)pKey;
	return name->Compare(pItem->name);
}

static void s_DestroySkinItem(void* pdata)
{
	KViewSkinItem* pItem = (KViewSkinItem*)pdata;
	pItem->pSkin->Release();
	delete pItem;
}

KSkinSystem::KSkinSystem()
{
	m_skins.SetDestroyDataProc(s_DestroySkinItem);
	m_skins.SetSortProc(s_CompareSkinItem);
}

KSkinSystem::~KSkinSystem()
{
	RemoveAll();
}

KViewSkin* KSkinSystem::GetSkin(KString skinname)
{
	int index = m_skins.FindByKey(s_CompareSkinName, &skinname);
	if (index < 0)
		return KGetViewDefaultSkin();

	KViewSkinItem* pItem = (KViewSkinItem*)m_skins.GetAt(index);
	return pItem->pSkin;
}

int KSkinSystem::GetSkinCount()
{
	return m_skins.GetCount();
}

KViewSkin* KSkinSystem::GetSkinAt(int index)
{
	KViewSkinItem* pItem = (KViewSkinItem*)m_skins.GetAt(index);
	if (pItem)
		return pItem->pSkin;
	return 0;
}


//if add to my list, please not call pSkin->Release again.
int KSkinSystem::AddSkin(KString skinname, KViewSkin* pSkin)
{
	int index = m_skins.FindByKey(s_CompareSkinName, &skinname);
	if (index < 0)
	{
		KViewSkinItem* pItem = new KViewSkinItem;
		pItem->name = skinname;
		pItem->pSkin = pSkin;

		index = m_skins.Add(pItem);
	}
	else
	{
		KViewSkinItem* pItem = (KViewSkinItem*)m_skins.GetAt(index);
		pItem->pSkin->Release();
		pItem->pSkin = pSkin;
	}

	return index;
}

void KSkinSystem::RemoveSkin(int index)
{
	m_skins.RemoveAt(index);
}

void KSkinSystem::RemoveSkin(KString skinname)
{
	int index = m_skins.FindByKey(s_CompareSkinName, &skinname);
	if (index >= 0)
	{
		m_skins.RemoveAt(index);
	}
}

int KSkinSystem::FindSkinIndex(KString skinname)
{
	return m_skins.FindByKey(s_CompareSkinName, &skinname);
}

void KSkinSystem::RemoveAll()
{
	m_skins.RemoveAll();
}

KViewSkin* KSkinSystem::CreateSkin(KString skinname)
{
	return 0;
}

COLORREF KSkinSystem::GetColor(double Saturation, double Luminance)
{
	return m_hslBase.GetColor(Saturation, Luminance);
}

BOOL KSkinSystem::LoadSkin(KString filename)
{
	KXmlDoc doc;
	if (!doc.LoadFromFile(filename.c_str()))
		return FALSE;

	KXmlNode* pRoot = doc.GetRoot();

	LPCTSTR szFlag = pRoot->GetName();
	if (g_sSkin != szFlag)
		return FALSE;

	RemoveAll();

	int n = pRoot->GetChildCount();
	for(int i=0; i<n; i++)
	{
		KXmlNode* pNode = pRoot->GetChild(i);
		if (pNode->GetName() != g_sSkin)
			continue;

		KString skinName = pNode->GetAttrValue(_T("type_name"), 0);
		KViewSkin* pSkin = CreateSkin(skinName);

		if (pSkin)
		{
			AddSkin(skinName, pSkin);
		}
	}
	return TRUE;
}

BOOL KSkinSystem::SaveSkin(KString filename)
{
	KXmlDoc doc;
	doc.CreateNewDoc(g_sSkin);
	KXmlNode* pRoot = doc.GetRoot();

	int n = m_skins.GetCount();
	for(int i=0; i<n; i++)
	{
		KViewSkinItem* pItem = (KViewSkinItem*)m_skins.GetAt(i);
		KXmlNode* pNode = pRoot->AddChild(g_sSkin);
		pNode->SetAttrValue(_T("type_name"), pItem->name);
		pItem->pSkin->SaveSkin(pNode);
	}

	return pRoot->SaveToFile(filename);
}

///////////////////////////////////////////////////////////////////







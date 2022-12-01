#include "stdafx.h"
#include <tchar.h>
#include "MsXmlParse.h"

static KString g_strTrue = _T("true");
static KString g_strFalse = _T("false");

void s_DestroyXmlNode(void* pdata)
{
	KXmlNode* pNode = (KXmlNode*)pdata;
	pNode->Release();
}

int s_CompareXmlNode(const void* data1, const void* data2)
{
	KXmlNode* pNode1 =(KXmlNode*)data1;
	KXmlNode* pNode2 =(KXmlNode*)data2;

	const wchar_t* p1 = pNode1->GetName();
	const wchar_t* p2 = pNode2->GetName();
	return _wcsicmp(pNode1->GetName(), pNode2->GetName());
}

int s_CompareXmlNodeByName(const void* pKey, const void* data)
{
	KString name = (const TCHAR*)pKey;
	KXmlNode* pNode =(KXmlNode*)data;

	return _wcsicmp(name, pNode->GetName());
}

void s_DestroyXmlAttr(void* pdata)
{
	KXmlAttr* pNode = (KXmlAttr*)pdata;
	delete pNode;
}

int s_CompareXmlAttr(const void* data1, const void* data2)
{
	KXmlAttr* pAttr1 =(KXmlAttr*)data1;
	KXmlAttr* pAttr2 =(KXmlAttr*)data2;

	return _wcsicmp(pAttr1->key.c_str(), pAttr2->key.c_str());
}

int s_CompareXmlAttrByName(const void* pKey, const  void* data)
{
	KString name = (const TCHAR*)pKey;
	KXmlAttr* pAttr =(KXmlAttr*)data;

	return _wcsicmp(name, pAttr->key.c_str());
}

///////////////////////////////////////////////////////////////////
KXmlNode::KXmlNode() : m_child(s_CompareXmlNode), m_attrs(s_CompareXmlAttr)
{
	m_child.SetDestroyDataProc(s_DestroyXmlNode);

	m_attrs.SetDestroyDataProc(s_DestroyXmlAttr);

	m_type = KXML_TYPE_ELEMENT;
}

KXmlNode::KXmlNode(NodeType type) : m_child(s_CompareXmlNode), m_attrs(s_CompareXmlAttr)
{
	m_child.SetDestroyDataProc(s_DestroyXmlNode);

	m_attrs.SetDestroyDataProc(s_DestroyXmlAttr);

	m_type = type;
}

KXmlNode::~KXmlNode()
{
	RemoveAttrAll();
	RemoveChildAll();	
}

KString KXmlNode::GetName()
{
	return m_name;
}

void KXmlNode::SetName(KString name)
{
	m_name = name;
}

int	KXmlNode::GetType()
{
	return m_type;
}

int	KXmlNode::GetChildCount()
{
	return m_child.GetCount();
}

int	KXmlNode::FindChild(KString name)
{
	int index = m_child.FindByKey(s_CompareXmlNodeByName, (void*)name.GetBuffer());
	return index;
}

KXmlNode* KXmlNode::GetChild(int index)
{
	//if (index < 0 || index >= m_child.GetCount())
	//	return 0;
	return (KXmlNode*)m_child.GetAt(index);
}

KXmlNode* KXmlNode::GetChild(KString name)
{
	int index = FindChild(name);
	if (index < 0)
		return 0;

	return GetChild(index);
}

KXmlNode* KXmlNode::GetChildByAttr(KString key, KString& val)
{
	if (0 == key.GetLength() || 0 == val.GetLength())
		return 0;

	int n = m_child.GetCount();
	for(int i=0; i<n; i++)
	{
		KXmlNode *pChild = (KXmlNode *)m_child.GetAt(i);
		const TCHAR* tval = pChild->GetAttrValue(key, 0);
		if (tval && val == tval)
			return pChild;
	}
	return 0;
}

KXmlNode* KXmlNode::GetChildByAttr(KString key, const TCHAR* val)
{
	if (0 == key.GetLength() || 0 == val)
		return 0;

	int n = m_child.GetCount();
	for(int i=0; i<n; i++)
	{
		KXmlNode *pChild = (KXmlNode *)m_child.GetAt(i);
		const TCHAR* tval = pChild->GetAttrValue(key, 0);
		if (tval && _wcsicmp(tval, val) == 0)
			return pChild;
	}
	return 0;
}

KXmlNode* KXmlNode::AddChild(KString name, KXmlNode::NodeType type)
{
	KXmlNode* pChild = new KXmlNode(type);
	pChild->SetName(name);

	m_child.Add(pChild);

	return pChild;
}

KXmlNode* KXmlNode::PickupNode(int index)
{
	KXmlNode* node = (KXmlNode *)m_child.GetAt(index);
	m_child.RemoveAt(index);

	return node;
}

KXmlNode* KXmlNode::PickupNode(KString name)
{
	int index = FindChild(name);
	if (index < 0)
		return NULL;

	return PickupNode(index);
}

void KXmlNode::AddChild(KXmlNode* pNode)
{
	m_child.Add(pNode);
}

void KXmlNode::RemoveChild(KString& name)
{
	int index = m_child.FindByKey(s_CompareXmlNodeByName, (void*)name.GetBuffer());
	if (index < 0)
		return;

	m_child.RemoveAt(index);
}

void KXmlNode::RemoveChild(const TCHAR* name)
{
	int index = m_child.FindByKey(s_CompareXmlNodeByName, (void*)name);
	if (index < 0)
		return;

	m_child.RemoveAt(index);
}

void KXmlNode::RemoveChild(int index)
{
	m_child.RemoveAt(index);
}

void KXmlNode::RemoveChildAll()
{
	m_child.RemoveAll();
}

int	KXmlNode::GetAttrCount()
{
	return m_attrs.GetCount();
}

KXmlAttr* KXmlNode::GetAttrAt(int index)
{
	return (KXmlAttr*)m_attrs.GetAt(index);
}

void KXmlNode::RemoveAttr(KString& key)
{
	if (0 == key.GetLength())
		return;

	int index = m_attrs.FindByKey(s_CompareXmlAttrByName, (void*)key.GetBuffer());
	if (index < 0)
		return;

	m_attrs.RemoveAt(index);
}

void KXmlNode::RemoveAttr(const TCHAR* key)
{
	if (0 == key)
		return;

	int index = m_attrs.FindByKey(s_CompareXmlAttrByName, (void*)key);
	if (index < 0)
		return;

	m_attrs.RemoveAt(index);
}

void KXmlNode::RemoveAttr(int index)
{
	m_attrs.RemoveAt(index);
}

void KXmlNode::RemoveAttrAll()
{
	m_attrs.RemoveAll();
}

KString	KXmlNode::GetChildValueText(KString childname, KString defval)
{	
	if (0 == childname.GetLength())
		return defval;
	
	int index = m_child.FindByKey(s_CompareXmlNodeByName, (void*)childname.GetBuffer());
	if (index < 0)
		return defval;

	KXmlNode* pNode = (KXmlNode*)m_child.GetAt(index);
	return pNode->GetName();
}

const TCHAR* KXmlNode::GetChildValueText(KString childname, const TCHAR* defval)
{
	if (0 == childname.GetLength())
		return defval;
	
	int index = m_child.FindByKey(s_CompareXmlNodeByName, (void*)childname.GetBuffer());
	if (index < 0)
		return defval;

	KXmlNode* pNode = (KXmlNode*)m_child.GetAt(index);
	return pNode->GetName();
}

int	KXmlNode::GetChildValueInt(KString childname, int defval)
{
	const TCHAR* tmp = GetChildValueText(childname, 0);
	if (0 == tmp)
		return defval;

	return _tcstol(tmp, 0, 10);
}

__int64	KXmlNode::GetChildValueInt64(KString childname, __int64 defval)
{
	const TCHAR* tmp = GetChildValueText(childname, 0);
	if (0 == tmp)
		return defval;

	return _tcstoi64(tmp, 0, 10);
}

double	KXmlNode::GetChildValueDouble(KString childname, double defval)
{
	const TCHAR* tmp = GetChildValueText(childname, 0);
	if (0 == tmp)
		return defval;

	return _tcstod(tmp, 0);
}

bool KXmlNode::GetChildValueBool(KString childname, bool defval)
{
	const TCHAR* tmp = GetChildValueText(childname, 0);
	if (0 == tmp)
		return defval;

	if (g_strTrue == tmp)
		return true;

	return false;
}

unsigned int KXmlNode::GetChildValueUInt(KString childname, unsigned int defval)
{
	const TCHAR* tmp = GetChildValueText(childname, 0);
	if (0 == tmp)
		return defval;

	return _tcstol(tmp, 0, 10);
}

void KXmlNode::AddChildValue(KString childname, KString& value)
{
	if (0 == childname.GetLength())
		return;

	KXmlNode* pNode = AddChild(childname, KXmlNode::KXML_TYPE_ELEMENT);

	if (0 != value.GetLength())
	{
		KXmlNode* pSubNode = AddChild(value, KXML_TYPE_TEXT);
	}
	m_child.Add(pNode);
}

void KXmlNode::AddChildValue(KString childname, const TCHAR* value)
{
	if (0 == childname.GetLength())
		return;

	KXmlNode* pNode = AddChild(childname, KXmlNode::KXML_TYPE_ELEMENT);

	if (0 != value && 0 != value[0])
	{
		KXmlNode* pSubNode = AddChild(value, KXML_TYPE_TEXT);
	}
	m_child.Add(pNode);
}

void KXmlNode::AddChildValue(KString childname, int value)
{
	KString strVal;
	strVal.Format(_T("%d"), value);
	AddChildValue(childname, strVal.c_str());
}

void KXmlNode::AddChildValue(KString childname, unsigned int value)
{
	KString strVal;
	strVal.Format(_T("%d"), value);
	AddChildValue(childname, strVal.c_str());
}

void KXmlNode::AddChildValue(KString childname, __int64 value)
{
	KString strVal;
	strVal.Format(_T("%I64d"), value);
	AddChildValue(childname, strVal.c_str());
}

void KXmlNode::AddChildValue(KString childname, double value)
{
	KString strVal;
	strVal.Format(_T("%f"), value);
	AddChildValue(childname, strVal.c_str());
}

void KXmlNode::AddChildValue(KString childname, bool value)
{
	KString strVal;
	if (value)
	{
		strVal = g_strTrue;
	}
	else
	{
		strVal = g_strFalse;
	}
	AddChildValue(childname, strVal.c_str());
}

const TCHAR* KXmlNode::GetAttrValue(KString key, const TCHAR* defval)
{
	if (0 == key.GetLength())
		return defval;

	int index = m_attrs.FindByKey(s_CompareXmlAttrByName, (void*)key.GetBuffer());
	if (index < 0)
		return defval;

	KXmlAttr* pAttr = (KXmlAttr* )m_attrs.GetAt(index);
	return pAttr->val.c_str();
}

KString	KXmlNode::GetAttrValue(KString key, KString& strVal)
{
	if (key.GetLength() == 0)
		return strVal;

	int index = m_attrs.FindByKey(s_CompareXmlAttrByName, (void*)key.GetBuffer());
	if (index < 0)
		return strVal;

	KXmlAttr* pAttr = (KXmlAttr* )m_attrs.GetAt(index);
	return pAttr->val;
}

int	KXmlNode::GetAttrValueInt(KString key, int defval)
{
	const TCHAR* tval = GetAttrValue(key, 0);
	if (0 == tval)
		return defval;

	return _tcstol(tval, 0, 10);
}

unsigned int KXmlNode::GetAttrValueUInt(KString key, unsigned int defval)
{
	const TCHAR* tval = GetAttrValue(key, 0);
	if (0 == tval)
		return defval;

	return _tcstol(tval, 0, 10);
}

__int64	KXmlNode::GetAttrValueInt64(KString key, __int64 defval)
{
	const TCHAR* tval = GetAttrValue(key, 0);
	if (0 == tval)
		return defval;

	return _tcstoi64(tval, 0, 10);
}

double KXmlNode::GetAttrValueDouble(KString key, double defval)
{
	const TCHAR* tval = GetAttrValue(key, 0);
	if (0 == tval)
		return defval;

	return _tcstod(tval, 0);
}

bool KXmlNode::GetAttrValueBool(KString key, bool defval)
{
	const TCHAR* tval = GetAttrValue(key, 0);
	if (0 == tval)
		return defval;

	if (g_strTrue == tval)
		return true;

	return false;
}

bool KXmlNode::SetAttrValue(KString key, KString& value)
{
	if (key.GetLength() == 0 || value.GetLength() == 0)
		return false;

	KXmlAttr* pAttr = 0;
	int index = m_attrs.FindByKey(s_CompareXmlAttrByName, (void*)key.GetBuffer());
	if (index < 0)
	{
		pAttr = new KXmlAttr;
		pAttr->key = key;
		m_attrs.Add(pAttr);
	}
	pAttr->val = value;

	return true;
}

bool KXmlNode::SetAttrValue(KString key, const TCHAR* value)
{
	if (0 == key || 0 == key[0] || 0 == value || 0 == value[0])
		return false;

	KXmlAttr* pAttr = 0;
	int index = m_attrs.FindByKey(s_CompareXmlAttrByName, (void*)key.GetBuffer());
	if (index < 0)
	{
		pAttr = new KXmlAttr;
		pAttr->key = key;
		m_attrs.Add(pAttr);
	}
	pAttr->val = value;

	return true;
}

bool KXmlNode::SetAttrValue(KString key, int value)
{
	KString strVal;
	strVal = value;
	return SetAttrValue(key, strVal.c_str());
}

bool KXmlNode::SetAttrValue(KString key, unsigned int value)
{
	KString strVal;
	strVal = value;
	return SetAttrValue(key, strVal.c_str());
}

bool KXmlNode::SetAttrValue(KString key, __int64 value)
{
	KString strVal;
	strVal = value;
	return SetAttrValue(key, strVal.c_str());
}

bool KXmlNode::SetAttrValue(KString key, double value)
{
	KString strVal;
	strVal = value;
	return SetAttrValue(key, strVal.c_str());
}

bool KXmlNode::SetAttrValue(KString key, bool value)
{
	KString strVal;
	if (value)
		strVal = g_strTrue;
	else
		strVal = g_strFalse;

	return SetAttrValue(key, strVal.c_str());
}

KStringW KXmlNode::ToString()
{
	CMsXmlParse parse;
	return parse.CreateXmlString(this);
}

bool KXmlNode::SaveToFile(const wchar_t* filename)
{
	CMsXmlParse parse;
	return parse.SaveXmlFile(filename, this);	
}

bool KXmlNode::SaveToFile(const char* filename)
{
	KStringW strFile = filename;
	return SaveToFile(strFile);
}


////////////////////////////////////////////////////////////////////////////////////////////////



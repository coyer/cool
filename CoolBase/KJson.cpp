#include "StdAfx.h"
#include <tchar.h>
#include <stdlib.h>

#include "../include/base/kjson.h"

//////////////////////////////////////////////////////////////////
//util functions.
static TCHAR* SkipBlankBuffer(const TCHAR* pbuf)
{
	int i = 0;
	while (*pbuf)
	{
		if (*pbuf <= 32)
		{
			pbuf++;
			continue;
		}
		break;
	}
	return (TCHAR*)pbuf;
}
//////////////////////////////////////////////////////////////////
KJson::KJson()
{
	
}
bool KJson::isValue()
{
	return getType() == JST_VALUE; 
}

bool KJson::isObject()
{ 
	return getType() == JST_OBJECT;
}

bool KJson::isArray()
{
	return getType() == JST_ARRAY;
}

bool KJson::saveToFile(const KString& filename)
{
	KTextFile file;
	if (!file.Create(filename, KTextFile::cf_utf8)) {
		return false;
	}
	KString str = toString();
	KStringA utf8 = KStringToUtf8(str);

	file.WriteString(str);
	file.Close();
		 
	return false;
}

KJson* KJson::FromFile(const KString& filename)
{
	KTextFile file;
	if (!file.Open(filename, false)) {
		return NULL;
	}
	__int64 filelen = file.GetFileSize();
	if (filelen > 100 * 1024 * 1024)
	{
		printf("file size > 100M, cannot read.");
		return NULL;
	}

	KString strJson;
	if (!file.ReadAllText(strJson)) return NULL;
	
	return KJson::FromString(strJson);
}

enum TokenFlag
{
	TF_VALUE = 0,
	TF_BraceLeft, // {
	TF_BraceRight,// }
	TF_BracketLeft,		// [
	TF_BracketRight,	// ]
	TF_SingleQuotationLeft,	// '
	TF_SingleQuotationRight, 
	TF_DoubleQuotationLeft, // "
	TF_DoubleQuotationRight,
	TF_Semicolon,	// ;
	TF_Colon,		// :
	TF_Comma,		// ,

};

struct JsonParseData {
	const TCHAR* pStart;
	TCHAR* pEnd;
	TokenFlag  token;
	int nodeLen;
	KJson* pNode;
};

//todo
int getToken(const TCHAR* str, JsonParseData od) {
	TCHAR* p = SkipBlankBuffer(str);
	JsonParseData d;
	d.pStart = p;
	d.pNode = NULL;

	if (*p == _T('{')) {
		d.token = TF_BraceLeft;
		d.pNode = new KJObject();
	}
	else if (*p == _T('[')) {
		d.token = TF_BracketLeft;
		d.pNode = new KJArr();
	}
	else if (*p == _T('"')) {	//ÊÇÒ»¸ökey
		d.token = TF_DoubleQuotationLeft;
		
	}

	return NULL;
}

TCHAR* parseJson(const TCHAR* str) {
	TCHAR* p = SkipBlankBuffer(str);
	return NULL;
}

KJson* KJson::FromString(const KString& str)
{
	const TCHAR* p = str.c_str();

	return 0;
}

/// <summary>
/// 
/// </summary>
/// 
/// 	KJValue(int val);
/// 
KJson::JSonType KJValue::getType()
{
	return KJson::JST_VALUE;
}

bool KJValue::isString()
{
	return getValueType() == JSVT_STRING;
}

bool KJValue::isInt()
{ 
	JSonValueType t = getValueType();
	return t == JSVT_INT || t == JSVT_INT64;
}

bool KJValue::isDouble()
{
	return getValueType() == JSVT_DOUBLE;
}

bool KJValue::isNumber()
{
	return isDouble() || isInt();
}

KString KJValue::toString()
{
	return asString();
}

/// <summary>
/// KJNumber
/// </summary>
/// 

KJNumber::KJNumber()
{
	m_type = JSVT_INT;
	m_data.val = 0;
}

KJNumber::KJNumber(KJNumber& num)
{
	m_type = num.m_type;
	m_data = num.m_data;
}

KJNumber::KJNumber(JSonValueType number_type)
{
	m_data.in64 = 0;
	m_data.dbl = 0;
	m_data.val = 0;
	if (number_type == JSVT_STRING) {
		//throw error.
		m_type = JSVT_NONE;
		return;
	}
	m_type = number_type;
}

KJNumber::KJNumber(int v)
{
	m_type = JSVT_INT;
	m_data.val = v;
}

KJNumber::KJNumber(__int64 v)
{
	m_type = JSVT_INT64;
	m_data.in64 = v;
}

KJNumber::KJNumber(double v)
{
	m_type = JSVT_DOUBLE;
	m_data.dbl = v;
}

KJson::JSonValueType KJNumber::getValueType()
{
	return m_type;
}

void KJNumber::clear()
{
	m_data.in64 = 0;
	m_data.dbl = 0;
	m_data.val = 0;
}

KJson* KJNumber::clone()
{
	KJNumber* p = new KJNumber(*this);
	return p;
}

int	KJNumber::asInt()
{
	if (m_type == JSVT_DOUBLE) {
		return (int)m_data.dbl;
	}
	else if (m_type == JSVT_INT64) {
		return (int)m_data.in64;
	}
	else if (m_type == JSVT_INT) {
		return m_data.val;
	}
	return 0;
}

double	KJNumber::asDouble()
{
	if (m_type == JSVT_DOUBLE) {
		return m_data.dbl;
	}
	else if (m_type == JSVT_INT64) {
		return m_data.in64 * 1.0;
	}
	else if (m_type == JSVT_INT) {
		return m_data.val *1.0;
	}
	return 0.0f;
}

__int64	KJNumber::asInt64()
{
	if (m_type == JSVT_DOUBLE) {
		return (__int64)m_data.dbl;
	}
	else if (m_type == JSVT_INT64) {
		return m_data.in64;
	}
	else if (m_type == JSVT_INT) {
		return m_data.val;
	}
	return 0.0f;
}

char KJNumber::asChar()
{
	if (m_type == JSVT_DOUBLE) {
		int t = (int)m_data.dbl;
		return (char)t;
	}
	else if (m_type == JSVT_INT64) {
		return (char)m_data.in64;
	}
	else if (m_type == JSVT_INT) {
		return (char)m_data.val;
	}
	return 0;
}

KString	KJNumber::asString()
{
	KString str;
	if (m_type == JSVT_DOUBLE) {
		str.Format(_T("%f"), m_data.dbl);
	}
	else if (m_type == JSVT_INT64) {
		str.Format(_T("%l"), m_data.in64);
	}
	else if (m_type == JSVT_INT) {
		str.Format(_T("%d"), m_data.val);
	}
	return str;
}

int KJNumber::get()
{
	return asInt();
}

void KJNumber::set(int v)
{
	m_type = JSVT_INT;
	m_data.val = v;
}

void KJNumber::set(__int64 v)
{
	m_type = JSVT_INT64;
	m_data.in64 = v;
}

void KJNumber::set(double v)
{
	m_type = JSVT_DOUBLE;
	m_data.dbl = v;
}

/// <summary>
/// KJString
/// </summary>
KJString::KJString()
{
	m_data = _T("");
}

KJString::KJString(KJString& obj)
{
	m_data = obj.m_data;
}

KJString::KJString(KJNumber& obj)
{
	m_data = obj.toString();
}

KJString::KJString(const KString& str)
{
	m_data = str;
}

KJString::KJString(const char* pStr)
{
	m_data = pStr;
}

KJString::KJString(const wchar_t* pStr)
{
	m_data = pStr;
}

KJson::JSonValueType KJString::getValueType()
{
	return JSVT_STRING;
}

void KJString::clear()
{
	m_data = _T("");
}

KJson* KJString::clone()
{
	KJson* p = new KJString(m_data);
	return p;
}
int	KJString::asInt()
{
	return m_data.toInt();
}

double	KJString::asDouble()
{
	return m_data.toDouble();
}

__int64	KJString::asInt64()
{
	return m_data.toInt64();
}

char KJString::asChar()
{
	return (char)asInt();
}

KString	KJString::asString()
{
	KString str;
	str.SetSize(m_data.GetLength() + 4);
	str = _T("'");
	str += m_data;
	str += _T("'");
	return str;
}

KString KJString::get()
{
	return m_data;
}

void KJString::set(const KString& str)
{
	m_data = str;
}
////////////////////////////////////////////////////////////////////////////////////////////


/// <summary>
/// KJArr
/// </summary>
void KJsonDestroyData(void* pData) {
	KJson* p = (KJson*)pData;
	p->Release();
}

KJArr::KJArr()
{
	m_data.SetDestroyDataProc(KJsonDestroyData);
}

KJArr::KJArr(KJArr& arr)
{
	m_data.SetDestroyDataProc(KJsonDestroyData);
	m_data = arr.m_data;
}

KJArr::~KJArr()
{

}

KJson::JSonType KJArr::getType()
{
	return KJson::JST_ARRAY;
}

KString KJArr::toString()
{
	int n = getCount();
	if (n == 0) return _T("[]");

	KStringArray str;
	str.Add(_T("["));

	for (int i = 0; i < n; i++) {
		str.Add(get(i)->toString());
		str.Add(_T(","));
	}
	str.SetAt(n - 1, _T("]"));
	return str.ToString();
}

void KJArr::clear()
{
	m_data.RemoveAll();
}

int KJArr::add(KJson* v)
{
	return m_data.Add(v);
}

void KJArr::set(int index, KJson* v)
{
	m_data.SetAt(index, v);
}

void KJArr::remove(int index, int count)
{
	m_data.Remove(index, count);
}

int KJArr::getCount()
{
	return m_data.GetCount();
}

KJson* KJArr::get(int index)
{
	return (KJson*)m_data.GetAt(index);
}

KJson* KJArr::operator[] (int index)
{
	return (KJson*)m_data.GetAt(index);
}

KJson* KJArr::clone()
{
	KJArr* p = new KJArr();
	for (int i = 0; i < m_data.GetCount(); i++) {
		KJson* obj = get(i)->clone();
		p->add(obj);
	}
	return p;
}

////
KJObjectPairData::KJObjectPairData() {

}

KJObjectPairData::KJObjectPairData(const KString& name, KJson* v)
{
	keyname = name;
	value = v;
}

KJObjectPairData::~KJObjectPairData()
{
	if (value) {
		value->Release();
		value = NULL;
	}
}

KJObjectPairData* KJObjectPairData::clone() {
	return new KJObjectPairData(keyname, value);
}

void KJObjectPairData::setValue(KJson* val)
{
	if (value) {
		value->Release();
	}
	value = val;
}

void s_JsonObjectDestroyData(void* pData) {
	KJObjectPairData* p = (KJObjectPairData*)pData;
	delete p;
}

int s_JsonObjectCompareProc(const void* data1, const void* data2)
{
	KJObjectPairData* p1 = (KJObjectPairData*)data1;
	KJObjectPairData* p2 = (KJObjectPairData*)data2;

	return p1->keyname.Compare(p2->keyname);
}

int s_JsonObjectCompareByKeyProc(const void* pKey, const void* data)
{
	const KString* p1 = (const KString*)pKey;
	KJObjectPairData* p2 = (KJObjectPairData*)data;

	//return p1->Compare(p2->keyname.c_str());
	return _tcscmp(p1->c_str(), p2->keyname.c_str());
}
/// <summary>
/// 
/// </summary>
KJObject::KJObject()
{
	m_data.SetDestroyDataProc(s_JsonObjectDestroyData);
	m_data.SetSortProc(s_JsonObjectCompareProc);
}

KJObject::~KJObject()
{

}

KJson::JSonType KJObject::getType()
{
	return KJson::JST_OBJECT;
}


void KJObject::set(const KString& name, int val)
{
	set(name, new KJNumber(val));
}

void KJObject::set(const KString& name, __int64 val)
{
	set(name, new KJNumber(val));
}

void KJObject::set(const KString& name, double val)
{
	set(name, new KJNumber(val));
}

void KJObject::set(const KString& name, const KString& val)
{
	set(name, new KJString(val));
}

void KJObject::set(const KString& name, const char* val)
{
	KString str = val;
	set(name, str);
}

void KJObject::set(const KString& name, const wchar_t* val)
{
	KString str = val;
	set(name, str);
}

void KJObject::set(const KString& name, KJson* val)
{
	if (!name) {
		if (val) val->Release();
		return;
	}

	KJObjectPairData* p = _FindPairData(name);
	if (p) {
		p->setValue(val);
	}
	else {
		m_data.Add(new KJObjectPairData(name, val));
	}
}

KJObjectPairData* KJObject::_FindPairData(const KString& name)
{
	if (!name) return NULL;

	int index = m_data.FindByKey(s_JsonObjectCompareByKeyProc, (VOID*)&name);
	if (index < 0) return NULL;

	KJObjectPairData* p = (KJObjectPairData*)m_data.GetAt(index);
	return p;
}

KJson* KJObject::get(const KString& name)
{
	KJObjectPairData* p = _FindPairData(name);
	return p->value;
}

KStringArray KJObject::getKeys()
{
	KStringArray ret;
	return ret;
}

KString KJObject::toString()
{
	if (m_data.IsNull()) return _T("");
	KString str;
	KStringArray arr;

	arr.Add(_T("{"));
	int n = m_data.GetCount();
	for (int i = 0; i < n; i++) {
		KJObjectPairData* pair = (KJObjectPairData*)m_data.GetAt(i);
		arr.Add(pair->keyname);
		arr.Add(_T(":"));
		arr.Add(pair->value->toString());
		arr.Add(_T(","));
	}
	arr.SetAt(n - 1, _T("}"));
	return arr.ToString();
}

void KJObject::clear()
 {
	m_data.RemoveAll();
}

KJson* KJObject::clone()
{
	KJObject* p = new KJObject();
	int n = m_data.GetCount();
	for (int i = 0; i < n; i++) {
		KJObjectPairData* pair = (KJObjectPairData * )m_data.GetAt(i);
		p->m_data.Add(pair->clone());
	}

	return p;
}
#include "stdafx.h"
#include "../include/base/kvar.h"

typedef struct tagKVariantPairData
{
	KVariant* key;
	KVariant* value;
}KVariantPairData;


KVariant::KVariant()
{
	m_type = KVariant::kvt_none;
	m_len = 0;
	m_data.in = 0;
	m_data.ptr = 0;
}

KVariant::KVariant(KVariant::varType type)
{
	m_type = type;
	m_data.ptr = 0;
	m_len = 0;
}

KVariant::~KVariant()
{
	Clear();

	switch(m_type)
	{
	case kvt_pair:
		{
			KVariantPairData* pair = (KVariantPairData*)m_data.ptr;
			delete pair;
			m_data.ptr = 0;
		}
		break;
	case kvt_arr:
		{
			KArrayPtr* pArr = (KArrayPtr*)m_data.ptr;
			delete pArr;
			m_data.ptr = 0;
		}
	default:
		break;
	}
	m_data.in  = 0;
}

KVariant::varType KVariant::GetType()
{
	return m_type;
}

void KVariant::Clear()
{
	if (m_data.ptr)
	{
		switch(m_type)
		{
		case kvt_pair:
			{
				KVariantPairData* pair = (KVariantPairData*)m_data.ptr;
				if (pair)
				{
					if (pair->key)
						pair->key->Release();
					if (pair->value)
						pair->value->Release();

					pair->key = 0;
					pair->value = 0;
				}
				m_data.ptr = 0;
			}
			break;
		case kvt_arr:
			{
				KArrayPtr* pArr = (KArrayPtr*)m_data.ptr;
				int n = pArr->GetCount();
				
				for(int i=0; i<n; i++)
				{
					KVariant* p = (KVariant*)pArr->GetAt(i);
					p->Release();
				}
				pArr->RemoveAll();
				m_data.ptr = 0;
			}
			break;
		case kvt_strw:
			{
				wchar_t* p = (wchar_t*)m_data.wstr;
				free(p);
			}
			break;
		case kvt_stra:
		case kvt_bin:
			{
				char* p = (char*)m_data.cstr;
				free(p);
			}
		}
	}

	m_data.in64 = 0;
	m_len = 0;
}

//if is char
char KVariant::GetValueChar()
{
	return (char)m_data.ch;
}

bool KVariant::SetValueChar(char val)
{
	Clear();
	m_type = kvt_char;
	m_data.ch = val;
	return true;
}

//if is short.
short KVariant::GetValueShort()
{
	return m_data.sh;
}

bool KVariant::SetValueShort(short val)
{
	Clear();
	m_type = kvt_short;
	m_data.sh = val;
	return true;
}

//if is int
int KVariant::GetValueInt()
{
	return m_data.in;
}

bool KVariant::SetValueInt(int val)
{
	Clear();
	m_type = kvt_int;
	m_data.in = val;
	return true;
}
//if is int64
__int64 KVariant::GetValueInt64()
{
	return m_data.in64;
}

bool KVariant::SetValueInt64(__int64 val)
{
	Clear();
	m_type = kvt_int64;
	//m_data = (void*)(val && 0x00000000FFFFFFFF);
	//m_len = val >> 32;
	m_data.in64 = val;
	return true;
}

//if is stra
const char* KVariant::GetValueStrA(int* len)
{
	if (len)
		*len = m_len;

	return (const char*)m_data.cstr;
}

bool KVariant::SetValueStrA(const char* str, int len)
{
	Clear();
	if (0 == str || 0 == len)
		return false;

	m_type = kvt_stra;
	if (len < 0)
		len = strlen(str);
	char* p = (char*)malloc(len + 1);
	if (!p) return false;
	m_data.cstr = p;
	memset(p, 0, len + 1);
	memcpy(p, str, len);

	return true;
}

//if is strw
const wchar_t* KVariant::GetValueStrW(int* len)
{
	if (len) *len = m_len;
	return (const wchar_t*)m_data.wstr;
}

bool KVariant::SetValueStrW(const wchar_t* str, int len)
{
	Clear();
	if (0 == str || 0 == len)
		return false;

	m_type = kvt_strw;
	if (len < 0)
		len = wcslen(str);
	wchar_t* t = (wchar_t*)malloc(sizeof(wchar_t) * (len + 1));
	if (!t) return false;
	memcpy(t, str, len * sizeof(wchar_t));
	t[len] = 0;
	m_data.wstr = t;
	return true;
}

//if is bin data
const void* KVariant::GetValueBin(int* len)
{
	if (len) *len = m_len;
	return m_data.ptr;
}

bool KVariant::SetValueBin(void* pBuf, int len)
{
	Clear();
	if (0 == pBuf || len <= 0)
		return false;

	m_type = kvt_bin;

	char* p = (char*)malloc(len + 1);
	if (!p) return false;
	m_data.ptr = p;
	memcpy(p, pBuf, len);
	p[len] = 0;

	return true;
}


//if is dic
KVariant* KVariant::PairGetKey()
{
	if (m_type != kvt_dic) return 0;
	KVariantPairData* pair = (KVariantPairData*)m_data.ptr;
	if (pair)
		return pair->key;

	return 0;
}

KVariant* KVariant::PairGetValue()
{
	if (m_type != kvt_dic) return 0;
	KVariantPairData* pair = (KVariantPairData*)m_data.ptr;
	if (pair)
		return pair->value;

	return 0;
}

bool KVariant::PairSetValue(KVariant* key, KVariant* value)
{
	if (m_type == kvt_pair) {
		KVariantPairData* pair = (KVariantPairData*)m_data.ptr;
		if (pair->key == key) {
			if (pair->value == value) return true;
			pair->value = value;

			if (pair->value) pair->value->Release();
			pair->value = value;
			return true;
		}
	}
	Clear();
	m_type = kvt_pair;

	KVariantPairData* pair = new KVariantPairData();

	pair->key = key;
	pair->value = value;
	m_data.ptr = pair;
	return false;
}

//if is array
int KVariant::ArrGetCount()
{
	if (m_type != kvt_arr) return -1;

	KArrayPtr* pArr = (KArrayPtr*)m_data.ptr;
	return pArr->GetCount();
}

KVariant* KVariant::ArrGetAt(int index)
{
	if (m_type != kvt_arr) return 0;

	KArrayPtr* pArr = (KArrayPtr*)m_data.ptr;
	return (KVariant * )pArr->GetAt(index);
}

bool KVariant::ArrAdd(KVariant* value)
{
	if (m_type != kvt_arr) return false;

	KArrayPtr* pArr = (KArrayPtr*)m_data.ptr;
	pArr->Add(value);
	return true;
}

bool KVariant::ArrRemove(int index)
{
	if (m_type != kvt_arr) return false;

	KArrayPtr* pArr = (KArrayPtr*)m_data.ptr;
	pArr->RemoveAt(index);

	return true;
}

bool KVariant::ArrRemoveAll()
{
	if (m_type != kvt_arr) return false;

	KArrayPtr* pArr = (KArrayPtr*)m_data.ptr;
	pArr->RemoveAll(true);

	return true;
}

KAPI KVariant* KVariant::LoadFile(const wchar_t* filename)
{
	return 0;
}

KAPI bool KVariant::SaveFile(const wchar_t* filename)
{
	return false;
}

KAPI KVariant* KVariant::New(KVariant::varType type)
{
	return new KVariant(type);
}
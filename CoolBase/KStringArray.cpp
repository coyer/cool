#include "stdafx.h"
#include "../include/base/kstringarray.h"

int s_CompareDataProcA(const void* data1, const void* data2)
{
	KStringA *p1 = (KStringA*)data1;
	KStringA *p2 = (KStringA*)data2;

	return p1->Compare(p2->c_str());
}

int s_CompareByKeyProcA(const void* pKey, const void* data)
{
	const char* p1 = (const char*)pKey;
	KStringA *p2 = (KStringA*)data;

	return strcmp(p1, p2->c_str());
}

void s_DestroyDataProcA(void* pdata)
{
	KStringA *p = (KStringA*)pdata;
	delete p;
}

KStringArrayA::KStringArrayA()
{
	m_sortMode = 1;
	m_datas.SetSortProc(s_CompareDataProcA);
	m_datas.SetDestroyDataProc(s_DestroyDataProcA);
}

KStringArrayA::~KStringArrayA()
{
	RemoveAll();
}

int	KStringArrayA::AddString(const char* str)
{
	return m_datas.Add(new KStringA(str));
}

int	KStringArrayA::AddString(const wchar_t* str)
{
	return m_datas.Add(new KStringA(str));
}

int	KStringArrayA::AddString(const KStringA& str)
{
	return m_datas.Add(new KStringA(str));
}

int	KStringArrayA::AddString(const KStringW& str)
{
	return m_datas.Add(new KStringA(str));
}
	
int	KStringArrayA::GetCount()
{
	return m_datas.GetCount();
}

const char*	KStringArrayA::GetAt(int index)
{
	KStringA* pStr = (KStringA*)m_datas.GetAt(index);
	return pStr->c_str();
}

KStringA* KStringArrayA::GetString(int index)
{
	return (KStringA*)m_datas.GetAt(index);
}

void KStringArrayA::RemoveAt(int index)
{
	m_datas.RemoveAt(index);
}

void KStringArrayA::RemoveAll()
{
	m_datas.RemoveAll();
}

int	KStringArrayA::FindIndex(const char* str)
{
	return m_datas.FindByKey(s_CompareByKeyProcA, (void*)str);
}

void KStringArrayA::SetSortMode(int enable)
{
	if (enable == m_sortMode)
		return;

	if (m_sortMode)
	{
		m_datas.SetSortProc(s_CompareDataProcA);
		m_datas.SetDestroyDataProc(s_DestroyDataProcA);

		KArrayPtr tmp;
		m_datas.Switch(tmp);
		
		void* p = tmp.GetHead();
		while(p)
		{
			m_datas.Add(p);
		}
	}
	else
	{
		m_datas.SetSortProc(0);
		m_datas.SetDestroyDataProc(0);
	}
}

int	KStringArrayA::GetSortMode()
{
	return m_sortMode;
}

KStringA KStringArrayA::ToString()
{
	KStringA str;
	int len = m_datas.GetCount();
	int tlen = 0;
	for (int i = 0; i < len; i++) {
		KStringA* ps = (KStringA*)m_datas.GetAt(i);
		tlen += ps->GetLength();
	}
	str.SetSize(tlen);
	for (int i = 0; i < len; i++) {
		KStringA* ps = (KStringA*)m_datas.GetAt(i);
		str.Append(ps->c_str());
	}
	return str;
}
//////////////////////////////////////////////////////////////////////////////


int s_CompareDataProcW(const void* data1, const void* data2)
{
	KStringW *p1 = (KStringW*)data1;
	KStringW *p2 = (KStringW*)data2;

	return p1->Compare(p2->c_str());
}

int s_CompareByKeyProcW(const void* pKey, const void* data)
{
	const wchar_t* p1 = (const wchar_t*)pKey;
	KStringW *p2 = (KStringW*)data;

	return wcscmp(p1, p2->c_str());
}

void s_DestroyDataProcW(void* pdata)
{
	KStringW *p = (KStringW*)pdata;
	delete p;
}

KStringArrayW::KStringArrayW()
{
	m_sortMode = 1;
	m_datas.SetSortProc(s_CompareDataProcW);
	m_datas.SetDestroyDataProc(s_DestroyDataProcW);
}

KStringArrayW::~KStringArrayW()
{
	RemoveAll();
}

int	KStringArrayW::AddString(const char* str)
{
	return m_datas.Add(new KStringW(str));
}

int	KStringArrayW::AddString(const wchar_t* str)
{
	return m_datas.Add(new KStringW(str));
}

int	KStringArrayW::AddString(const KStringA& str)
{
	return m_datas.Add(new KStringW(str));
}

int	KStringArrayW::AddString(const KStringW& str)
{
	return m_datas.Add(new KStringW(str));
}
	
int	KStringArrayW::GetCount()
{
	return m_datas.GetCount();
}

const wchar_t* KStringArrayW::GetAt(int index)
{
	KStringW* pStr = (KStringW*)m_datas.GetAt(index);
	return pStr->c_str();
}

KStringW* KStringArrayW::GetString(int index)
{
	return (KStringW * )m_datas.GetAt(index);
}

void KStringArrayW::RemoveAt(int index)
{
	m_datas.RemoveAt(index);
}

void KStringArrayW::RemoveAll()
{
	m_datas.RemoveAll();
}

int	KStringArrayW::FindIndex(const wchar_t* str)
{
	return m_datas.FindByKey(s_CompareByKeyProcW, (wchar_t*)str);
}

void KStringArrayW::SetSortMode(int enable)
{
	if (enable == m_sortMode)
		return;

	if (m_sortMode)
	{
		m_datas.SetSortProc(s_CompareDataProcA);
		m_datas.SetDestroyDataProc(s_DestroyDataProcA);

		KArrayPtr tmp;
		m_datas.Switch(tmp);
		
		void* p = tmp.GetHead();
		while(p)
		{
			m_datas.Add(p);
		}
	}
	else
	{
		m_datas.SetSortProc(0);
		m_datas.SetDestroyDataProc(0);
	}
}

int	KStringArrayW::GetSortMode()
{
	return m_sortMode;
}

KStringW KStringArrayW::ToString()
{
	KStringW str;
	int len = m_datas.GetCount();
	int tlen = 0;
	for (int i = 0; i < len; i++) {
		KStringW* ps = (KStringW*)m_datas.GetAt(i);
		tlen += ps->GetLength();
	}
	str.SetSize(tlen);
	for (int i = 0; i < len; i++) {
		KStringW* ps = (KStringW*)m_datas.GetAt(i);
		str.Append(ps->c_str());
	}
	return str;
}
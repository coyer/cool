#include "stdafx.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

class KStringDataA : public KArrayT<char>,
					 public KRefBase
{
	friend class KStringA;
public:
	KStringDataA*	Clone()
	{
		KStringDataA* p = new KStringDataA();
		p->SetData(m_pData, m_nCount);

		return p;
	}

	char* GetData()
	{
		if (0 == m_pData)
			return "";

		return m_pData;
	}

	void SetData(const char* pBuf, int len)
	{
		if (pBuf && len < 0)
		{
			len = CalcLen(pBuf);
		}

		__super::SetData(pBuf, len);
	}

	int Splice(int start = 0, int len = 1)
	{
		if (start < 0) start = 0;
		if (len < 0) len = m_nCount;
		if (start + len > m_nCount) len = m_nCount - start;
		if (len == 0) return len;
		int copylen = m_nCount - start - len;
		memcpy(m_pData + start, m_pData + start + len, copylen);
		m_nCount -= len;
		m_pData[m_nCount] = 0;
		return len;
	}
};

class KStringDataW : public KBufferW,
					 public KRefBase
{
	friend class KStringW;
public:
	KStringDataW*	Clone()
	{
		KStringDataW* p = new KStringDataW();
		p->SetData(m_pData, m_nCount);

		return p;
	}

	wchar_t* GetData()
	{
		if (0 == m_pData)
			return L"";

		return m_pData;
	}

	void SetData(const wchar_t* pBuf, int len)
	{
		if (pBuf && len < 0)
		{
			len = CalcLen(pBuf);
		}

		__super::SetData(pBuf, len);
	}

	int Splice(int start = 0, int len = 1)
	{
		if (start < 0) start = 0;
		if (len < 0) len = m_nCount;
		if (start + len > m_nCount) len = m_nCount - start;
		if (len == 0) return len;
		int copylen = m_nCount - start - len;
		memcpy(m_pData + start, m_pData + start + len, copylen * sizeof(wchar_t));
		m_nCount -= len;
		m_pData[m_nCount] = 0;
		return len;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void KStringA::_AllocString()
{
	if (m_pStr->GetRef() > 1)
	{
		m_pStr->Release();
		m_pStr = m_pStr->Clone();
	}
}

KStringA::KStringA()
{
	m_pStr = new KStringDataA();
}

KStringA::KStringA(const char* str, int len)
{
	m_pStr = new KStringDataA();	
	m_pStr->SetData(str, len);
}

KStringA::KStringA(const wchar_t* str, int len)
{
	m_pStr = new KStringDataA();
	SetData(str, len);
}

KStringA::KStringA(KStringA& str)
{
	m_pStr = str.m_pStr;
	if (m_pStr)
		m_pStr->AddRef();
}

KStringA::KStringA(KStringW& str)
{
	m_pStr = new KStringDataA();
	SetData(str);
}

KStringA::~KStringA()
{
	m_pStr->Release();
}

int KStringA::Append(char ch)
{
	_AllocString();
	return m_pStr->Add(ch);
}

int	KStringA::Append(const char* str, int len)
{
	if (0 == str)
		return -1;

	if (len < 0)
		len = KStringDataA::CalcLen(str);

	_AllocString();
	return m_pStr->Add(str, len);
}

int	KStringA::Append(KStringA str)
{
	_AllocString();
	return m_pStr->Add(str.GetData(), str.GetSize());
}

void KStringA::Clear()
{
	_AllocString();
	m_pStr->Clear();
}

bool KStringA::IsNull()
{
	return m_pStr->IsNull();
}

int KStringA::Splice(int start, int len)
{
	_AllocString();
	return m_pStr->Splice(start, len);
}

int	KStringA::SetData(const char* str, int len)
{
	if (0 == str)
		return -1;

	if (len < 0)
		len = KStringDataA::CalcLen(str);

	_AllocString();
	m_pStr->SetData(str, len);

	return GetLength();
}

int	KStringA::SetData(const wchar_t* str, int len)
{
	_AllocString();
	m_pStr->Clear();

	int n = ::WideCharToMultiByte(CP_ACP, 0, str, len, 0, 0, 0, 0);
	if (len < 0)
		n--;

	if (n <= 0)
		return 0;
	

	m_pStr->SetAllocSize(n);
	char *s = m_pStr->GetBuffer();
	::WideCharToMultiByte(CP_ACP, 0, str, len, s, n, 0, 0);
	
	m_pStr->SetSize(n);

	return n;
}

int	KStringA::SetData(KStringA& str)
{
	if (m_pStr != str.m_pStr)
	{
		m_pStr->Release();
		m_pStr = str.m_pStr;
		m_pStr->AddRef();
	}

	return m_pStr->GetCount();
}

int	KStringA::SetData(KStringW& str)
{
	return SetData(str.GetBuffer(), str.GetLength());
}
	
const char*	KStringA::GetData() const
{
	return m_pStr->GetData();
}

const char* KStringA::c_str() const
{
	if (0 == m_pStr)
		return "";
	return m_pStr->GetData();
}

char* KStringA::GetBuffer(int buflen)
{
	if (buflen <= 0)
	{
		if (0 == m_pStr)
			return 0;
		return m_pStr->GetBuffer();
	}

	_AllocString();
	m_pStr->SetAllocSize(buflen);
	return m_pStr->GetBuffer();
}

void KStringA::ReleaseBuffer(int buflen)
{
	_AllocString();
	m_pStr->SetSize(buflen);
}

int	KStringA::GetLength()
{
	return m_pStr->GetCount();
}

int	KStringA::GetSize()
{
	return m_pStr->GetCount();
}

void KStringA::SetSize(int size)
{
	_AllocString();
	m_pStr->SetAllocSize(size);
}

char KStringA::GetFirst()
{
	return m_pStr->GetHead();
}

char KStringA::GetLast()
{
	return m_pStr->GetTail();
}

char KStringA::GetAt(int index)
{
	return m_pStr->GetAt(index);
}

void KStringA::SetAt(int index, char ch)
{
	_AllocString();
	m_pStr->SetAt(index, ch);
}

int	KStringA::InsertAt(int index, char ch)
{
	_AllocString();
	return m_pStr->InsertAt(index, ch);
}

int	KStringA::InsertAt(int index, const char* str, int len)
{
	_AllocString();
	return m_pStr->InsertAt(index, str, len);
}

const char*	KStringA::operator()()
{
	return m_pStr->GetBuffer();
}

KStringA::operator const char* () const
{
	return m_pStr->GetBuffer();
}

char KStringA::operator[](int index)
{
	_AllocString();
	char* p = m_pStr->GetBuffer();
	return p[index];
}

KStringA& KStringA::operator = (const char* str)
{
	SetData(str);
	return *this;
}

KStringA& KStringA::operator = (KStringA& str)
{
	SetData(str);
	return *this;
}

KStringA& KStringA::operator = (KStringW& str)
{
	SetData(str);
	return *this;
}

KStringA& KStringA::operator = (const wchar_t* str)
{
	SetData(str, -1);
	return *this;
}

KStringA& KStringA::operator = (int val)
{
	char szbuf[32] = {0};
	sprintf_s(szbuf, "%d", val);

	SetData(szbuf);
	return *this;
}

KStringA& KStringA::operator = (unsigned int val)
{
	char szbuf[32] = {0};
	sprintf_s(szbuf, "%d", val);

	SetData(szbuf);
	return *this;
}

KStringA& KStringA::operator = (__int64 val)
{
	char szbuf[64] = {0};
	sprintf_s(szbuf, "%I64d", val);

	SetData(szbuf);
	return *this;
}

KStringA& KStringA::operator = (double val)
{
	char szbuf[32] = {0};
	sprintf_s(szbuf, "%f", val);

	SetData(szbuf);
	return *this;
}

KStringA& KStringA::operator + (char ch)
{
	Append(ch);
	return *this;
}

KStringA& KStringA::operator + (const char* str)
{
	Append(str);
	return *this;
}

KStringA& KStringA::operator + (KStringA& str)
{
	Append(str);
	return *this;
}


KStringA& KStringA::operator += (char ch)
{
	Append(ch);
	return *this;
}

KStringA& KStringA::operator += (const char* str)
{
	Append(str);
	return *this;
}

KStringA& KStringA::operator += (KStringA& str)
{
	Append(str);
	return *this;
}

int	KStringA::Compare(const char* str)
{
	if (0 == str)
	{
		if (0 == m_pStr->GetCount())
			return 0;
		return 1;
	}

	if (0 == m_pStr->GetCount())
	{
		if (0 == str || str[0] == 0)
			return 0;
		return -1;
	}

	return KStringDataA::ComperData(m_pStr->GetData(), str);
}

int	KStringA::Compare(const KStringA& str)
{
	if (m_pStr == str.m_pStr)
		return 0;

	if (0 == str.m_pStr->GetCount())
	{
		if (0 == m_pStr->GetCount())
			return 0;
		return 1;
	}

	if (0 == m_pStr->GetCount())
	{
		if (0 == str.m_pStr->GetCount())
			return 0;
		return -1;
	}

	return KStringDataA::ComperData(m_pStr->GetData(), str.m_pStr->GetData());
}


int	KStringA::CompareNoCase(const char* str)
{
	if (0 == str)
	{
		if (0 == m_pStr->GetCount())
			return 0;
		return 1;
	}

	if (0 == m_pStr->GetCount())
	{
		if (0 == str || str[0] == 0)
			return 0;
		return -1;
	}

	return _stricmp(m_pStr->GetData(), str);
}

int	KStringA::CompareNoCase(KStringA& str)
{
	if (m_pStr == str.m_pStr)
		return 0;

	if (0 == str.m_pStr->GetCount())
	{
		if (0 == m_pStr->GetCount())
			return 0;
		return 1;
	}

	if (0 == m_pStr->GetCount())
	{
		if (0 == str.m_pStr->GetCount())
			return 0;
		return -1;
	}
	
	return _stricmp(m_pStr->GetData(), str.m_pStr->GetData());
}

void KStringA::MakeUpper()
{
	_AllocString();
	_strupr(m_pStr->GetBuffer());
}

void KStringA::MakeLower()
{
	_AllocString();
	_strlwr(m_pStr->GetBuffer());
}

KStringA KStringA::Left(int length)
{
	if (length < 0)	
		length = 0;
	else if (length > m_pStr->GetCount())
		length = m_pStr->GetCount();

	if (length >= m_pStr->GetCount())
	{
		return *this;
	}

	return KStringA(m_pStr->GetData(), length);
}

KStringA KStringA::Mid(int startPos, int length)
{
	if (startPos < 0)
		startPos = 0;
	else if (startPos > m_pStr->GetCount())
		startPos = m_pStr->GetCount();

	if (length < 0 || length + startPos > m_pStr->GetCount())
		length = m_pStr->GetCount() - startPos;

	if (0 == length)
		return KStringA();

	if (length == m_pStr->GetCount())
	{
		return *this;
	}

	const char* data = m_pStr->GetData();
	data += startPos;

	return KStringA(data, length);
}

KStringA KStringA::Right(int length)
{
	if (length < 0 || length >= m_pStr->GetCount())
	{
		return *this;
	}

	const char* data = m_pStr->GetData();
	data += (m_pStr->GetCount() - length);

	return KStringA(data, length);
}


KStringA KStringA::TrimLeft(char ch)
{
	_AllocString();

	char* lpStr = m_pStr->GetData();

	int n = 0;
	while(n < m_pStr->m_nCount)
	{
		if (m_pStr->GetAt(n) != ch)
		{
			memmove(lpStr, lpStr + n, sizeof(char) * (m_pStr->m_nCount - n));
			m_pStr->m_nCount -= n;
			lpStr[m_pStr->m_nCount] = 0;
			break;
		}
		n ++;
	}

	return *this;
}

KStringA KStringA::TrimRight(char ch)
{
	_AllocString();
	char* lpStr = m_pStr->GetData();

	int n = m_pStr->m_nCount - 1;
	while(n > 0)
	{
		if (lpStr[n] != ch)
		{
			m_pStr->m_nCount = n + 1;
			lpStr[m_pStr->m_nCount] = 0;
			break;
		}
		n--;
	}

	return *this;
}

KStringA KStringA::Trim(char ch)
{
	TrimRight(ch);
	TrimLeft(ch);
	return *this;
}

int	KStringA::Find(char ch, int startPos)
{
	if (0 == m_pStr->GetCount())
		return -1;

	if (startPos >= m_pStr->GetCount())
		return -1;
	else if (startPos < 0)
		startPos = 0;

	const char* lpStr = m_pStr->GetData();
	const char* p = lpStr + startPos;
	while(*p)
	{
		if (*p == ch)
			break;
		p++;
	}

	if (*p)
		return (int)(p - lpStr);

	return -1;
}

int	KStringA::Find(const char* str, int startPos)
{
	if (0 == str)
		return -1;

	if (0 == m_pStr->GetCount())
		return -1;

	if (startPos < 0 || startPos >= m_pStr->GetCount())
		return -1;

	const char* ts = m_pStr->GetData();
	const char* p = strstr(ts + startPos, str);
	if (0 == p)
		return -1;

	return (int)(p - ts);
}

int	KStringA::ReverseFind(char ch)
{
	if (0 == m_pStr)
		return -1;
	const char* lpStr = m_pStr->GetData();
	const char* p = strrchr(lpStr, ch);
	if (0 == p)
		return -1;
	return (int)(p - lpStr);
}

int	KStringA::Replace(const char* strOld, const char* strNew)
{
	if (0 == m_pStr->GetCount())
		return 0;

	if (0 == strOld)
		return -1;

	if (0 == strNew)
		strNew = "";

	KStringA sTemp;
	int nCount = 0;
	
	int prePos = 0;
	int nPos = Find(strOld);
	if( nPos < 0 ) return 0;

	int cchOld = (int) KStringDataA::CalcLen(strOld);
	int cchNew = (int) KStringDataA::CalcLen(strNew);

	while( nPos >= 0 ) 
	{
		sTemp += Mid(prePos, nPos - prePos);
		sTemp += strNew;
	
		prePos = nPos + cchOld;
		nPos = Find(strOld, nPos + cchNew);

		nCount++;
	}

	sTemp += Mid(prePos);

	m_pStr->Release();
	m_pStr = sTemp.m_pStr;
	m_pStr->AddRef();

	return nCount;
}

int	KStringA::Format(const char* pstrFormat, ...)
{
	if (0 == pstrFormat)
		return 0;

	int n = KStringDataA::CalcLen(pstrFormat);
	if (0 == n)
		return 0;

	char *szBuffer = static_cast<char*>(malloc(sizeof(char) * (n + 10240)));
	memset(szBuffer, 0, sizeof(char) * (n + 1024));

	va_list argList;
	va_start(argList, pstrFormat);
	int iRet = vsprintf_s(szBuffer, n + 10240, pstrFormat, argList);
	va_end(argList);

	SetData(szBuffer, iRet);
	free(szBuffer);

	return iRet;
}

long KStringA::GetHashCode()
{
	if (0 == m_pStr)
		return 0;

	const char* lpStr = m_pStr->GetBuffer();
	int n = m_pStr->GetCount();

	unsigned int hash = 0;;
	unsigned char *p = 0;

	for(int i=0; i<n; i++)
		hash = 31 * hash + lpStr[i];
	hash =hash & 0x7FFFFFFF;

	return hash;
}

int	KStringA::FormatTime(time_t tmval)
{
	char buff[32] = {0};
	__time32_t v = tmval;
	struct tm * pt = _localtime32(&v);
	asctime_s(buff, sizeof(buff), pt);
	return SetData(buff);
}

void KStringA::FillChar(char ch, int len)
{
	if (len <= 0)
		return;

	_AllocString();

	m_pStr->SetSize(len);
	char* lpStr = m_pStr->GetData();
	memset(lpStr, ch, len);
}

int KStringA::toInt(int radix)
{
	return strtol(m_pStr->GetBuffer(), 0, radix);
}
__int64 KStringA::toInt64(int radix)
{
	return _strtoi64(m_pStr->GetBuffer(), 0, radix);
}

double KStringA::toDouble()
{
	return strtod(m_pStr->GetBuffer(), 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void KStringW::_AllocString()
{
	if (m_pStr->GetRef() > 1)
	{
		m_pStr->Release();
		m_pStr = m_pStr->Clone();
	}
}

KStringW::KStringW()
{
	m_pStr = new KStringDataW();
}

KStringW::KStringW(const wchar_t* str, int len)
{
	m_pStr = new KStringDataW();	
	m_pStr->SetData(str, len);
}

KStringW::KStringW(const char* str, int len)
{
	m_pStr = new KStringDataW();
	SetData(str, len);
}

KStringW::KStringW(KStringW& str)
{
	m_pStr = str.m_pStr;
	if (m_pStr)
		m_pStr->AddRef();
}

KStringW::KStringW(KStringA& str)
{
	m_pStr = new KStringDataW();
	SetData(str);
}

KStringW::~KStringW()
{
	m_pStr->Release();
}

int KStringW::Append(wchar_t ch)
{
	_AllocString();
	return m_pStr->Add(ch);
}

int	KStringW::Append(const wchar_t* str, int len)
{
	if (0 == str)
		return -1;

	if (len < 0)
		len = KStringDataW::CalcLen(str);

	_AllocString();
	return m_pStr->Add(str, len);
}

int	KStringW::Append(KStringW str)
{
	_AllocString();
	return m_pStr->Add(str.GetData(), str.GetSize());
}

void KStringW::Clear()
{
	_AllocString();
	m_pStr->Clear();
}

const bool KStringW::IsNull()
{
	return m_pStr->IsNull();
}

int	KStringW::Splice(int start, int len)
{
	_AllocString();
	return m_pStr->Splice(start, len);
}

int	KStringW::SetData(const wchar_t* str, int len)
{
	if (0 == str)
		return -1;

	if (len < 0)
		len = KStringDataW::CalcLen(str);

	_AllocString();
	m_pStr->SetData(str, len);

	return GetLength();
}

int	KStringW::SetData(const char* str, int len)
{
	_AllocString();
	m_pStr->Clear();

	int n = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, len, 0, 0);
	if (len < 0)
		n--;

	if (n <= 0)
		return 0;

	m_pStr->SetAllocSize(n);
	wchar_t *wstr = m_pStr->GetBuffer();

	::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, len, wstr, n);
	m_pStr->SetSize(n);

	return n;
}

int	KStringW::SetData(KStringW& str)
{
	if (m_pStr != str.m_pStr)
	{
		m_pStr->Release();
		m_pStr = str.m_pStr;
		m_pStr->AddRef();
	}

	return m_pStr->GetCount();
}

int	KStringW::SetData(KStringA& str)
{
	return SetData(str.GetBuffer(), str.GetLength());
}
	
const wchar_t*	KStringW::GetData() const
{
	return m_pStr->GetData();
}

const wchar_t* KStringW::c_str() const
{
	if (0 == m_pStr)
		return L"";
	return m_pStr->GetData();
}

wchar_t* KStringW::GetBuffer(int buflen)
{
	if (buflen <= 0)
	{
		if (0 == m_pStr)
			return 0;
		return m_pStr->GetBuffer();
	}

	_AllocString();
	m_pStr->SetAllocSize(buflen);
	return m_pStr->GetBuffer();
}

void KStringW::ReleaseBuffer(int buflen)
{
	_AllocString();
	m_pStr->SetSize(buflen);
}

int	KStringW::GetLength()
{
	return m_pStr->GetCount();
}

int	KStringW::GetSize()
{
	return m_pStr->GetCount();
}

void KStringW::SetSize(int size)
{
	_AllocString();
	m_pStr->SetAllocSize(size);
}

wchar_t KStringW::GetFirst()
{
	return m_pStr->GetHead();
}

wchar_t KStringW::GetLast()
{
	return m_pStr->GetTail();
}

wchar_t KStringW::GetAt(int index)
{
	return m_pStr->GetAt(index);
}

void KStringW::SetAt(int index, wchar_t ch)
{
	_AllocString();
	m_pStr->SetAt(index, ch);
}

int	KStringW::InsertAt(int index, wchar_t ch)
{
	_AllocString();
	return m_pStr->InsertAt(index, ch);
}

int	KStringW::InsertAt(int index, const wchar_t* str, int len)
{
	_AllocString();
	return m_pStr->InsertAt(index, str, len);
}

const wchar_t* KStringW::operator()()
{
	return m_pStr->GetBuffer();
}

KStringW::operator const wchar_t* () const
{
	return m_pStr->GetBuffer();
}

wchar_t KStringW::operator[](int index)
{
	_AllocString();
	wchar_t* p = m_pStr->GetBuffer();
	return p[index];
}

KStringW& KStringW::operator = (const wchar_t* str)
{
	SetData(str);
	return *this;
}

KStringW& KStringW::operator = (KStringW& str)
{
	SetData(str);
	return *this;
}

KStringW& KStringW::operator = (KStringA& str)
{
	SetData(str);
	return *this;
}

KStringW& KStringW::operator = (const char* str)
{
	SetData(str);
	return *this;
}

KStringW& KStringW::operator = (int val)
{
	wchar_t szbuf[32] = {0};
	wsprintf(szbuf, L"%d", val);

	SetData(szbuf);
	return *this;
}

KStringW& KStringW::operator = (unsigned int val)
{
	wchar_t szbuf[32] = {0};
	wsprintf(szbuf, L"%d", val);

	SetData(szbuf);
	return *this;
}

KStringW& KStringW::operator = (__int64 val)
{
	wchar_t szbuf[32] = {0};
	wsprintf(szbuf, L"%I64d", val);

	SetData(szbuf);
	return *this;
}

KStringW& KStringW::operator = (double val)
{
	wchar_t szbuf[32] = {0};
	wsprintf(szbuf, L"%f", val);

	SetData(szbuf);
	return *this;
}

KStringW& KStringW::operator + (wchar_t ch)
{
	Append(ch);
	return *this;
}

KStringW& KStringW::operator + (const wchar_t* str)
{
	Append(str);
	return *this;
}

KStringW& KStringW::operator + (KStringW& str)
{
	Append(str);
	return *this;
}


KStringW& KStringW::operator += (wchar_t ch)
{
	Append(ch);
	return *this;
}

KStringW& KStringW::operator += (const wchar_t* str)
{
	Append(str);
	return *this;
}

KStringW& KStringW::operator += (KStringW& str)
{
	Append(str);
	return *this;
}

int	KStringW::Compare(const wchar_t* str)
{
	if (0 == str)
	{
		if (0 == m_pStr->GetCount())
			return 0;
		return 1;
	}

	if (0 == m_pStr->GetCount())
	{
		if (0 == str || str[0] == 0)
			return 0;
		return -1;
	}

	return KStringDataW::ComperData(m_pStr->GetData(), str);
}

int	KStringW::Compare(const KStringW& str)
{
	if (m_pStr == str.m_pStr)
		return 0;

	if (0 == str.m_pStr->GetCount())
	{
		if (0 == m_pStr->GetCount())
			return 0;
		return 1;
	}

	if (0 == m_pStr->GetCount())
	{
		if (0 == str.m_pStr->GetCount())
			return 0;
		return -1;
	}

	return KStringDataW::ComperData(m_pStr->GetData(), str.m_pStr->GetData());
}


int	KStringW::CompareNoCase(const wchar_t* str)
{
	if (0 == str)
	{
		if (0 == m_pStr->GetCount())
			return 0;
		return 1;
	}

	if (0 == m_pStr->GetCount())
	{
		if (0 == str || str[0] == 0)
			return 0;
		return -1;
	}

	return _wcsicmp(m_pStr->GetData(), str);
}

int	KStringW::CompareNoCase(KStringW& str)
{
	if (m_pStr == str.m_pStr)
		return 0;

	if (0 == str.m_pStr->GetCount())
	{
		if (0 == m_pStr->GetCount())
			return 0;
		return 1;
	}

	if (0 == m_pStr->GetCount())
	{
		if (0 == str.m_pStr->GetCount())
			return 0;
		return -1;
	}
	
	return _wcsicmp(m_pStr->GetData(), str.m_pStr->GetData());
}


void KStringW::MakeUpper()
{
	_AllocString();
	_wcsupr(m_pStr->GetBuffer());
}

void KStringW::MakeLower()
{
	_AllocString();
	_wcslwr(m_pStr->GetBuffer());
}

KStringW KStringW::Left(int length)
{
	if (length < 0)	
		length = 0;
	else if (length > m_pStr->GetCount())
		length = m_pStr->GetCount();

	if (length >= m_pStr->GetCount())
	{
		return *this;
	}

	return KStringW(m_pStr->GetData(), length);
}

KStringW KStringW::Mid(int startPos, int length)
{
	if (startPos < 0)
		startPos = 0;
	else if (startPos > m_pStr->GetCount())
		startPos = m_pStr->GetCount();

	if (length < 0 || length + startPos > m_pStr->GetCount())
		length = m_pStr->GetCount() - startPos;

	if (0 == length)
		return KStringW();

	if (length == m_pStr->GetCount())
	{
		return *this;
	}

	const wchar_t* data = m_pStr->GetData();
	data += startPos;

	return KStringW(data, length);
}

KStringW KStringW::Right(int length)
{
	if (length < 0 || length >= m_pStr->GetCount())
	{
		return *this;
	}

	const wchar_t* data = m_pStr->GetData();
	data += (m_pStr->GetCount() - length);

	return KStringW(data, length);
}


KStringW KStringW::TrimLeft(wchar_t ch)
{
	_AllocString();

	wchar_t* lpStr = m_pStr->GetData();

	int n = 0;
	while(n < m_pStr->m_nCount)
	{
		if (m_pStr->GetAt(n) != ch)
		{
			memmove(lpStr, lpStr + n, sizeof(wchar_t) * (m_pStr->m_nCount - n));
			m_pStr->m_nCount -= n;
			lpStr[m_pStr->m_nCount] = 0;
			break;
		}
		n ++;
	}

	return *this;
}

KStringW KStringW::TrimRight(wchar_t ch)
{
	_AllocString();
	wchar_t* lpStr = m_pStr->GetData();

	int n = m_pStr->m_nCount - 1;
	while(n > 0)
	{
		if (lpStr[n] != ch)
		{
			m_pStr->m_nCount = n + 1;
			lpStr[m_pStr->m_nCount] = 0;
			break;
		}
		n--;
	}

	return *this;
}

KStringW KStringW::Trim(wchar_t ch)
{
	TrimRight(ch);
	TrimLeft(ch);
	return *this;
}

int	KStringW::Find(wchar_t ch, int startPos)
{
	if (0 == m_pStr->GetCount())
		return -1;

	if (startPos >= m_pStr->GetCount())
		return -1;
	else if (startPos < 0)
		startPos = 0;

	const wchar_t* lpStr = m_pStr->GetData();
	const wchar_t* p = lpStr + startPos;
	while(*p)
	{
		if (*p == ch)
			break;
		p++;
	}

	if (*p)
		return (int)(p - lpStr);

	return -1;
}

int	KStringW::Find(const wchar_t* str, int startPos)
{
	if (0 == str)
		return -1;

	if (0 == m_pStr->GetCount())
		return -1;

	if (startPos < 0 || startPos >= m_pStr->GetCount())
		return -1;

	const wchar_t* ts = m_pStr->GetData();
	const wchar_t* p = wcsstr(ts + startPos, str);
	if (0 == p)
		return -1;

	return (int)(p - ts);
}

int	KStringW::ReverseFind(wchar_t ch)
{
	if (0 == m_pStr)
		return -1;
	const wchar_t* lpStr = m_pStr->GetData();
	const wchar_t* p = wcsrchr(lpStr, ch);
	if (0 == p)
		return -1;
	return (int)(p - lpStr);
}

int	KStringW::Replace(const wchar_t* strOld, const wchar_t* strNew)
{
	if (0 == m_pStr->GetCount())
		return 0;

	if (0 == strOld)
		return -1;

	if (0 == strNew)
		strNew = L"";

	KStringW sTemp;
	int nCount = 0;
	
	int prePos = 0;
	int nPos = Find(strOld);
	if( nPos < 0 ) return 0;

	int cchOld = (int) KStringDataW::CalcLen(strOld);
	int cchNew = (int) KStringDataW::CalcLen(strNew);

	while( nPos >= 0 ) 
	{
		sTemp += Mid(prePos, nPos - prePos);
		sTemp += strNew;
	
		prePos = nPos + cchOld;
		nPos = Find(strOld, nPos + cchNew);

		nCount++;
	}

	sTemp += Mid(prePos);

	m_pStr->Release();
	m_pStr = sTemp.m_pStr;
	m_pStr->AddRef();

	return nCount;
}

int	KStringW::Format(const wchar_t* pstrFormat, ...)
{
	if (0 == pstrFormat)
		return 0;

	int n = KStringDataW::CalcLen(pstrFormat);
	if (0 == n)
		return 0;

	wchar_t *szBuffer = static_cast<wchar_t*>(malloc(sizeof(wchar_t) * (n + 10240)));
	memset(szBuffer, 0, sizeof(wchar_t) * (n + 1024));

	va_list argList;
	va_start(argList, pstrFormat);
	int iRet = vswprintf_s(szBuffer, n + 10240, pstrFormat, argList);
	va_end(argList);

	SetData(szBuffer, iRet);
	free(szBuffer);

	return iRet;
}

long KStringW::GetHashCode()
{
	if (0 == m_pStr)
		return 0;

	const wchar_t* lpStr = m_pStr->GetBuffer();
	int n = m_pStr->GetCount();

	unsigned int hash = 0;;

	for(int i=0; i<n; i++)
		hash = 31 * hash + lpStr[i];
	hash =hash & 0x7FFFFFFF;

	return hash;
}

int	KStringW::FormatTime(time_t tmval)
{
	wchar_t buff[32] = {0};
	__time32_t v = tmval;
	struct tm * pt = _localtime32(&v);
	_wasctime_s(buff, 32, pt);
	return SetData(buff);
}

void KStringW::FillChar(wchar_t ch, int len)
{
	if (len <= 0)
		return;

	_AllocString();

	m_pStr->SetSize(len);
	wchar_t* lpStr = m_pStr->GetData();
	for(int i=0; i<len; i++)
	{
		lpStr[i] = ch;
	}
}

int KStringW::SetUtf8String(const char* str, int len)
{
	_AllocString();
	m_pStr->Clear();

	int n = ::MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, str, len, 0, 0);
	if (len < 0)
		n--;

	if (n <= 0)
		return 0;

	m_pStr->SetAllocSize(n);
	wchar_t *wstr = m_pStr->GetBuffer();

	::MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, str, len, wstr, n);
	m_pStr->SetSize(n);

	return n;
}

KStringA KStringW::GetUtf8String()
{
	KStringA strA;
	if (0 == m_pStr)
		return strA;

	int n = ::WideCharToMultiByte(CP_UTF8, 0, m_pStr->GetBuffer(), m_pStr->GetSize(), 0, 0, 0, 0);
	if (n <= 0)
		return strA;

	KStringDataA* pStr = strA.m_pStr;
	pStr->SetAllocSize(n);
	char *s = pStr->GetBuffer();
	::WideCharToMultiByte(CP_UTF8, 0, m_pStr->GetBuffer(), m_pStr->GetSize(), s, n, 0, 0);
	
	pStr->SetSize(n);

	return strA;
}

int KStringW::toInt(int radix)
{
	return wcstol(m_pStr->GetBuffer(), 0, radix);
}

__int64 KStringW::toInt64(int radix)
{
	return _wcstoi64(m_pStr->GetBuffer(), 0, radix);
}

double KStringW::toDouble()
{
	return wcstod(m_pStr->GetBuffer(), 0);
}
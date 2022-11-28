#include "StdAfx.h"
#include <tchar.h>
#include <stdlib.h>

#include "../include/base/kxobj.h"

//////////////////////////////////////////////////////////////////
//util functions.
static char* SkipBlankBuffer(const char* pbuf, int len)
{
	int i = 0;
	while (len-- > 0)
	{
		if (*pbuf == '\n' || *pbuf == '\r' || *pbuf == '\t' || *pbuf == ' ')
		{
			pbuf ++;
			continue;
		}
		break;
	}
	return (char*)pbuf;
}
//////////////////////////////////////////////////////////////////

KXObjBase::KXObjBase()
{
}

KXObjBase::~KXObjBase()
{
}

KXObjBase::OType KXObjBase::GetType()
{
	return KNull;
}

int KXObjBase::Size()
{
	return 0;
}

void KXObjBase::Release()
{
	delete this;
}

//////////////////////////////////////////////////////////////////
class KXObjInt32 : public KXObjBase
{
public:
	int m_value;

	KXObjInt32()
	{
		m_value = 0;
	}

	OType GetType()
	{
		return KInt32;
	}

	virtual int Size()
	{
		return sizeof(int);
	}

	operator int()
	{
		return m_value;
	}

	int GetVal()
	{
		return m_value;
	}

	void SetVal(int val)
	{
		m_value = val;
	}

	int _CalcBufferLen(int ndepth)
	{
		char szBuf[12] = {0};
		sprintf(szBuf, "%d\n", m_value);
		return strlen(szBuf);	//add endline flag
	}

	char* Print2Buffer(char* pbuf, int depth)
	{
		char szBuf[12] = {0};
		sprintf(szBuf, "%d\n", m_value);
		int n = strlen(szBuf);
		memcpy(pbuf, szBuf, n);
		pbuf += n;

		return pbuf;
	}

	bool ParseBuffer(char* pbuf, int len, char** pend)
	{
		bool neg = false;
		while(len-- > 0)
		{
			if (*pbuf == '-')
			{
				if (len == 0)
				{	
					if (pend) *pend = pbuf;
					return false;
				}
				len --;
				pbuf ++;
				neg = true;
				break;
			}
			else if ((*pbuf >= '0') && (*pbuf <= '9'))
				break;
			pbuf++;
		}

		char* p = pbuf;
		while(len-- > 0)
		{
			if ((*p < '0') || (*p > '9'))
				break;
			p ++;
		}

		m_value = atoi(pbuf);
		if (neg)
			m_value = 0 - m_value;
		if (pend)	*pend = p;
		return true;
	}
};

class KXObjInt64 : public KXObjBase
{
public:
	__int64 m_value;

	KXObjInt64()
	{
		m_value = 0;
	}

	OType GetType()
	{
		return KInt64;
	}

	virtual int Size()
	{
		return sizeof(__int64);
	}

	operator __int64()
	{
		return m_value;
	}

	void SetVal(__int64 val)
	{
		m_value = val;
	}

	__int64 GetVal()
	{
		return m_value;
	}

	int _CalcBufferLen(int ndepth)
	{
		char szBuf[24] = {0};
		sprintf(szBuf, "0x%X\n", (unsigned __int64)m_value);
		return strlen(szBuf);
	}

	char* Print2Buffer(char* pbuf, int depth)
	{
		char szBuf[24] = {0};
		sprintf(szBuf, "0x%X\n", (unsigned __int64)m_value);
		int n = strlen(szBuf);
		memcpy(pbuf, szBuf, n);
		pbuf += n;

		return pbuf;
	}

	bool ParseBuffer(char* pbuf, int len, char** pend)
	{
		char* p = pbuf;
		while(len-- > 0)
		{
			if(*p <= ' ')
				break;
			p ++;
		}

		m_value = _atoi64(pbuf);
		if (pend)	*pend = p;
		return true;
	}
};

class KXObjStrA : public KXObjBase
{
public:
	KArrayT<char>	m_value;

	KXObjStrA()
	{
		m_bUtf8 = false;
	}

	OType GetType()
	{
		return KStrA;
	}

	bool IsUtf8()
	{
		return m_bUtf8;
	}

	virtual int Size()
	{
		return m_value.GetSize();
	}

	operator LPCSTR()
	{
		return (const char*)m_value.GetData();
	}

	void SetVal(const char* val, int len = -1, bool utf8 = false)
	{
		if (0 != val)
			m_value.SetData(val, len);
		else
			m_value.Clear();

		m_bUtf8 = utf8;
	}

	void SetVal(KStringA& str, bool utf8 = false)
	{
		m_value.SetData(str.GetBuffer(), str.GetLength());
		m_bUtf8 = utf8;
	}

	const char* GetVal(int *len = 0)
	{
		if (len)
			*len = m_value.GetSize();
		return m_value.GetData();
	}

	int _CalcBufferLen(int ndepth)
	{
		int n = 3;	//'$' + flag + endline
		int nSize = m_value.GetSize();
		n += nSize;
		while(nSize > 0)
		{
			nSize /= 0xFF;
			n ++;
		}
		return n;
	}

	char* Print2Buffer(char* pbuf, int depth)
	{
		*pbuf ++ = 'A';
		char *pflag = pbuf;
		pbuf ++;

		int nSize = m_value.GetSize();
		while(nSize > 0)
		{
			memset(pbuf++, (nSize % 0xFF), 1);
			nSize /= 0xFF;
		}
		unsigned char ntype = 2;
		if (m_bUtf8)
			ntype = 3;
		*pflag = (((0xF & ntype) << 4) | (0xF & (pbuf - pflag - 1)));
		memcpy(pbuf, m_value.GetData(), m_value.GetSize() * sizeof(char));
		pbuf += m_value.GetSize();

		*pbuf++ = '\n';
		return pbuf;
	}

	bool ParseBuffer(char* pbuf, int len, char** pend)
	{
		return false;
	}
protected:
	bool m_bUtf8;
};

class KXObjStrW : public KXObjBase
{
public:
	KArrayT<wchar_t> m_value;

	KXObjStrW()
	{
	}

	OType GetType()
	{
		return KStrW;
	}

	virtual int Size()
	{
		return m_value.GetSize();
	}

	operator LPCWSTR()
	{
		return m_value.GetData();
	}

	void SetVal(const wchar_t* val, int len)
	{
		if (0 != val)
		{
			if (len < 0)
				len = wcslen(val);
			m_value.SetData(val, len);
		}
		else
			m_value.Clear();
	}

	void SetVal(KStringW& str)
	{
		m_value.SetData(str.GetBuffer(), str.GetLength());
	}

	const wchar_t* GetVal(int* len = 0)
	{
		if (len)
			*len = m_value.GetSize();
		return m_value.GetData();
	}

	int _CalcBufferLen(int ndepth)
	{
		int n = 3;	//'$' + flag + endline
		int nSize = Size();
		n += sizeof(wchar_t) * nSize;
		while(nSize > 0)
		{
			nSize /= 0xFF;
			n ++;
		}
		return n;
	}

	char* Print2Buffer(char* pbuf, int depth)
	{
		*pbuf ++ = 'W';
		char *pflag = pbuf;
		pbuf ++;

		int nSize = m_value.GetSize();
		while(nSize > 0)
		{
			memset(pbuf++, (nSize % 0xFF), 1);
			nSize /= 0xFF;
		}
		unsigned char ntype = 4;
		*pflag = (((0xF & ntype) << 4) | (0xF & (pbuf - pflag - 1)));
		memcpy(pbuf, m_value.GetData(), m_value.GetSize() * sizeof(wchar_t));
		pbuf += m_value.GetSize() * sizeof(wchar_t);

		*pbuf++ = '\n';
		return pbuf;
	}

	bool ParseBuffer(char* pbuf, int len, char** pend)
	{
		return false;
	}
};

class KXObjMem : public KXObjBase
{
public:
	KArrayT<char> m_value;

	KXObjMem()
	{
	}

	KXObjMem(const char* val, int len)
	{
		m_value.SetData(val, len);
	}

	OType GetType()
	{
		return KMem;
	}

	virtual int Size()
	{
		return m_value.GetSize();
	}

	operator LPSTR()
	{
		return m_value.GetBuffer();
	}

	void SetVal(const char* val, int len)
	{
		m_value.SetData(val, len);
	}

	const char* GetVal(int* len = 0)
	{
		if (len)
			*len = m_value.GetSize();
		return m_value.GetBuffer();
	}

	int _CalcBufferLen(int ndepth)
	{
		int n = 3;
		int nSize = Size();
		n += nSize;
		while(nSize > 0)
		{
			nSize /= 0xFF;
			n ++;
		}
		return n;
	}

	char* Print2Buffer(char* pbuf, int depth)
	{
		*pbuf ++ = 'B';
		char *pflag = pbuf;
		pbuf ++;

		int nSize = m_value.GetSize();
		while(nSize > 0)
		{
			memset(pbuf++, (nSize % 0xFF), 1);
			nSize /= 0xFF;
		}
		unsigned char ntype = 1;
		*pflag = (((0xF & ntype) << 4) | (0xF & (pbuf - pflag - 1)));
		memcpy(pbuf, m_value.GetBuffer(), m_value.GetSize());
		pbuf += m_value.GetSize();

		*pbuf++ = '\n';
		return pbuf;
	}

	bool ParseBuffer(char* pbuf, int len, char** pend)
	{
		return false;
	}
};


//////////////////////////////////////////////////////////////////

class KXObjDicName : public KXObj
{
	KArrayT<char>	m_Name;
	KXObjBase*	m_pData;

public:
	KXObjDicName()
	{
		m_pData = 0;
	}

	virtual ~KXObjDicName()
	{
		if (m_pData)
			m_pData->Release();
	}

	virtual OType GetType()
	{
		return KName;
	}

	KXObjDicName(const char* name)
	{
		m_pData = 0;
		m_Name.SetData(name, -1);
	}

	KXObjDicName(const char* name, int val)
	{
		m_pData = 0;
		m_Name.SetData(name, -1);
		SetVal(val);
	}

	KXObjDicName(const char* name, __int64 val)
	{
		m_pData = 0;
		m_Name.SetData(name, -1);
		SetVal(val);
	}

	const char* GetName()
	{
		return m_Name.GetData();
	}

	void SetName(const char* name, int len = -1)
	{
		m_Name.SetData(name, -1);
	}

	KXObjBase* GetObj()
	{
		return m_pData;
	}
	
	void SetVal(int val)
	{
		if (m_pData && m_pData->GetType() != KInt32)
		{
			m_pData->Release();
			m_pData = 0;
		}

		if (0 == m_pData)
			m_pData = new KXObjInt32();

		((KXObjInt32*)m_pData)->SetVal(val);
	}

	void SetVal(__int64 val)
	{
		if (m_pData && m_pData->GetType() != KInt64)
		{
			m_pData->Release();
			m_pData = 0;
		}

		if (0 == m_pData)
			m_pData = new KXObjInt64();

		((KXObjInt64*)m_pData)->SetVal(val);
	}

	void SetVal(const char* val, int len = -1, bool bUtf8 = FALSE)
	{
		if (m_pData && m_pData->GetType() != KStrA)
		{
			m_pData->Release();
			m_pData = 0;
		}

		if (0 == m_pData)
			m_pData = new KXObjStrA();

		((KXObjStrA*)m_pData)->SetVal(val, len, bUtf8);
	}

	void SetVal(const wchar_t* val, int len = -1)
	{
		if (m_pData && m_pData->GetType() != KStrW)
		{
			m_pData->Release();
			m_pData = 0;
		}

		if (0 == m_pData)
			m_pData = new KXObjStrW();

		((KXObjStrW*)m_pData)->SetVal(val, len);
	}

	void SetVal(KStringA& val, bool isutf8)
	{
		if (m_pData && m_pData->GetType() != KStrA)
		{
			m_pData->Release();
			m_pData = 0;
		}

		if (0 == m_pData)
			m_pData = new KXObjStrA();

		((KXObjStrA*)m_pData)->SetVal(val, isutf8);
	}

	void SetVal(KStringW& val)
	{
		if (m_pData && m_pData->GetType() != KStrW)
		{
			m_pData->Release();
			m_pData = 0;
		}

		if (0 == m_pData)
			m_pData = new KXObjStrW();

		((KXObjStrW*)m_pData)->SetVal(val);
	}

	void SetMem(const char* mem, int len)
	{
		if (m_pData && m_pData->GetType() != KMem)
		{
			m_pData->Release();
			m_pData = 0;
		}

		if (0 == m_pData)
			m_pData = new KXObjMem();

		((KXObjMem*)m_pData)->SetVal(mem, len);
	}

	void SetObj(KXObjInt32 *pObj)
	{
		if (pObj)
			SetVal(pObj->GetVal());
	}

	void SetObj(KXObjInt64 *pObj)
	{
		if (pObj)
			SetVal(pObj->GetVal());
	}

	void SetObj(KXObjStrA *pObj)
	{
		if (pObj)
			SetVal(pObj->m_value, pObj->IsUtf8());
	}

	void SetObj(KXObjStrW *pObj)
	{
		if (pObj)
			SetVal(pObj->m_value);
	}

	void SetObj(KXObjMem *pObj)
	{
		if (pObj)
			SetMem(pObj->GetVal(), pObj->Size());
	}

	void SetObj(KXObj* pObj)
	{
		DelVal();
		if (!pObj)
			return;

		m_pData = pObj;
		pObj->AddRef();
	}

	void DelVal()
	{
		if (m_pData)
			m_pData->Release();
		m_pData = 0;
	}

	int GetInt32()
	{
		if (!m_pData)
			return 0;
		switch(m_pData->GetType())
		{
		case KInt32:
			{
				KXObjInt32* p = (KXObjInt32*)m_pData;
				return p->GetVal();
			}
			break;
		case KInt64:
			{
				KXObjInt64* p = (KXObjInt64*)m_pData;
				return (int)p->GetVal();
			}
			break;
		default:
			break;
		}
		return 0;
	}

	__int64 GetInt64()
	{
		if (!m_pData)
			return 0;
		switch(m_pData->GetType())
		{
		case KInt32:
			{
				KXObjInt32* p = (KXObjInt32*)m_pData;
				return p->GetVal();
			}
			break;
		case KInt64:
			{
				KXObjInt64* p = (KXObjInt64*)m_pData;
				return p->GetVal();
			}
			break;
		default:
			break;
		}
		return 0;
	}

	const char* GetStrA(int *len = 0)
	{
		if (len)
			*len = 0;

		if (m_pData && m_pData->GetType() == KStrA)
		{
			KXObjStrA* p = (KXObjStrA*)m_pData;
			return p->GetVal(len);
		}
		return 0;
	}

	const wchar_t* GetStrW(int *len = 0)
	{
		if (len)
			*len = 0;

		if (m_pData && m_pData->GetType() == KStrA)
		{
			KXObjStrW* p = (KXObjStrW*)m_pData;
			return p->GetVal(len);
		}
		return 0;
	}

	const char* GetMem(int* len = 0)
	{
		if (len)
			*len = 0;

		if (!m_pData)
			return 0;

		switch(m_pData->GetType())
		{
		case KInt32:
			{
				KXObjInt32* p = (KXObjInt32*)m_pData;
				if (len)
					*len = p->Size();
				return (const char*)(&(p->m_value));
			}
			break;
		case KInt64:
			{
				KXObjInt64* p = (KXObjInt64*)m_pData;
				if (len)
					*len = p->Size();
				return (const char*)(&(p->m_value));
			}
			break;
		case KStrA:
			{
				KXObjStrA* p = (KXObjStrA*)m_pData;
				return p->GetVal(len);
			}
			break;
		case KStrW:
			{
				KXObjStrW* p = (KXObjStrW*)m_pData;
				if (len)
					*len = p->Size() * sizeof(wchar_t);
				return (const char*)((LPWSTR)p);
			}
			break;
		case KMem:
			{
				KXObjMem* p = (KXObjMem*)m_pData;
				return p->GetVal(len);
			}
			break;
		}
		return 0;
	}

	virtual int _CalcBufferLen(int nDepth)
	{
		if (0 == m_pData)
			return 0;
		//3 = '/' + ' '; no end line
		return m_Name.GetSize() + 2 + m_pData->_CalcBufferLen(nDepth + 1);
	}

	char* Print2Buffer(char* pbuf, int depth)
	{
		if (0 == m_pData || m_Name.GetSize() == 0)
			return pbuf;
		*pbuf ++ = '/';

		memcpy(pbuf, m_Name.GetData(), m_Name.GetSize());
		pbuf += m_Name.GetSize();

		memcpy(pbuf, " ", 1);
		pbuf ++;

		pbuf = m_pData->Print2Buffer(pbuf, depth + 1);
		return pbuf;
	}

	bool ParseBuffer(char* pbuf, int len, char** pEnd)
	{
		if (m_nRef != 1)
			return false;

		if (0 == len || 0 == pbuf)
			return false;

		m_Name.Free();
		if (m_pData)
		{
			m_pData->Release();
			m_pData = 0;
		}

		//1  parse name.
		do
		{
			if (*pbuf == '/')
				break;
			pbuf ++;
		}while(--len > 0);

		if (len <= 0)
			return false;

		pbuf++;
		char *pkey = pbuf;
		while(--len > 0)
		{
			if ((*pkey == ' ' || *pkey == '\r') ||
				(*pkey == '\n' && *pkey == '\t'))
				break;
			pkey ++;
		}

		if (pkey == pbuf)
			return false;

		//2 name is ok
		m_Name.SetData(pbuf, pkey - pbuf);

		pbuf = pkey + 1;
		//len -= m_Name.GetSize() + 1;

		//3  parse data
		while(len-- > 0)
		{
			if (*pbuf == '-' || (*pbuf >= '1' && *pbuf <= '9'))	//number
			{
				char* pE = 0;
				KXObjInt32* pObj = new KXObjInt32();
				if (pObj->ParseBuffer(pbuf, len, &pE))
				{
					m_pData = pObj;
					if (pEnd) *pEnd = pE;
					return true;
				}
				else
				{
					pObj->Release();
					return false;
				}
			}
			else if (*pbuf == '0')
			{
				char* pE = 0;
				KXObjInt64* pObj = new KXObjInt64();
				if (pObj->ParseBuffer(pbuf, len, &pE))
				{
					m_pData = pObj;
					if (pEnd) *pEnd = pE;
					return true;
				}
				else
				{
					pObj->Release();
					return false;
				}
			}
			else if (*pbuf == '$')
			{
				len--;
				pbuf++;
				if (len <= 0)
					return false;

				unsigned char ch = (unsigned char)*pbuf;
				int ntype = (ch >> 4) & 0xF;
				int bytecount = ch & 0xF;
				if (len < bytecount)
					return false;

				long nsize = 0;
				for(int i=0; i<bytecount; i++)
				{
					pbuf++;
					nsize *= 256;
					nsize += (unsigned char)(*pbuf);
				}

				if (len < nsize)
					return false;

				len--;
				pbuf++;

				if (ntype == 1)	//memory
				{
					KXObjMem* pMem = new KXObjMem();
					pMem->SetVal(pbuf, nsize);
					pbuf += nsize;
					m_pData = pMem;
				}
				else if (ntype == 2)
				{
					KXObjStrA* pStr = new KXObjStrA();
					pStr->SetVal(pbuf, nsize, false);
					pbuf += nsize;
					m_pData = pStr;
				}
				else if (ntype == 3)
				{
					KXObjStrA* pStr = new KXObjStrA();
					pStr->SetVal(pbuf, nsize, true);
					pbuf += nsize;
					m_pData = pStr;
				}
				else if (ntype == 4)
				{
					KXObjStrW* pStr = new KXObjStrW();
					pStr->SetVal((wchar_t*)pbuf, nsize);
					pbuf += (2 * nsize);
					m_pData = pStr;
				}
				else
					return false;

				len--;
				pbuf++;
				if (pEnd) *pEnd = pbuf;
				return true;
			}
			else if (*pbuf == '<')
			{
				char* pE = 0;
				KXObjDic* pObj = KXObjDic::CreateObj();
				if (pObj->ParseBuffer(pbuf, len, &pE))
				{
					m_pData = pObj;
					if (pEnd) *pEnd = pE;
					return true;
				}
				else
				{
					pObj->Release();
					return false;
				}
			}
			else if (*pbuf == '[')
			{
				char* pE = 0;
				KXObjArr* pObj = KXObjArr::CreateObj();
				if (pObj->ParseBuffer(pbuf, len, &pE))
				{
					m_pData = pObj;
					if (pEnd) *pEnd = pE;
					return true;
				}
				else
				{
					pObj->Release();
					return false;
				}
			}

			pbuf++;
		}// end while

		if (pEnd) *pEnd = pbuf;
		return false;
	}
};

//////////////////////////////////////////////////////////////////
KXObj::KXObj(void)
{
	m_nRef = 1;
}

KXObj::~KXObj(void)
{
}

KXObjBase::OType KXObj::GetType()
{
	return KObj;
}

void KXObj::AddRef()
{
	m_nRef ++;
}

void KXObj::Release()
{
	m_nRef --;
	if (m_nRef <= 0)
		delete this;
}

char* KXObj::ToBuffer(int *len)
{
	if (len) *len = 0;

	int n = _CalcBufferLen(0);
	if (n == 0)
		return 0;

	if (len) *len = n;

	char* pbuf = (char*)malloc(n + 2);
	memset(pbuf, 0, n + 2);

	Print2Buffer(pbuf, 0);

	return pbuf;
}

void KXObj::FreeBuffer(char* pbuf)
{
	free((void*)pbuf);
}

KXObj* KXObj::CreateFromBuffer(const char* pbuf, int len, char** endbuffer)
{
	if (0 == pbuf || len <= 0)
		return 0;

	KXObj* pRoot = 0;

	//1  Skin unknow character, only read '[' or '<'
	int i = 0;
	while(i < len)
	{
		if (pbuf[i] == '<')
		{
			pRoot = KXObjDic::CreateObj();
			break;
		}
		else if (pbuf[i] == '[')
		{
			pRoot = KXObjArr::CreateObj();
			break;
		}
		i++;
	}

	if (0 == pRoot)
	{
		if (endbuffer)
			*endbuffer = (char*)(pbuf + i);
		return pRoot;
	}

	if (!pRoot->ParseBuffer((char*)(pbuf + i), len - i, endbuffer))
	{
		pRoot->Release();
		pRoot = 0;
	}
	return pRoot;
}

//////////////////////////////////////////////////////////////////
static int DicCompareByKey(const void* data1, const void* data2)
{
	KXObjDicName* p1 = (KXObjDicName*)data1;
	KXObjDicName* p2 = (KXObjDicName*)data2;
	const char* name1 = p1->GetName();
	const char* name2 = p2->GetName();
	return strcmp(name1, name2);
}

static void DicDestroyData(void* pdata)
{
	KXObjDicName* p = (KXObjDicName*)pdata;
	p->Release();
}

KXObjDic::KXObjDic()
{
	SetSortProc(DicCompareByKey);
	SetDestroyDataProc(DicDestroyData);
}

KXObjDic::~KXObjDic()
{
}

KXObjDic* KXObjDic::CreateObj()
{
	return new KXObjDic();
}

KXObjBase::OType KXObjDic::GetType()
{
	return KObjDic;
}

int KXObjDic::Size()
{
	return GetCount();
}

void KXObjDic::AddInt32(const char* name, int val)
{
	KXObjDicName *data = new KXObjDicName(name, val);
	Add(data);
}

void KXObjDic::AddInt64(const char* name, __int64 val)
{
	KXObjDicName *data = new KXObjDicName(name, val);
	Add(data);
}

void KXObjDic::AddStr(const char* name, const char* val, int len, bool isutf8)
{
	KXObjDicName *data = new KXObjDicName(name);
	data->SetVal(val, len, isutf8);
	Add(data);
}

void KXObjDic::AddStr(const char* name, const wchar_t* val, int len)
{
	KXObjDicName *data = new KXObjDicName(name);
	data->SetVal(val, len);
	Add(data);
}

void KXObjDic::AddMem(const char* name, const char* mem, int len)
{
	KXObjDicName *data = new KXObjDicName(name);
	data->SetMem(mem, len);
	Add(data);
}

void KXObjDic::AddObj(const char* name, KXObj* obj)
{
	KXObjDicName *data = new KXObjDicName(name);
	data->SetObj(obj);
	obj->Release();
	Add(data);
}

int KXObjDic::_CalcBufferLen(int nDepth)
{
	int n = 2;	// '<' + endline
	for(int i= Size() - 1; i>=0; i--)
	{
		n += nDepth + 1;
		KXObjDicName* pName = (KXObjDicName*)GetAt(i);
		n += pName->_CalcBufferLen(nDepth + 1);
	}
	n += nDepth;
	n += 2;	// '>' + endline
	return n;
}

char* KXObjDic::Print2Buffer(char* pbuf, int depth)
{
	memcpy(pbuf, "<\n", 2);
	pbuf += 2;

	for(int i=0; i<Size(); i++)
	{
		memset(pbuf, ' ', depth + 1);
		pbuf += depth + 1;
		KXObjDicName* pName = (KXObjDicName*)GetAt(i);
		pbuf = pName->Print2Buffer(pbuf, depth + 1);
	}
	memset(pbuf, ' ', depth);
	pbuf += depth;
	memcpy(pbuf, ">\n", 2);
	pbuf += 2;
	return pbuf;
}

bool KXObjDic::ParseBuffer(char* pbuf, int len, char** pEnd)
{
	if (0 == len || 0 == pbuf)
		return false;

	RemoveAll();

	do
	{
		if (*pbuf == '<')
		{
			pbuf ++;
			len --;
			break;
		}
		pbuf ++;
	}while(--len > 0);

	if (len <= 0)
		return false;

	while(len--)
	{
		if (*pbuf == '/')
		{
			char* pE = 0;
			KXObjDicName* pName = new KXObjDicName();
			if (pName->ParseBuffer(pbuf, len, &pE))
			{
				Add(pName);
				len -= (pE - pbuf);
				pbuf = pE;
			}
			else
			{
				if (pEnd)	*pEnd = pE;
				return false;
			}
		}
		else if (*pbuf == '>')
		{
			if (pEnd) *pEnd = pbuf + 1;
			return true;
		}
		else
			pbuf++;
	}

	if (pEnd) *pEnd = pbuf;
	return false;
}
//////////////////////////////////////////////////////////////////

KXObjArr::KXObjArr()
{
}

KXObjArr::~KXObjArr()
{
	RemoveAll();
}

KXObjArr* KXObjArr::CreateObj()
{
	return new KXObjArr();
}

KXObjBase::OType KXObjArr::GetType()
{
	return KObjArr;
}

int KXObjArr::Size()
{
	return GetCount();
}

int KXObjArr::AddObj(KXObj* pObj)
{
	if (0 == pObj)
		return -1;
	return Add(pObj);
}

int KXObjArr::InsertObj(KXObj* pObj, int index)
{
	if (0 == pObj)
		return -1;

	return InsertData(index, pObj);
}

void KXObjArr::DelObj(int index)
{
	RemoveAt(index, 1);
}

int KXObjArr::FindObj(KXObj* pObj)
{
	for(int i = Size() - 1; i >= 0; i--)
	{
		if (pObj == GetAt(i))
			return i;
	}
	return -1;
}

int KXObjArr::AddInt(int val)
{
	KXObjInt32* obj = new KXObjInt32();
	obj->SetVal(val);
	return Add(obj);
}

int KXObjArr::AddInt(__int64 val)
{
	KXObjInt64* obj = new KXObjInt64();
	obj->SetVal(val);
	return Add(obj);
}

int KXObjArr::AddStrA(const char* str, int len, bool isutf8)
{
	KXObjStrA* obj = new KXObjStrA();
	obj->SetVal(str, len, isutf8);
	return Add(obj);
}

int KXObjArr::AddStrW(const wchar_t* str, int len)
{
	KXObjStrW* obj = new KXObjStrW();
	obj->SetVal(str, len);
	return Add(obj);
}

int KXObjArr::AddMem(const char* mem, int len)
{
	KXObjMem* obj = new KXObjMem();
	obj->SetVal(mem, len);
	return Add(obj);
}

KXObjBase* KXObjArr::GetObj(int index)
{
	KXObjBase* p = (KXObjBase*)GetAt(index);
	return p;
}

KXObjDic* KXObjArr::GetDic(int index)
{
	KXObjBase* p = (KXObjBase*)GetAt(index);
	if (p && p->GetType() == KObjDic)
		return (KXObjDic*)p;

	return 0;
}

KXObjArr* KXObjArr::GetArr(int index)
{
	KXObjBase* p = (KXObjBase*)GetAt(index);
	if (p && p->GetType() == KObjArr)
		return (KXObjArr*)p;

	return 0;
}

int KXObjArr::GetInt(int index)
{
	KXObjBase* pObj = (KXObjBase*)GetAt(index);
	if (pObj && pObj->GetType() == KInt32)
	{
		KXObjInt32 *p = (KXObjInt32 *)pObj;
		return p->GetVal();
	}
	if (pObj && pObj->GetType() == KInt64)
	{
		KXObjInt64 *p = (KXObjInt64 *)pObj;
		return (int)p->GetVal();
	}
	return 0;
}

__int64 KXObjArr::GetInt64(int index)
{
	KXObjBase* pObj = (KXObjBase*)GetAt(index);
	if (pObj && pObj->GetType() == KInt32)
	{
		KXObjInt32 *p = (KXObjInt32 *)pObj;
		return p->GetVal();
	}
	if (pObj && pObj->GetType() == KInt64)
	{
		KXObjInt64 *p = (KXObjInt64 *)pObj;
		return p->GetVal();
	}
	return 0;
}

const char* KXObjArr::GetStrA(int index, int* len, bool* isutf8)
{
	if (len)
		*len = 0;

	KXObjBase* pObj = (KXObjBase*)GetAt(index);
	if (pObj && pObj->GetType() == KStrA)
	{
		KXObjStrA *p = (KXObjStrA *)pObj;
		if (isutf8)
			*isutf8 = p->IsUtf8();
		return p->GetVal(len);
	}
	return 0;
}

const wchar_t* KXObjArr::GetStrW(int index, int* len )
{
	if (len)
		*len = 0;

	KXObjBase* pObj = (KXObjBase*)GetAt(index);
	if (pObj && pObj->GetType() == KStrW)
	{
		KXObjStrW *p = (KXObjStrW *)pObj;
		return p->GetVal(len);
	}
	return 0;
}

const char* KXObjArr::GetMem(int index, int* len)
{
	if (len)
		*len = 0;

	KXObjBase* pObj = (KXObjBase*)GetAt(index);
	if (pObj && pObj->GetType() == KMem)
	{
		KXObjMem *p = (KXObjMem *)pObj;
		return p->GetVal(len);
	}
	return 0;
}

bool KXObjArr::RemoveAt(int index, int n)
{
	KXObjBase* pObj = (KXObjBase*)GetAt(index);
	KArrayPtr::RemoveAt(index);
	if (pObj)
	{
		pObj->Release();
		return true;
	}
	return false;
}

void KXObjArr::RemoveAll()
{
	for(int i=0; i<KXObjArr::Size(); i++)
	{
		KXObjBase* pObj = (KXObjBase*)GetAt(i);
		if (pObj)
			pObj->Release();
	}
	KArrayPtr::RemoveAll();
}

int KXObjArr::_CalcBufferLen(int nDepth)
{
	int n = 2;	// '<' + endline
	for(int i=Size() - 1; i>=0; i--)
	{
		n += nDepth + 1;
		KXObjBase* pObj = (KXObjBase*)GetAt(i);
		n += pObj->_CalcBufferLen(nDepth + 1);
	}
	n += nDepth;
	n += 2;	// '>' + endline
	return n;
}

char* KXObjArr::Print2Buffer(char* pbuf, int depth)
{
	memcpy(pbuf, "[\n", 2);
	pbuf += 2;

	for(int i=0; i<Size(); i++)
	{
		memset(pbuf, ' ', depth + 1);
		pbuf += depth + 1;
		
		KXObjBase* pObj = (KXObjBase*)GetAt(i);
		pbuf = pObj->Print2Buffer(pbuf, depth + 1);
	}

	memset(pbuf, ' ', depth);
	pbuf += depth;

	memcpy(pbuf, "]\n", 2);
	pbuf += 2;

	return pbuf;
}

bool KXObjArr::ParseBuffer(char* pbuf, int len, char** pEnd)
{
	if (0 == len || 0 == pbuf)
		return false;

	RemoveAll();

	do
	{
		if (*pbuf == '[')
		{
			pbuf ++;
			len --;
			break;
		}
		pbuf ++;
	}while(--len > 0);

	if (len <= 0)
		return false;

	bool bret = false;
	
	while(len--)
	{
		if (*pbuf == '<')
		{
			char* pE = 0;
			KXObjDic* pDic = KXObjDic::CreateObj();
			if (pDic->ParseBuffer(pbuf, len, &pE))
			{
				AddObj(pDic);
				len -= (pE - pbuf);
				pbuf = pE;

			}
			else
			{
				pDic->Release();
				if (pEnd)	*pEnd = pE;
				return false;
			}
		}
		else if (*pbuf == ']')
		{
			len --;
			pbuf ++;
			bret = true;
			break;
		}
		else if (*pbuf == '/')
		{
			char* pE = 0;
			KXObjDicName* pName = new KXObjDicName();
			if (pName->ParseBuffer(pbuf, len, &pE))
				Add(pName);
			else
			{
				if (pEnd)	*pEnd = pE;
				return false;
			}
			len -= (pE - pbuf);
			pbuf = pE;
		}
		else
			pbuf++;
	}

	if (pEnd) *pEnd = pbuf;
	return bret;

}
//////////////////////////////////////////////////////////////////

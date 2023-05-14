#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : karrayt.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : KArrayT is a template array.

*******************************************************************************************/
#include <malloc.h>

#define MEMORY_INC_SIZE	2048
#define MEMORY_INC_MIN	64

#ifndef U8
#define U8 unsigned char
#endif // !U8


#ifndef U16
#define U16 unsigned short
#endif // !U16

#ifndef  U32
#define U32	unsigned int
#endif // ! U32

#ifndef U64
#define U64 unsigned long long
#endif // !U64



template <class T>
class KArrayT
{
protected:
	T*	m_pData;
	U32	 m_nAlloc;
	U32  m_nCount;

private:
	void _Init()
	{
		m_pData = 0;
		m_nAlloc = MEMORY_INC_MIN;
		m_nCount = 0;
	}

	bool _AllocBuffer(unsigned int n)
	{
		if (n == 0) return false;
		if (0 == m_pData)
		{			
			m_pData = (T*)malloc((n + 1) * sizeof(T));
			if (!m_pData) return false;

			memset(m_pData, 0, (n +1) * sizeof(T));
			m_nAlloc = n;
			m_nCount = 0;
		}
		else if (n > m_nAlloc)
		{
			unsigned int tlen = m_nAlloc;
			if (tlen <= MEMORY_INC_MIN) {
				tlen = MEMORY_INC_MIN;
			}
			else if (tlen <= n) {
				while (tlen < n)
				{
					tlen *= 2;
				}
			}
			else {
				tlen = n;
			}

			void* pt = (T*)realloc(m_pData, (1 + tlen) * sizeof(T));
			if (!pt) return false;
			m_nAlloc = tlen;
			m_pData =(T*)pt;
		}

		return true;
	}

public:
	KArrayT(void)
	{
		_Init();
	}

	~KArrayT(void)
	{
		if (m_pData)
			free(m_pData);
	}

	KArrayT(const T* pBuf, int len)
	{
		_Init();
		SetData(pBuf, len);
	}

	KArrayT(KArrayT<T>& nmem)
	{
		_Init();
		SetData(nmem.m_pData, nmem.m_nCount * sizeof(T));
	}

	KArrayT& operator=(KArrayT<T>& nmem)
	{
		if (this == &nmem)
			return *this;

		SetData(nmem.m_pData, nmem.m_nCount * sizeof(T));
		return *this;
	}

	KArrayT<T>* Clone()
	{
		KArrayT<T>* p= new KArrayT<T>(m_pData, m_nCount);
		return p;
	}

	bool Attach(T* pBuf, int len)
	{
		if (pBuf == m_pData)
			return false;

		Free();
		m_pData = pBuf;
		m_nCount = len;
		m_nAlloc = len;
		return true;
	}

	T* Detach()
	{
		T* p = m_pData;
		m_pData = 0;
		m_nCount = 0;
		m_nAlloc = 0;

		return p;
	}

	T* GetBuffer() const
	{
		return m_pData;
	}

	operator T* () const
	{
		return m_pData;
	}

	bool operator == (KArrayT<T>& nmem)	{	return (0 == Compare(nmem));	}
	bool operator != (KArrayT<T>& nmem)	{	return (!Compare(nmem));		}
	bool operator > (KArrayT<T>& nmem)	{	return (Compare(nmem) > 0);		}
	bool operator < (KArrayT<T>& nmem)	{	return (Compare(nmem) < 0);		}
	bool operator <= (KArrayT<T>& nmem)	{	return !(Compare(nmem) > 0);	}
	bool operator >= (KArrayT<T>& nmem)	{	return !(Compare(nmem) < 0);	}

	int	Compare(T* pBuf, int len)
	{
		int n = memcmp(m_pData, pBuf, min(len, m_nCount) * sizeof(T));
		if (0 != n)
			return n;
		if (m_nCount > len)
			return 1;
		else
			return -1;
	}

	int Compare(KArrayT<T>& nmem)
	{
		return Compare(nmem.m_pData, nmem.m_nCount);
	}

	int GetAllocSize()
	{
		return m_nAlloc;
	}

	int SetAllocSize(U32 n)
	{
		_AllocBuffer(n);
		return m_nAlloc;
	}

	void SetSize(U32 n)
	{
		if (n > m_nAlloc)
		{
			_AllocBuffer(n);
		}
		m_nCount = n;
		m_pData[m_nCount] = 0;
	}

	int GetCount()
	{
		return m_nCount;
	}

	int	GetSize()
	{
		return m_nCount;
	}

	void Free()
	{
		if (m_pData)
		{
			free(m_pData);
			m_pData = 0;
		}
		_Init();
	}

	void Clear()
	{
		memset(m_pData, 0, m_nCount * sizeof(T));
		m_nCount = 0;

		//if (m_pData)
		//	m_pData[m_nCount] = 0;
	}

	const bool IsNull() {
		return m_nCount == 0 ? true : false;
	}

	T*	GetData() const
	{
		return m_pData;
	}

	void SetData(const T* pBuf, int len)
	{
		if (pBuf && len > 0)
		{
			_AllocBuffer(len);
			memcpy(m_pData, pBuf, len * sizeof(T));
			m_nCount = len;
			m_pData[m_nCount] = 0;
		}
		else
		{
			Clear();
		}
	}

	void FillBuffer(int ch, U32 nstart, U32 nlen)
	{
		if (nstart + nlen > m_nCount) nlen = m_nCount - nstart;
		memset((void*)(m_pData + nstart), ch, nlen * sizeof(T));
	}

	T& GetAt(U32 index)
	{
		return m_pData[index];
	}

	T& operator[] (U32 index)
	{
		return m_pData[index];
	}

	void SetAt(U32 index, const T& ch)
	{
		m_pData[index] = ch;
	}

	U32 Add(T val)
	{
		_AllocBuffer(m_nCount + 1);
		m_pData[m_nCount++] = val;

		return m_nCount;
	}

	U32 Add(const T* arr, int count)
	{
		if (0 ==  arr || 0 == count)
			return -1;

		_AllocBuffer(count + m_nCount);
		memcpy(m_pData + m_nCount * sizeof(T), arr, count * sizeof(T));

		m_nCount += count;
		m_pData[m_nCount] = 0;

		return m_nCount;
	}

	U32 Add(KArrayT<T>& arr)
	{
		return Add(arr.m_pData, arr.m_nCount);
	}

	U32 InsertAt(U32 index, T& t)
	{
		if (index < 0)
			index = 0;
		else if (index >= m_nCount)
			return Add(t);
		
		if (!_AllocBuffer(1 + m_nCount))
			return -1;
		
		memmove(m_pData + index + 1, m_pData + index, sizeof(T) * (m_nCount - index));
		m_pData[index] = t;
		m_nCount ++;
		m_pData[m_nCount] = 0;

		return index;
	}

	int InsertAt(U32 index, const T* pData, int n)
	{
		if (index < 0)
			index = 0;
		else if (index >= m_nCount)
			return Add(pData, n);

		if (!_AllocBuffer(n + m_nCount))
			return -1;

		memmove(m_pData + index + n, m_pData + index, sizeof(T) * (m_nCount - index));
		memcpy(m_pData + index, pData, sizeof(T) * n);

		m_nCount += n;

		m_pData[m_nCount] = 0;
		return m_nCount;
	}

	U32 AddHead(const T& t) {
		return InsertAt(0, t);
	}

	U32 AddHead(const T* arr, U32 len) {
		return InsertAt(0, arr, len);
	}

	U32 AddTail(const T& t) {
		return Add(t);
	}

	U32 AddTail(const T* arr, U32 len) {
		return Add(arr, len);
	}

	bool RemoveAt(U32 index, int n = 1)
	{
		if (index < 0 || index >= m_nCount)
			return false;

		if ((index + n > m_nCount) || n < 0)
			n = m_nCount - index;
		memmove(m_pData + index, m_pData + index + n, sizeof(T) * (m_nCount - index - n));
		m_nCount -= n;
		m_pData[m_nCount] = 0;
		return true;
	}

	void RemoveAll()
	{
		Clear();
	}

	int Find(T& t)
	{
		for(int i=0; i<m_nCount; i++)
		{
			if (m_pData[i] == t)
				return i;
		}
		return -1;
	}

	T GetHead(bool remove = false)
	{
		T t = 0;
		if (m_nCount > 0)
		{
			t = m_pData[0];
			if (remove)
			{
				memmove(m_pData, &(m_pData[1]), (m_nCount--) * sizeof(T));
			}
		}

		return t;
	}

	T GetTail(bool remove = false)
	{
		T t = 0;
		if (m_nCount > 0)
		{
			t = m_pData[m_nCount - 1];
			if (remove)
			{
				m_pData[--m_nCount] = 0;
			}
		}

		return t;
	}

	static int CalcLen(const T* lpcs)
	{
        const T *eos = lpcs;

        while( *eos++ ) ;

        return((int)(eos - lpcs - 1));
	}

	static int ComperData(const T* src, const T* dst)
	{
		int ret = 0 ;
		while(!(ret = (int)(*src - *dst)) && *dst)
			++src, ++dst;

		if ( ret < 0 )
			ret = -1 ;
		else if ( ret > 0 )
			ret = 1 ;

		return( ret );
	}


};


typedef KArrayT<int>		KArrayInt;
typedef KArrayT<char>		KArrayChar;
typedef KArrayT<__int64>	KArrayInt64;
typedef KArrayT<float>		KarrayFloat;
typedef KArrayT<double>		KArrayDouble;

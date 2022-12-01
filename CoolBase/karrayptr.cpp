#include "stdafx.h"
#include "../include/base/kinc.h"

class KArrayPtrData : public KRefBase, public KArrayT<PVOID>
{
public:
	KArrayPtrData():KRefBase(), KArrayT<PVOID>() {

	}

	KArrayPtrData* Clone()
	{
		KArrayPtrData* p = new KArrayPtrData();
		p->SetData((const PVOID*)m_pData, m_nCount);

		return p;
	}

	void SetData(const PVOID* pBuf, int len)
	{
		if (pBuf && len < 0)
		{
			len = CalcLen((PVOID*)pBuf);
		}

		__super::SetData((PVOID*)pBuf, len);
	}
};

////////////////////////////////////////////////////////////////////////////////
bool KArrayPtr::_AllocArray()
{
	if (m_pData->GetRef() > 1)
	{
		KArrayPtrData* p = m_pData->Clone();
		m_pData->Release();
		m_pData = p;
	}
	return true;
}

KArrayPtr::KArrayPtr(KCompareDataProc	 proc)
{
	m_pData = new KArrayPtrData();
	m_procDestroyData = 0;
	m_procCompareData = proc;
}

KArrayPtr::KArrayPtr(KArrayPtr& arr)
{
	m_procDestroyData = arr.m_procDestroyData;
	m_procCompareData = arr.m_procCompareData;

	m_pData->AddRef();
	m_pData = arr.m_pData;
}

KArrayPtr::~KArrayPtr()
{
	RemoveAll();
	m_pData->Release();
}

int	KArrayPtr::GetCount()
{
	return m_pData->GetCount();
}

bool KArrayPtr::IsNull()
{
	return m_pData->IsNull();
}

const PVOID KArrayPtr::GetAt(int index)
{
	return m_pData->GetAt(index);
}

const PVOID KArrayPtr::GetData()
{
	return m_pData->GetData();
}

int	KArrayPtr::Add(PVOID pData)
{
	if (0 == pData)
		return -1;

	_AllocArray();
	int nCount = m_pData->GetCount();

	if (!m_procCompareData || !nCount)
	{
		return m_pData->Add(pData);
	}

	if (nCount == 1)
	{
		int t = m_procCompareData(pData, m_pData->GetAt(0));
		if (t < 0)
			m_pData->InsertAt(0, pData);
		else
			m_pData->Add(pData);
	}
	else
	{
		int t = m_procCompareData(pData, m_pData->GetTail());
		if (t >= 0)
			m_pData->Add(pData);
		else
		{
			t = m_procCompareData(pData, m_pData->GetHead());
			if (t <= 0)
				m_pData->InsertAt(0, pData);
			else
			{
				int left, right, mid;
				left = 0;
				right = nCount - 1;

				while(left < right)
				{
					mid = (left + right ) / 2;
					if (mid == left)
					{
						m_pData->InsertAt(mid+1, pData);
						break;
					}
					t = m_procCompareData(pData, GetAt(mid));
					if (t == 0)
					{
						m_pData->InsertAt(mid, pData);
						break;
					}
					else if (t < 0)
						right = mid;
					else
						left = mid;
				}
			}
		}
	}

	return m_pData->GetCount();
}

int	KArrayPtr::Add(PVOID* pData, int count)
{
	if (0 == pData || 0 == count)
		return -1;

	_AllocArray();
	if (0 == m_procCompareData)
	{
		return m_pData->Add(pData, count);
	}

	for(int i=0; i<count; i++)
	{
		Add(pData[i]);
	}

	return m_pData->GetCount();
}

PVOID KArrayPtr::RemoveAt(int index, bool no_destroy)
{
	_AllocArray();
	PVOID p = m_pData->GetAt(index);
	if (m_procDestroyData && !no_destroy)
	{
		m_procDestroyData(p);
		p = NULL;
	}
	m_pData->RemoveAt(index);
	return p;
}

void KArrayPtr::Remove(int index, int count)
{
	_AllocArray();
	if (m_procDestroyData)
	{
		for(int i = index; i<count; i++)
		{
			m_procDestroyData(m_pData->GetAt(i));
		}
	}
	m_pData->RemoveAt(index, count);
}

void KArrayPtr::RemoveAll()
{
	if (m_procDestroyData)
	{
		for(int i = 0; i<m_pData->GetCount(); i++)
		{
			m_procDestroyData(m_pData->GetAt(i));
		}
	}

	m_pData->Clear();
}

void  KArrayPtr::SetSortProc(KCompareDataProc proc)
{
	m_procCompareData = proc;
}

void KArrayPtr::SetDestroyDataProc(KDestroyDataProc proc)
{
	m_procDestroyData = proc;
}

int	KArrayPtr::FindIndex(PVOID pData)
{
	int nCount = m_pData->GetCount();
	if (0 == nCount)
		return -1;

	if (0 == m_procCompareData)
	{
		for(int i=0; i<nCount; i++)
		{
			if (m_pData[i] == pData)
				return i;
		}

		return -1;
	}

	return FindByKey(m_procCompareData, pData);
}

int	KArrayPtr::FindByKey(KCompareByKeyProc proc, PVOID pKey)
{
	int nCount = m_pData->GetCount();
	if (0 == nCount || 0 == proc)
		return -1;

	int t = proc(pKey, m_pData->GetAt(0));
	if (t == 0)
		return 0;
	else if (t < 0)
		return -1;
	else if (m_pData->GetCount() == 1)
		return -1;
	else
	{
		t = proc(pKey, m_pData->GetAt(nCount - 1));
		if (t > 0)
			return -1;
		else if (t == 0)
			return nCount - 1;
		else
		{
			int left, right, mid;
			left = 0;
			right = nCount - 1;

			while(left < right)
			{
				mid = (left + right ) / 2;
				if (mid == left)
					return -1;

				t = proc(pKey, m_pData->GetAt(mid));
				if (t == 0)
					return mid;
				else if (t < 0)
					right = mid;
				else
					left = mid;
			}
		}
	}

	return -1;
}

int	KArrayPtr::SetAllocSize(int count)
{
	_AllocArray();
	return m_pData->SetAllocSize(count);
}

KArrayPtr& KArrayPtr::operator = (KArrayPtr& arr)
{
	if (m_pData == arr.m_pData)
		return *this;
	
	RemoveAll();
	m_pData->Release();
	
	arr.m_pData->AddRef();
	m_pData = arr.m_pData;

	return *this;
}

void KArrayPtr::Switch(KArrayPtr& dst)
{
	KArrayPtrData*	t = dst.m_pData;
	dst.m_pData = this->m_pData;
	this->m_pData = t;
}

const PVOID KArrayPtr::operator[](int index)
{
	return m_pData->GetAt(index);
}

//if this is a sort arry, cannot call these function.
int	KArrayPtr::InsertData(int index, PVOID pData)
{
	_AllocArray();
	return m_pData->InsertAt(index, pData);
}

int	KArrayPtr::InsertData(int index, PVOID* pDatas, int count)
{
	_AllocArray();
	return m_pData->InsertAt(index, pDatas, count);
}

void KArrayPtr::SetAt(int index, PVOID pData)
{
	_AllocArray();
	PVOID p = m_pData->GetAt(index);
	if (m_procDestroyData)
	{
		m_procDestroyData(p);
	}
	m_pData->SetAt(index, pData);
}

void KArrayPtr::SetCount(int count)
{
	_AllocArray();
	m_pData->SetSize(count);
}

const PVOID KArrayPtr::GetHead(bool remove)
{
	if (remove)
		_AllocArray();

	return m_pData->GetHead(remove);
}

const PVOID KArrayPtr::GetTail(bool remove)
{
	if (remove)
		_AllocArray();

	return m_pData->GetTail(remove);
}
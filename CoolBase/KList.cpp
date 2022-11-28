#include "stdafx.h"
#include "../include/base/KListPtr.h"
#include "../include/base/klistt.h"

class KListPtrData : public KListT<PVOID>,
					 public KRefBase
{
public:
	KListPtrData()
	{
	};
};

KListPtr::KListPtr()
{
	m_pData = new KListPtrData();
}

KListPtr::~KListPtr()
{
	m_pData->Release();
}

void KListPtr::Switch(KListPtr& dst)
{
	KListPtrData* t = dst.m_pData;
	dst.m_pData = this->m_pData;
	this->m_pData = t;
}

KListPtr& KListPtr::operator = (KListPtr& lst)
{
	if (m_pData == lst.m_pData)
		return *this;

	m_pData->Release();
	lst.m_pData->AddRef();
	m_pData = lst.m_pData;

	return *this;
}

int KListPtr::GetCount()
{
	return m_pData->GetCount();
}

PVOID KListPtr::GetAt(KPosition pos)
{
	if (0 == pos)
		return 0;

	return m_pData->GetData(pos);
}

KPosition KListPtr::GetHeadPos()
{
	return m_pData->GetHeadPos();
}

KPosition KListPtr::GetTailPos()
{
	return m_pData->GetTailPos();
}

KPosition KListPtr::FindDataPos(PVOID ptr)
{
	return m_pData->FindDataPos(ptr);
}

bool KListPtr::GetNext(KPosition& pos, PVOID & ptr)
{
	return m_pData->GetNext(pos, ptr);
}

bool KListPtr::GetPrev(KPosition& pos, PVOID & ptr)
{
	return m_pData->GetPrev(pos, ptr);
}

int KListPtr::AddHead(PVOID ptr)
{
	return m_pData->AddHead(ptr);
}

int KListPtr::AddTail(PVOID ptr)
{
	return m_pData->AddTail(ptr);
}

PVOID KListPtr::GetHead(bool remove)
{
	if (m_pData->IsEmpty())
		return 0;
	return m_pData->GetHead(remove);
}

PVOID KListPtr::GetTail(bool remove)
{
	if (m_pData->IsEmpty())
		return 0;

	return m_pData->GetTail(remove);
}

void KListPtr::push_front(PVOID ptr)
{
	m_pData->push_front(ptr);
}

void KListPtr::push_back(PVOID ptr)
{
	m_pData->push_back(ptr);
}
	
PVOID KListPtr::pop_front()
{
	if (m_pData->IsEmpty())
		return 0;
	return m_pData->pop_front();
}

PVOID KListPtr::pop_back()
{
	if (m_pData->IsEmpty())
		return 0;
	return m_pData->pop_back();
}

void KListPtr::RemoveAll()
{
	return m_pData->RemoveAll();
}
	
bool KListPtr::RemoveAt(KPosition pos)
{
	return m_pData->RemoveAt(pos);
}

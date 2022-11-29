#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved
Header File : klist.h
Author : coyer@126.com
Create :
Update : 2012-11-05     version 0.0.1
Detail : KListT only support PVOID type.
		 KList只支持浅拷贝，如果要做深度拷贝，需要外面来执行。
*******************************************************************************************/

#include <malloc.h>
#include "kdef.h"

template <class T>
class KListSimpleT
{
	typedef struct tagListData
	{
		T	data;
		struct tagListData*	next;
	}ListData;

	ListData*	m_pHead;
	ListData*	m_pTail;
	int			m_nCount;
public:

	KListSimpleT()
	{
		m_pHead = 0;
		m_pTail = 0;
		m_nCount = 0;
	}

	~KListSimpleT()
	{
		RemoveAll();
	}

	bool IsEmpty()
	{
		return m_nCount == 0 ? true : false;
	}

	KPosition GetHeadPos()
	{
		return m_pHead;
	}

	bool GetNext(KPosition& pos, T& t)
	{
		if (0 == pos)
			return false;

		ListData* pd = (ListData*)pos;
		t = pd->data;

		pos = pd->next;

		return true;
	}

	int	GetCount()
	{
		return m_nCount;
	}

	int AddHead(T& t)
	{
		ListData* p = new ListData;
		p->data = t;
		p->next = 0;

		if (m_pHead)
		{
			p->next = m_pHead;
			m_pHead = p;
		}
		else
		{
			m_pHead = p;
			m_pTail = p;
		}

		m_nCount++;
		return m_nCount;
	}

	int AddTail(T& t)
	{
		ListData* p = new ListData;
		p->data = t;
		p->next = 0;

		if (m_pTail)
		{
			m_pTail->next = p;
			m_pTail = p;
		}
		else
		{
			m_pHead = p;
			m_pTail = p;
		}

		m_nCount++;
		return m_nCount;
	}

	void push_front(T& t)
	{
		AddHead(t);
	}

	void push_back(T& t)
	{
		AddTail(t);
	}

	T& pop_front()
	{
		T& t = m_pHead->data;
		ListData* p = m_pHead;
		m_pHead = m_pHead->next;

		if (0 == m_pHead)
			m_pTail = 0;

		delete p;
		m_nCount--;
		return t;
	}

	T& GetHead()
	{
		return m_pHead->data;
	}

	T& GetTail()
	{
		return m_pTail->data;
	}

	void RemoveAll()
	{
		if (0 == m_pHead)
			return;

		ListData* p = m_pHead;

		do
		{
			ListData* tmp = p;
			p = p->next;

			delete tmp;
		}while(p);

		m_pHead = 0;
		m_pTail = 0;
		m_nCount = 0;
	}
};

template <class T>
class KListT
{
	typedef struct tagListData
	{
		T	data;
		struct tagListData*	prev;
		struct tagListData*	next;
	}ListData;

	ListData*	m_pHead;
	ListData*	m_pTail;
	int			m_nCount;

public:
	KListT()
	{
		m_pHead = 0;
		m_pTail = 0;
		m_nCount = 0;
	}

	~KListT()
	{
		RemoveAll();
	}

	bool IsEmpty()
	{
		return m_nCount == 0 ? true : false;
	}

	int	GetCount()
	{
		return m_nCount;
	}

	KPosition GetHeadPos()
	{
		if (0 == m_nCount)
			return 0;

		return m_pHead;
	}

	KPosition GetTailPos()
	{
		if (0 == m_nCount)
			return 0;

		return m_pTail;
	}

	bool GetNext(KPosition& pos, T& t)
	{
		if (0 == pos)
			return false;

		ListData* pd = (ListData*)pos;
		t = pd->data;

		pos = pd->next;

		return true;
	}

	bool GetPrev(KPosition& pos, T& t)
	{
		if (0 == pos)
			return false;

		ListData* pd = (ListData*)pos;
		t = pd->data;

		pos = pd->prev;

		return true;
	}

	int AddHead(T& t)
	{
		ListData* p = new ListData;
		p->data = t;
		p->prev = 0;
		p->next = 0;

		if (m_pHead)
		{
			m_pHead->prev = p;
			p->next = m_pHead;
			m_pHead = p;
		}
		else
		{
			m_pHead = p;
			m_pTail = p;
		}

		m_nCount++;
		return m_nCount;
	}

	int AddTail(T& t)
	{
		ListData* p = new ListData;
		p->data = t;
		p->next = 0;

		if (m_pTail)
		{
			m_pTail->next = p;
			p->prev = m_pTail;
			m_pTail = p;
		}
		else
		{
			m_pHead = p;
			m_pTail = p;
		}

		m_nCount++;
		return m_nCount;
	}

	void push_front(T& t)
	{
		AddHead(t);
	}

	void push_back(T& t)
	{
		AddTail(t);
	}

	T& pop_front()
	{
		T& t = m_pHead->data;
		ListData* p = m_pHead;
		m_pHead = m_pHead->next;

		if (m_pHead)
			m_pHead->prev = 0;
		else
			m_pTail = 0;

		delete p;
		m_nCount--;
		return t;
	}

	T& pop_back()
	{
		T& t = m_pTail->data;
		ListData* p = m_pTail;
		m_pTail = m_pTail->prev;

		if (m_pTail)
			m_pTail->next = 0;
		else
			m_pHead = 0;

		delete p;
		m_nCount--;

		return t;
	}

	T& GetHead(bool remove)
	{
		if (!remove)
		{
			return m_pHead->data;
		}
		else
		{
			return pop_front();
		}
	}

	T& GetTail(bool remove)
	{
		if (!remove)
		{
			return m_pTail->data;
		}
		else
		{
			return pop_back();
		}
	}

	void RemoveAll()
	{
		if (0 == m_pHead)
			return;

		ListData* p = m_pHead;

		do
		{
			ListData* tmp = p;
			p = p->next;

			delete tmp;
		}while(p);

		m_pHead = 0;
		m_pTail = 0;
		m_nCount = 0;
	}

	KPosition FindDataPos(T& t)
	{
		ListData* p = m_pHead;

		do
		{
			if (p->data == t)
				return p;

			p = p->next;			
		}while(p);

		return 0;
	}

	T& GetData(KPosition pos)
	{
		ListData* p = (ListData*)pos;
		return p->data;
	}

	bool RemoveAt(KPosition& pos)
	{
		if (m_nCount == 0)
			return false;
		ListData* p = (ListData*)pos;
		ListData* prev = p->prev;
		ListData* next = p->next;

		if (prev)
			prev->next = next;
		if (next)
			next->prev = prev;

		delete p;
		pos = next;
		m_nCount--;

		return true;
	}
};
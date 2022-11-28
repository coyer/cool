#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : klist.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : KList only support PVOID type.
		 KList只支持浅拷贝，如果要做深度拷贝，需要外面来执行。
*******************************************************************************************/

class KListPtrData;

class KAPI KListPtr
{
	KListPtrData*	m_pData;
public:
	KListPtr();
	~KListPtr();

	KListPtr& operator = (KListPtr& lst);

	int GetCount();
	PVOID GetAt(KPosition pos);

	KPosition GetHeadPos();
	KPosition GetTailPos();
	KPosition FindDataPos(PVOID ptr);

	bool GetNext(KPosition& pos, PVOID & ptr);
	bool GetPrev(KPosition& pos, PVOID & ptr);

	int AddHead(PVOID ptr);
	int AddTail(PVOID ptr);
	PVOID GetHead(bool remove);
	PVOID GetTail(bool remove);

	void  push_front(PVOID ptr);
	void  push_back(PVOID ptr);
	PVOID pop_front();
	PVOID pop_back();

	void  RemoveAll();
	bool  RemoveAt(KPosition pos);

	void  Switch(KListPtr& dst);
};
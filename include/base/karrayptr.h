#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : karray.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : KArray is a PVOID ptr array. only for PVOID type. if you want string array, please use KStringArray.
		 This array support sort callback function.
*******************************************************************************************/

#include "kdef.h"

class KArrayPtrData;


class KAPI KArrayPtr
{
	KArrayPtrData*		m_pData;
	KCompareDataProc	m_procCompareData;
	KDestroyDataProc	m_procDestroyData;

	bool _AllocArray();

public:
	KArrayPtr(KCompareDataProc	 proc = 0);
	KArrayPtr(KArrayPtr& arr);
	~KArrayPtr();

	int		GetCount();
	bool    IsNull();
	const PVOID	GetAt(int index);
	const PVOID	GetData();

	int		Add(PVOID pData);
	int		Add(PVOID* pData, int count);

	PVOID	RemoveAt(int index, bool no_destroy = false);
	void	Remove(int index, int count = 1);
	void	RemoveAll();

	void	SetSortProc(KCompareDataProc proc);
	void	SetDestroyDataProc(KDestroyDataProc proc);

	int		FindIndex(PVOID pData);
	int		FindByKey(KCompareByKeyProc proc, PVOID pKey);

	int		SetAllocSize(int count);

	KArrayPtr& operator = (KArrayPtr& arr);
	const PVOID	operator[](int index);

	const PVOID	GetHead(bool remove = false);
	const PVOID	GetTail(bool remove = false);

	void	Switch(KArrayPtr& dst);

	//if this is a sort arry, do not call fellow functions.
	int		InsertData(int index, PVOID pData);
	int		InsertData(int index, PVOID* pDatas, int count);

	void	SetAt(int index, PVOID pData);
	void	SetCount(int count);
};
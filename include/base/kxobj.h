#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : KXObj.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : KXObj is an obj which save all var type. maybe a VARIANT type.
*******************************************************************************************/

#include "kdef.h"
#include "karrayptr.h"


class KAPI KXObjBase
{
public:
	enum OType
	{
		KNull = 0,
		KInt32,
		KInt64,
		KStrA,
		KStrW,
		KMem,
		KName,
		KObj,
		KObjArr,
		KObjDic,
		KUnkown
	};

	KXObjBase();
	virtual ~KXObjBase();
	virtual OType GetType();
	virtual int Size();
	virtual void Release();

	virtual char* Print2Buffer(char* pbuf, int depth) = 0;
	virtual int _CalcBufferLen(int nDepth) = 0;
	virtual bool ParseBuffer(char* pbuf, int len, char** pend) = 0;
};

class KAPI KXObj : public KXObjBase
{
public:
	KXObj(void);
	virtual ~KXObj(void);
	virtual OType GetType();
	void AddRef();
	virtual void Release();

	char* ToBuffer(int *len);
	static void FreeBuffer(char* pbuf);

	static  KXObj* CreateFromBuffer(const char* buffer, int len, char** endbuffer = 0);
protected:
	int m_nRef;
};

////////////////////////////////////////////////////////

class KAPI KXObjDic : public KXObj,
						 protected KArrayPtr
{
	friend class KXObj;
	KXObjDic();
public:
	~KXObjDic();
	static KXObjDic* CreateObj();

	int Size();
	OType GetType();
	void AddInt32(const char* name, int val);
	void AddInt64(const char* name, __int64 val);
	void AddStr(const char* name, const char* val, int len = -1, bool isutf8 = false);
	void AddStr(const char* name, const wchar_t* val, int len = -1);

	void AddMem(const char* name, const char* mem, int len);
	void AddObj(const char* name, KXObj* obj);

	bool ParseBuffer(char* pbuf, int len, char** pend);
protected:
	int _CalcBufferLen(int nDepth);
	char* Print2Buffer(char* pbuf, int depth);
};

/////////////////////////////////////////////////

class KAPI KXObjArr : public KXObj,
					  protected KArrayPtr
{
	KXObjArr();
public:
	~KXObjArr();

	static KXObjArr* CreateObj();

	OType GetType();
	int Size();

	int AddObj(KXObj* pObj);
	int AddInt(int val);
	int AddInt(__int64 val);
	int AddStrA(const char* str, int len = -1, bool isutf8 = false);
	int AddStrW(const wchar_t* str, int len = -1);
	int AddMem(const char* mem, int len);
	int InsertObj(KXObj* pObj, int index);
	void DelObj(int index);
	int FindObj(KXObj* pObj);

	KXObjBase* GetObj(int index);
	KXObjDic* GetDic(int index);
	KXObjArr* GetArr(int index);
	int GetInt(int index);
	__int64 GetInt64(int index);
	const char* GetStrA(int index, int* len = 0, bool* isutf8 = false);
	const wchar_t* GetStrW(int index, int* len = 0);
	const char* GetMem(int index, int* len=0);

	bool RemoveAt(int index, int n = 1);
	void RemoveAll();
	bool ParseBuffer(char* pbuf, int len, char** pend);

protected:
	int _CalcBufferLen(int nDepth);
	char* Print2Buffer(char* pbuf, int depth);
};
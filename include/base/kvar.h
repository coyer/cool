#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : kvar.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : 

*******************************************************************************************/


#include "kdef.h"
#include "kbase.h"

class KAPI KVariant : public KRefBase
{
public:
	typedef enum tagVarType
	{
		kvt_none = 0,
		kvt_char,
		kvt_short,
		kvt_int,
		kvt_int64,
		kvt_strw,
		kvt_stra,
		kvt_bin,
		kvt_pair,
		kvt_arr,
		kvt_dic,
	}varType;

	KVariant();
	KVariant(KVariant::varType type);
	~KVariant();

	KVariant::varType GetType();
	void    Clear();

	//if is char
	char  GetValueChar();
	bool  SetValueChar(char val);

	//if is short.
	short GetValueShort();
	bool  SetValueShort(short val);

	//if is int
	int   GetValueInt();
	bool  SetValueInt(int val);

	//if is int64
	__int64 GetValueInt64();
	bool    SetValueInt64(__int64 val);

	//if is stra
	const char*   GetValueStrA(int* len = 0);
	bool  SetValueStrA(const char* str, int len = -1);

	//if is strw
	const wchar_t*   GetValueStrW(int* len = 0);
	bool  SetValueStrW(const wchar_t* str, int len = -1);

	//if is bin data
	const void*   GetValueBin(int* len = 0);
	bool  SetValueBin(void* pBuf, int len);

	//if is pair
	KVariant* PairGetKey();
	KVariant* PairGetValue();
	bool      PairSetValue(KVariant* key, KVariant* value);

	//if is array
	int		  ArrGetCount();
	KVariant* ArrGetAt(int index);
	bool      ArrAdd(KVariant* value);
	bool      ArrRemove(int index);
	bool      ArrRemoveAll();


	static KVariant*  LoadFile(const wchar_t* filename);
	static bool       SaveFile(const wchar_t* filename);

	static KVariant*  New(KVariant::varType type);
protected:
	varType		m_type;
	int			m_len;

	union VarData
	{
		char  ch;
		short sh;
		int	  in;
		void* ptr;
		char* cstr;
		wchar_t* wstr;
		__int64 in64;
	} m_data;

};


#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : kstringarray.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : A array for save string.

*******************************************************************************************/

#include "kdef.h"
#include "kstring.h"
#include "karrayptr.h"

class KAPI KStringArrayA
{
public:
	KStringArrayA();
	~KStringArrayA();

	int	Add(const char* str);
	int	Add(const wchar_t* str);
	int	Add(const KStringA& str);
	int	Add(const KStringW& str);
	
	int		GetCount();
	KStringA GetString(int index);
	const char*	GetAt(int index);

	bool	SetAt(int index, const KString& str);
	bool    SetAt(int index, const char* str);
	bool    SetAt(int index, const wchar_t* str);
	void    Remove(int index, int count = 1);
	KStringA	RemoveAt(int index);

	void	RemoveAll();

	int		FindIndex(const char* str);

	void	SetSortMode(int enable);
	int		GetSortMode();

	KStringA ToString();
protected:
	KArrayPtr	m_datas;
	int		m_sortMode;
};

class KAPI KStringArrayW
{
public:
	KStringArrayW();
	~KStringArrayW();

	int	Add(const char* str);
	int	Add(const wchar_t* str);
	int	Add(const KStringA& str);
	int	Add(const KStringW& str);
	
	int		GetCount();
	KStringW GetString(int index);
	const wchar_t* GetAt(int index);
	bool	SetAt(int index, const KString& str);
	bool    SetAt(int index, const char* str);
	bool    SetAt(int index, const wchar_t* str);
	void    Remove(int index, int count = 1);
	KStringW	RemoveAt(int index);
	void	RemoveAll();

	int		FindIndex(const wchar_t* str);
	void	SetSortMode(int enable);
	int		GetSortMode();
	KStringW ToString();
protected:
	KArrayPtr	m_datas;
	int		m_sortMode;
};


#ifdef UNICODE
#define KStringArray	KStringArrayW
#else
#define KStringArray	KStringArrayA
#endif
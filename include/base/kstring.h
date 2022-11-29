#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : KString.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : I love MFC CString and not love std::string, this is Same as CString.

*******************************************************************************************/

#include "kdef.h"

class KStringDataA;
class KStringDataW;

class KAPI KStringA
{
	KStringDataA*	m_pStr;
	friend class KStringW;
public:
	KStringA();
	KStringA(const char* str, int len = -1);
	KStringA(const wchar_t* str, int len = -1);
	KStringA(KStringA& str);
	KStringA(KStringW& str);
	~KStringA();

	int		Append(char ch);
	int		Append(const char* str, int len = -1);
	int		Append(KStringA str);

	void	Clear();
	bool	IsNull();

	int		Splice(int start = 0, int len = -1);
	int		SetData(const char* str, int len = -1);
	int		SetData(const wchar_t* str, int len = -1);
	int		SetData(KStringA& str);
	int		SetData(KStringW& str);
	
	const char*	GetData() const;
	const char* c_str() const;

	char*	GetBuffer(int buflen = -1);
	void	ReleaseBuffer(int buflen);

	int		GetLength();

	int		GetSize();
	void	SetSize(int size);

	char	GetAt(int index);
	void	SetAt(int index, char ch);
	char	GetLast();
	char	GetFirst();

	int		InsertAt(int index, char ch);
	int		InsertAt(int index, const char* str, int len = -1);

	operator const char* () const;
	const char*	operator()();

	char	operator[](int index);

	KStringA& operator = (const char* str);
	KStringA& operator = (KStringA& str);
	KStringA& operator = (KStringW& str);
	KStringA& operator = (const wchar_t* str);
	KStringA& operator = (int val);
	KStringA& operator = (unsigned int val);
	KStringA& operator = (__int64 val);
	KStringA& operator = (double val);
	

	KStringA& operator + (char ch);
	KStringA& operator + (const char* str);
	KStringA& operator + (KStringA& str);

	KStringA& operator += (char ch);
	KStringA& operator += (const char* str);
	KStringA& operator += (KStringA& str);

	int		Compare(const char* str);
	int		Compare(const KStringA& str);

	inline bool operator == (const char* str) { return (0 == Compare(str));	}
	inline bool operator != (const char* str) {	return (0 != Compare(str));	}
	inline bool operator <= (const char* str) {	return (Compare(str) <= 0);	}
	inline bool operator <  (const char* str) {	return (Compare(str) < 0);	}
	inline bool operator >= (const char* str) {	return (Compare(str) >= 0);	}
	inline bool operator >  (const char* str) {	return (Compare(str) > 0);	}
	inline bool operator == (const KStringA& str) { return (0 == Compare(str));	}
	inline bool operator != (const KStringA& str) {	return (0 != Compare(str));	}
	inline bool operator <= (const KStringA& str) {	return (Compare(str) <= 0);	}
	inline bool operator <  (const KStringA& str) {	return (Compare(str) < 0);	}
	inline bool operator >= (const KStringA& str) {	return (Compare(str) >= 0);	}
	inline bool operator >  (const KStringA& str) {	return (Compare(str) > 0);	}

	int		CompareNoCase(const char* str);
	int		CompareNoCase(KStringA& str);

	void	MakeUpper();
	void	MakeLower();

	KStringA	Left(int length);
	KStringA	Mid(int startPos, int length = -1);
	KStringA	Right(int length);

	KStringA	TrimLeft(char ch = ' ');
	KStringA	TrimRight(char ch = ' ');
	KStringA	Trim(char ch = ' ');

	int		Find(char ch, int startPos = 0);
	int		Find(const char* str, int startPos = 0);
	int		ReverseFind(char ch);

	int		Replace(const char* strOld, const char* strNew);
	int		Format(const char* pstrFormat, ...);

	long	GetHashCode();
	int		FormatTime(time_t tmval);
	void	FillChar(char ch, int len);

	int		toInt(int radix = 10);
	__int64 toInt64(int radix = 10);
	double	toDouble();

protected:
	void	_AllocString();
};

class KAPI KStringW
{
	KStringDataW*	m_pStr;
	friend class KStringW;
public:
	KStringW();
	KStringW(const wchar_t* str, int len = -1);
	KStringW(const char* str, int len = -1);
	KStringW(KStringW& str);
	KStringW(KStringA& str);
	~KStringW();

	int		Append(wchar_t ch);
	int		Append(const wchar_t* str, int len = -1);
	int		Append(KStringW str);

	void	Clear();
	const bool	IsNull();

	int		Splice(int start = 0, int len = -1);
	int		SetData(const wchar_t* str, int len = -1);
	int		SetData(const char* str, int len = -1);
	int		SetData(KStringW& str);
	int		SetData(KStringA& str);
	
	const wchar_t*	GetData() const;
	const wchar_t*  c_str() const;
	wchar_t*	GetBuffer(int buflen = -1);
	void	ReleaseBuffer(int buflen);

	int		GetLength();
	

	int		GetSize();
	void	SetSize(int size);

	wchar_t	GetAt(int index);
	void	SetAt(int index, wchar_t ch);
	wchar_t GetLast();
	wchar_t GetFirst();

	int		InsertAt(int index, wchar_t ch);
	int		InsertAt(int index, const wchar_t* str, int len = -1);

	operator const wchar_t* () const;

	const wchar_t* operator()();

	wchar_t	operator[](int index);

	KStringW& operator = (const wchar_t* str);
	KStringW& operator = (KStringW& str);
	KStringW& operator = (KStringA& str);
	KStringW& operator = (const char* str);
	KStringW& operator = (int val);
	KStringW& operator = (unsigned int val);
	KStringW& operator = (__int64 val);
	KStringW& operator = (double val);
	
	KStringW& operator + (wchar_t ch);
	KStringW& operator + (const wchar_t* str);
	KStringW& operator + (KStringW& str);

	KStringW& operator += (wchar_t ch);
	KStringW& operator += (const wchar_t* str);
	KStringW& operator += (KStringW& str);

	int		Compare(const wchar_t* str);
	int		Compare(const KStringW& str);

	inline bool operator == (const wchar_t* str) { return (0 == Compare(str));	}
	inline bool operator != (const wchar_t* str) {	return (0 != Compare(str));	}
	inline bool operator <= (const wchar_t* str) {	return (Compare(str) <= 0);	}
	inline bool operator <  (const wchar_t* str) {	return (Compare(str) < 0);	}
	inline bool operator >= (const wchar_t* str) {	return (Compare(str) >= 0);	}
	inline bool operator >  (const wchar_t* str) {	return (Compare(str) > 0);	}

	inline bool operator == (const KStringW& str) { return (0 == Compare(str));	}
	inline bool operator != (const KStringW& str) {	return (0 != Compare(str));	}
	inline bool operator <= (const KStringW& str) {	return (Compare(str) <= 0);	}
	inline bool operator <  (const KStringW& str) {	return (Compare(str) < 0);	}
	inline bool operator >= (const KStringW& str) {	return (Compare(str) >= 0);	}
	inline bool operator >  (const KStringW& str) {	return (Compare(str) > 0);	}

	int		CompareNoCase(const wchar_t* str);
	int		CompareNoCase(KStringW& str);

	void	MakeUpper();
	void	MakeLower();

	KStringW	Left(int length);
	KStringW	Mid(int startPos, int length = -1);
	KStringW	Right(int length);

	KStringW	TrimLeft(wchar_t ch = ' ');
	KStringW	TrimRight(wchar_t ch = ' ');
	KStringW	Trim(wchar_t ch = ' ');

	int		Find(wchar_t ch, int startPos = 0);
	int		Find(const wchar_t* str, int startPos = 0);
	int		ReverseFind(wchar_t ch);

	int		Replace(const wchar_t* strOld, const wchar_t* strNew);
	int		Format(const wchar_t* pstrFormat, ...);

	long	GetHashCode();
	int		FormatTime(time_t tmval);
	void	FillChar(wchar_t ch, int len);

	//differ stringa
	int			SetUtf8String(const char* str, int len = -1);
	KStringA	GetUtf8String();

	int		toInt(int radix = 10);
	__int64 toInt64(int radix = 10);
	double	toDouble();

protected:
	void	_AllocString();
};

#ifdef UNICODE
#define KString	KStringW
#else
#define KString	KStringA
#endif
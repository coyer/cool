#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved
Header File : kJson.h
Author : coyer@126.com
Create :
Update : 2012-11-05     version 0.0.1
Detail : KRefBase is a base Obj class. some other class is derived from this class.
*******************************************************************************************/

#include "kdef.h"
#include "karrayptr.h"
#include "kstring.h"

class KAPI KJSObj
{
public:
	enum JSonType
	{
		Void,
		String,
		NUM_int,
		NUM_int64,
		NUM_double,
		Object,
		Array
	};

	KJSObj();
	virtual JSonType GetType() = 0;
	virtual BOOL fromString(const KString& str) = 0;
	virtual KString toString() = 0;
	virtual void Clear() = 0;
};

class KAPI KJSValue : public KJSObj
{
public:
	virtual int	asInt() = 0;
	virtual double	asDouble() = 0;
	virtual __int64	asInt64() = 0;
	virtual char	asChar() = 0;
	virtual KString	asString() = 0;
};

class KAPI KJSInt : public KJSValue {
public:
	virtual JSonType GetType();
	virtual BOOL fromString(KString& str);
	virtual KString toString();
	virtual void Clear();
	virtual int	asInt();
	virtual double	asDouble();
	virtual __int64	asInt64();
	virtual char	asChar();
	virtual KString	asString();
protected:
	int	m_data;
};

class KAPI KJSInt64 : public KJSValue {
public:
	virtual JSonType GetType();
	virtual BOOL fromString(const KString& str);
	virtual KString toString();
	virtual void Clear();
	virtual int	asInt();
	virtual double	asDouble();
	virtual __int64	asInt64();
	virtual char	asChar();
	virtual KString	asString();
protected:
	__int64	m_data;
};

class KAPI KJSDouble : public KJSValue {
public:
	virtual JSonType GetType();
	virtual BOOL fromString(const KString& str);
	virtual KString toString();
	virtual void Clear();
	virtual int	asInt();
	virtual double	asDouble();
	virtual __int64	asInt64();
	virtual char asChar();
	virtual KString	asString();
protected:
	double	m_data;
};

class KAPI KJSString : public KJSValue {
public:
	virtual JSonType GetType();
	virtual BOOL fromString(const KString& str);
	virtual KString toString();
	virtual void Clear();
	virtual int	asInt();
	virtual double	asDouble();
	virtual __int64	asInt64();
	virtual char	asChar();
	virtual KString	asString();
protected:
	KString	m_data;
};

class KAPI KJSDic {
	friend class KJSArr;
public:
	KJSDic();
	~KJSDic();

	void SetValue(KString key, int val);
	void SetValue(KString key, __int64 val);
	void SetValue(KString key, double val);
	void SetValue(KString key, KString val);
	void SetValue(KString key, LPCTSTR val);
	void SetValue(KString key, KJSObj* val);
	void SetValue(KString key, KJSArr* val);

	BOOL fromString(const KString& str);
	KString toString();
private:

};

class KAPI KJSArr {
public:
	KJSArr();
	~KJSArr();
};

class KAPI KJSon
{
public:

public:
	KJSon();
	virtual~KJSon();

	BOOL fromString(const KString* str);
	KString toString();
};
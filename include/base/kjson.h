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

class KAPI KJson : public KRefBase
{
public:
	enum JSonType
	{
		JST_UNKNOWN,
		JST_VALUE,
		JST_OBJECT,
		JST_ARRAY,
	};

	enum JSonValueType
	{
		JSVT_NONE,
		JSVT_INT,
		JSVT_DOUBLE,
		JSVT_INT64,
		JSVT_STRING
	};

	KJson();
	virtual JSonType getType() = 0;
	virtual KString toString() = 0;
	virtual void clear() = 0;
	virtual KJson* clone() = 0;

	bool saveToFile(const KString& filename);

	bool isValue();
	bool isObject();
	bool isArray();

	static KJson* FromFile(const KString& filename);
	static KJson* FromString(const KString& str);
};

class KAPI KJValue : public KJson
{
public:
	virtual JSonType getType();
	virtual KString toString();

	virtual int		asInt() = 0;
	virtual double	asDouble() = 0;
	virtual __int64	asInt64() = 0;
	virtual char	asChar() = 0;
	virtual KString	asString() = 0;
	virtual JSonValueType getValueType() = 0;

	bool isString();
	bool isInt();
	bool isDouble();
	bool isNumber();
};

class KAPI KJNumber : public KJValue 
{
public:
	KJNumber();
	KJNumber(KJNumber& num);
	KJNumber(JSonValueType number_type);
	KJNumber(int v);
	KJNumber(__int64 v);
	KJNumber(double v);
	virtual JSonValueType getValueType();
	virtual void clear();
	virtual KJson* clone();

	virtual int	asInt();
	virtual double	asDouble();
	virtual __int64	asInt64();
	virtual char	asChar();
	virtual KString	asString();

	int get();
	void set(int v);
	void set(__int64 v);
	void set(double v);

protected:
	union VarData
	{
		double dbl;
		int val;
		__int64 in64;
	} m_data;
	JSonValueType m_type;
};

class KAPI KJString : public KJValue
{
public:
	KJString();
	KJString(KJString& obj);
	KJString(KJNumber& obj);
	KJString(const KString& str);
	KJString(const char* pStr);
	KJString(const wchar_t* pStr);
	virtual JSonValueType getValueType();

	virtual void clear();
	virtual KJson* clone();

	virtual int	asInt();
	virtual double	asDouble();
	virtual __int64	asInt64();
	virtual char	asChar();
	virtual KString	asString();
	KString get();
	void set(const KString& str);

protected:
	KString	m_data;
};

/// <summary>
/// 
/// </summary>
class KAPI KJArr : public KJson {
public:
	KJArr();
	KJArr(KJArr& arr);
	~KJArr();
	virtual JSonType getType();
	virtual KString toString();
	void clear();
	KJson* clone();
	
	int add(KJson* v);
	void set(int index, KJson* v);
	void remove(int index, int count = 1);
	int getCount();
	KJson* get(int index);
	KJson* operator[] (int index);
	
protected:
	KArrayPtr m_data;
};

class KJObjectPairData : public KRefBase
{
	KJObjectPairData();
public:
	KString keyname;
	KJson* value;

	KJObjectPairData(const KString& name, KJson* v);
	~KJObjectPairData();

	void setValue(KJson* val);

	KJObjectPairData* clone();
};

class KAPI KJObject : public KJson {
public:
	KJObject();
	~KJObject();
	virtual JSonType getType();
	virtual KString toString();
	virtual void clear();
	virtual KJson* clone();

	void set(const KString& name, int val);
	void set(const KString& name, __int64 val);
	void set(const KString& name, double val);
	void set(const KString& name, const KString& val);
	void set(const KString& name, const char* val);
	void set(const KString& name, const wchar_t* val);
	void set(const KString& name, KJson* val);

	KJson* get(const KString& name);
	KStringArray getKeys();

private:
	KJObjectPairData* _FindPairData(const KString& name);

private:
	KArrayPtr	m_data;
};


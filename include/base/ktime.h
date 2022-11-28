#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : KXObj.h
Author : coyer@126.com
Create : 
Update : 2015-5-20     version 0.0.1
Detail : KTime is a wrapper of time. like java Time or MFC CTime
*******************************************************************************************/
#include "kbase.h"

class KTimeDelta
{
public:
	KTimeDelta();
	~KTimeDelta();

protected:
	explicit KTimeDelta(__int64 delta);
	friend class KTimeTicks;

public:
	static KTimeDelta FromDays(__int64 days);
	static KTimeDelta FromHours(__int64 hours);
	static KTimeDelta FromMinutes(__int64 minutes);
	static KTimeDelta FromSeconds(__int64 seconds);
	static KTimeDelta FromMilliseconds(__int64 ms);

public:
	int ToDays() const;
	int ToHours() const;
	int ToMinutes() const;
	__int64 ToSeconds() const;
	__int64 ToMilliseconds() const;

public:
	KTimeDelta& operator=(KTimeDelta other);

	KTimeDelta operator +(KTimeDelta other) const;
	KTimeDelta operator -(KTimeDelta other) const;
	KTimeDelta& operator +=(KTimeDelta other);
	KTimeDelta& operator -=(KTimeDelta other);
	KTimeDelta operator -() const;
	KTimeDelta operator *(__int64 a) const;
	KTimeDelta operator /(__int64 a) const;
	KTimeDelta& operator *=(__int64 a);
	KTimeDelta& operator /=(__int64 a);
	__int64 operator /(KTimeDelta a) const;
	KTimeTicks operator +(KTimeTicks t) const;
	BOOL operator ==(KTimeDelta other) const;
	BOOL operator !=(KTimeDelta other) const;
	BOOL operator <(KTimeDelta other) const;
	BOOL operator <=(KTimeDelta other) const;
	BOOL operator >(KTimeDelta other) const;
	BOOL operator >=(KTimeDelta other) const;

private:
	__int64 m_delta;  // ∫¡√Î ˝

	friend class KTimeTicks;
};

class KAPI KTimeTicks
{
	KTimeTicks();
	~KTimeTicks();

protected:
	explicit KTimeTicks(__int64 ticks);

public:
	static KTimeTicks Now();

public:
	BOOL IsNull() const;
	__int64 ToValue() const;

public:
	KTimeTicks& operator =(KTimeTicks other);
	BOOL operator ==(KTimeTicks other) const;
	BOOL operator !=(KTimeTicks other) const;
	BOOL operator <(KTimeTicks other) const;
	BOOL operator <=(KTimeTicks other) const;
	BOOL operator >(KTimeTicks other) const;
	BOOL operator >=(KTimeTicks other) const;
	KTimeDelta operator -(KTimeTicks other) const;
	KTimeTicks& operator +=(KTimeDelta delta);
	KTimeTicks& operator -=(KTimeDelta delta);
	KTimeTicks operator +(KTimeDelta delta) const;
	KTimeTicks operator -(KTimeDelta delta) const;

private:
	__int64 m_ticks;  // ∫¡√Î ˝

	friend KTimeDelta;
};

class KAPI KTime
{
public:
	KTime();
	KTime(__int64 ms);
	~KTime();

	KStringA	ToStringA();
	KStringW	ToStringW();
	KString		ToString();

protected:
	SYSTEMTIME	m_time;
};
#include "stdafx.h"
#include "../include/base/ktime.h"
#include <time.h>

static const __int64 kMillisecondsPerSecond = 1000;
static const __int64 kMillisecondsPerMinute = kMillisecondsPerSecond * 60;
static const __int64 kMillisecondsPerHour = kMillisecondsPerMinute * 60;
static const __int64 kMillisecondsPerDay = kMillisecondsPerHour * 24;

KTimeDelta::KTimeDelta()
	: m_delta(0)
{

}

KTimeDelta::KTimeDelta(__int64 delta)
	: m_delta(delta)
{

}

KTimeDelta::~KTimeDelta()
{

}

// static
KTimeDelta KTimeDelta::FromDays(__int64 days)
{
	return KTimeDelta(days * kMillisecondsPerDay);
}

// static
KTimeDelta KTimeDelta::FromHours(__int64 hours)
{
	return KTimeDelta(hours * kMillisecondsPerHour);
}

// static
KTimeDelta KTimeDelta::FromMinutes(__int64 minutes)
{
	return KTimeDelta(minutes * kMillisecondsPerMinute);
}

// static
KTimeDelta KTimeDelta::FromSeconds(__int64 seconds)
{
	return KTimeDelta(seconds * kMillisecondsPerSecond);
}

// static
KTimeDelta KTimeDelta::FromMilliseconds(__int64 ms)
{
	return KTimeDelta(ms);
}

int KTimeDelta::ToDays() const
{
	return static_cast<int>(m_delta / kMillisecondsPerDay);
}

int KTimeDelta::ToHours() const
{
	return static_cast<int>(m_delta / kMillisecondsPerHour);
}

int KTimeDelta::ToMinutes() const
{
	return static_cast<int>(m_delta / kMillisecondsPerMinute);
}

__int64 KTimeDelta::ToSeconds() const
{
	return static_cast<int>(m_delta / kMillisecondsPerSecond);
}

__int64 KTimeDelta::ToMilliseconds() const
{
	return m_delta;
}

KTimeTicks KTimeDelta::operator +(KTimeTicks t) const
{
	return KTimeTicks(t.m_ticks + m_delta);
}

KTimeDelta& KTimeDelta::operator =(KTimeDelta other)
{
	m_delta = other.m_delta;
	return *this;
}

KTimeDelta KTimeDelta::operator +(KTimeDelta other) const
{
	return KTimeDelta(m_delta + other.m_delta);
}

KTimeDelta KTimeDelta::operator -(KTimeDelta other) const
{
	return KTimeDelta(m_delta - other.m_delta);
}

KTimeDelta& KTimeDelta::operator +=(KTimeDelta other)
{
	m_delta += other.m_delta;
	return *this;
}

KTimeDelta& KTimeDelta::operator -=(KTimeDelta other)
{
	m_delta -= other.m_delta;
	return *this;
}

KTimeDelta KTimeDelta::operator -() const
{
	return KTimeDelta(-m_delta);
}

KTimeDelta KTimeDelta::operator *(__int64 a) const
{
	return KTimeDelta(m_delta * a);
}

KTimeDelta KTimeDelta::operator /(__int64 a) const
{
	return KTimeDelta(m_delta / a);
}

KTimeDelta& KTimeDelta::operator *=(__int64 a)
{
	m_delta *= a;
	return *this;
}

KTimeDelta& KTimeDelta::operator /=(__int64 a)
{
	m_delta /= a;
	return *this;
}

__int64 KTimeDelta::operator /(KTimeDelta a) const
{
	return m_delta / a.m_delta;
}

BOOL KTimeDelta::operator ==(KTimeDelta other) const
{
	return m_delta == other.m_delta;
}

BOOL KTimeDelta::operator !=(KTimeDelta other) const
{
	return m_delta != other.m_delta;
}

BOOL KTimeDelta::operator <(KTimeDelta other) const
{
	return m_delta < other.m_delta;
}

BOOL KTimeDelta::operator <=(KTimeDelta other) const
{
	return m_delta <= other.m_delta;
}

BOOL KTimeDelta::operator >(KTimeDelta other) const
{
	return m_delta > other.m_delta;
}

BOOL KTimeDelta::operator >=(KTimeDelta other) const
{
	return m_delta >= other.m_delta;
}

static __int64 g_rollover_ms = 0;
static DWORD g_last_seen_now = 0;
static KThreadLock g_rollover_lock;

KTimeTicks::KTimeTicks()
	: m_ticks(0)
{

}

KTimeTicks::KTimeTicks(__int64 ticks)
	: m_ticks(ticks)
{

}

KTimeTicks::~KTimeTicks()
{

}

// static
KTimeTicks KTimeTicks::Now()
{
	// 由于timeGetTime()返回系统自启动以来总毫秒数，在32位平台上每49.71天会重置为0
	KThreadLockAuto locked(&g_rollover_lock);
	DWORD now = ::GetTickCount();
	if(now < g_last_seen_now)
	{
		g_rollover_ms += 0x100000000I64;  //49.71天
	}
	g_last_seen_now = now;

	return KTimeTicks(now + g_rollover_ms);
}

BOOL KTimeTicks::IsNull() const
{
	return m_ticks == 0;
}

__int64 KTimeTicks::ToValue() const
{
	return m_ticks;
}

KTimeTicks& KTimeTicks::operator =(KTimeTicks other)
{
	m_ticks = other.m_ticks;
	return *this;
}

BOOL KTimeTicks::operator ==(KTimeTicks other) const
{
	return m_ticks == other.m_ticks;
}

BOOL KTimeTicks::operator !=(KTimeTicks other) const
{
	return m_ticks != other.m_ticks;
}

BOOL KTimeTicks::operator <(KTimeTicks other) const
{
	return m_ticks < other.m_ticks;
}

BOOL KTimeTicks::operator <=(KTimeTicks other) const
{
	return m_ticks <= other.m_ticks;
}

BOOL KTimeTicks::operator >(KTimeTicks other) const
{
	return m_ticks > other.m_ticks;
}

BOOL KTimeTicks::operator >=(KTimeTicks other) const
{
	return m_ticks >= other.m_ticks;
}

KTimeDelta KTimeTicks::operator -(KTimeTicks other) const
{
	return KTimeDelta(m_ticks - other.m_ticks);
}

KTimeTicks& KTimeTicks::operator +=(KTimeDelta delta)
{
	m_ticks += delta.m_delta;
	return *this;
}

KTimeTicks& KTimeTicks::operator -=(KTimeDelta delta)
{
	m_ticks -= delta.m_delta;
	return *this;
}

KTimeTicks KTimeTicks::operator +(KTimeDelta delta) const
{
	return KTimeTicks(m_ticks + delta.m_delta);
}

KTimeTicks KTimeTicks::operator -(KTimeDelta delta) const
{
	return KTimeTicks(m_ticks - delta.m_delta);
}

//////////////////////////////////////////////////////////////////////////
KTime::KTime()
{
	::GetLocalTime(&m_time);
}

KTime::KTime(__int64 ms)
{
	//m_time = ms;
}

KStringA KTime::ToStringA()
{
	return KStringA("");
}

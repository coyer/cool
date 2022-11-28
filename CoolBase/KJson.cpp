#include "StdAfx.h"
#include <tchar.h>
#include <stdlib.h>

#include "../include/base/kjson.h"

//////////////////////////////////////////////////////////////////
//util functions.
static char* SkipBlankBuffer(const char* pbuf, int len)
{
	int i = 0;
	while (len-- > 0)
	{
		if (*pbuf == '\n' || *pbuf == '\r' || *pbuf == '\t' || *pbuf == ' ')
		{
			pbuf++;
			continue;
		}
		break;
	}
	return (char*)pbuf;
}
//////////////////////////////////////////////////////////////////
KJSObj::KJSObj()
{
	
}


/// <summary>
/// 
/// </summary>
/// 
/// 	KJSValue(int val);
KJSObj::JSonType KJSInt::GetType()
{
	return KJSObj::NUM_int;
}

BOOL KJSInt::fromString(KString& str)
{
	m_data = str.toInt();
	return TRUE;
}

KString KJSInt::toString()
{
	return asString();
}

void KJSInt::Clear()
{
	m_data = 0;
}

int	KJSInt::asInt()
{
	return m_data;
}

double	KJSInt::asDouble()
{
	return m_data * 1.0;
}

__int64	KJSInt::asInt64()
{
	return m_data;
}

char KJSInt::asChar()
{
	return (char)m_data;
}

KString	KJSInt::asString()
{
	KString str;
	str.Format(_T("%d"), m_data);
	return str;
}
///////////////////////////////////////////////////////////////////////////////////////
KJSObj::JSonType KJSInt64::GetType()
{
	return KJSObj::NUM_int64;
}

BOOL KJSInt64::fromString(const KString& str)
{
	return FALSE;
}

KString KJSInt64::toString()
{
	return asString();
}

void KJSInt64::Clear()
{
	m_data = 0;
}

int	KJSInt64::asInt()
{
	return (int)m_data;
}

double	KJSInt64::asDouble()
{
	return m_data * 1.0;
}

__int64	KJSInt64::asInt64()
{
	return m_data;
}

char	KJSInt64::asChar()
{
	return (char)m_data;
}

KString	KJSInt64::asString()
{
	KString str;
	str.Format(_T("%l"), m_data);
	return str;
}
///////////////////////////////////////////////////////////////////////////////////
KJSObj::JSonType KJSDouble::GetType()
{
	return KJSObj::NUM_double;
}

BOOL KJSDouble::fromString(const KString& str)
{
	return FALSE;
}

KString KJSDouble::toString()
{
	return asString();
}

void KJSDouble::Clear()
{
	m_data = 0;
}

int	KJSDouble::asInt()
{
	return (int)m_data;
}

double	KJSDouble::asDouble()
{
	return m_data;
}

__int64	KJSDouble::asInt64()
{
	return (__int64)m_data;
}

char	KJSDouble::asChar()
{
	return (char)m_data;
}

KString	KJSDouble::asString()
{
	KString str;
	str.Format(_T("%f"), m_data);
	return str;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
KJSObj::JSonType KJSString::GetType()
{
	return KJSObj::String;
}

BOOL KJSString::fromString(const KString& str)
{
	m_data = str;
	return TRUE;
}

KString KJSString::toString()
{
	return asString();
}

void KJSString::Clear()
{
	m_data = _T("");
}

int	KJSString::asInt()
{
	return m_data.toInt();
}

double	KJSString::asDouble()
{
	return m_data.toDouble();
}

__int64	KJSString::asInt64()
{
	return m_data.toInt64();
}

char KJSString::asChar()
{
	return (char)asInt();
}

KString	KJSString::asString()
{
	return m_data;
}


////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// 
/// </summary>
KJSDic::KJSDic()
{

}

KJSDic::~KJSDic()
{

}
void KJSDic::SetValue(KString key, int val)
{

}
void KJSDic::SetValue(KString key, __int64 val)
{

}
void KJSDic::SetValue(KString key, double val)
{

}
void KJSDic::SetValue(KString key, KString val)
{

}
void KJSDic::SetValue(KString key, LPCTSTR val)
{

}
void KJSDic::SetValue(KString key, KJSObj* val)
{

}
void KJSDic::SetValue(KString key, KJSArr* val)
{

}

/// <summary>
/// KJSArr
/// </summary>

KJSArr::KJSArr()
{

}

KJSArr::~KJSArr()
{

}

KJSon::KJSon()
{

}

KJSon::~KJSon()
{

}
BOOL KJSon::fromString(const KString* str)
{
	return FALSE;
}

KString KJSon::toString()
{
	KString str;
	return str;
}

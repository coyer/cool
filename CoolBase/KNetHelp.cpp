#include "stdafx.h"
#include "../include/base/knethelp.h"
#include <wchar.h>


#ifdef WIN32
#include <WinSock2.h>
#endif

KAPI bool KIPCheck(const char* strIPv4)
{
	if (0 == strIPv4 || 0 == strIPv4[0])
		return false;

	int len = strlen(strIPv4);
	if (len < 7 || len > 15)
		return FALSE;

	int dotnum = 0;
	int secnum = 0;
	const char* p = strIPv4;
	while(*p)
	{
		if (*p == '.')
		{
			if (p == strIPv4)
				return false;
			dotnum++;
			secnum = 0;
		}
		else if (*p >= '0' && *p <= '9')
		{
			secnum = secnum * 10 + (*p - '0');
			if (secnum > 255)
				return false;
		}
		else
		{
			return false;
		}
		p++;
	}
	if (dotnum != 3)
		return false;

	return true;
}

KAPI bool KIPCheck(const wchar_t* strIPv4)
{
	if (0 == strIPv4 || 0 == strIPv4[0])
		return false;

	int len = wcslen(strIPv4);
	if (len < 7 || len > 15)
		return FALSE;

	int dotnum = 0;
	int secnum = 0;
	const wchar_t* p = strIPv4;
	while(*p)
	{
		if (*p == '.')
		{
			if (p == strIPv4)
				return false;
			dotnum++;
			secnum = 0;
		}
		else if (*p >= '0' && *p <= '9')
		{
			secnum = secnum * 10 + (*p - '0');
			if (secnum > 255)
				return false;
		}
		else
		{
			return false;
		}
		p++;
	}
	if (dotnum != 3)
		return false;

	return true;
}

KAPI int KIPCheckV6(const char* strIPv6)
{
	return IPV6_STRING_TYPE_ERROR;
}

KAPI int KIPCheckV6(const wchar_t* strIPv6)
{
	return IPV6_STRING_TYPE_ERROR;
}

KAPI unsigned int KIP2Int(const char* strIPv4)
{
	return inet_addr(strIPv4);
}

KAPI KStringA KIP2StringA(unsigned char ipv4[4])
{
	KStringA strOut;
	strOut.Format("%d.%d.%d.%d", ipv4[0], ipv4[1], ipv4[2], ipv4[3]);

	return strOut;
}

KAPI KStringW KIP2StringW(unsigned char ipv4[4])
{
	KStringW strOut;
	strOut.Format(L"%d.%d.%d.%d", ipv4[0], ipv4[1], ipv4[2], ipv4[3]);

	return strOut;
}

//todo, not support now.
KAPI KStringA KIP2StringA(unsigned char ipv6[16], int strtype)
{
	KStringA strOut;
	if (IPV6_STRING_TYPE_SIMPLE == strtype)
	{
		unsigned short* p = (unsigned short*)ipv6;
		strOut.Format("%X:%X:%X:%X:%X:%X:%X:%X", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
	}
	else //if (IPV6_STRING_TYPE_FULL == strtype)
	{
		strOut.Format("%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X", ipv6[0], ipv6[1], ipv6[2], ipv6[3], ipv6[4], ipv6[5], ipv6[6], ipv6[7], ipv6[8], ipv6[9], ipv6[10], ipv6[11], ipv6[12], ipv6[13], ipv6[14], ipv6[15]);
	}

	return strOut;
}

KAPI KStringW KIP2StringW(unsigned char ipv6[16], int strtype)
{
	KStringW strOut;
	if (IPV6_STRING_TYPE_SIMPLE == strtype)
	{
		unsigned short* p = (unsigned short*)ipv6;
		strOut.Format(L"%X:%X:%X:%X:%X:%X:%X:%X", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
	}
	else //if (IPV6_STRING_TYPE_FULL == strtype)
	{
		strOut.Format(L"%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X", ipv6[0], ipv6[1], ipv6[2], ipv6[3], ipv6[4], ipv6[5], ipv6[6], ipv6[7], ipv6[8], ipv6[9], ipv6[10], ipv6[11], ipv6[12], ipv6[13], ipv6[14], ipv6[15]);
	}

	return strOut;
}

KAPI bool KString2IP(const char* str, unsigned char ipv4[4])
{
	if (0 == str || 0 == str[0])
		return false;

	unsigned long ip = inet_addr(str);
	memcpy(ipv4, &ip, 4);
	return true;
}

KAPI bool KString2IP(const wchar_t* str, unsigned char ipv4[4])
{
	if (0 == str || 0 == str[0])
		return false;

	KStringA ip = str;

	return KString2IP(ip, ipv4);
}

KAPI bool KString2IPV6(const char* str, unsigned char ipv6[16])
{
	return false;
}

KAPI bool KString2IPV6(const wchar_t* str, unsigned char ipv6[16])
{
	return false;
}


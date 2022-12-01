#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : knethelp.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : Convert string to other type is very importent.

		inet_ntop or inet_pton only support in WIN7, not valid in xp.
		you can use it of use my function.

*******************************************************************************************/

#include "kdef.h"
#include "kstring.h"

#define IPV6_STRING_TYPE_ERROR	0
#define IPV6_STRING_TYPE_FULL	1
#define IPV6_STRING_TYPE_SIMPLE	2
#define IPV6_STRING_TYPE_FIX	3

//check str is an ip.
KAPI bool		KIPCheck(const char* strIPv4);
KAPI bool		KIPCheck(const wchar_t* strIPv4);

//if error, return IPV6_STRING_TYPE_ERROR
KAPI int		KIPCheckV6(const char* strIPv6);
KAPI int		KIPCheckV6(const wchar_t* strIPv6);

//convert "1.2.3.4" to a int value.
KAPI unsigned int	KIP2Int(const char* strIPv4);

KAPI KStringA		KIP2StringA(unsigned char ipv4[4]);
KAPI KStringW		KIP2StringW(unsigned char ipv4[4]);

//todo, not support now.
KAPI KStringA		KIP2StringA(unsigned char ipv6[16], int strtype);
KAPI KStringW		KIP2StringW(unsigned char ipv6[16], int strtype);

//inet_pton
KAPI bool		KString2IP(const char* str, unsigned char ipv4[4]);
KAPI bool		KString2IP(const wchar_t* str, unsigned char ipv4[4]);

//todo, not support now.
KAPI bool		KString2IPV6(const char* str, unsigned char ipv6[16]);
KAPI bool		KString2IPV6(const wchar_t* str, unsigned char ipv6[16]);


#ifdef UNICODE
#define KIP2StringW				KIP2String
#else
#define KIP2StringA				KIP2String
#endif


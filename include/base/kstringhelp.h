#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : kstringhelp.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : Convert string to other type is very importent.

*******************************************************************************************/

#include "kdef.h"
#include "kstring.h"

KAPI KStringA	KStringW2A(const wchar_t* strW, int len = -1);
KAPI KStringA	KStringW2A(KStringW strW);

KAPI KStringA	KStringW2Utf8(const wchar_t* strW, int len = -1);
KAPI KStringA	KStringW2Utf8(KStringW strW);

KAPI KStringW	KStringA2W(const char* strA, int len = -1);
KAPI KStringW	KStringA2W(KStringA strA);

KAPI KStringW	KStringUtf82W(const char* strA, int len = -1);
KAPI KStringW	KStringUtf82W(KStringA strA);

KAPI KStringA	KStringUtf82A(const char* utf8, int len = -1);
KAPI KStringA	KStringA2Utf8(const char* strA, int len = -1);

KAPI int		KString2Int(KStringA str);
KAPI int		KString2Int(KStringW str);

KAPI double		KString2Double(KStringA str);
KAPI double		KString2Double(KStringW str);

KAPI KStringA	KBuffer2HexStringA(unsigned char* buf, int buflen, int insertblank = 1, int changeline = 1, int linenumber = 10);
KAPI KStringW	KBuffer2HexStringW(unsigned char* buf, int buflen, int insertblank = 1, int changeline = 1, int linenumber = 10);

KAPI int		KHexString2Buffer(KStringA& strHex, unsigned char* outbuffer);
KAPI int		KHexString2Buffer(KStringW& strHex, unsigned char* outbuffer);
KAPI int		KHexString2Buffer(const wchar_t* strHex, int len, unsigned char* outbuffer);
KAPI int		KHexString2Buffer(const char* strHex, int len, unsigned char* outbuffer);

//fellow function have move to knethelp.h
//KAPI bool		KString2IP(KStringA str, unsigned char ipv4[4]);
//KAPI bool		KString2IP(KStringW str, unsigned char ipv4[4]);
//KAPI bool		KString2IPV6(KStringA str, unsigned char ipv4[16]);
//KAPI bool		KString2IPV6(KStringW str, unsigned char ipv4[16]);
//
//KAPI KStringA	KStringAFromIP(unsigned char ipv4[4]);
//KAPI KStringW	KStringWFromIP(unsigned char ipv4[4]);
//KAPI bool		KStringAFromIPV6(unsigned char ipv4[16], KStringA& strOut);
//KAPI bool		KStringWFromIPV6(unsigned char ipv4[16], KStringW& strOut);

#ifdef UNICODE
#define KBufferToHexString		KBuffer2HexStringW
#define KStringToUtf8			KStringW2Utf8
#define KStringFromUtf8			KStringUtf82W
#else
#define KBufferToHexString		KBuffer2HexStringA
#define KStringToUtf8			KStringA2Utf8
#define KStringFromUtf8			KStringUtf82A
#endif


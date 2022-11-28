#include "stdafx.h"
#include "../include/base/kstringhelp.h"
#include <wchar.h>
#include <stdlib.h>

KAPI KStringA KStringW2A(const wchar_t* strW, int len)
{
	KStringA strA(strW, len);
	return strA;
}

KAPI KStringA KStringW2A(KStringW strW)
{
	KStringA strA(strW);
	return strA;
}

KAPI KStringA KStringW2Utf8(const wchar_t* strW, int len)
{
	KStringW str(strW, len);
	return str.GetUtf8String();
}

KAPI KStringA KStringW2Utf8(KStringW strW)
{
	return strW.GetUtf8String();
}

KAPI KStringW KStringA2W(const char* strA, int len)
{
	KStringW strW(strA, len);
	return strW;
}

KAPI KStringW KStringA2W(KStringA strA)
{
	KStringW strW(strA);
	return strW;
}

KAPI KStringW KStringUtf82W(const char* strA, int len)
{
	KStringW strW;
	strW.SetUtf8String(strA, len);

	return strW;
}

KAPI KStringW KStringUtf82W(KStringA strA)
{
	KStringW strW;
	strW.SetUtf8String(strA.c_str(), strA.GetLength());

	return strW;
}

KAPI KStringA	KStringUtf82A(const char* utf8, int len)
{
	KStringW strW;
	strW.SetUtf8String(utf8, len);

	KStringA strA(strW);
	return strA;
}

KAPI KStringA	KStringA2Utf8(const char* strA, int len)
{
	KStringW strW;
	strW.SetData(strA, len);

	return strW.GetUtf8String();
}

KAPI int KString2Int(KStringA str)
{
	const char* sz = str.c_str();
	if (0 == sz)
		return 0;

	return atoi(sz);
}

KAPI int KString2Int(KStringW str)
{
	const wchar_t* sz = str.c_str();
	if (0 == sz)
		return 0;

	return _wtoi(sz);
}

KAPI double	KString2Double(KStringA str)
{
	const char* sz = str.c_str();
	if (0 == sz)
		return 0.0f;

	return strtod(sz, 0);
}

KAPI double	KString2Double(KStringW str)
{
	const wchar_t* sz = str.c_str();
	if (0 == sz)
		return 0.0f;

	return wcstod(sz, 0);
}

KAPI KStringA KBuffer2HexStringA(unsigned char* buf, int buflen, int insertblank, int changeline, int linenumber)
{
	static char* hexstringA = "0123456789ABCDEF";

	KStringA str;
	str.SetSize(buflen * 2 + insertblank * buflen + changeline * 2 * buflen / linenumber);
	
	for(int i=0; i<buflen; i++)
	{
		unsigned char ch = buf[i];
		str += hexstringA[(ch & 0xF0) >> 4];
		str += hexstringA[ch & 0x0F];

		if (i == buflen - 1)
			break;

		if (changeline && ((i + 1) % linenumber == 0))
			str += "\r\n";
		else if (insertblank)
			str += ' ';
	}
	return str;
}

KAPI KStringW	KBuffer2HexStringW(unsigned char* buf, int buflen, int insertblank, int changeline, int linenumber)
{
	static wchar_t* hexstringW = L"0123456789ABCDEF";
	
	KStringW str;
	str.SetSize(buflen * 2 + insertblank * buflen + changeline * 2 * buflen / linenumber);

	for(int i=0; i<buflen; i++)
	{
		unsigned char ch = buf[i];
		str += hexstringW[(ch & 0xF0) >> 4];
		str += hexstringW[ch & 0x0F];

		if (i == buflen - 1)
			break;

		if (changeline && ((i + 1) % linenumber == 0))
			str += L"\r\n";
		else if (insertblank)
			str += (' ');
	}
	
	return str;
}

KAPI int KHexString2Buffer(KStringA& strHex, unsigned char* outbuffer)
{
	return KHexString2Buffer(strHex.c_str(), strHex.GetLength(), outbuffer);
}

KAPI int KHexString2Buffer(KStringW& strHex, unsigned char* outbuffer)
{
	return KHexString2Buffer(strHex.c_str(), strHex.GetLength(), outbuffer);;
}

KAPI int KHexString2Buffer(const wchar_t* strHex, int len, unsigned char* outbuffer)
{
	if (0 == strHex)
		return 0;
	if (len < 0)
		len = wcslen(strHex);

	if (len < 2)
		return 0;

	unsigned char val = 0;
	int half = false;
	int index = 0;
	const wchar_t* buf = strHex;
	while(*buf)
	{
		char ch = (char)(*buf);
		if (ch >= 'a' && ch <= 'f')
		{
			if(half)
			{
				val *= 16;
				val += ch - 'a' + 10;
				half = false;
				if (outbuffer)
					outbuffer[index] = val;
				index++;
			}
			else
			{
				val = ch - 'a' + 10;
				half = true;
			}
		}
		else if (ch >= '0' && ch <= '9')
		{
			if(half)
			{
				val *= 16;
				val += ch - '0';
				half = false;
				if (outbuffer)
					outbuffer[index] = val;
				index++;
			}
			else
			{
				val = ch - '0';
				half = true;
			}
		}
		else if (ch >= 'A' && ch <= 'F')
		{
			if(half)
			{
				val *= 16;
				val += ch - 'A' + 10;
				half = false;
				if (outbuffer)
					outbuffer[index] = val;
				index++;
			}
			else
			{
				val = ch - 'A'  + 10;
				half = true;
			}
		}
		else
		{
			if (half)
			{
				half = false;
				if (outbuffer)
					outbuffer[index] = val;
				index++;
			}
		}

		buf++;
	}

	return index;
}

KAPI int KHexString2Buffer(const char* strHex, int len, unsigned char* outbuffer)
{
	if (0 == strHex)
		return 0;
	if (len < 0)
		len = strlen(strHex);

	if (len < 2)
		return 0;

	unsigned char val = 0;
	int half = false;
	int index = 0;
	const char* buf = strHex;
	while(*buf)
	{
		char ch = *buf;
		if (ch >= 'a' && ch <= 'f')
		{
			if(half)
			{
				val *= 16;
				val += ch - 'a' + 10;
				half = false;
				if (outbuffer)
					outbuffer[index] = val;
				index++;
			}
			else
			{
				val = ch - 'a' + 10;
				half = true;
			}
		}
		else if (ch >= '0' && ch <= '9')
		{
			if(half)
			{
				val *= 16;
				val += ch - '0';
				half = false;
				if (outbuffer)
					outbuffer[index] = val;
				index++;
			}
			else
			{
				val = ch - '0';
				half = true;
			}
		}
		else if (ch >= 'A' && ch <= 'F')
		{
			if(half)
			{
				val *= 16;
				val += ch - 'A' + 10;
				half = false;
				if (outbuffer)
					outbuffer[index] = val;
				index++;
			}
			else
			{
				val = ch - 'A'  + 10;
				half = true;
			}
		}
		else
		{
			if (half)
			{
				half = false;
				if (outbuffer)
					outbuffer[index] = val;
				index++;
			}
		}

		buf++;
	}

	return index;
}

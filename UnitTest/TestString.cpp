#include "StdAfx.h"
#include "TestString.h"


CTestString::CTestString(void)
{
}


CTestString::~CTestString(void)
{
}

#define ORG_STRING	"1234567890"
#define WORG_STRING	L"1234567890"

int CTestString::Test1()
{
	KStringA str1;
	str1 = WORG_STRING;

	if (str1.Compare(ORG_STRING) != 0)
		return 1;

	str1.Append(str1);
	if (str1.Compare(ORG_STRING ORG_STRING) != 0)
		return 2;

	KStringA str2;
	str2 = str1.Left(10);
	if (str2.Compare(ORG_STRING) != 0)
		return 4;

	KStringA str3;
	str3 = str1.Right(10);
	if (str3.Compare(ORG_STRING) != 0)
		return 5;

	KStringA str4;
	str4.Format("%d-%d", 100, 200);
	if (str4.Compare("100-200") != 0)
		return 6;

	KStringA str5(str1);
	if (str5 != ORG_STRING ORG_STRING)
		return 7;

	if (str5.Find("456") != 3)
		return 8;

	str5.Replace("456", "aaa");
	if (str5 != "123aaa7890123aaa7890")
		return 9;

	if (str5.CompareNoCase("123aaa7890123AAA7890") != 0)
		return 10;

	str5.MakeUpper();
	if (str5 != "123AAA7890123AAA7890")
		return 11;

	KStringA str6 = str5.TrimRight('0');
	if (str6 != "123AAA7890123AAA789")
		return 12;

	str6.Clear();
	if (str6 != "")
		return 13;

	if (str1.Compare(ORG_STRING ORG_STRING) != 0)
		return 3;

	return 0;
}


int CTestString::Test2()
{
	KStringW str1;
	str1 = ORG_STRING;

	if (str1.Compare(WORG_STRING) != 0)
		return 1;

	str1.Append(str1);
	if (str1.Compare(WORG_STRING WORG_STRING) != 0)
		return 2;

	KStringW str2;
	str2 = str1.Left(10);
	if (str2.Compare(WORG_STRING) != 0)
		return 4;

	KStringW str3;
	str3 = str1.Right(10);
	if (str3.Compare(WORG_STRING) != 0)
		return 5;

	KStringW str4;
	str4.Format(L"%d-%d", 100, 200);
	if (str4.Compare(L"100-200") != 0)
		return 6;

	KStringW str5(str1);
	if (str5 != WORG_STRING WORG_STRING)
		return 7;

	if (str5.Find(L"456") != 3)
		return 8;

	str5.Replace(L"456", L"aaa");
	if (str5 != L"123aaa7890123aaa7890")
		return 9;

	if (str5.CompareNoCase(L"123aaa7890123AAA7890") != 0)
		return 10;

	str5.MakeUpper();
	if (str5 != L"123AAA7890123AAA7890")
		return 11;

	KStringW str6 = str5.TrimRight(L'0');
	if (str6 != L"123AAA7890123AAA789")
		return 12;

	str6.Clear();
	if (str6 != L"")
		return 13;

	if (str1.Compare(WORG_STRING WORG_STRING) != 0)
		return 3;

	str6.Format(L"%I64d", 123456789012345);
	if (str6 != L"123456789012345")
		return 14;

	return 0;
}

int CTestString::Test3()
{
	unsigned char pHex[18];
	for(int i=0; i<18; i++)
	{
		pHex[i] = i;
	}

	KString str1 = KBuffer2HexString(pHex, 18, 1, 0);
	if (str1 != _T("00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11"))
		return 1;

	KString str2 = KBuffer2HexString(pHex, 18, 0, 0);
	if (str2 != _T("000102030405060708090A0B0C0D0E0F1011"))
		return 2;

	KString str3 = KBuffer2HexString(pHex, 18, 0, 1, 10);
	if (str3 != _T("00010203040506070809\r\n0A0B0C0D0E0F1011"))
		return 3;

	unsigned char szOutHex[18];
	int retlen = KHexString2Buffer(_T("00010203040506070809\r\n0A0B0C0D0E0F1011"), -1, szOutHex);
	if (retlen != 18)
		return 4;
	if (memcmp(szOutHex, pHex, retlen) != 0)
		return 5;

	retlen = KHexString2Buffer(_T("000102030405060708090A0B0C0D0E0F1011"), -1, szOutHex);
	if (retlen != 18)
		return 6;
	if (memcmp(szOutHex, pHex, retlen) != 0)
		return 7;

	retlen = KHexString2Buffer(_T("00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11"), -1, szOutHex);
	if (retlen != 18)
		return 8;
	if (memcmp(szOutHex, pHex, retlen) != 0)
		return 9;

	return 0;
}

int CTestString::Test4()
{
	KStringArray arr;
	arr.AddString("lll");
	arr.AddString("bbb");
	arr.AddString("ttt");
	arr.AddString("ccc");
	arr.AddString("ddd");
	arr.AddString("fff");
	arr.AddString("ggg");
	arr.AddString("hhh");
	arr.AddString("iii");
	arr.AddString("rrr");
	arr.AddString("jjj");
	arr.AddString("kkk");
	arr.AddString("mmm");	
	arr.AddString("nnn");
	arr.AddString("ooo");
	arr.AddString("ppp");
	arr.AddString("qqq");
	arr.AddString("aaa");
	arr.AddString("eee");
	arr.AddString("sss");

	for(int i=0; i<arr.GetCount(); i++)
	{
		wprintf(L"\n%s", arr.GetAt(i));
	}
	

	if (arr.GetCount() != 20)
		return 1;

	KString aaa = "aaa";
	if (aaa != arr.GetAt(0))
		return 2;

	if (19 != arr.FindIndex(L"ttt"))
		return 3;


	return 0;
}
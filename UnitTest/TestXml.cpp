#include "StdAfx.h"
#include "TestXml.h"


CTestXml::CTestXml(void)
{
}


CTestXml::~CTestXml(void)
{
}

int	CTestXml::Test1()
{
	KFile file;
	file.Open(_T("D:\\2.xml"), KFile::modeCreate | KFile::modeWrite);
	
	const wchar_t* str1 = L"abcdefg";
	const wchar_t* str2 = L"hijklmn";
	file.WriteString(str1);	
	file.WriteString(str2);
	file.Close();

	if (!file.Open(_T("D:\\2.xml"), KFile::modeRead))
		return 3;

	KStringW	kstr1, kstr2;
	file.ReadString(kstr1);
	file.ReadString(kstr2);
	if (kstr1 != str1 || kstr2 != str2)
		return 4;

	file.Close();

	KXmlDoc doc;
	if (!doc.LoadFromFile("book.xml"))
		return 1;

	KXmlNode* pRoot = doc.GetRoot();
	if (0 == pRoot)
		return 2;

	KString strXml = pRoot->ToString();
	const wchar_t* p = strXml.c_str();

	pRoot->SaveToFile("d:\\1.xml");

	return 0;
}
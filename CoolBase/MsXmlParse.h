#pragma once

//#import <msxml4.dll> raw_interfaces_only named_guids
//using namespace MSXML2;
#include <msxml6.h>


class CMsXmlParse
{
public:
	CMsXmlParse(void);
	~CMsXmlParse(void);

	KXmlNode* LoadFile(const TCHAR* strFilename);
	KXmlNode* LoadFile(const KString& strFilename);
	KXmlNode* LoadString(const TCHAR* strXml);
	KXmlNode* LoadString(const KString& strXml);

	KString	  CreateXmlString(KXmlNode* pRoot);

	bool	SaveXmlFile(const wchar_t* strFilename, KXmlNode* pRoot);
	bool	SaveXmlFile(const char* strFilename, KXmlNode* pRoot);
	bool	SaveXmlFile(const KString& strFilename, KXmlNode* pRoot);
protected:
	KXmlNode* ParseElement(IXMLDOMNode* pMsNode);
	KXmlNode* ParseXmlDoc(IXMLDOMDocument * pXmlDoc);

	bool ParseStdElement(IXMLDOMNode* pMsNode, KXmlNode* pKNode);
	bool ParseTextElement(IXMLDOMNode* pMsNode, KXmlNode* pKNode);

	void PrintXmlToKString(KStringW& str, KXmlNode* pNode, int deep);

};


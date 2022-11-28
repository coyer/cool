#pragma once

#import <msxml3.dll> raw_interfaces_only


class CMsXmlParse
{
public:
	CMsXmlParse(void);
	~CMsXmlParse(void);

	KXmlNode* LoadFile(const wchar_t* strFilename);
	KXmlNode* LoadString(const wchar_t* strXml);

	KString	  CreateXmlString(KXmlNode* pRoot);

	bool	SaveXmlFile(const wchar_t* strFilename, KXmlNode* pRoot);
	bool	SaveXmlFile(const char* strFilename, KXmlNode* pRoot);

protected:
	KXmlNode* ParseElement(IXMLDOMNode* pMsNode);
	KXmlNode* ParseXmlDoc(IXMLDOMDocument * pXmlDoc);

	bool ParseStdElement(IXMLDOMNode* pMsNode, KXmlNode* pKNode);
	bool ParseTextElement(IXMLDOMNode* pMsNode, KXmlNode* pKNode);

	void PrintXmlToKString(KStringW& str, KXmlNode* pNode, int deep);

};


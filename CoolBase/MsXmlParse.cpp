#include "StdAfx.h"
#include "MsXmlParse.h"
#include "../include/base/kfile.h"


#pragma comment(lib, "msxml6.lib")

CMsXmlParse::CMsXmlParse(void)
{
}


CMsXmlParse::~CMsXmlParse(void)
{
}

KXmlNode* CMsXmlParse::LoadFile(const TCHAR* strFilename)
{
	if (!strFilename) return NULL;

	HRESULT hr;
	IXMLDOMDocument * pXmlDoc = 0;
	KXmlNode* pRet = 0;
	
	CoInitialize(NULL);

	hr = CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)&pXmlDoc);
	
	if (pXmlDoc)
	{
		VARIANT_BOOL bRet = false;

		VARIANT vtFile;
		vtFile.vt = VT_BSTR;
		vtFile.bstrVal = ::SysAllocString(strFilename);

		hr = pXmlDoc->load(vtFile, &bRet);

		::SysFreeString(vtFile.bstrVal);
		if (SUCCEEDED(hr) && bRet == VARIANT_TRUE)
		{
			pRet = ParseXmlDoc(pXmlDoc);			
		}

		pXmlDoc->Release();
	}

	::CoUninitialize();
	return pRet;
}

KXmlNode* CMsXmlParse::LoadFile(const KString& strFilename)
{
	return LoadFile(strFilename.c_str());
}

KXmlNode* CMsXmlParse::LoadString(const TCHAR* strXml)
{
	if (!strXml) return NULL;

	HRESULT hr;
	IXMLDOMDocument * pXmlDoc = 0;
	KXmlNode* pRet = 0;

	CoInitialize(NULL);
	
	hr = CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)&pXmlDoc);

	if (pXmlDoc)
	{
		VARIANT_BOOL bRet = false;

		BSTR bstrXml = ::SysAllocString(strXml);

		hr = pXmlDoc->loadXML(bstrXml, &bRet);

		::SysFreeString(bstrXml);

		if (SUCCEEDED(hr) && bRet == VARIANT_TRUE)
		{
			pRet = ParseXmlDoc(pXmlDoc);
		}

		pXmlDoc->Release();
	}

	::CoUninitialize();
	return pRet;
}

KXmlNode* CMsXmlParse::LoadString(const KString& strXml)
{
	return LoadString(strXml.c_str());
}

KXmlNode* CMsXmlParse::ParseElement(IXMLDOMNode* pMsNode)
{
	DOMNodeType nodetype;
	pMsNode->get_nodeType(&nodetype);
	if (nodetype == NODE_INVALID)
		return 0;

	KXmlDoc doc;
	KXmlNode* pKNode = 0;
	switch(nodetype)
	{
	case NODE_ELEMENT:
		{
			pKNode = doc.CreateNode(KXmlNode::KXML_TYPE_ELEMENT);
			ParseStdElement(pMsNode, pKNode);
		}
		break;
	case NODE_ATTRIBUTE:
		break;
	case NODE_TEXT:
		{
			pKNode = doc.CreateNode(KXmlNode::KXML_TYPE_TEXT);
			ParseTextElement(pMsNode, pKNode);
		}
		break;
	case NODE_CDATA_SECTION:
		{
			pKNode = doc.CreateNode(KXmlNode::KXML_TYPE_CDATA_SECTION);
			ParseTextElement(pMsNode, pKNode);
		}
		break;
	case NODE_COMMENT:
		{
			pKNode = doc.CreateNode(KXmlNode::KXML_TYPE_COMMENT);
			ParseTextElement(pMsNode, pKNode);
		}
		break;
	case NODE_DOCUMENT_TYPE:
		{
			pKNode = doc.CreateNode(KXmlNode::KXML_TYPE_DOCUMENT_TYPE);
			ParseTextElement(pMsNode, pKNode);
		}
		break;
	default:
		break;
	}
	
	return pKNode;
}

KXmlNode* CMsXmlParse::ParseXmlDoc(IXMLDOMDocument * pXmlDoc)
{
	if (0 == pXmlDoc)
		return false;

	IXMLDOMElement* pRoot = 0;
	pXmlDoc->get_documentElement(&pRoot);

	if (0 == pRoot)
		return 0;

	KXmlDoc doc;
	KXmlNode* pKNode = doc.CreateNode();
	
	ParseStdElement(pRoot, pKNode);

	return pKNode;
}

bool CMsXmlParse::ParseStdElement(IXMLDOMNode* pMsNode, KXmlNode* pKNode)
{
	BSTR name;
	pMsNode->get_baseName(&name);
	pKNode->SetName(name);
	::SysFreeString(name);

	IXMLDOMNamedNodeMap * pAttrMap = 0;
	pMsNode->get_attributes(&pAttrMap);
	if (pAttrMap)
	{
		long count = 0;
		pAttrMap->get_length(&count);

		for(int i=0; i<count; i++)
		{
			IXMLDOMNode* pAttr = 0;
			pAttrMap->get_item(i, &pAttr);

			BSTR name,value;
			pAttr->get_baseName(&name);
			pAttr->get_text(&value);

			pKNode->SetAttrValue(name, value);
			::SysFreeString(name);
			::SysFreeString(value);
		}
	}

	IXMLDOMNodeList* pChildList = 0;
	pMsNode->get_childNodes(&pChildList);
	
	if (pChildList)
	{
		long count = 0;
		pChildList->get_length(&count);

		for(int i=0; i<count; i++)
		{
			IXMLDOMNode* pChild = 0;
			pChildList->get_item(i, &pChild);

			pKNode->AddChild(ParseElement(pChild));
		}
	}

	return true;
}

bool CMsXmlParse::ParseTextElement(IXMLDOMNode* pMsNode, KXmlNode* pKNode)
{
	BSTR bstr;
	pMsNode->get_text(&bstr);
	pKNode->SetName(bstr);
	::SysFreeString(bstr);

	return true;
}


////////////////////////////////////////////////////////////////////////////////////////////

KString	CMsXmlParse::CreateXmlString(KXmlNode* pRoot)
{
	KStringW str;
	str.SetSize(10240);

	PrintXmlToKString(str, pRoot, 0);

	return str;
}


typedef struct 
{ 
	LPCTSTR left;
	int leftlen;
	LPCTSTR right;
	int rightlen;
}KXMLClearTag;

static KXMLClearTag KXMLClearTags[] =
{
    {    L"<![CDATA[",9,  L"]]>"      ,3},
    {    L"<!DOCTYPE",9,  L">"        ,1},
    {    L"<!--"     ,4,  L"-->"      ,3},
    {    L"<PRE>"    ,5,  L"</PRE>"   ,6},
//  {    _T("<Script>") ,8,  _T("</Script>"),9},
    {    NULL           ,0,  NULL           ,0}
};

typedef struct 
{ 
	const TCHAR* s; 
	int l; 
	const TCHAR* c;
}KXMLCharacterEntity;

static KXMLCharacterEntity KXMLEntities[] =
{
    { L"&amp;" , 5, L"&" },
    { L"&lt;"  , 4, L"<" },
    { L"&gt;"  , 4, L">" },
    { L"&quot;", 6, L"\""},
    //{ L"&apos;", 6, L"\'"},
    { NULL     , 0, L"\0"}
};


void ReplaceXmlString(KString& strSrc)
{
	for(int i=0; i<sizeof(KXMLEntities) / sizeof(KXMLEntities[0]); i++)
	{
		if (KXMLEntities[i].s == 0)
			break;

		strSrc.Replace(KXMLEntities[i].c, KXMLEntities[i].s);
	}
}

void CMsXmlParse::PrintXmlToKString(KStringW& str, KXmlNode* pNode, int deep)
{
	KString valTmp;
	KString tmpName = pNode->GetName();;
	ReplaceXmlString(tmpName);

	int type = pNode->GetType();

	if (type == KXmlNode::KXML_TYPE_TEXT)
	{
		str.Append(tmpName);
		return;
	}

	KStringW strBlank;
	if (deep > 0)
	{
		strBlank.FillChar(L' ', deep);
		str.Append(strBlank);
	}

	if (type == KXmlNode::KXML_TYPE_CDATA_SECTION)
	{
		str.Append(KXMLClearTags[0].left, KXMLClearTags[0].leftlen);
		str.Append(tmpName);
		str.Append(KXMLClearTags[0].right, KXMLClearTags[0].rightlen);
		str.Append(L"\r\n", 2);

		return;
	}	
	else if (type == KXmlNode::KXML_TYPE_COMMENT)
	{
		str.Append(KXMLClearTags[2].left, KXMLClearTags[2].leftlen);
		str.Append(tmpName);
		str.Append(KXMLClearTags[2].right, KXMLClearTags[2].rightlen);
		str.Append(L"\r\n", 2);

		return;
	}
	else if (type == KXmlNode::KXML_TYPE_DOCUMENT_TYPE)
	{
		str.Append(KXMLClearTags[1].left, KXMLClearTags[1].leftlen);
		str.Append(tmpName);
		str.Append(KXMLClearTags[1].right, KXMLClearTags[1].rightlen);
		str.Append(L"\r\n", 2);

		return;
	}
	//else do fellowing.

	
	str.Append(L'<');
	str += tmpName;

	for(int i=0; i<pNode->GetAttrCount(); i++)
	{
		KXmlAttr* pAttr = pNode->GetAttrAt(i);
		str.Append(L' ');
		str.Append(pAttr->key);
		str.Append(L"=\"", 2);

		valTmp = pAttr->val;
		ReplaceXmlString(valTmp);
		str.Append(valTmp);
		str.Append(L'\"');
	}

	int nCount = pNode->GetChildCount();
	if (0 == nCount)
	{
		str.Append(L"/>", 2);
	}
	else 
	{
		str.Append(L'>');

		BOOL bNewLine = TRUE;
		if (pNode->GetChildCount() > 0)
		{
			KXmlNode* pChild = pNode->GetChild(0);
			if (pChild->GetType() == KXmlNode::KXML_TYPE_TEXT)
				bNewLine = FALSE;
		}
		
		if (bNewLine)
			str.Append(L"\r\n", 2);


		for(int i=0; i<pNode->GetChildCount(); i++)
		{
			KXmlNode* pChild = pNode->GetChild(i);
			PrintXmlToKString(str, pChild, deep + 1);
		}

		if (str.GetAt(str.GetLength() - 1) == L'\n')
		{
			str+= strBlank;
		}

		str.Append(L"</", 2);
		str.Append(tmpName);
		str.Append(L">\r\n", 3);
	}
}

const wchar_t * xmlhead_utf16 = L"<?xml version=\"1.0\"?>";

bool CMsXmlParse::SaveXmlFile(const wchar_t* strFilename, KXmlNode* pRoot)
{
	KStringW strXml;

	KTextFile file;
	if (!file.Create(strFilename, KTextFile::cf_utf8));
		return false;

	PrintXmlToKString(strXml, pRoot, 0);
	file.WriteString(xmlhead_utf16);
	//file.WriteString(strXml);
	file.Write(strXml.GetBuffer(), sizeof(wchar_t) * strXml.GetLength());
	file.Close();
	return true;
}

const char* xmlhead_utf8 = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>";
bool CMsXmlParse::SaveXmlFile(const char* strFilename, KXmlNode* pRoot)
{
	KStringW strXml;

	KTextFile file;
	if (!file.Open(strFilename,false))
		return false;

	PrintXmlToKString(strXml, pRoot, 0);
	KStringA strUtf8 = strXml.GetUtf8String();

	file.WriteString(xmlhead_utf8);
	file.Write(strUtf8.GetBuffer(), strUtf8.GetLength());
	file.Close();
	return true;
}

bool CMsXmlParse::SaveXmlFile(const KString& strFilename, KXmlNode* pRoot)
{
	return SaveXmlFile(strFilename.c_str(), pRoot);
}
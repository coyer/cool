#include "stdafx.h"
#include "../include/base/kxml.h"
#include "MsXmlParse.h"

////////////////////////////////////////////////////////////////////////////////////////////////
KXmlDoc::KXmlDoc()
{
	m_pRoot = 0;
}

KXmlDoc::KXmlDoc(KXmlNode* pRoot)
{
	m_pRoot = pRoot;
}

KXmlDoc::~KXmlDoc()
{
	if (m_pRoot)
		m_pRoot->Release();
}

KXmlNode* KXmlDoc::GetRoot()
{
	return m_pRoot;
}

void KXmlDoc::Attach(KXmlNode* pRoot)
{
	if (m_pRoot)
		m_pRoot->Release();
	m_pRoot = pRoot;
}

KXmlNode* KXmlDoc::Detch()
{
	KXmlNode* p = m_pRoot;
	m_pRoot = 0;

	return p;
}

KXmlNode* KXmlDoc::CreateNode(KXmlNode::NodeType type)
{
	return new KXmlNode(type);
}

bool KXmlDoc::CreateNewDoc(const wchar_t* rootNodeName)
{
	if (0 == rootNodeName || 0 == rootNodeName[0])
		return false;

	if (m_pRoot)
		m_pRoot->Release();

	m_pRoot = new KXmlNode(KXmlNode::KXML_TYPE_ELEMENT);
	m_pRoot->SetName(rootNodeName);

	return true;
}

bool KXmlDoc::LoadFromFile(const char* filename)
{
	KStringW strFilename = filename;
	return LoadFromFile(strFilename.c_str());
}

bool KXmlDoc::LoadFromString(const char* strXml)
{
	KStringW strXmlW = strXml;
	return LoadFromString(strXmlW.c_str());
}

bool KXmlDoc::LoadFromUtf8String(const char* strXml)
{
	KStringW strXmlW;
	strXmlW.SetUtf8String(strXml);

	return LoadFromString(strXmlW.c_str());
}

#ifdef _WIN32
bool KXmlDoc::LoadFromFile(const wchar_t* filename)
{
	if (m_pRoot)
		m_pRoot->Release();

	CMsXmlParse parse;
	m_pRoot = parse.LoadFile(filename);

	if (0 == m_pRoot)
		return false;

	return true;
}

bool KXmlDoc::LoadFromString(const wchar_t* strXml)
{
	if (m_pRoot)
		m_pRoot->Release();

	CMsXmlParse parse;
	m_pRoot = parse.LoadStringW(strXml);

	if (0 == m_pRoot)
		return false;

	return true;
}
#else
bool KXmlDoc::LoadFromFile(const wchar_t* filename)
{
	return false;
}

bool KXmlDoc::LoadFromString(const wchar_t* strXml)
{
	return false;
}
#endif
#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : kxml.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : xml class is importent, windows and linux all need it.
		KXmlDoc is only a wrapper for m_pRoot, so if your code is:
		KXmlDoc newvar = oldXmlDoc;

		newvar's m_pRoot is same one of oldXmlDoc's m_pRoot.
*******************************************************************************************/


#include "kstring.h"
#include "karrayptr.h"
#include "kbase.h"


typedef struct tagKXmlNodeAttr
{
	KString key;
	KString val;
}KXmlAttr;

class KAPI KXmlNode : public KRefBase
{
	friend class KXmlDoc;
public:
	typedef enum tagNodeType
	{
		KXML_TYPE_UNKNOWN = 0,
		KXML_TYPE_ELEMENT,
		KXML_TYPE_TEXT,
		KXML_TYPE_CDATA_SECTION,
		KXML_TYPE_COMMENT,
		KXML_TYPE_DOCUMENT_TYPE,
	}NodeType;

protected:
	KString	m_name;
	int		m_type;
	KArrayPtr	m_child;
	KArrayPtr	m_attrs;

	KXmlNode();
	KXmlNode(NodeType type);

public:
	virtual ~KXmlNode();

	KString		GetName();
	void		SetName(KString name);
	int			GetType();

	virtual KStringW	ToString();
	virtual bool SaveToFile(const wchar_t* filename);
	virtual bool SaveToFile(const char* filename);

	int			GetChildCount();
	KXmlNode*	GetChild(int index);
	KXmlNode*	GetChild(KString name);
	KXmlNode*	GetChildByAttr(KString key, const TCHAR* val);
	KXmlNode*	GetChildByAttr(KString key, KString& val);
	KXmlNode*	AddChild(KString name, NodeType type = KXML_TYPE_ELEMENT);

	int			FindChild(KString name);
	KXmlNode*	PickupNode(int index);
	KXmlNode*	PickupNode(KString name);

	void		AddChild(KXmlNode* pNode);

	void		RemoveChild(KString& name);
	void		RemoveChild(const TCHAR* name);
	void		RemoveChild(int index);
	void		RemoveChildAll();

	int			GetAttrCount();
	KXmlAttr*	GetAttrAt(int index);
	
	void		RemoveAttr(KString& key);
	void		RemoveAttr(const TCHAR* key);
	void		RemoveAttr(int index);
	void		RemoveAttrAll();

	KString			GetChildValueText(KString childname, KString defval);
	const TCHAR*	GetChildValueText(KString childname, const TCHAR* defval = 0);
	int				GetChildValueInt(KString childname, int defval = 0);
	__int64			GetChildValueInt64(KString childname, __int64 defval = 0L);
	double			GetChildValueDouble(KString childname, double defval = 0.0f);
	bool			GetChildValueBool(KString childname, bool defval = false);
	unsigned int	GetChildValueUInt(KString childname, unsigned int defval = 0);

	void			AddChildValue(KString childname, KString& value);
	void			AddChildValue(KString childname, const TCHAR* value);
	void			AddChildValue(KString childname, int value);
	void			AddChildValue(KString childname, unsigned int value);
	void			AddChildValue(KString childname, __int64 value);
	void			AddChildValue(KString childname, double value);
	void			AddChildValue(KString childname, bool value);

	KString			GetAttrValue(KString key, KString& strVal);
	const TCHAR*	GetAttrValue(KString key, const TCHAR* defval = 0);
	int				GetAttrValueInt(KString key, int defval = 0);
	unsigned int	GetAttrValueUInt(KString key, unsigned int defval = 0);
	__int64			GetAttrValueInt64(KString key, __int64 defval = 0L);
	double			GetAttrValueDouble(KString key, double defval = 0.0f);
	bool			GetAttrValueBool(KString key, bool defval = false);

	bool			SetAttrValue(KString key, KString& value);
	bool			SetAttrValue(KString key, const TCHAR* value);
	bool			SetAttrValue(KString key, int value);
	bool			SetAttrValue(KString key, unsigned int value);
	bool			SetAttrValue(KString key, __int64 value);
	bool			SetAttrValue(KString key, double value);
	bool			SetAttrValue(KString key, bool value);
};

class KAPI KXmlDoc
{
protected:
	KXmlNode*	m_pRoot;

public:
	KXmlDoc();
	KXmlDoc(KXmlNode* pRoot);
	~KXmlDoc();

	KXmlNode*	GetRoot();

	void		Attach(KXmlNode* pRoot);
	KXmlNode*	Detch();

	virtual  KXmlNode*	CreateNode(KXmlNode::NodeType type = KXmlNode::KXML_TYPE_ELEMENT);

	virtual  bool	CreateNewDoc(const wchar_t* rootNodeName);
	virtual  bool	LoadFromFile(const wchar_t* filename);
	virtual  bool	LoadFromFile(const char* filename);
	virtual  bool	LoadFromString(const char* strXml);
	virtual  bool	LoadFromString(const wchar_t* strXml);
	virtual  bool	LoadFromUtf8String(const char* strXml);
};
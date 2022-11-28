#include "StdAfx.h"
#include "KExtMgr.h"


IKExtMgr*	KExtMgrCreate()
{
	return new KExtMgr();
}

void KExtMgrDestroy(IKExtMgr* pMgr)
{
	if (pMgr)
		delete pMgr;
}

//////////////////////////////////////////////////////

KExtMgr::KExtMgr(void)
{
}


KExtMgr::~KExtMgr(void)
{
}

KString	KExtMgr::GetFuncName()
{
	return FN_EXTMGR;
}

int	KExtMgr::GetFuncType()
{
	return FT_EXTMGR;
}

int	KExtMgr::DoAction(int argc, wchar_t** argv)
{
	return 0;
}

int	KExtMgr::GetFuncCount()
{
	return m_funcs.GetCount();
}

IKExt*	KExtMgr::GetFuncAt(int index)
{
	if (index < 0 || index >= m_funcs.GetCount())
		return 0;

	return (IKExt*)m_funcs.GetAt(index);
}

IKExt*	KExtMgr::GetFunc(KString name)
{
	int index = FindFunc(name);

	return GetFuncAt(index);
}

IKExt*	KExtMgr::GetFunc(int type)
{
	int index = FindFunc(type);

	return GetFuncAt(index);
}

int	KExtMgr::FindFunc(KString name)
{
	for(int i=0; i<m_funcs.GetCount(); i++)
	{
		IKExt* pFunc = (IKExt*)m_funcs.GetAt(i);
		if (pFunc->GetFuncName().Compare(name) == 0)
			return i;
	}

	return -1;
}

int	KExtMgr::FindFunc(int type)
{
	for(int i=0; i<m_funcs.GetCount(); i++)
	{
		IKExt* pFunc = (IKExt*)m_funcs.GetAt(i);
		if (pFunc->GetFuncType() == type)
			return i;
	}

	return -1;
}

int	KExtMgr::AddFunc(IKExt* pFunc)
{
	int t = FindFunc(pFunc->GetFuncType());
		
	if (t >= 0)
		return -1;

	return m_funcs.Add(pFunc);
}

void KExtMgr::RemoveFunc(int index)
{
	m_funcs.RemoveAt(index);
}

void KExtMgr::RemoveFuncAll()
{
	m_funcs.RemoveAll();
}

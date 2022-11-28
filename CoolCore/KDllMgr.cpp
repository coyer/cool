#include "StdAfx.h"
#include "KDllMgr.h"

typedef struct tagDllDllInfo
{
	KDll			dll;
	KDllInfo		info;
}DllDllInfo;

IKDllMgr* KDllMgrCreate()
{
	return new KDllMgr();
}

void KDllMgrDestroy(IKDllMgr* pMgr)
{
	delete pMgr;
}

KDllMgr::KDllMgr(void)
{
}

KDllMgr::~KDllMgr(void)
{
	UnloadAll();
}

int	KDllMgr::GetCount()
{
	return m_dlls.GetCount();
}

KDllInfo* KDllMgr::GetInfoAt(int index)
{
	if (index < 0 || index >= m_dlls.GetCount())
		return 0;

	DllDllInfo* p = (DllDllInfo*)m_dlls.GetAt(index);
	return &(p->info);
}

KDll* KDllMgr::GetDllAt(int index)
{
	if (index < 0 || index >= m_dlls.GetCount())
		return 0;

	DllDllInfo* p = (DllDllInfo*)m_dlls.GetAt(index);
	return &(p->dll);
}

void KDllEnumFileCallBack(KString filepath, KString filename, int filesize, unsigned int attr, time_t create, time_t write, time_t access, void* pVoid)
{
	KStringArray *arrs = (KStringArray *)pVoid;

	KString str = filepath;
	str += filename;
	arrs->AddString(str);
}

int	KDllMgr::LoadPath(const wchar_t* DllFolder, const wchar_t* Dllext)
{
	KStringArray arrs;
	KFileEnum(DllFolder, Dllext, false, KDllEnumFileCallBack, &arrs);

	if (arrs.GetCount() == 0)
		return 0;

	int count = 0;
	for(int i=0; i<arrs.GetCount(); i++)
	{
		if (LoadDll(arrs.GetAt(i)) == 0)
			count++;
	}

	return count;
}

int	KDllMgr::LoadDll(const wchar_t* fullpath)
{
	int ret = 0;
	DllDllInfo *p = new DllDllInfo();
	p->info.filepath = fullpath;

	if (p->dll.Load(fullpath))
	{
		DllGetInfoProc procGetInfo = (DllGetInfoProc)(p->dll.GetProc("DllGetInfo"));
		DllInitProc procInit = (DllInitProc)(p->dll.GetProc("DllInit"));

		if (0 == procGetInfo || 0 == procInit)
		{
			ret = -2;
		}
		else
		{
			procGetInfo(&(p->info));
			//if (KAPP_VERSION != proc(&(p->info)))
			//{
			//	ret = -3;
			//}
		}
	}
	else
	{
		ret = -1;
	}

	if (ret != 0)
	{
		p->dll.Close();
		delete p;
	}
	else
	{
		m_dlls.Add(p);
	}

	return ret;
}

void KDllMgr::UnloadAll()
{
	for(int i=0; i<m_dlls.GetCount(); i++)
	{
		DllDllInfo *p = (DllDllInfo *)m_dlls.GetAt(i);
		DllUninitProc proc = (DllUninitProc)(p->dll.GetProc("DllUninit"));
		if (proc)
			proc();

		p->dll.Close();
		delete p;
	}
	m_dlls.RemoveAll();
}

int	KDllMgr::FindByID(KString idname)
{
	for(int i=0; i<m_dlls.GetCount(); i++)
	{
		DllDllInfo *p = (DllDllInfo *)m_dlls.GetAt(i);
		if (p->info.idname == idname)
			return i;		
	}

	return -1;
}

void KDllMgr::UnloadDll(KString idname)
{
	int index = FindByID(idname);
	if (index < 0)
		return;

	DllDllInfo *p = (DllDllInfo *)m_dlls.GetAt(index);
	DllUninitProc proc = (DllUninitProc)(p->dll.GetProc("DllUninit"));
	if (proc)
		proc();

	p->dll.Close();

	m_dlls.RemoveAt(index);
	delete p;
}

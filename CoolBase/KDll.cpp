#include "stdafx.h"
#include "../include/base/kdll.h"

KDll::KDll()
{
	m_dll = 0;
}

KDll::~KDll()
{
	Close();
}

bool KDll::Load(const wchar_t* filename, unsigned int openflag)
{
	m_filename = filename;
#ifdef WIN32
	m_dll = LoadLibrary(m_filename);
#else
	m_dll = dlopen(m_filename, openflag);
#endif
	return m_dll != 0;
}

bool KDll::Load(const char* filename, unsigned int openflag)
{
	m_filename = filename;
#ifdef WIN32
	m_dll = LoadLibrary(m_filename);
#else
	m_dll = dlopen(filename, openflag);
#endif
	return m_dll != 0;
}

bool KDll::Load(KString& filename, unsigned int openflag)
{
	return Load(filename.c_str(), openflag);
}

void* KDll::Detach()
{
	void* t = m_dll;
	m_dll = 0;
	return t;
}

void KDll::Attach(void* hDll)
{
	Close();
	m_dll = hDll;
}

void KDll::Close()
{
	if (m_dll)
	{
		void* pTmp = m_dll;
		m_dll = 0;

#ifdef WIN32
		FreeLibrary((HMODULE)pTmp);
#else
		dlclose(pTmp);
#endif
		m_filename.Clear();
	}
}

KString	KDll::GetFilename()
{
	return m_filename;
}

bool KDll::IsOpened()
{
	if (m_dll)
		return true;
	return false;
}

void* KDll::GetProc(const char* procname)
{
	if (0 == m_dll || 0 == procname)
		return 0;
#ifdef WIN32
	return ::GetProcAddress((HMODULE)m_dll, procname);
#else
	return dlsym (m_dll, "procname");
#endif
}
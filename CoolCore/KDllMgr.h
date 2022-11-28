#pragma once
#include "../include/core/idll.h"

class KDllMgr : public IKDllMgr
{
public:
	KDllMgr(void);
	~KDllMgr(void);

	virtual int				GetCount();
	virtual KDllInfo*		GetInfoAt(int index);
	virtual KDll*			GetDllAt(int index);
	
	//pluginext is file filter, like ".dll"
	virtual int			LoadPath(const wchar_t* pluginFolder, const wchar_t* pluginext);

	//return 0 means ok, else is error code.
	virtual int			LoadDll(const wchar_t* fullpath);

	virtual void		UnloadAll();
	virtual void		UnloadDll(KString idname);

	virtual int			FindByID(KString idname);
protected:
	KArray			m_dlls;
};


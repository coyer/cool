#pragma once
#include "../include/base/karrayt.h"

class KExtMgr : public IKExtMgr
{
public:
	KExtMgr(void);
	~KExtMgr(void);

	virtual KString		GetFuncName();
	virtual int			GetFuncType();
	virtual int			DoAction(int argc, wchar_t** argv);

	virtual int			GetFuncCount();
	virtual IKExt*		GetFuncAt(int index);
	virtual IKExt*		GetFunc(KString name);
	virtual IKExt*		GetFunc(int type);

	virtual int			FindFunc(KString name);
	virtual int			FindFunc(int type);

	virtual int			AddFunc(IKExt* pFunc);
	virtual void		RemoveFunc(int index);
	virtual void		RemoveFuncAll();

protected:
	KArrayT<void*>		m_funcs;
};


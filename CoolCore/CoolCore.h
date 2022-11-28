#pragma once
#include "../include/core/iapp.h"
#include "../include/core/idll.h"


IKExtMgr*	KExtMgrCreate();
void		KExtMgrDestroy(IKExtMgr* pMgr);

IKDllMgr*	KDllMgrCreate();
void		KDllMgrDestroy(IKDllMgr* pMgr);

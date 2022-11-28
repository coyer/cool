#include "stdafx.h"

KObj::KObj()
{
}

KObj::~KObj()
{
}

//////////////////////////////////////////////////////////////////////

KRefBase::KRefBase()
{
	m_ref = 1;
}

KRefBase::~KRefBase()
{
}

int	KRefBase::GetRef()
{
	return m_ref;
}

void KRefBase::AddRef()
{
	::InterlockedIncrement(&m_ref);
}

void KRefBase::Release()
{
	::InterlockedDecrement(&m_ref);

	if (m_ref <= 0)
		delete this;
}

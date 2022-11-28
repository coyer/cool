#include "stdafx.h"

KEdit::KEdit()
{
	m_charMax = 0;
	m_charMin = 0;
}

KEdit::~KEdit()
{

}

int	KEdit::GetMaxChars()
{
	return m_charMax;
}
int	KEdit::GetMinChars()
{
	return m_charMin;
}

void KEdit::SetMaxChars(int max_count)
{
	m_charMax = max_count;
}

void KEdit::SetMinChars(int min_count)
{
	m_charMin = min_count;
}
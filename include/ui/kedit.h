#pragma once

#include "kuidef.h"
#include "kview.h"

class KUI_API KEdit : public KView 
{
public:
	KEdit();
	virtual ~KEdit();
	
	int		GetMaxChars();
	int		GetMinChars();
	void	SetMaxChars(int max_count);
	void	SetMinChars(int min_count);

private:
	int m_charMax;
	int m_charMin;
};
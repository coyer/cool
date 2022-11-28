#pragma once

#include "kuidef.h"

class KUI_API KLabel : public KView
{
public:
	KLabel();
	virtual ~KLabel();

	virtual		KString		GetViewTypeName();
};
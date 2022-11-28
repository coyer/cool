#pragma once

#include "kuidef.h"

class KUI_API KImage
{
public:
	KImage();
	~KImage();

	BOOL	LoadImage(KString strFilename);

protected:
	Image*	m_image;
};
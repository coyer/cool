#include "stdafx.h"

KImage::KImage()
{
	m_image = 0;
}

KImage::~KImage()
{
}

BOOL KImage::LoadImage(KString strFilename)
{
	if (m_image)
		delete m_image;

	m_image = new Image(strFilename.GetBuffer(), FALSE);
	


	return TRUE;
}

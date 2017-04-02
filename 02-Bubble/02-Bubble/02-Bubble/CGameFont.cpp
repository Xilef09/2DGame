#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "CGameFont.h"

void CGameFont::CreateFont(const std::string& strTypeface,
	int iFontHeight,
	int iFontWeight)
{
	if (!m_hDeviceContext)
	{
		string strError = "Impossible to create the font: ";
		strError += strTypeface;
		throw CException(strError);
		return;
	}

	// Ask openGL to generate a contiguous set of 255 display lists.
	m_uiListBase = glGenLists(255);
	if (m_uiListBase == 0)
	{
		string strError = "Impossible to create the font: ";
		strError += strTypeface;
		throw CException(strError);
		return;
	}

	// Create the Windows font
	m_hFont = ::CreateFont(-iFontHeight,
		0,
		0,
		0,
		iFontWeight,
		FALSE,
		FALSE,
		FALSE,
		ANSI_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH,
		strTypeface.c_str());
	if (m_hFont == NULL)
	{
		m_uiListBase = 0;
		string strError = "Impossible to create the font: ";
		strError += strTypeface;
		throw CException(strError);
		return;
	}

	// Select the newly create font into the device context (and save the previous
	// one).
	HFONT hOldFont = (HFONT)SelectObject(m_hDeviceContext, m_hFont);
	// Generate the font display list (for the 255 characters) starting
	// at display list m_uiListBase.
	wglUseFontBitmaps(m_hDeviceContext, 0, 255, m_uiListBase);
	// Set the original font back in the device context
	SelectObject(m_hDeviceContext, hOldFont);
}

#pragma once
#include "EuroScopePlugIn.h"
#include "requestMenu.hpp"

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"

class CVCHApp : public CWinApp
{
public:
	CVCHApp();

	// Overrides
public:
	virtual BOOL InitInstance();
	CVCHPlugin * gpMyPlugin = NULL;
	DECLARE_MESSAGE_MAP()
};

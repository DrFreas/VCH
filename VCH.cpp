// VCH.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "EuroScopePlugIn.h"
#include "requestMenu.hpp"

CVCHPlugin * gpMyPlugin = NULL;

void    __declspec (dllexport)    EuroScopePlugInInit(EuroScopePlugIn::CPlugIn ** ppPlugInInstance)
{

		// create the instance
	* ppPlugInInstance = gpMyPlugin = new CVCHPlugin();
}


//---EuroScopePlugInExit-----------------------------------------------

void    __declspec (dllexport)    EuroScopePlugInExit(void)
{

		// delete the instance
		delete gpMyPlugin;
}

/*
Changelog
0.6.2:
- added option to disable tracking requirement for CTL to show
0.6.1:
- added "cleared to land" function
0.6.0:
- added online version checking
0.5.1:
- changed option parameter for request items for consistency
0.5.0:
- added ability to change colors of tag items to user set colors
- updated compatibility for "new" groundstates
0.4.6:
- added command "release" which will release the aircraft specified in the command
0.4.5: 
- added option "noblink" so all requests and hold-shorts will not flash but appear in solid colours
- added option "option" to either show or hide specific request types 

*/
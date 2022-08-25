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
0.8.2:
- added new tag item: short form of request (format: C1, P2 etc.)
- added a buffer for erasing requests automatically, should fix a bug where one station deleted a request automaticaly which was set by another station
0.8.1:
- added "reminder" flag for whatever use. Manual trigger only and not synced with anyone (except it is active, you are tracking and handing off a tag)!
0.8.0:
- added syncing with other stations (finally), now FlightStripAnnotations are ditched, everything is in remarks now
0.7.1:
- added "CTL" flag which only showws when CTL is active
- added "CFT" flag which only shows when an aircraft has ground state "DEPA" and is within 1nm of it's origin
0.7.0:
- Rewritten the way requests are stored in the plugin from scratch, making live a lot easier and completely ditching vectors
0.6.3:
- fixed a bug where the sequence vectors would not clean up non-valid callsigns
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
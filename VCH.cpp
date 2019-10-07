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
		//gpMyPlugin->syncServerThread._Abandon();
		delete gpMyPlugin;
}
#pragma once
#include "EuroScopePlugIn.h"
#include <iostream>
#include <string>
#include <algorithm>
#include "Constant.hpp"
#include <chrono>
//#include <future>
//#include "syncClient.hpp"
//#include "syncServer.hpp"

using namespace std;
using namespace EuroScopePlugIn;

const string MY_PLUGIN_NAME = "VCH";
const string MY_PLUGIN_VERSION = "0.1.1";
const string MY_PLUGIN_DEVELOPER = "Jan Fries";
const string MY_PLUGIN_COPYRIGHT = "GPL v3";
const string MY_PLUGIN_VIEW_AVISO = "Kleine Helferlein in Euroscope";

class CVCHPlugin: public EuroScopePlugIn::CPlugIn
{
public:
	CVCHPlugin();
	virtual ~CVCHPlugin();

	virtual void OnGetTagItem(CFlightPlan FlightPlan, CRadarTarget RadarTarget, int ItemCode, int TagData, char sItemString[16], int * pColorCode, COLORREF * pRGB, double * pFontSize);

	virtual void OnFunctionCall(int FunctionId, const char * sItemString, POINT Pt, RECT Area);

	virtual void OnTimer(int Counter);

	//future<void> syncServerThread;

	//future<void> syncClientThread;

	virtual void displayMessage(string message);

	virtual void displayError(string message);

	virtual int findSequence(vector<string> seqMe, CFlightPlan seqThis);

	virtual void setupSyncServer();

	virtual void setupSyncClient();

	virtual void setClearence(bool status, CFlightPlan flightPlan);

	virtual void setPushback(bool status, CFlightPlan flightPlan);
	
	virtual void setTaxi(bool status, CFlightPlan flightPlan);

	virtual void resetRequest(CFlightPlan flightPlan);

};

#pragma once
#include "EuroScopePlugIn.h"
#include "usefulStuff.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "Constant.hpp"
#include <chrono>
#include <thread>
#include <algorithm>

using namespace std;
using namespace EuroScopePlugIn;

const string MY_PLUGIN_NAME = "VCH";
const string MY_PLUGIN_VERSION = "0.4.5";
const string MY_PLUGIN_DEVELOPER = "Jan Fries";
const string MY_PLUGIN_COPYRIGHT = "GPL v3";
const string MY_PLUGIN_VIEW_AVISO = "Kleine Helferlein in Euroscope";

class CVCHPlugin: public EuroScopePlugIn::CPlugIn
{
public:
	// Euroscope functions

	CVCHPlugin();
	virtual ~CVCHPlugin();

	virtual void OnGetTagItem(CFlightPlan flightPlan, CRadarTarget RadarTarget, int ItemCode, int TagData, char sItemString[16], int * pColorCode, COLORREF * pRGB, double * pFontSize);

	virtual void OnFunctionCall(int FunctionId, const char * sItemString, POINT Pt, RECT Area);

	virtual bool OnCompileCommand(const char* sCommandLine);

	virtual void OnTimer(int Counter);

	// Plugin functions

	// Request section

	virtual void statusToVector(CFlightPlan flightPlan, bool add);

	virtual void setStatus(string status, CFlightPlan flightPlan);

	virtual string getStatus(CFlightPlan flightPlan);

	virtual COLORREF getTimeColour(int time);

	virtual void delStatus(CFlightPlan flightPlan);

	virtual void setSequence(vector<string> *thisVector, vector<int> *thisSeq);

	virtual void cleanVectors();

	// Hold-short section

	virtual void setHoldShort(string holdShort, CFlightPlan flightPlan);

	virtual void delHoldShort(CFlightPlan flightPlan);

	virtual string getHoldShort(CFlightPlan flightPlan);

	// Common section

	virtual void displayMessage(string message);

	virtual void displayError(string message);

	virtual void pushStrip(CFlightPlan flightPlan);

	string getAirport();

};

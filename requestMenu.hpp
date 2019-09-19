#pragma once
#include "EuroScopePlugIn.h"
#include <string>
#include <algorithm>
#include "Constant.hpp"
#include <chrono>

const string MY_PLUGIN_NAME = "VCH";
const string MY_PLUGIN_VERSION = "0.0.3";
const string MY_PLUGIN_DEVELOPER = "Jan Fries";
const string MY_PLUGIN_COPYRIGHT = "GPL v3";
const string MY_PLUGIN_VIEW_AVISO = "Kleine Helferlein in Euroscope";

using namespace std;
using namespace EuroScopePlugIn;

class CVCHPlugin: public EuroScopePlugIn::CPlugIn
{
public:
	CVCHPlugin();
	virtual ~CVCHPlugin();

	virtual void OnGetTagItem(CFlightPlan FlightPlan, CRadarTarget RadarTarget, int ItemCode, int TagData, char sItemString[16], int * pColorCode, COLORREF * pRGB, double * pFontSize);

	virtual void OnFunctionCall(int FunctionId, const char * sItemString, POINT Pt, RECT Area);

	virtual void OnTimer(int Counter);
};

#pragma once
#include "EuroScopePlugIn.h"
#include "usefulStuff.hpp"
#include <iostream>
#include <map>
#include "Constant.hpp"
#include <thread>
#include "internet.hpp"

using namespace std;
using namespace EuroScopePlugIn;

const string MY_PLUGIN_NAME = "VCH";
const string MY_PLUGIN_VERSION = "0.8.1";
const string MY_PLUGIN_DEVELOPER = "Jan Fries";
const string MY_PLUGIN_COPYRIGHT = "GPL v3";
const string MY_PLUGIN_VIEW_AVISO = "Kleine Helferlein in Euroscope";

typedef struct requestBox {
	char type{};
	int time = -1;
	int lastSeen = -1;
	bool active() {
		return getTime() - 10 < lastSeen;
	}
	bool erase() {
		return getTime() - 600 > lastSeen;
	}
	int age() {
		return getTime() - time;
	}
};

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

	virtual void createRequest(string callsign, string requestString);

	virtual bool hasRequest(string callsign);

	virtual void setStatus(string status, CFlightPlan *flightPlan);

	virtual string getStatus(CFlightPlan *flightPlan);

	virtual string getSequence(string callsign, char type);

	virtual void cleanRequests();

	virtual COLORREF getTimeColor(int time);

	virtual COLORREF getTextColor(int tagItem);

	// Hold-short section

	virtual void setHoldShort(string holdShort, CFlightPlan *flightPlan);

	virtual string getHoldShort(CFlightPlan *flightPlan);

	// Cleared-to-land section

	virtual void setClearedToLand(CFlightPlan *flightPlan, bool state);

	virtual bool isClearedToLand(CFlightPlan *flightPlan);

	// Reminder section

	virtual void switchReminder(CFlightPlan *flightPlan);

	virtual bool hasReminder(CFlightPlan *flightPlan);

	// Common section

	virtual void delAnnotation(int field, CFlightPlan *flightPlan);

	virtual void versionCheck();

	virtual void displayMessage(string message);

	virtual void displayError(string message);

	virtual void displayDebug(string message);

	virtual bool isLanding(double distance);

	virtual bool isDeparting(double distance);

	virtual bool getTracking(bool tracking);

	//virtual int getAltitudeAboveAirport(CFlightPlan flightPlan);

	string getAirport();

};

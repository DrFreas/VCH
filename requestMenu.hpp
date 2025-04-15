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
const string MY_PLUGIN_VERSION = "0.8.6";
const string MY_PLUGIN_DEVELOPER = "Jan Fries";
const string MY_PLUGIN_COPYRIGHT = "GPL v3";
const string MY_PLUGIN_VIEW_AVISO = "Kleine Helferlein in Euroscope";

struct requestBox {
	char type{};
	time_t time = 0;
	time_t lastSeen = 0;
	bool byMe = false;
	time_t eraseTime = 0;
	bool active() {
		return getTime() - 10 < lastSeen;
	}
	bool erase() {
		return getTime() - 600 > lastSeen;
	}
	time_t age() {
		return getTime() - time;
	}
};

struct landingBox {
	bool byMe = false;
	time_t eraseTime = 0;
	bool exists = true;
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

	virtual void modRequest(string callsign, string requestString);

	virtual bool hasRequest(string callsign);

	virtual void setStatus(string status, CFlightPlan *flightPlan);

	virtual char checkGroundState(CFlightPlan* flightPlan);

	virtual string getStatus(CFlightPlan *flightPlan);

	virtual string getSequence(string callsign, char type);

	virtual void cleanRequests();

	virtual COLORREF getTimeColor(time_t time);

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

	virtual void antiAmendSpam(CFlightPlan* flightPlan);

	virtual void versionCheck();

	virtual void displayMessage(string message);

	virtual void displayError(string message);

	virtual void displayDebug(string message);

	virtual bool isLanding(double distance);

	virtual bool isDeparting(double distance);

	virtual bool getTracking(bool tracking);

	virtual bool isAmendable(string callsign);

	virtual int countRequests(map<string, requestBox>* request, char requestType);

	//virtual int getAltitudeAboveAirport(CFlightPlan flightPlan);

	string getAirport();

};

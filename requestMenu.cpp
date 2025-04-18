#include "stdafx.h"
#include "requestMenu.hpp"


clock_t timerOn;

bool FLASH, NOBLINK, NOCTLT = false;
bool DEBUG = false;
bool NORC, NORS, NORP, NORT, NORD = false;
int requests[5]{ 0,0,0,0,0 }; // C, S, P, T, D
double CTEN = SEQ_TEN_ZERO;
double STEN = SEQ_TEN_ZERO;
double PTEN = SEQ_TEN_ZERO;
double TTEN = SEQ_TEN_ZERO;
double DTEN = SEQ_TEN_ZERO;
COLORREF colorHOS{ TAG_YELLOW }, colorRQC{ TAG_GREEN }, colorRQP{ TAG_YELLOW }, colorRQS{ TAG_YELLOW }, colorRQT{ TAG_ORANGE }, colorRQD{ TAG_RED }, colorNCTL{ TAG_RED }, colorCTL{ TAG_GREEN }, colorCFT{ TAG_ORANGE }, colorREM{ TAG_CYAN }, colorNRM{ TAG_GREY };
double distanceCTL = 40;
double distanceCTLBlink = 40;
double speedCTL = 40;
double distanceCFT = 2;
string reminderSymbol = "@";
string reminderSymbolOff = "_";
string lastError, lastDebug, lastDebug2, lastAmend, nextAmend = "";

map<string, requestBox> request;  // type, time, lastSeen, byMe
map<string, landingBox> landing;  // byMe, eraseTime

internetConnection interNet(MY_PLUGIN_VERSION);

// Euroscope functions

CVCHPlugin::CVCHPlugin() : EuroScopePlugIn::CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE, MY_PLUGIN_NAME.c_str(), MY_PLUGIN_VERSION.c_str(), MY_PLUGIN_DEVELOPER.c_str(), MY_PLUGIN_COPYRIGHT.c_str()) {

	RegisterTagItemType("Pending request", TAG_ITEM_VCH_REQ);
	RegisterTagItemType("Pending request (short)", TAG_ITEM_VCH_SRQ);
	RegisterTagItemType("Pending request time", TAG_ITEM_VCH_RQT);
	RegisterTagItemFunction("Request menu", TAG_FUNC_VCH_RMEN);

	RegisterTagItemType("Active Hold Short", TAG_ITEM_VCH_HOS);
	RegisterTagItemFunction("Hold Short menu", TAG_FUNC_VCH_HMEN);

	RegisterTagItemType("Pending Landing Clearence", TAG_ITEM_VCH_CTL);
	RegisterTagItemType("CTL flag only when active", TAG_ITEM_VCH_SCL);
	RegisterTagItemType("Cleared For Takeoff", TAG_ITEM_VCH_CFT);
	RegisterTagItemFunction("Switch Landing Clearence", TAG_FUNC_VCH_CTL);

	RegisterTagItemType("Reminder", TAG_ITEM_VCH_REM);
	RegisterTagItemType("Reminder only when active", TAG_ITEM_VCH_SRM);
	RegisterTagItemFunction("Switch Reminder", TAG_FUNC_VCH_REM);

	timerOn = clock();

	const char* settingLoad;
	if ((settingLoad = GetDataFromSettings("vch_blink")) != NULL) {
		if (atoi(settingLoad) == false) {
			NOBLINK = true;
		}
		else
			NOBLINK = false;
	}
	if ((settingLoad = GetDataFromSettings("vch_rqc")) != NULL) {
		if (atoi(settingLoad) == false)
			NORC = true;
		else
			NORC = false;
	}
	if ((settingLoad = GetDataFromSettings("vch_rqp")) != NULL) {
		if (atoi(settingLoad) == false)
			NORP = true;
		else
			NORP = false;
	}
	if ((settingLoad = GetDataFromSettings("vch_rqs")) != NULL) {
		if (atoi(settingLoad) == false)
			NORS = true;
		else
			NORS = false;
	}
	if ((settingLoad = GetDataFromSettings("vch_rqt")) != NULL) {
		if (atoi(settingLoad) == false)
			NORT = true;
		else
			NORT = false;
	}
	if ((settingLoad = GetDataFromSettings("vch_rqd")) != NULL) {
		if (atoi(settingLoad) == false)
			NORD = true;
		else
			NORD = false;
	}
	if ((settingLoad = GetDataFromSettings("vch_ctl")) != NULL) {
		if (atof(settingLoad) != 0)
			distanceCTL = atof(settingLoad);
	}
	if ((settingLoad = GetDataFromSettings("vch_ctlblinkdist")) != NULL) {
		if (atof(settingLoad) != 0)
			distanceCTLBlink = atof(settingLoad);
	}
	if ((settingLoad = GetDataFromSettings("vch_cft")) != NULL) {
		if (atof(settingLoad) != 0)
			distanceCFT = atof(settingLoad);
	}
	if ((settingLoad = GetDataFromSettings("vch_rem")) != NULL) {
		if (strcmp(settingLoad, "") != 0)
			reminderSymbol = settingLoad;
	}
	if ((settingLoad = GetDataFromSettings("vch_nrm")) != NULL) {
		if (strcmp(settingLoad, "") != 0)
			reminderSymbolOff = settingLoad;
	}
	/*if ((settingLoad = GetDataFromSettings("vch_rto")) != NULL) {
		if (atof(settingLoad) != 0)
			rtimeout = atof(settingLoad);
	}*/
	if ((settingLoad = GetDataFromSettings("vch_c_hos")) != NULL) {
		colorHOS = stringToColor(settingLoad);
		if (colorHOS == RGB(2, 2, 2)) {
			colorHOS = TAG_YELLOW;
		}
	}
	if ((settingLoad = GetDataFromSettings("vch_c_rqc")) != NULL) {
		colorRQC = stringToColor(settingLoad);
		if (colorRQC == RGB(2, 2, 2)) {
			colorRQC = TAG_GREEN;
		}
	}
	if ((settingLoad = GetDataFromSettings("vch_c_rqp")) != NULL) {
		colorRQP = stringToColor(settingLoad);
		if (colorRQP == RGB(2, 2, 2)) {
			colorRQP = TAG_YELLOW;
		}
	}
	if ((settingLoad = GetDataFromSettings("vch_c_rqs")) != NULL) {
		colorRQS = stringToColor(settingLoad);
		if (colorRQS == RGB(2, 2, 2)) {
			colorRQS = TAG_YELLOW;
		}
	}
	if ((settingLoad = GetDataFromSettings("vch_c_rqt")) != NULL) {
		colorRQT = stringToColor(settingLoad);
		if (colorRQT == RGB(2, 2, 2)) {
			colorRQT = TAG_ORANGE;
		}
	}
	if ((settingLoad = GetDataFromSettings("vch_c_rqd")) != NULL) {
		colorRQD = stringToColor(settingLoad);
		if (colorRQD == RGB(2, 2, 2)) {
			colorRQD = TAG_RED;
		}
	}
	if ((settingLoad = GetDataFromSettings("vch_c_ctl")) != NULL) {
		colorCTL = stringToColor(settingLoad);
		if (colorCTL == RGB(2, 2, 2)) {
			colorCTL = TAG_GREEN;
		}
	}
	if ((settingLoad = GetDataFromSettings("vch_c_nlc")) != NULL) {
		colorNCTL = stringToColor(settingLoad);
		if (colorNCTL == RGB(2, 2, 2)) {
			colorNCTL = TAG_RED;
		}
	}
	if ((settingLoad = GetDataFromSettings("vch_c_cft")) != NULL) {
		colorCFT = stringToColor(settingLoad);
		if (colorCFT == RGB(2, 2, 2)) {
			colorCFT = TAG_ORANGE;
		}
	}
	if ((settingLoad = GetDataFromSettings("vch_c_rem")) != NULL) {
		colorREM = stringToColor(settingLoad);
		if (colorREM == RGB(2, 2, 2)) {
			colorREM = TAG_CYAN;
		}
	}
	if ((settingLoad = GetDataFromSettings("vch_c_nrm")) != NULL) {
		colorNRM = stringToColor(settingLoad);
		if (colorNRM == RGB(2, 2, 2)) {
			colorNRM = TAG_GREY;
		}
	}
	if ((settingLoad = GetDataFromSettings("vch_ctl_t")) != NULL) {
		if (atof(settingLoad) != 0)
			NOCTLT = atof(settingLoad);
	}
	if ((settingLoad = GetDataFromSettings("vch_ctl_s")) != NULL) {
		if (atof(settingLoad) != 0)
			speedCTL = atof(settingLoad);
	}
	if ((settingLoad = GetDataFromSettings("vch_ten_rqc")) != NULL) {
		if (atof(settingLoad) != 0)
			CTEN = atof(settingLoad);
	}
	if ((settingLoad = GetDataFromSettings("vch_ten_rqs")) != NULL) {
		if (atof(settingLoad) != 0)
			STEN = atof(settingLoad);
	}
	if ((settingLoad = GetDataFromSettings("vch_ten_rqp")) != NULL) {
		if (atof(settingLoad) != 0)
			PTEN = atof(settingLoad);
	}
	if ((settingLoad = GetDataFromSettings("vch_ten_rqt")) != NULL) {
		if (atof(settingLoad) != 0)
			TTEN = atof(settingLoad);
	}
	if ((settingLoad = GetDataFromSettings("vch_ten_rqd")) != NULL) {
		if (atof(settingLoad) != 0)
			DTEN = atof(settingLoad);
	}
	thread sendInternet(&CVCHPlugin::versionCheck, this);
	sendInternet.detach();
	DisplayUserMessage("Message", "VCH", string("Version " + MY_PLUGIN_VERSION + " loaded").c_str(), false, false, false, false, false);

}

CVCHPlugin::~CVCHPlugin()
{
}

void CVCHPlugin::OnGetTagItem(CFlightPlan flightPlan, CRadarTarget RadarTarget, int ItemCode, int TagData, char sItemString[16], int* pColorCode, COLORREF* pRGB, double* pFontSize) {
	if (ItemCode == TAG_ITEM_VCH_REQ) {
		if (flightPlan.IsValid() && hasRequest(flightPlan.GetCallsign())) {
			string status = "-";
			switch (request[flightPlan.GetCallsign()].type) {
			case 'C':
				if (colorRQC != RGB(1, 1, 1)) {
					*pColorCode = TAG_COLOR_RGB_DEFINED;
					*pRGB = getTextColor(TAG_ITEM_VCH_RQC);
				}
				else {
					*pColorCode = TAG_COLOR_DEFAULT;
				}
				break;
			case 'P':
				if (colorRQP != RGB(1, 1, 1)) {
					*pColorCode = TAG_COLOR_RGB_DEFINED;
					*pRGB = getTextColor(TAG_ITEM_VCH_RQP);
				}
				else {
					*pColorCode = TAG_COLOR_DEFAULT;
				}
				break;
			case 'S':
				if (colorRQS != RGB(1, 1, 1)) {
					*pColorCode = TAG_COLOR_RGB_DEFINED;
					*pRGB = getTextColor(TAG_ITEM_VCH_RQS);
				}
				else {
					*pColorCode = TAG_COLOR_DEFAULT;
				}
				break;
			case 'T':
				if (colorRQT != RGB(1, 1, 1)) {
					*pColorCode = TAG_COLOR_RGB_DEFINED;
					*pRGB = getTextColor(TAG_ITEM_VCH_RQT);
				}
				else {
					*pColorCode = TAG_COLOR_DEFAULT;
				}
				break;
			case 'D':
				if (colorRQD != RGB(1, 1, 1)) {
					*pColorCode = TAG_COLOR_RGB_DEFINED;
					*pRGB = getTextColor(TAG_ITEM_VCH_RQD);
				}
				else {
					*pColorCode = TAG_COLOR_DEFAULT;
				}
				break;
			default: *pColorCode = TAG_COLOR_DEFAULT;
			}

			status = "R" + getSequence(flightPlan.GetCallsign(), request[flightPlan.GetCallsign()].type) + request[flightPlan.GetCallsign()].type;
			strcpy_s(sItemString, 16, status.c_str());
		}
		else {
			*pColorCode = TAG_COLOR_DEFAULT;
			strcpy_s(sItemString, 16, "-");
		}
	}

	if (ItemCode == TAG_ITEM_VCH_SRQ) {
		if (flightPlan.IsValid() && hasRequest(flightPlan.GetCallsign())) {
			string status = "-";
			switch (request[flightPlan.GetCallsign()].type) {
			case 'C':
				if (colorRQC != RGB(1, 1, 1)) {
					*pColorCode = TAG_COLOR_RGB_DEFINED;
					*pRGB = getTextColor(TAG_ITEM_VCH_RQC);
				}
				else {
					*pColorCode = TAG_COLOR_DEFAULT;
				}
				break;
			case 'P':
				if (colorRQP != RGB(1, 1, 1)) {
					*pColorCode = TAG_COLOR_RGB_DEFINED;
					*pRGB = getTextColor(TAG_ITEM_VCH_RQP);
				}
				else {
					*pColorCode = TAG_COLOR_DEFAULT;
				}
				break;
			case 'S':
				if (colorRQS != RGB(1, 1, 1)) {
					*pColorCode = TAG_COLOR_RGB_DEFINED;
					*pRGB = getTextColor(TAG_ITEM_VCH_RQS);
				}
				else {
					*pColorCode = TAG_COLOR_DEFAULT;
				}
				break;
			case 'T':
				if (colorRQT != RGB(1, 1, 1)) {
					*pColorCode = TAG_COLOR_RGB_DEFINED;
					*pRGB = getTextColor(TAG_ITEM_VCH_RQT);
				}
				else {
					*pColorCode = TAG_COLOR_DEFAULT;
				}
				break;
			case 'D':
				if (colorRQD != RGB(1, 1, 1)) {
					*pColorCode = TAG_COLOR_RGB_DEFINED;
					*pRGB = getTextColor(TAG_ITEM_VCH_RQD);
				}
				else {
					*pColorCode = TAG_COLOR_DEFAULT;
				}
				break;
			default: *pColorCode = TAG_COLOR_DEFAULT;
			}

			status = request[flightPlan.GetCallsign()].type + getSequence(flightPlan.GetCallsign(), request[flightPlan.GetCallsign()].type);
			strcpy_s(sItemString, 16, status.c_str());
		}
		else {
			*pColorCode = TAG_COLOR_DEFAULT;
			strcpy_s(sItemString, 16, "-");
		}
	}

	if (ItemCode == TAG_ITEM_VCH_RQT) {
		if (flightPlan.IsValid()) {
			if (getStatus(&flightPlan) != "" && request[flightPlan.GetCallsign()].time != -1) {
				*pColorCode = TAG_COLOR_RGB_DEFINED;
				*pRGB = getTimeColor(request[flightPlan.GetCallsign()].time);
				string rtime = to_string((int)floor((getTime() - request[flightPlan.GetCallsign()].time) / 60));
				rtime += "m";
				strcpy_s(sItemString, 16, rtime.c_str());
			}
			else {
				*pColorCode = TAG_COLOR_DEFAULT;
				strcpy_s(sItemString, 16, "-");
			}
		}
	}

	if (ItemCode == TAG_ITEM_VCH_HOS) {
		if (flightPlan.IsValid()) {
			if (getHoldShort(&flightPlan) != "") {
				if (colorHOS != RGB(1, 1, 1)) {
					*pColorCode = TAG_COLOR_RGB_DEFINED;
					*pRGB = getTextColor(TAG_ITEM_VCH_HOS);
				}
				else {
					*pColorCode = TAG_COLOR_DEFAULT;
				}
				string status{ getHoldShort(&flightPlan) };
				strcpy_s(sItemString, 16, status.c_str());
			}
			else {
				*pColorCode = TAG_COLOR_DEFAULT;
				strcpy_s(sItemString, 16, "-");
			}
		}
	}

	if (ItemCode == TAG_ITEM_VCH_CTL) {
		if (flightPlan.IsValid() && isLanding(flightPlan.GetDistanceToDestination()) && RadarTarget.GetGS() >= speedCTL && getTracking(flightPlan.GetTrackingControllerIsMe())) {

			if (!isClearedToLand(&flightPlan)) {
				if (colorNCTL != RGB(1, 1, 1)) {
					// Enable blinking if aircraft is reaching defined distance to aerodrome
					if (flightPlan.GetDistanceToDestination() <= distanceCTLBlink) {
						*pColorCode = TAG_COLOR_RGB_DEFINED;
						*pRGB = getTextColor(TAG_ITEM_VCH_CTL);
					}
					// Otherwise just show NCTL color
					else {
						*pColorCode = TAG_COLOR_RGB_DEFINED;
						*pRGB = colorNCTL;
					}
				}
				else {
					*pColorCode = TAG_COLOR_DEFAULT;
				}
			}
			else {
				if (colorCTL != RGB(1, 1, 1)) {
					*pColorCode = TAG_COLOR_RGB_DEFINED;
					*pRGB = colorCTL;
				}
				else {
					*pColorCode = TAG_COLOR_DEFAULT;
				}
			}

			strcpy_s(sItemString, 16, "CTL");
		}
	}

	if (ItemCode == TAG_ITEM_VCH_SCL) {
		if (flightPlan.IsValid() && isLanding(flightPlan.GetDistanceToDestination()) && RadarTarget.GetGS() >= speedCTL && getTracking(flightPlan.GetTrackingControllerIsMe())) {

			if (isClearedToLand(&flightPlan)) {
				if (colorCTL != RGB(1, 1, 1)) {
					*pColorCode = TAG_COLOR_RGB_DEFINED;
					*pRGB = colorCTL;
				}
				else {
					*pColorCode = TAG_COLOR_DEFAULT;
				}
				strcpy_s(sItemString, 16, "CTL");
			}

		}
	}

	if (ItemCode == TAG_ITEM_VCH_CFT) {
		if (flightPlan.IsValid() && isDeparting(flightPlan.GetDistanceFromOrigin()) && RadarTarget.GetGS() < 30) {
			if (string(flightPlan.GetGroundState()) == "DEPA") {
				if (colorCFT != RGB(1, 1, 1)) {
					*pColorCode = TAG_COLOR_RGB_DEFINED;
					*pRGB = colorCFT;
				}
				else {
					*pColorCode = TAG_COLOR_DEFAULT;
				}
				strcpy_s(sItemString, 16, "CFT");
			}

		}
	}

	if (ItemCode == TAG_ITEM_VCH_REM) {
		if (flightPlan.IsValid()) {
			if (hasReminder(&flightPlan)) {
				if (colorREM != RGB(1, 1, 1)) {
					*pColorCode = TAG_COLOR_RGB_DEFINED;
					*pRGB = colorREM;
				}
				else {
					*pColorCode = TAG_COLOR_DEFAULT;
				}
				strcpy_s(sItemString, 16, reminderSymbol.c_str());
			}
			else {
				if (colorNRM != RGB(1, 1, 1)) {
					*pColorCode = TAG_COLOR_RGB_DEFINED;
					*pRGB = colorNRM;
				}
				else {
					*pColorCode = TAG_COLOR_DEFAULT;
				}
				strcpy_s(sItemString, 16, reminderSymbolOff.c_str());
			}

		}
	}

	if (ItemCode == TAG_ITEM_VCH_SRM) {
		if (flightPlan.IsValid()) {
			if (hasReminder(&flightPlan)) {
				if (colorREM != RGB(1, 1, 1)) {
					*pColorCode = TAG_COLOR_RGB_DEFINED;
					*pRGB = colorREM;
				}
				else {
					*pColorCode = TAG_COLOR_DEFAULT;
				}
				strcpy_s(sItemString, 16, reminderSymbol.c_str());
			}

		}
	}

	// Automatic handling of requests and CTL flag
	if (flightPlan.IsValid()) {
		
		if (hasRequest(flightPlan.GetCallsign())) {
			request[flightPlan.GetCallsign()].lastSeen = getTime();
		}

		// automatically disabling requests or CTL flag depending on the ground state
		if (hasRequest(flightPlan.GetCallsign()) && request[flightPlan.GetCallsign()].age() > 3 && isAmendable(flightPlan.GetCallsign())) {
			string callsign = flightPlan.GetCallsign();
			bool eraseRequest = false;
			if (flightPlan.GetClearenceFlag() && request[callsign].type == 'C') {
				eraseRequest = true;
				if (request[callsign].eraseTime == 0) {
					displayDebug("Clear C " + callsign);
					request[callsign].eraseTime = getTime();
				}
			}
			
			// Throw out request if ground state has been changed accordingly
			char groundState = checkGroundState(&flightPlan);
			if (groundState == request[callsign].type) {
				eraseRequest = true;
				if (request[callsign].eraseTime == 0) {
					string msg = "Clear ";
					msg += groundState;
					msg += " ";
					msg += callsign;
					displayDebug(msg);
					request[callsign].eraseTime = getTime();
				}				
			}
			
			// Throw out request if aircraft is definetly moving
			if (RadarTarget.GetGS() > 20 && request[callsign].type != 'D') {
				eraseRequest = true;
				if (request[callsign].eraseTime == 0) {
					displayDebug("Clear GS " + callsign);
					request[callsign].eraseTime = getTime();
				}
			} else if (RadarTarget.GetGS() > 50) {
				eraseRequest = true;
				if (request[callsign].eraseTime == 0) {
					displayDebug("Clear Speed > 50 " + callsign);
					request[callsign].eraseTime = getTime();
				}
			}
			
			// Erase request with time buffer, depending if you were creating the request or not
			if (eraseRequest) {
				if (request[callsign].byMe || request[callsign].eraseTime + ControllerMyself().GetFacility() + 2 == getTime()) {
					//setStatus("", &flightPlan);
					request.erase(callsign);
				}			
			}
		}
		else if (hasRequest(flightPlan.GetCallsign()) && !isAmendable(flightPlan.GetCallsign())) {
			request.erase(flightPlan.GetCallsign());
		}

		// Throw out CTL flag when callsign has landed (or crashed or so, anyway he is very slow for whatever reason)
		if (RadarTarget.GetGS() < speedCTL && RadarTarget.GetVerticalSpeed() == 0 && flightPlan.GetDistanceToDestination() <= 3 && isClearedToLand(&flightPlan) && isAmendable(flightPlan.GetCallsign())) {
			if (landing[flightPlan.GetCallsign()].eraseTime == 0) {
				landing[flightPlan.GetCallsign()].eraseTime = getTime();
			}		
			if (landing[flightPlan.GetCallsign()].byMe || landing[flightPlan.GetCallsign()].eraseTime + ControllerMyself().GetFacility() + 2 == getTime()) {
				setClearedToLand(&flightPlan, false);
				string msg = "Clear CTL ";
				msg += flightPlan.GetCallsign();
				displayDebug(msg);
				landing.erase(flightPlan.GetCallsign());
			}	
		}

		// If there is a request, enter it into the internal data storage, else delete it
		/*if (getStatus(&flightPlan) != "") {
			if (!hasRequest(flightPlan.GetCallsign())) {
				createRequest(flightPlan.GetCallsign(), getStatus(&flightPlan));
				string msg = "Created Request: ";
				msg += flightPlan.GetCallsign();
				displayDebug(msg);
			} else {
				modRequest(flightPlan.GetCallsign(), getStatus(&flightPlan));
			}
		} else {
			if (hasRequest(flightPlan.GetCallsign()) && (getTime() - request[flightPlan.GetCallsign()].time > 5)) {
				request.erase(flightPlan.GetCallsign());
				string msg = "Deleted Request: ";
				msg += flightPlan.GetCallsign();
				displayDebug(msg);
			}
		}*/

	}

}

void CVCHPlugin::OnFunctionCall(int FunctionId, const char* sItemString, POINT Pt, RECT Area)
{
	CFlightPlan flightPlan = FlightPlanSelectASEL();
	if (FunctionId == TAG_FUNC_VCH_RMEN) {
		OpenPopupList(Area, "Request menu", 1);
		if (isAmendable(flightPlan.GetCallsign())) {
			if (!NORC) AddPopupListElement("Clearence", "", TAG_FUNC_VCH_CLEARENCE, false, POPUP_ELEMENT_NO_CHECKBOX, flightPlan.GetClearenceFlag());
			if (!NORS) AddPopupListElement("Startup", "", TAG_FUNC_VCH_STARTUP, false, POPUP_ELEMENT_NO_CHECKBOX, checkGroundState(&flightPlan) == 'S');
			if (!NORP) AddPopupListElement("Pushback", "", TAG_FUNC_VCH_PUSHBACK, false, POPUP_ELEMENT_NO_CHECKBOX, checkGroundState(&flightPlan) == 'P');
			if (!NORT) AddPopupListElement("Taxi", "", TAG_FUNC_VCH_TAXI, false, POPUP_ELEMENT_NO_CHECKBOX, checkGroundState(&flightPlan) == 'T');
			if (!NORD) AddPopupListElement("Departure", "", TAG_FUNC_VCH_DEPARTURE, false, POPUP_ELEMENT_NO_CHECKBOX, checkGroundState(&flightPlan) == 'D');
			AddPopupListElement("No request", "", TAG_FUNC_VCH_REQRESET, false, POPUP_ELEMENT_NO_CHECKBOX, !hasRequest(flightPlan.GetCallsign()));
		} else {
			AddPopupListElement("Aircraft tracked by someone else!", "", 0, false, POPUP_ELEMENT_NO_CHECKBOX, true);
		}
	}

	if (FunctionId == TAG_FUNC_VCH_CLEARENCE) {
		if (flightPlan.GetClearenceFlag()) {
			displayError("Aircraft already cleared!");
		} else {
			request[flightPlan.GetCallsign()].byMe = true;
			setStatus("C", &flightPlan);
		}
	}

	if (FunctionId == TAG_FUNC_VCH_STARTUP) {
		if (checkGroundState(&flightPlan) == 'S') {
			displayError("Aircraft already started!");
		} else {
			request[flightPlan.GetCallsign()].byMe = true;
			setStatus("S", &flightPlan);
		}		
	}

	if (FunctionId == TAG_FUNC_VCH_PUSHBACK) {
		if (checkGroundState(&flightPlan) == 'P') {
			displayError("Aircraft already pushed!");
		} else {
			request[flightPlan.GetCallsign()].byMe = true;
			setStatus("P", &flightPlan);
		}
	}

	if (FunctionId == TAG_FUNC_VCH_TAXI) {
		if (checkGroundState(&flightPlan) == 'T') {
			displayError("Aircraft already taxiing!");
		} else {
			request[flightPlan.GetCallsign()].byMe = true;
			setStatus("T", &flightPlan);
		}
	}

	if (FunctionId == TAG_FUNC_VCH_DEPARTURE) {
		if (checkGroundState(&flightPlan) == 'D') {
			displayError("Aircraft already departing!");
		} else {
			request[flightPlan.GetCallsign()].byMe = true;
			setStatus("D", &flightPlan);
		}
	}

	if (FunctionId == TAG_FUNC_VCH_REQRESET) {
		if (hasRequest(flightPlan.GetCallsign()))
			request.erase(flightPlan.GetCallsign());
			//setStatus("", &flightPlan);
		else
			displayError("Aircraft has no request!");
	}

	if (FunctionId == TAG_FUNC_VCH_HMEN) {
		OpenPopupList(Area, "Hold Short menu", 2);
		AddPopupListElement("Add Hold Short", "", TAG_FUNC_VCH_HOSTEXT, false, POPUP_ELEMENT_NO_CHECKBOX, false, true);
		AddPopupListElement("No Hold Short", "", TAG_FUNC_VCH_HOSRESET, false, POPUP_ELEMENT_NO_CHECKBOX, false, true);
	}

	if (FunctionId == TAG_FUNC_VCH_HOS) {
		string comp = "Enter Hold Short";
		if (strcmp(sItemString, comp.c_str()) != 0 && strcmp(sItemString, "") != 0) {
			setHoldShort(sItemString, &flightPlan);
		}
	}

	if (FunctionId == TAG_FUNC_VCH_HOSTEXT) {
		OpenPopupEdit(Area, TAG_FUNC_VCH_HOS, "Enter Hold Short");
	}

	if (FunctionId == TAG_FUNC_VCH_HOSRESET) {
		setHoldShort("", &flightPlan);
	}

	if (FunctionId == TAG_FUNC_VCH_CTL) {
		if (isClearedToLand(&flightPlan)) {
			setClearedToLand(&flightPlan, false);	
		} else {
			setClearedToLand(&flightPlan, true);
		}
		landing[flightPlan.GetCallsign()].byMe = true;
	}

	if (FunctionId == TAG_FUNC_VCH_REM) {
		switchReminder(&flightPlan);
	}

}

bool CVCHPlugin::OnCompileCommand(const char* sCommandLine) {
	if (startsWith(".vch blink", sCommandLine)) {
		string buffer{ sCommandLine };
		if (buffer.length() > 11) {
			buffer.erase(0, 11);
			if (buffer == "on") {
				NOBLINK = false;
			}
			else if (buffer == "off") {
				NOBLINK = true;
			}
			else {
				displayError("Could not process parameter for .vch blink!");
				return true;
			}
		}
		else {
			NOBLINK = !NOBLINK;
		}

		if (!NOBLINK) {
			SaveDataToSettings("vch_blink", "Blinking of requests or hold-shorts", "1");
			displayMessage("Blinking switched on!");
		}
		else {
			SaveDataToSettings("vch_blink", "Blinking of requests or hold-shorts", "0");
			displayMessage("Blinking switched off!");
		}

		return true;
	}

	if (startsWith(".vch option", sCommandLine)) {
		string buffer{ sCommandLine };
		buffer.erase(0, 12);
		string setting{ buffer };
		bool mod = false;
		string mods{ "vch_" };
		if (buffer.length() > 2) {
			setting.erase(3, setting.length());
			buffer.erase(0, 4);
			if (buffer == "on") {
				mod = false;
			}
			else if (buffer == "off") {
				mod = true;
			}
			else {
				displayError("Could not process parameter for .vch option!");
				return true;
			}
			mods += setting;
		}
		else {
			displayError("Could not process parameter for .vch option!");
			return true;
		}
		// To fix: invalid options which are not recognized change settings which don't do anything
		string setMsg{ "Visibility of option: " + setting + " in request drop-down menu" };
		if (setting == "rqc")
			NORC = mod;
		if (setting == "rqp")
			NORP = mod;
		if (setting == "rqs")
			NORS = mod;
		if (setting == "rqt")
			NORT = mod;
		if (setting == "rqd")
			NORD = mod;
		if (!mod) {
			SaveDataToSettings(mods.c_str(), setMsg.c_str(), "1");
			displayMessage("Visibility of: " + setting + " switched on!");
		}
		else {
			SaveDataToSettings(mods.c_str(), setMsg.c_str(), "0");
			displayMessage("Visibility of: " + setting + " switched off!");
		}
		return true;
	}

	if (startsWith(".vch release", sCommandLine)) {
		string buffer{ sCommandLine };
		buffer.erase(0, 13);
		CFlightPlan flightPlan = FlightPlanSelect(buffer.c_str());
		if (flightPlan.IsValid() && flightPlan.GetTrackingControllerIsMe()) {
			flightPlan.EndTracking();
			displayMessage("Ended tracking for: " + buffer);
			/*} else if (buffer == "all") {
				CFlightPlan flightPlanF = FlightPlanSelectFirst();
				flightPlan = flightPlanF;
				check:
				if (flightPlan.GetTrackingControllerIsMe()) {
					flightPlan.EndTracking();
				}
				flightPlan = FlightPlanSelectNext(flightPlan);
				if (flightPlan.GetCallsign() != flightPlanF.GetCallsign()) {
					goto check;
				}
				displayMessage("Ended tracking for all aircraft.");*/
		}
		else {
			displayError("Aircraft " + buffer + " doesn't exist or isn't tracked by me!");
		}

		return true;
	}

	if (startsWith(".vch color", sCommandLine)) {
		string buffer{ sCommandLine };
		buffer.erase(0, 11);
		if (buffer.length() == 13) {
			string colorCode = buffer;
			colorCode.erase(0, 4);
			buffer.erase(3, buffer.length());
			if (buffer == "hos") {
				colorHOS = stringToColor(colorCode);
				if (colorHOS == RGB(2, 2, 2)) {
					colorHOS = TAG_YELLOW;
				}
				SaveDataToSettings("vch_c_hos", "Color of tag item: Hold Short", colorCode.c_str());
			}
			else if (buffer == "rqc") {
				colorRQC = stringToColor(colorCode);
				if (colorRQC == RGB(2, 2, 2)) {
					colorRQC = TAG_GREEN;
				}
				SaveDataToSettings("vch_c_rqc", "Color of tag item: Request Clearence", colorCode.c_str());
			}
			else if (buffer == "rqp") {
				colorRQP = stringToColor(colorCode);
				if (colorRQP == RGB(2, 2, 2)) {
					colorRQP = TAG_YELLOW;
				}
				SaveDataToSettings("vch_c_rqs", "Color of tag item: Request Startup", colorCode.c_str());
			}
			else if (buffer == "rqt") {
				colorRQT = stringToColor(colorCode);
				if (colorRQT == RGB(2, 2, 2)) {
					colorRQT = TAG_ORANGE;
				}
				SaveDataToSettings("vch_c_rqp", "Color of tag item: Request Pushback", colorCode.c_str());
			}
			else if (buffer == "rqs") {
				colorRQS = stringToColor(colorCode);
				if (colorRQS == RGB(2, 2, 2)) {
					colorRQS = TAG_YELLOW;
				}
				SaveDataToSettings("vch_c_rqt", "Color of tag item: Request Taxi", colorCode.c_str());
			}
			else if (buffer == "rqd") {
				colorRQD = stringToColor(colorCode);
				if (colorRQD == RGB(2, 2, 2)) {
					colorRQD = TAG_RED;
				}
				SaveDataToSettings("vch_c_rqd", "Color of tag item: Request Departure", colorCode.c_str());
			}
			else if (buffer == "ctl") {
				colorCTL = stringToColor(colorCode);
				if (colorCTL == RGB(2, 2, 2)) {
					colorCTL = TAG_GREEN;
				}
				SaveDataToSettings("vch_c_ctl", "Color of tag item: Cleared to Land", colorCode.c_str());
			}
			else if (buffer == "nlc") {
				colorNCTL = stringToColor(colorCode);
				if (colorNCTL == RGB(2, 2, 2)) {
					colorNCTL = TAG_RED;
				}
				SaveDataToSettings("vch_c_nlc", "Color of tag item: Not Cleared to Land", colorCode.c_str());
			}
			else if (buffer == "cft") {
				colorCFT = stringToColor(colorCode);
				if (colorCFT == RGB(2, 2, 2)) {
					colorCFT = TAG_ORANGE;
				}
				SaveDataToSettings("vch_c_cft", "Color of tag item: Cleared for Takeoff", colorCode.c_str());
			}
			else if (buffer == "rem") {
				colorREM = stringToColor(colorCode);
				if (colorREM == RGB(2, 2, 2)) {
					colorREM = TAG_CYAN;
				}
				SaveDataToSettings("vch_c_rem", "Color of tag item: Reminder active", colorCode.c_str());
			}
			else if (buffer == "nrm") {
				colorNRM = stringToColor(colorCode);
				if (colorNRM == RGB(2, 2, 2)) {
					colorNRM = TAG_GREY;
				}
				SaveDataToSettings("vch_c_nrm", "Color of tag item: Reminder not active", colorCode.c_str());
			}
			else {
				displayError("Parameter for .vch color invalid!");
			}
		}
		else {
			displayError("Invalid format for: .vch color!");
		}
		return true;
	}

	if (startsWith(".vch ctldist", sCommandLine)) {
		string buffer{ sCommandLine };
		buffer.erase(0, 13);
		distanceCTL = stod(buffer);
		SaveDataToSettings("vch_ctl", "Trigger distance of CTL flag", to_string(distanceCTL).c_str());
		return true;
	}

	if (startsWith(".vch ctlblinkdist", sCommandLine)) {
		string buffer{ sCommandLine };
		buffer.erase(0, 18);
		distanceCTLBlink = stod(buffer);
		SaveDataToSettings("vch_ctlblinkdist", "Trigger distance of CTL blinking", to_string(distanceCTLBlink).c_str());
		return true;
	}

	if (startsWith(".vch ctltrack", sCommandLine)) {
		string buffer{ sCommandLine };
		NOCTLT = !NOCTLT;
		SaveDataToSettings("vch_ctl_t", "Tracking requirement for CTL flag", to_string(NOCTLT).c_str());
		displayMessage("Tracking requirement for CTL flag set to: " + to_string(NOCTLT));
		return true;
	}

	if (startsWith(".vch ctlspeed", sCommandLine)) {
		string buffer{ sCommandLine };
		buffer.erase(0, 14);
		distanceCTL = stod(buffer);
		SaveDataToSettings("vch_ctl_s", "Speed threshold for CTL flag", to_string(speedCTL).c_str());
		return true;
	}

	if (startsWith(".vch cftdist", sCommandLine)) {
		string buffer{ sCommandLine };
		buffer.erase(0, 13);
		distanceCFT = stod(buffer);
		SaveDataToSettings("vch_cft", "Trigger distance of CFT flag", to_string(distanceCFT).c_str());
		return true;
	}

	if (startsWith(".vch reminderon", sCommandLine)) {
		string buffer{ sCommandLine };
		buffer.erase(0, 16);
		if (buffer.size() > 20) {
			buffer.resize(20);
		}
		reminderSymbol = buffer;
		SaveDataToSettings("vch_rem", "Symbol to display as reminder active", reminderSymbol.c_str());
		return true;
	}

	if (startsWith(".vch reminderoff", sCommandLine)) {
		string buffer{ sCommandLine };
		buffer.erase(0, 17);
		if (buffer.size() > 20) {
			buffer.resize(20);
		}
		reminderSymbolOff = buffer;
		SaveDataToSettings("vch_nrm", "Symbol to display as reminder not active", reminderSymbolOff.c_str());
		return true;
	}

	/*if (startsWith(".vch rtimeout", sCommandLine)) {
		string buffer{ sCommandLine };
		buffer.erase(0, 14);
		rtimeout = stod(buffer);
		SaveDataToSettings("vch_rto", "Time that passes until a request is permanently deleted", to_string(rtimeout).c_str());
		return true;
	}*/

	if (startsWith(".vch debug", sCommandLine)) {
		DEBUG = !DEBUG;
		if (DEBUG) {
			displayDebug("Debug Mode activated!");
		} else {
			displayMessage("Debug Mode deactivated!");
		}
		return true;
	}

	if (startsWith(".vch data", sCommandLine)) {
		string buffer{ sCommandLine };
		buffer.erase(0, 10);
		displayDebug(buffer);
		if (hasRequest(buffer)) {
			displayDebug("Type: " + to_string(request[buffer].type));
			displayDebug("Time: " + to_string(request[buffer].time));
			displayDebug("Last: " + to_string(request[buffer].lastSeen));
			displayDebug("ByMe: " + to_string(request[buffer].byMe));
			displayDebug("ErTi: " + to_string(request[buffer].eraseTime));
			displayDebug("Acti: " + to_string(request[buffer].active()));
			displayDebug("Eras: " + to_string(request[buffer].erase()));
			displayDebug("Age : " + to_string(request[buffer].age()));
			displayDebug("LaMe: " + to_string(landing[buffer].byMe));
			displayDebug("LaTi: " + to_string(landing[buffer].eraseTime));
		} else {
			displayDebug("No request found.");
		}
		return true;
	}

	if (startsWith(".vch track", sCommandLine)) {
		string buffer{ sCommandLine };
		buffer.erase(0, 11);
		displayDebug(buffer);
		string msg = "CS:_";
		msg += FlightPlanSelect(buffer.c_str()).GetTrackingControllerCallsign();
		msg += to_string(sizeof(FlightPlanSelect(buffer.c_str()).GetTrackingControllerCallsign()));
		msg += ".";
		displayDebug(msg);
		msg = "ID:_";
		msg += FlightPlanSelect(buffer.c_str()).GetTrackingControllerId();
		msg += to_string(sizeof(FlightPlanSelect(buffer.c_str()).GetTrackingControllerId()));
		msg += ".";
		displayDebug(msg);
		msg = "Amendable: ";
		msg += to_string(isAmendable(buffer));
		displayDebug(msg);
		return true;
	}

	if (startsWith(".vch ctlcrit", sCommandLine)) {
		string buffer{ sCommandLine };
		buffer.erase(0, 13);
		string msg = buffer + " GS: ";
		msg += to_string(RadarTargetSelect(buffer.c_str()).GetGS());
		msg += " - VS: ";
		msg += to_string(RadarTargetSelect(buffer.c_str()).GetVerticalSpeed());
		msg += " - Distance: ";
		msg += to_string(FlightPlanSelect(buffer.c_str()).GetDistanceToDestination());
		displayDebug(msg);
		return true;
	}

	// To-Do: harmonizing setting parameters
	if (startsWith(".vch leadzero", sCommandLine)) {
		string buffer{ sCommandLine };
		buffer.erase(0, 14);
		string setting{ buffer };
		double mod = 0;
		string mods{ "vch_ten_" };
		if (buffer.length() > 2) {
			setting.erase(3, setting.length());
			buffer.erase(0, 4);
			if (setting == "rqc" || setting == "rqs" || setting == "rqp" || setting == "rqt" || setting == "rqd") {
				mods += setting;
			} else {
				displayError("Invalid first parameter for .vch leadzero: " + setting);
				return true;
			}
			if (isNumber(buffer) && (stoi(buffer) != 0) && (stoi(buffer) <= SEQ_TEN_ZERO)) {
				mod = stoi(buffer);
			} else {
				displayError("Invalid second parameter for .vch leadzero: " + buffer);
				return true;
			}
		} else {
			displayError("Could not process first parameter for .vch leadzero!");
			return true;
		}
		// To fix: invalid options which are not recognized change settings which don't do anything
		string setMsg{ "Leading zero of: " + setting + " in sequence numbering" };
		if (setting == "rqc")
			CTEN = mod;
		if (setting == "rqp")
			PTEN = mod;
		if (setting == "rqs")
			STEN = mod;
		if (setting == "rqt")
			TTEN = mod;
		if (setting == "rqd")
			DTEN = mod;
		SaveDataToSettings(mods.c_str(), setMsg.c_str(), to_string(mod).c_str());
		string value = "";
		switch ((int)mod) {
			case SEQ_NO_ZERO: value = "No leading zero"; break;
			case SEQ_LEAD_ZERO: value = "Always leading zero"; break;
			case SEQ_TEN_ZERO: value = "When >=10 leading zero"; break;
			default: displayError("This should not happen, but sure did! Investigate!");
				return true; break;
		}
		displayMessage("Leading zero of: " + setting + " set to: " + value + "!");
		
		return true;
	}

	if (startsWith(".vch stats", sCommandLine)) {
		displayDebug("Numbers of current requests:");
		displayDebug("Clearences: " + to_string(requests[0]));
		displayDebug("Startups:   " + to_string(requests[1]));
		displayDebug("Pushbacks:  " + to_string(requests[2]));
		displayDebug("Taxis:      " + to_string(requests[3]));
		displayDebug("Departures: " + to_string(requests[4]));
		return true;
	}

	return false;
}

void CVCHPlugin::OnTimer(int Counter)
{
	FLASH = false;
	if (((clock() - timerOn) / CLOCKS_PER_SEC) > 4) {
		FLASH = true;
		timerOn = clock();
		cleanRequests();
	}
	requests[0] = countRequests(&request, 'C');
	requests[1] = countRequests(&request, 'S');
	requests[2] = countRequests(&request, 'P');
	requests[3] = countRequests(&request, 'T');
	requests[4] = countRequests(&request, 'D');
}

// Plugin functions

// Request section

void CVCHPlugin::createRequest(string callsign, string requestString) {
	string buf = requestString;
	buf.resize(1);
	requestString.erase(requestString.begin(), requestString.begin() + 1);
	//request[callsign] = { charThis(buf), stoi(requestString), (int)getTime() };
	request[callsign].type = charThis(buf);
	request[callsign].byMe = true;
	request[callsign].time = stoi(requestString);
	request[callsign].lastSeen = (int)getTime();
}

void CVCHPlugin::modRequest(string callsign, string requestString) {
	string buf = requestString;
	buf.resize(1);
	if (strcmp(&request[callsign].type, buf.c_str()) != 0) {
		//request.erase(callsign);
		createRequest(callsign, requestString);
	}
}

bool CVCHPlugin::hasRequest(string callsign) {
	auto lookup = request.find(callsign);
	if (lookup != request.end() && request[callsign].time != -1) {
		return true;
	}
	else {
		return false;
	}
}

void CVCHPlugin::setStatus(string status, CFlightPlan* flightPlan) {

	/*string buf = flightPlan->GetFlightPlanData().GetRemarks();

	if (buf.find("/VCH/REQ/") != string::npos && buf.find("/REQ/VCH/") != string::npos) {
		size_t buf1 = buf.find("/VCH/REQ/");
		size_t buf2 = buf.find("/REQ/VCH/");
		while (buf1 > buf2) {
			buf.erase(buf2, 9);
			buf1 = buf.find("/VCH/REQ/");
			buf2 = buf.find("/REQ/VCH/");
		}
		buf.erase(buf1, buf2 - buf1 + 9);
	}

	if (status != "") {
		status += to_string(getTime());
		buf += "/VCH/REQ/" + status + "/REQ/VCH/";
	}

	flightPlan->GetFlightPlanData().SetRemarks(buf.c_str());
	nextAmend = buf;
	antiAmendSpam(flightPlan);

	string msg = "Amended Request: ";
	msg += flightPlan->GetCallsign();
	displayDebug(msg);*/

	if (hasRequest(flightPlan->GetCallsign())) {
		request.erase(flightPlan->GetCallsign());
	}

	status += to_string(getTime());

	createRequest(flightPlan->GetCallsign(), status);

	//flightPlan->GetControllerAssignedData().SetFlightStripAnnotation(TAG_STRIP_ANNO_REQ, status.c_str());
}

char CVCHPlugin::checkGroundState(CFlightPlan* flightPlan) {
	string groundState = string(flightPlan->GetGroundState());

	if (groundState == "STUP") {
		return 'S';
	}
	if (groundState == "PUSH") {
		return 'P';
	}
	if (groundState == "TAXI") {
		return 'T';
	}
	if (groundState == "DEPA") {
		return 'D';
	}

	return 'X';
}

// Get request from FlightPlan Remarks
string CVCHPlugin::getStatus(CFlightPlan* flightPlan) {
	/*string buf = flightPlan->GetFlightPlanData().GetRemarks();
	if (buf.find("/VCH/REQ/") != string::npos && buf.find("/REQ/VCH/") != string::npos) {
		size_t buf1 = buf.find("/VCH/REQ/");
		size_t buf2 = buf.find("/REQ/VCH/");
		if (buf1 > buf2) {
			string errorMsg = "DATA ERROR: ";
			errorMsg += flightPlan->GetCallsign();
			errorMsg += " ";
			errorMsg += buf;
			displayError(errorMsg);
			return "ERROR";
		}
		buf.erase(0, buf.find("/VCH/REQ/") + 9);
		buf.erase(buf.find("/REQ/VCH/"), buf.size());

		return buf;

	}*/

	if (hasRequest(flightPlan->GetCallsign())) {
		return request[flightPlan->GetCallsign()].type + to_string(request[flightPlan->GetCallsign()].time);
	}

	return "";
}

string CVCHPlugin::getSequence(string callsign, char type) {
	auto& sequence = request[callsign];
	// Depending on setting: leading 0 or not
	double tenSetting = 0;
	int requestType = 0;
	bool leadZero = false;
	int seqNumber = count_if(request.begin(), request.end(), [&sequence, type](auto& requestData) { return requestData.second.active() && requestData.second.type == type && requestData.second.time < sequence.time; }) + 1;
	switch (type) {
		case 'C': tenSetting = CTEN; requestType = 0; break;
		case 'S': tenSetting = STEN; requestType = 1; break;
		case 'P': tenSetting = PTEN; requestType = 2; break;
		case 'T': tenSetting = TTEN; requestType = 3; break;
		case 'D': tenSetting = DTEN; requestType = 4; break;
		default: displayError("Something went wrong trying to determine the type of the sequence!");
	}
	if (tenSetting == SEQ_LEAD_ZERO)
		leadZero = true;
	else if (tenSetting == SEQ_TEN_ZERO) {
		if (requests[requestType] >= 10)
			leadZero = true;
	}
	if (!leadZero || seqNumber >= 10) {
		return to_string(seqNumber);
	} else {
		return "0" + to_string(seqNumber);
	}
}

void CVCHPlugin::cleanRequests() {
	erase_if(request, [](auto& requestData) { return requestData.second.erase(); });
}

// Finds out, what colour a specific time is

COLORREF CVCHPlugin::getTimeColor(time_t time) {
	if (time == NULL) {
		return TAG_GREY;
	}
	else {
		time = getTime() - time;
		if (time > 0 && time < 120) {
			return TAG_GREY;
		}
		else if (time >= 120 && time < 300) {
			return TAG_YELLOW;
		}
		else if (time >= 300) {
			return TAG_RED;
		}
		return TAG_GREY;
	}
}

COLORREF CVCHPlugin::getTextColor(int tagItem) {
	COLORREF colorRef = RGB(0, 0, 0);
	switch (tagItem) {
	case TAG_ITEM_VCH_HOS: colorRef = colorHOS;	break;
	case TAG_ITEM_VCH_RQC: colorRef = colorRQC; break;
	case TAG_ITEM_VCH_RQP: colorRef = colorRQP; break;
	case TAG_ITEM_VCH_RQS: colorRef = colorRQS; break;
	case TAG_ITEM_VCH_RQT: colorRef = colorRQT; break;
	case TAG_ITEM_VCH_RQD: colorRef = colorRQD; break;
	case TAG_ITEM_VCH_CTL: colorRef = colorNCTL; break;
	default: break;
	}
	if (FLASH || NOBLINK) {
		return colorRef;
	}
	else {
		int rgb[3]{ GetRValue(colorRef), GetGValue(colorRef), GetBValue(colorRef) };
		for (int i = 0; i < 3; i++) {
			if (rgb[i] <= 130) {
				rgb[i] = 130;
			}
			else {
				rgb[i] = 220;
			}
		}
		colorRef = RGB(rgb[0], rgb[1], rgb[2]);
		return colorRef;
	}

}

// Hold-short section

void CVCHPlugin::setHoldShort(string holdShort, CFlightPlan* flightPlan) {
	/*string buf = flightPlan->GetFlightPlanData().GetRemarks();
	if (holdShort.size() > 5) {
		holdShort.resize(5);
	}
	holdShort = capStr(holdShort);

	if (buf.find("/VCH/HOS/") != string::npos && buf.find("/HOS/VCH/") != string::npos) {
		size_t buf1 = buf.find("/VCH/HOS/");
		size_t buf2 = buf.find("/HOS/VCH/");
		while (buf1 > buf2) {
			buf.erase(buf2, 9);
			buf1 = buf.find("/VCH/HOS/");
			buf2 = buf.find("/HOS/VCH/");
		}
		buf.erase(buf1, buf2 - buf1 + 9);
	}
	if (holdShort != "") {
		buf += "/VCH/HOS/" + holdShort + "/HOS/VCH/";
	}

	flightPlan->GetFlightPlanData().SetRemarks(buf.c_str());
	nextAmend = buf;
	antiAmendSpam(flightPlan);

	string msg = "Amended HoldShort: ";
	msg += flightPlan->GetCallsign();
	displayDebug(msg);*/

	flightPlan->GetControllerAssignedData().SetFlightStripAnnotation(TAG_STRIP_ANNO_HOS, holdShort.c_str());
}

string CVCHPlugin::getHoldShort(CFlightPlan* flightPlan) {
	/*string buf = flightPlan->GetFlightPlanData().GetRemarks();
	if (buf.find("/VCH/HOS/") != string::npos && buf.find("/HOS/VCH/") != string::npos) {
		size_t buf1 = buf.find("/VCH/HOS/");
		size_t buf2 = buf.find("/HOS/VCH/");
		if (buf1 > buf2) {
			string errorMsg = "DATA ERROR: ";
			errorMsg += flightPlan->GetCallsign();
			errorMsg += " ";
			errorMsg += buf;
			displayError(errorMsg);
			return "ERROR";
		}
		buf.erase(0, buf.find("/VCH/HOS/") + 9);
		buf.erase(buf.find("/HOS/VCH/"), buf.size());
		if (buf.size() > 5) {
			setHoldShort("", flightPlan);
			string errorMsg = "DATA ERROR - TEXT EXCEEDS 5 CHARACTERS: ";
			errorMsg += flightPlan->GetCallsign();
			errorMsg += " ";
			errorMsg += buf;
			displayError(errorMsg);
			return "ERROR";
		}
		return buf;

	}

	return "";*/

	return flightPlan->GetControllerAssignedData().GetFlightStripAnnotation(TAG_STRIP_ANNO_HOS);;
}

// Cleared-to-land section

void CVCHPlugin::setClearedToLand(CFlightPlan* flightPlan, bool state) {

	/*string buf = flightPlan->GetFlightPlanData().GetRemarks();
	if (state) {
		buf += "/VCH/CTL/";
	} else {
		buf.erase(buf.find("/VCH/CTL/"), 10);
	}
	flightPlan->GetFlightPlanData().SetRemarks(buf.c_str());
	nextAmend = buf;
	antiAmendSpam(flightPlan);

	string msg = "Amended ClearedToLand: ";
	msg += flightPlan->GetCallsign();
	displayDebug(msg);*/

	if (isClearedToLand(flightPlan)) {
		flightPlan->GetControllerAssignedData().SetFlightStripAnnotation(TAG_STRIP_ANNO_CTL, "");
	}
	else {
		flightPlan->GetControllerAssignedData().SetFlightStripAnnotation(TAG_STRIP_ANNO_CTL, "CTL");
	}
}

bool CVCHPlugin::isClearedToLand(CFlightPlan* flightPlan) {
	/*string buf = flightPlan->GetFlightPlanData().GetRemarks();
	if (buf.find("/VCH/CTL/") != string::npos) {
		landing[flightPlan->GetCallsign()].exists = true;
		return true;
	}
	else {
		return false;
	}*/

	if (strcmp(flightPlan->GetControllerAssignedData().GetFlightStripAnnotation(TAG_STRIP_ANNO_CTL), "CTL") == 0) {
		return true;
	}
	else {
		return false;
	}
}

// Reminder section

void CVCHPlugin::switchReminder(CFlightPlan* flightPlan) {
	if (hasReminder(flightPlan)) {
		flightPlan->GetControllerAssignedData().SetFlightStripAnnotation(TAG_STRIP_ANNO_REM, "");
	}
	else {
		flightPlan->GetControllerAssignedData().SetFlightStripAnnotation(TAG_STRIP_ANNO_REM, "REM");
	}
}

bool CVCHPlugin::hasReminder(CFlightPlan* flightPlan) {
	if (strcmp(flightPlan->GetControllerAssignedData().GetFlightStripAnnotation(TAG_STRIP_ANNO_REM), "REM") == 0) {
		return true;
	}
	else {
		return false;
	}
}

// Common section

// Function that makes sure that no unnecessary/double amends are spammed into the network or amends which straight up won't work because a flight is already tracked
void CVCHPlugin::antiAmendSpam(CFlightPlan* flightPlan) {
	if (strcmp(nextAmend.c_str(), lastAmend.c_str()) == 0 && strcmp(nextAmend.c_str(), flightPlan->GetFlightPlanData().GetRemarks()) == 0) {
		displayDebug("Filtered Amend by lastAmend or nextAmend");
		displayDebug(lastAmend);
		displayDebug(flightPlan->GetFlightPlanData().GetRemarks());
	} else {

		if (isAmendable(flightPlan->GetCallsign())) {
			//flightPlan->GetFlightPlanData().AmendFlightPlan();
			lastAmend = nextAmend;
			displayDebug("lastAmend");
			displayDebug(lastAmend);
			displayDebug(flightPlan->GetFlightPlanData().GetRemarks());
		}
	}
}

void CVCHPlugin::displayMessage(string message) {
	DisplayUserMessage("Message", "VCH", message.c_str(), false, false, false, false, false);
}

void CVCHPlugin::displayError(string message) {
	if (message != lastError) {
		DisplayUserMessage("VCH", "Error", message.c_str(), true, true, false, true, false);
		lastError = message;
	}
}

void CVCHPlugin::displayDebug(string message) {
	if (DEBUG) {
		if (message != lastDebug) {
			DisplayUserMessage("VCH_DEBUG", "DATA", message.c_str(), true, true, false, true, false);
			lastDebug2 = lastDebug;
			lastDebug = message;
			if (lastDebug2 == lastDebug) {
				DisplayUserMessage("VCH_DEBUG", "ERROR", "Duplicate Debug Message!", true, true, false, true, false);
			}
		}

	}
}

bool CVCHPlugin::isLanding(double distance) {
	if (distance <= distanceCTL)
		return true;
	else
		return false;
}

bool CVCHPlugin::isDeparting(double distance) {
	if (distance <= distanceCFT)
		return true;
	else
		return false;
}

/*
To be figured out how to get airport elevation data without providing extra data
int CVCHPlugin::getAltitudeAboveAirport(CFlightPlan flightPlan) {
	string airport = flightPlan.GetFlightPlanData().GetDestination();

}*/

string CVCHPlugin::getAirport() {
	string airport = ControllerMyself().GetCallsign();
	if (ControllerMyself().IsController() && GetConnectionType() != CONNECTION_TYPE_NO) {
		airport.resize(4);
		return airport;
	}
	else {
		return "XXXX";
	}
}

bool CVCHPlugin::getTracking(bool tracking) {
	if (NOCTLT) {
		return true;
	} else {
		return tracking;
	}
}

bool CVCHPlugin::isAmendable(string callsign) {
	if (FlightPlanSelect(callsign.c_str()).GetTrackingControllerIsMe()) {
		return true;
	}
	string controller = FlightPlanSelect(callsign.c_str()).GetTrackingControllerId();
	if (controller.length() == 0) {
		return true;
	}
	return false;
}

int CVCHPlugin::countRequests(map<std::string, requestBox>* cRequest, char requestType) {
	int count = 0;
	for (auto& [key, req] : *cRequest) {
		if (req.type == requestType && req.active()) {
			++count;
		}
	}
	return count;
}

void CVCHPlugin::versionCheck() {
	displayMessage("Checking VCH version...");
	string answer = interNet.getData();
	if (interNet.hasError) {
		displayError("Version checking Error: " + answer);
		return;
	}
	if (answer.find_first_of("ERROR") == 0) {
		displayError("ERROR DATA: " + answer);
		return;
	}
	if (answer == MY_PLUGIN_VERSION) {
		displayMessage("VCH is up to date!");
	}
	else {
		string versionMsg = "New VCH version available: ";
		versionMsg += answer;
		versionMsg += ". Your version is: ";
		versionMsg += MY_PLUGIN_VERSION;
		versionMsg += ".";
		displayError(versionMsg);
	}
}

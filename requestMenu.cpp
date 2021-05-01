#include "stdafx.h"
#include "requestMenu.hpp"


clock_t timerOn;

bool FLASH, sync, NOBLINK = false;
bool NORC, NORS, NORP, NORT, NORD = false;

vector<string> AircraftRequestingClearence;
vector<int> AircraftClearenceSequence;
vector<string> AircraftRequestingPushback;
vector<int> AircraftPushbackSequence;
vector<string> AircraftRequestingStartup;
vector<int> AircraftStartupSequence;
vector<string> AircraftRequestingDeparture;
vector<int> AircraftDepartureSequence;
vector<string> AircraftRequestingTaxi;
vector<int> AircraftTaxiSequence;

// Euroscope functions

CVCHPlugin::CVCHPlugin() : EuroScopePlugIn::CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE, MY_PLUGIN_NAME.c_str(), MY_PLUGIN_VERSION.c_str(), MY_PLUGIN_DEVELOPER.c_str(), MY_PLUGIN_COPYRIGHT.c_str()) {

	RegisterTagItemType("Pending request", TAG_ITEM_VCH_REQ);
	RegisterTagItemType("Pending request time", TAG_ITEM_VCH_RQT);
	RegisterTagItemFunction("Request menu", TAG_FUNC_VCH_RMEN);

	RegisterTagItemType("Active Hold Short", TAG_ITEM_VCH_HOS);
	RegisterTagItemFunction("Hold Short menu", TAG_FUNC_VCH_HMEN);

	timerOn = clock();

	const char* settingLoad;
	if ((settingLoad = GetDataFromSettings("vch_blink")) != NULL) {
		if (atoi(settingLoad) == false) {
			NOBLINK = true;
		}
		else
			NOBLINK = false;
	} 
	if ((settingLoad = GetDataFromSettings("vch_rc")) != NULL) {
		if (atoi(settingLoad) == false)
			NORC = true;
		else
			NORC = false;
	}
	if ((settingLoad = GetDataFromSettings("vch_rp")) != NULL) {
		if (atoi(settingLoad) == false)
			NORP = true;
		else
			NORP = false;
	}
	if ((settingLoad = GetDataFromSettings("vch_rs")) != NULL) {
		if (atoi(settingLoad) == false)
			NORS = true;
		else
			NORS = false;
	}
	if ((settingLoad = GetDataFromSettings("vch_rt")) != NULL) {
		if (atoi(settingLoad) == false)
			NORT = true;
		else
			NORT = false;
	}
	if ((settingLoad = GetDataFromSettings("vch_rd")) != NULL) {
		if (atoi(settingLoad) == false)
			NORD = true;
		else
			NORD = false;
	}

	DisplayUserMessage("Message", "VCH", string("Version " + MY_PLUGIN_VERSION + " loaded").c_str(), false, false, false, false, false);
}

CVCHPlugin::~CVCHPlugin()
{
}

void CVCHPlugin::OnGetTagItem(CFlightPlan flightPlan, CRadarTarget RadarTarget, int ItemCode, int TagData, char sItemString[16], int * pColorCode, COLORREF * pRGB, double * pFontSize) {
	if (ItemCode == TAG_ITEM_VCH_REQ) {
		if (flightPlan.IsValid()) {
			string status = "-";
			if (findInSVector(&AircraftRequestingClearence, flightPlan.GetCallsign())) {
				*pColorCode = TAG_COLOR_RGB_DEFINED;
				if (FLASH || NOBLINK)
					*pRGB = TAG_GREEN;
				else
					*pRGB = TAG_LIGHTGREEN;
				status = "R" + to_string(findSequence(&AircraftRequestingClearence, flightPlan.GetCallsign()) + 1) + "C";				
				strcpy_s(sItemString, 16, status.c_str());
			}
			else if (findInSVector(&AircraftRequestingPushback, flightPlan.GetCallsign())) {
				*pColorCode = TAG_COLOR_RGB_DEFINED;
				if (FLASH || NOBLINK)
					*pRGB = TAG_YELLOW;
				else
					*pRGB = TAG_LIGHTYELLOW;
				status = "R" + to_string(findSequence(&AircraftRequestingPushback, flightPlan.GetCallsign()) + 1) + "P";
				strcpy_s(sItemString, 16, status.c_str());
			}
			else if (findInSVector(&AircraftRequestingStartup, flightPlan.GetCallsign())) {
				*pColorCode = TAG_COLOR_RGB_DEFINED;
				if (FLASH || NOBLINK)
					*pRGB = TAG_YELLOW;
				else
					*pRGB = TAG_LIGHTYELLOW;
				status = "R" + to_string(findSequence(&AircraftRequestingStartup, flightPlan.GetCallsign()) + 1) + "S";
				strcpy_s(sItemString, 16, status.c_str());
			}
			else if (findInSVector(&AircraftRequestingTaxi, flightPlan.GetCallsign())) {
				*pColorCode = TAG_COLOR_RGB_DEFINED;
				if (FLASH || NOBLINK)
					*pRGB = TAG_ORANGE;
				else
					*pRGB = TAG_LIGHTORANGE;
				status = "R" + to_string(findSequence(&AircraftRequestingTaxi, flightPlan.GetCallsign()) + 1) + "T";
				strcpy_s(sItemString, 16, status.c_str());
			}
			else if (findInSVector(&AircraftRequestingDeparture, flightPlan.GetCallsign())) {
				*pColorCode = TAG_COLOR_RGB_DEFINED;
				if (FLASH || NOBLINK)
					*pRGB = TAG_RED;
				else
					*pRGB = TAG_LIGHTRED;
				status = "R" + to_string(findSequence(&AircraftRequestingDeparture, flightPlan.GetCallsign()) + 1) + "D";
				strcpy_s(sItemString, 16, status.c_str());
			}
			else {
				*pColorCode = TAG_COLOR_RGB_DEFINED;
				*pRGB = TAG_GREY;
				strcpy_s(sItemString, 16, "-");
			}
		}
	}

	if (ItemCode == TAG_ITEM_VCH_RQT) {
		if (flightPlan.IsValid()) {
			if (getStatus(flightPlan) != "") {
				*pColorCode = TAG_COLOR_RGB_DEFINED;
				string time = getStatus(flightPlan);
				time.erase(time.begin(), time.begin() + 1);
				*pRGB = getTimeColour(stoi(time));
				time = to_string((int)floor((getTime() - stoi(time)) / 60));
				time += "m";
				strcpy_s(sItemString, 16, time.c_str());
			} else {
				*pColorCode = TAG_COLOR_RGB_DEFINED;
				*pRGB = TAG_GREY;
				strcpy_s(sItemString, 16, "-");
			}
		}
	}
	
	if (ItemCode == TAG_ITEM_VCH_HOS) {
		if (flightPlan.IsValid()) {
			if (getHoldShort(flightPlan) != "") {
				*pColorCode = TAG_COLOR_RGB_DEFINED;
				if (FLASH || NOBLINK)
					*pRGB = TAG_YELLOW;
				else
					*pRGB = TAG_LIGHTYELLOW;
				string status{ getHoldShort(flightPlan) };
				strcpy_s(sItemString, 16, status.c_str());
			} else {
				*pColorCode = TAG_COLOR_RGB_DEFINED;
				*pRGB = TAG_GREY;
				strcpy_s(sItemString, 16, "-");
			}
		}
	}

	// automatically disabling requests depending on the ground state and sorting the requests
	if (flightPlan.IsValid()) {
		if (flightPlan.GetClearenceFlag() && findInSVector(&AircraftRequestingClearence, flightPlan.GetCallsign())) {
			delStatus(flightPlan);
		}
		string groundState = string(flightPlan.GetGroundState());
		if (groundState == "PUSH" && findInSVector(&AircraftRequestingPushback, flightPlan.GetCallsign())) {
			delStatus(flightPlan);
		}
		if (groundState == "ST-UP" && findInSVector(&AircraftRequestingStartup, flightPlan.GetCallsign())) {
			delStatus(flightPlan);
		}
		if (groundState == "TAXI" && findInSVector(&AircraftRequestingTaxi, flightPlan.GetCallsign())) {
			delStatus(flightPlan);
		}
		if (groundState == "DEPA") {
			delStatus(flightPlan);
		}
		// Throw out the callsign if it is not here anymore
		if (RadarTarget.GetGS() > 60) {
			delStatus(flightPlan);
		}
	}

	if (getStatus(flightPlan) != "") {
		statusToVector(flightPlan, true);
	} else {
		statusToVector(flightPlan, false);
	}

	/*if (sync && flightPlan.IsValid() && flightPlan.GetTrackingControllerIsMe()) {
		pushStrip(flightPlan);
	}*/
}

void CVCHPlugin::OnFunctionCall(int FunctionId, const char * sItemString, POINT Pt, RECT Area)
{
	if (FunctionId == TAG_FUNC_VCH_RMEN) {
		OpenPopupList(Area, "Request menu", 1);
		if (!NORC) AddPopupListElement("Clearence", "", TAG_FUNC_VCH_CLEARENCE, false, POPUP_ELEMENT_NO_CHECKBOX, false);
		if (!NORP) AddPopupListElement("Pushback", "", TAG_FUNC_VCH_PUSHBACK, false, POPUP_ELEMENT_NO_CHECKBOX, false);
		if (!NORS) AddPopupListElement("Startup", "", TAG_FUNC_VCH_STARTUP, false, POPUP_ELEMENT_NO_CHECKBOX, false);
		if (!NORT) AddPopupListElement("Taxi", "", TAG_FUNC_VCH_TAXI, false, POPUP_ELEMENT_NO_CHECKBOX, false);
		if (!NORD) AddPopupListElement("Departure", "", TAG_FUNC_VCH_DEPARTURE, false, POPUP_ELEMENT_NO_CHECKBOX, false);
		AddPopupListElement("No request", "", TAG_FUNC_VCH_REQRESET, false, POPUP_ELEMENT_NO_CHECKBOX, false);
	}

	if (FunctionId == TAG_FUNC_VCH_CLEARENCE) {
		setStatus("C", FlightPlanSelectASEL());
	}

	if (FunctionId == TAG_FUNC_VCH_PUSHBACK) {
		setStatus("P", FlightPlanSelectASEL());
	}

	if (FunctionId == TAG_FUNC_VCH_STARTUP) {
		setStatus("S", FlightPlanSelectASEL());
	}

	if (FunctionId == TAG_FUNC_VCH_TAXI) {
		setStatus("T", FlightPlanSelectASEL());
	}

	if (FunctionId == TAG_FUNC_VCH_DEPARTURE) {
		setStatus("D", FlightPlanSelectASEL());
	}

	if (FunctionId == TAG_FUNC_VCH_REQRESET) {
		delStatus(FlightPlanSelectASEL());
	}

	if (FunctionId == TAG_FUNC_VCH_HMEN) {
		OpenPopupList(Area, "Hold Short menu", 2);
		AddPopupListElement("Add Hold Short", "", TAG_FUNC_VCH_HOSTEXT, false, POPUP_ELEMENT_NO_CHECKBOX, false, true);
		AddPopupListElement("No Hold Short", "", TAG_FUNC_VCH_HOSRESET, false, POPUP_ELEMENT_NO_CHECKBOX, false, true);	
	}

	if (FunctionId == TAG_FUNC_VCH_HOS) {
		string comp = "Enter Hold Short";
		if (strcmp(sItemString, comp.c_str()) != 0 && strcmp(sItemString, "") != 0) {
			setHoldShort(sItemString, FlightPlanSelectASEL());
		}
	}

	if (FunctionId == TAG_FUNC_VCH_HOSTEXT) {
		OpenPopupEdit(Area, TAG_FUNC_VCH_HOS, "Enter Hold Short");	
	}

	if (FunctionId == TAG_FUNC_VCH_HOSRESET) {
		delHoldShort(FlightPlanSelectASEL());
	}
}

bool CVCHPlugin::OnCompileCommand(const char* sCommandLine) {
	if (startsWith(".vch blink", sCommandLine))	{
		string buffer{ sCommandLine };
		if (buffer.length() > 11) {
			buffer.erase(0, 11);
			if (buffer == "on") {
				NOBLINK = false;
			} else if (buffer == "off") {
				NOBLINK = true;
			} else {
				displayError("Could not process parameter for .vch blink!");
				return true;
			}
		} else {
			NOBLINK = !NOBLINK;
		}

		if (!NOBLINK) {
			SaveDataToSettings("vch_blink", "Blinking of requests or hold-shorts", "1");
			displayMessage("Blinking switched on!");
		} else {
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
			setting.erase(2, setting.length());
			buffer.erase(0, 3);
			if (buffer == "on") {
				mod = false;
			} else if (buffer == "off") {
				mod = true;
			} else {
				displayError("Could not process parameter for .vch option!");
				return true;
			}		
			mods += setting;
		} else {
			displayError("Could not process parameter for .vch option!");
			return true;
		}
		string setMsg{ "Visibility of option: " + setting + " in request drop-down menu" };
		if (setting == "rc")
			NORC = mod;
		if (setting == "rp")
			NORP = mod;
		if (setting == "rs")
			NORS = mod;
		if (setting == "rt")
			NORT = mod;
		if (setting == "rd")
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
	return false;
}

void CVCHPlugin::OnTimer(int Counter)
{
	sync = false;
	FLASH = false;
	if (((clock() - timerOn) / CLOCKS_PER_SEC) > 4) {
		FLASH = true;
		timerOn = clock();
		sync = true;
		cleanVectors();
	}

	setSequence(&AircraftRequestingClearence, &AircraftClearenceSequence);
	setSequence(&AircraftRequestingPushback, &AircraftPushbackSequence);
	setSequence(&AircraftRequestingStartup, &AircraftStartupSequence);
	setSequence(&AircraftRequestingTaxi, &AircraftTaxiSequence);
	setSequence(&AircraftRequestingDeparture, &AircraftDepartureSequence);
}

// Plugin functions

// Request section

void CVCHPlugin::statusToVector(CFlightPlan flightPlan, bool add) {
	if (add) {
		string buffer = getStatus(flightPlan);
		string stat = buffer;
		stat.resize(1);
		buffer.erase(buffer.begin(), buffer.begin() + 1);
		int seqBuf = stoi(buffer);
		char switcher = charThis(stat);
		switch (switcher) {
			case 'C':	addToVector(&AircraftRequestingClearence, flightPlan.GetCallsign(), &AircraftClearenceSequence , seqBuf); 
						removeFromVector(&AircraftRequestingPushback, &AircraftPushbackSequence, flightPlan.GetCallsign()); 
						removeFromVector(&AircraftRequestingStartup, &AircraftStartupSequence, flightPlan.GetCallsign());
						removeFromVector(&AircraftRequestingTaxi, &AircraftTaxiSequence, flightPlan.GetCallsign());
						removeFromVector(&AircraftRequestingDeparture, &AircraftDepartureSequence, flightPlan.GetCallsign());
						break;
			case 'P':	addToVector(&AircraftRequestingPushback, flightPlan.GetCallsign(), &AircraftPushbackSequence, seqBuf); 
						removeFromVector(&AircraftRequestingClearence, &AircraftClearenceSequence, flightPlan.GetCallsign());
						removeFromVector(&AircraftRequestingStartup, &AircraftStartupSequence, flightPlan.GetCallsign());
						removeFromVector(&AircraftRequestingTaxi, &AircraftTaxiSequence, flightPlan.GetCallsign());
						removeFromVector(&AircraftRequestingDeparture, &AircraftDepartureSequence, flightPlan.GetCallsign());
						break;
			case 'S':	addToVector(&AircraftRequestingStartup, flightPlan.GetCallsign(), &AircraftStartupSequence, seqBuf); 
						removeFromVector(&AircraftRequestingClearence, &AircraftClearenceSequence, flightPlan.GetCallsign());
						removeFromVector(&AircraftRequestingPushback, &AircraftPushbackSequence, flightPlan.GetCallsign());
						removeFromVector(&AircraftRequestingTaxi, &AircraftTaxiSequence, flightPlan.GetCallsign());
						removeFromVector(&AircraftRequestingDeparture, &AircraftDepartureSequence, flightPlan.GetCallsign());
						break;
			case 'T':	addToVector(&AircraftRequestingTaxi, flightPlan.GetCallsign(), &AircraftTaxiSequence, seqBuf);
						removeFromVector(&AircraftRequestingClearence, &AircraftClearenceSequence, flightPlan.GetCallsign());
						removeFromVector(&AircraftRequestingPushback, &AircraftPushbackSequence, flightPlan.GetCallsign());
						removeFromVector(&AircraftRequestingStartup, &AircraftStartupSequence, flightPlan.GetCallsign());
						removeFromVector(&AircraftRequestingDeparture, &AircraftDepartureSequence, flightPlan.GetCallsign());
						break;
			case 'D':	addToVector(&AircraftRequestingDeparture, flightPlan.GetCallsign(), &AircraftDepartureSequence, seqBuf);
						removeFromVector(&AircraftRequestingClearence, &AircraftClearenceSequence, flightPlan.GetCallsign());
						removeFromVector(&AircraftRequestingPushback, &AircraftPushbackSequence, flightPlan.GetCallsign());
						removeFromVector(&AircraftRequestingStartup, &AircraftStartupSequence, flightPlan.GetCallsign());
						removeFromVector(&AircraftRequestingTaxi, &AircraftTaxiSequence, flightPlan.GetCallsign());
						break;
			default: displayError("Error setting the request"); break;
		}
	} else {
		removeFromVector(&AircraftRequestingClearence, &AircraftClearenceSequence, flightPlan.GetCallsign());
		removeFromVector(&AircraftRequestingPushback, &AircraftPushbackSequence, flightPlan.GetCallsign());
		removeFromVector(&AircraftRequestingStartup, &AircraftStartupSequence, flightPlan.GetCallsign());
		removeFromVector(&AircraftRequestingTaxi, &AircraftTaxiSequence, flightPlan.GetCallsign());
		removeFromVector(&AircraftRequestingDeparture, &AircraftDepartureSequence, flightPlan.GetCallsign());
	}
}

void CVCHPlugin::setStatus(string status, CFlightPlan flightPlan) {
	status += to_string(getTime());
	if (!flightPlan.GetFlightPlanData().IsAmended()) {
		flightPlan.GetFlightPlanData().AmendFlightPlan();
	}
	flightPlan.GetControllerAssignedData().SetFlightStripAnnotation(FSTRIP_ANNOTATION_REQ, status.c_str());
}

string CVCHPlugin::getStatus(CFlightPlan flightPlan) {
	return flightPlan.GetControllerAssignedData().GetFlightStripAnnotation(FSTRIP_ANNOTATION_REQ);
}

// Finds out, what colour a specific time is
COLORREF CVCHPlugin::getTimeColour(int time) {
	if (time == NULL) {
		return TAG_GREY;
	} else {
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

void CVCHPlugin::delStatus(CFlightPlan flightPlan) {
	flightPlan.GetControllerAssignedData().SetFlightStripAnnotation(FSTRIP_ANNOTATION_REQ, "");
}

void CVCHPlugin::setSequence(vector<string>* thisVector, vector<int>* thisSeq) {
	int sorted = 0;
	if (thisVector->size() != thisSeq->size()) {
		displayError("Mismatch between callsign-vector and sequence-vector!");
		displayError(vectorSToString(thisVector));
		displayError(vectorIToString(thisSeq));
		return;
	}
	if (thisVector->size() > 1) {
		while (sorted != thisVector->size() - 1) {
			sorted = 0;
			for (int i = 0; i < thisVector->size() - 1; i++) {
				if (thisSeq->at(i) > thisSeq->at(i + 1)) {
					int iBuf = thisSeq->at(i);
					string sBuf = thisVector->at(i);
					thisSeq->at(i) = thisSeq->at(i + 1);
					thisSeq->at(i + 1) = iBuf;
					thisVector->at(i) = thisVector->at(i + 1);
					thisVector->at(i + 1) = sBuf;
				}
				else {
					sorted++;
				}
			}
		}
	}
}

void CVCHPlugin::cleanVectors() {
	for (string callsign : AircraftRequestingClearence) {
		CFlightPlan flightPlan = FlightPlanSelect(callsign.c_str());
		CRadarTarget radarTarget = RadarTargetSelect(callsign.c_str());
		if (!flightPlan.IsValid() || radarTarget.GetGS() > 80) {
			delStatus(flightPlan);
			removeFromVector(&AircraftRequestingClearence, &AircraftClearenceSequence, flightPlan.GetCallsign());
		}
	}
	for (string callsign : AircraftRequestingPushback) {
		CFlightPlan flightPlan = FlightPlanSelect(callsign.c_str());
		CRadarTarget radarTarget = RadarTargetSelect(callsign.c_str());
		if (!flightPlan.IsValid() || radarTarget.GetGS() > 80) {
			delStatus(flightPlan);
			removeFromVector(&AircraftRequestingPushback, &AircraftPushbackSequence, flightPlan.GetCallsign());
		}
	}
	for (string callsign : AircraftRequestingStartup) {
		CFlightPlan flightPlan = FlightPlanSelect(callsign.c_str());
		CRadarTarget radarTarget = RadarTargetSelect(callsign.c_str());
		if (!flightPlan.IsValid() || radarTarget.GetGS() > 80) {
			delStatus(flightPlan);
			removeFromVector(&AircraftRequestingStartup, &AircraftStartupSequence, flightPlan.GetCallsign());
		}
	}
	for (string callsign : AircraftRequestingStartup) {
		CFlightPlan flightPlan = FlightPlanSelect(callsign.c_str());
		CRadarTarget radarTarget = RadarTargetSelect(callsign.c_str());
		if (!flightPlan.IsValid() || radarTarget.GetGS() > 80) {
			delStatus(flightPlan);
			removeFromVector(&AircraftRequestingStartup, &AircraftStartupSequence, flightPlan.GetCallsign());
		}
	}
	for (string callsign : AircraftRequestingDeparture) {
		CFlightPlan flightPlan = FlightPlanSelect(callsign.c_str());
		CRadarTarget radarTarget = RadarTargetSelect(callsign.c_str());
		if (!flightPlan.IsValid() || radarTarget.GetGS() > 80) {
			delStatus(flightPlan);
			removeFromVector(&AircraftRequestingDeparture, &AircraftDepartureSequence, flightPlan.GetCallsign());
		}
	}
}

// Hold-short section

void CVCHPlugin::setHoldShort(string holdShort, CFlightPlan flightPlan) {
	if (!flightPlan.GetFlightPlanData().IsAmended()) {
		flightPlan.GetFlightPlanData().AmendFlightPlan();
	}
	if (holdShort.size() > 5) {
		holdShort.resize(5);
	}
	holdShort = capStr(holdShort);
	flightPlan.GetControllerAssignedData().SetFlightStripAnnotation(FSTRIP_ANNOTATION_HOS, holdShort.c_str());
}

void CVCHPlugin::delHoldShort(CFlightPlan flightPlan) {
	if (!flightPlan.GetFlightPlanData().IsAmended()) {
		flightPlan.GetFlightPlanData().AmendFlightPlan();
	}
	flightPlan.GetControllerAssignedData().SetFlightStripAnnotation(FSTRIP_ANNOTATION_HOS, "");
}

string CVCHPlugin::getHoldShort(CFlightPlan flightPlan) {
	return flightPlan.GetControllerAssignedData().GetFlightStripAnnotation(FSTRIP_ANNOTATION_HOS);
}

// Common section

void CVCHPlugin::displayMessage(string message) {
	DisplayUserMessage("Message", "VCH", message.c_str(), false, false, false, false, false);
}

void CVCHPlugin::displayError(string message) {
	DisplayUserMessage("VCH", "Error", message.c_str(), true, true, false, true, false);
}

void CVCHPlugin::pushStrip(CFlightPlan flightPlan) {
	flightPlan.PushFlightStrip(ControllerMyself().GetPositionId());
	vector<string> controllers;
	string callsign, buffer = ControllerSelectFirst().GetCallsign();
	buffer.resize(4);
	if (buffer == getAirport()) {
		controllers.push_back(callsign);
	}
	while (!findInSVector(&controllers, ControllerSelectNext(ControllerSelect(callsign.c_str())).GetCallsign())) {
		callsign = ControllerSelectNext(ControllerSelect(callsign.c_str())).GetCallsign();
		controllers.push_back(callsign);
	}
	for (string callsign : controllers) {
		flightPlan.PushFlightStrip(ControllerSelect(callsign.c_str()).GetPositionId());
	}
	controllers.clear();
}

string CVCHPlugin::getAirport() {
	string airport = ControllerMyself().GetCallsign();
	if (ControllerMyself().IsController() && GetConnectionType() != CONNECTION_TYPE_NO) {
		airport.resize(4);
		return airport;
	} else {
		return "XXXX";
	}
}
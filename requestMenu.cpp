#include "stdafx.h"
#include "requestMenu.hpp"

clock_t timerOn;

bool FLASH = false;

int sync = SYNC_NO;

vector<string> AircraftRequestingClearence;
vector<string> AircraftRequestingPushback;
vector<string> AircraftRequestingTaxi;

void CVCHPlugin::displayMessage(string message) {
	DisplayUserMessage("Message", "VCH", message.c_str(), false, false, false, false, false);
}

void CVCHPlugin::displayError(string message) {
	DisplayUserMessage("VCH", "Error", message.c_str(), true, true, false, true, false);
}

int CVCHPlugin::findSequence(vector<string> seqMe, CFlightPlan seqThis) {
	// Get the sequence of a request
	return distance(seqMe.begin(), find(seqMe.begin(), seqMe.end(), seqThis.GetCallsign())) + 1;
}

void CVCHPlugin::setupSyncServer() {
	// Setting up the Server
	//SyncServer syncerServer;
	displayMessage("Server SyncPort created");
}

void CVCHPlugin::setupSyncClient() {
	// Setting up the Client
	//SyncClient syncerClient;

	displayMessage("Client SyncPort created");
}

CVCHPlugin::CVCHPlugin() : EuroScopePlugIn::CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE,
	MY_PLUGIN_NAME.c_str(),
	MY_PLUGIN_VERSION.c_str(),
	MY_PLUGIN_DEVELOPER.c_str(),
	MY_PLUGIN_COPYRIGHT.c_str()) {

	RegisterTagItemType("Pending request", TAG_ITEM_VCH_STS);
	RegisterTagItemFunction("Request menu", TAG_FUNC_VCH_MENU);

	timerOn = clock();

	DisplayUserMessage("Message", "VCH", string("Version " + MY_PLUGIN_VERSION + " loaded").c_str(), false, false, false, false, false);
}

CVCHPlugin::~CVCHPlugin()
{

}

void CVCHPlugin::OnGetTagItem(CFlightPlan FlightPlan, CRadarTarget RadarTarget, int ItemCode, int TagData, char sItemString[16], int * pColorCode, COLORREF * pRGB, double * pFontSize) {
	if (ItemCode == TAG_ITEM_VCH_STS) {
		if (FlightPlan.IsValid()) {
			if (std::find(AircraftRequestingClearence.begin(), AircraftRequestingClearence.end(), FlightPlan.GetCallsign()) != AircraftRequestingClearence.end()) {
				
				*pColorCode = TAG_COLOR_RGB_DEFINED;
				if (FLASH)
					*pRGB = TAG_GREEN;
				else
					*pRGB = TAG_LIGHTGREEN;

				string status{"R" + to_string(findSequence(AircraftRequestingClearence, FlightPlan)) + "C"};

				strcpy_s(sItemString, 16, status.c_str());

			}
			else if (std::find(AircraftRequestingPushback.begin(), AircraftRequestingPushback.end(), FlightPlan.GetCallsign()) != AircraftRequestingPushback.end()) {

				*pColorCode = TAG_COLOR_RGB_DEFINED;
				if (FLASH)
					*pRGB = TAG_YELLOW;
				else
					*pRGB = TAG_LIGHTYELLOW;

				string status{ "R" + to_string(findSequence(AircraftRequestingPushback, FlightPlan)) + "P" };

				strcpy_s(sItemString, 16, status.c_str());
			}
			else if (std::find(AircraftRequestingTaxi.begin(), AircraftRequestingTaxi.end(), FlightPlan.GetCallsign()) != AircraftRequestingTaxi.end()) {

				*pColorCode = TAG_COLOR_RGB_DEFINED;
				if (FLASH)
					*pRGB = TAG_RED;
				else
					*pRGB = TAG_LIGHTRED;

				string status{ "R" + to_string(findSequence(AircraftRequestingTaxi, FlightPlan)) + "T" };

				strcpy_s(sItemString, 16, status.c_str());

			}
			else {
				*pColorCode = TAG_COLOR_RGB_DEFINED;
				*pRGB = TAG_GREY;

				strcpy_s(sItemString, 16, "-");
			}
		}
	}

	// automatically disabling requests depending on the ground state

	if (FlightPlan.IsValid()) {
		if (FlightPlan.GetClearenceFlag()) {
			if (std::find(AircraftRequestingClearence.begin(), AircraftRequestingClearence.end(), FlightPlan.GetCallsign()) != AircraftRequestingClearence.end()) {
				AircraftRequestingClearence.erase(std::remove(AircraftRequestingClearence.begin(), AircraftRequestingClearence.end(), FlightPlan.GetCallsign()), AircraftRequestingClearence.end());
			}
		}
		string groundState = string(FlightPlan.GetGroundState());

		if (groundState == "PUSH") {
			if (std::find(AircraftRequestingPushback.begin(), AircraftRequestingPushback.end(), FlightPlan.GetCallsign()) != AircraftRequestingPushback.end()) {
				AircraftRequestingPushback.erase(std::remove(AircraftRequestingPushback.begin(), AircraftRequestingPushback.end(), FlightPlan.GetCallsign()), AircraftRequestingPushback.end());
			}
		}
		if (groundState == "TAXI" || groundState == "DEPA") {
			if (std::find(AircraftRequestingPushback.begin(), AircraftRequestingPushback.end(), FlightPlan.GetCallsign()) != AircraftRequestingPushback.end()) {
				AircraftRequestingPushback.erase(std::remove(AircraftRequestingPushback.begin(), AircraftRequestingPushback.end(), FlightPlan.GetCallsign()), AircraftRequestingPushback.end());
			}
			if (std::find(AircraftRequestingTaxi.begin(), AircraftRequestingTaxi.end(), FlightPlan.GetCallsign()) != AircraftRequestingTaxi.end()) {
				AircraftRequestingTaxi.erase(std::remove(AircraftRequestingTaxi.begin(), AircraftRequestingTaxi.end(), FlightPlan.GetCallsign()), AircraftRequestingTaxi.end());
			}
		}
	}
}

void CVCHPlugin::OnFunctionCall(int FunctionId, const char * sItemString, POINT Pt, RECT Area)
{
	if (FunctionId == TAG_FUNC_VCH_MENU) {
		OpenPopupList(Area, "Request menu", 1);
		AddPopupListElement("Clearence", "", TAG_FUNC_VCH_CLEARENCE, false, 2, false);
		AddPopupListElement("Pushback", "", TAG_FUNC_VCH_PUSHBACK, false, 2, false);
		AddPopupListElement("Taxi", "", TAG_FUNC_VCH_TAXI, false, 2, false);
		AddPopupListElement("No request", "", TAG_FUNC_VCH_RESET, false, 2, false);
	}

	if (FunctionId == TAG_FUNC_VCH_CLEARENCE) {
		setClearence(true, FlightPlanSelectASEL());
	}

	if (FunctionId == TAG_FUNC_VCH_PUSHBACK) {
		setPushback(true, FlightPlanSelectASEL());
	}

	if (FunctionId == TAG_FUNC_VCH_TAXI) {
		setTaxi(true, FlightPlanSelectASEL());
	}

	if (FunctionId == TAG_FUNC_VCH_RESET) {
		resetRequest(FlightPlanSelectASEL());
	}
}

void CVCHPlugin::setClearence(bool status, CFlightPlan flightPlan) {
	if (status) {
		if (flightPlan.IsValid()) {
			if (find(AircraftRequestingClearence.begin(), AircraftRequestingClearence.end(), flightPlan.GetCallsign()) == AircraftRequestingClearence.end()) {
				AircraftRequestingClearence.push_back(flightPlan.GetCallsign());
			}
			setPushback(false, flightPlan);	
			setTaxi(false, flightPlan);
		} else {
			displayError("Flightplan not valid!");
		}
	} else {	
		if (std::find(AircraftRequestingClearence.begin(), AircraftRequestingClearence.end(), flightPlan.GetCallsign()) != AircraftRequestingClearence.end()) {
			AircraftRequestingClearence.erase(std::remove(AircraftRequestingClearence.begin(), AircraftRequestingClearence.end(), flightPlan.GetCallsign()), AircraftRequestingClearence.end());
		}
	}
}

void CVCHPlugin::setPushback(bool status, CFlightPlan flightPlan) {
	if (status) {
		if (flightPlan.IsValid()) {
			if (find(AircraftRequestingPushback.begin(), AircraftRequestingPushback.end(), flightPlan.GetCallsign()) == AircraftRequestingPushback.end()) {
				AircraftRequestingPushback.push_back(flightPlan.GetCallsign());
			}
			setClearence(false, flightPlan);
			setTaxi(false, flightPlan);
		} else {
			displayError("Flightplan not valid!");
		}		
	} else {
		if (std::find(AircraftRequestingPushback.begin(), AircraftRequestingPushback.end(), flightPlan.GetCallsign()) != AircraftRequestingPushback.end()) {
			AircraftRequestingPushback.erase(std::remove(AircraftRequestingPushback.begin(), AircraftRequestingPushback.end(), flightPlan.GetCallsign()), AircraftRequestingPushback.end());
		}
	}

}

void CVCHPlugin::setTaxi(bool status, CFlightPlan flightPlan) {
	if (status) {
		if (flightPlan.IsValid()) {
			if (find(AircraftRequestingTaxi.begin(), AircraftRequestingTaxi.end(), flightPlan.GetCallsign()) == AircraftRequestingTaxi.end()) {
				AircraftRequestingTaxi.push_back(flightPlan.GetCallsign());
			}
			setClearence(false, flightPlan);
			setTaxi(false, flightPlan);
		} else {
			displayError("Flightplan not valid!");
		}
	} else {
		if (std::find(AircraftRequestingTaxi.begin(), AircraftRequestingTaxi.end(), flightPlan.GetCallsign()) != AircraftRequestingTaxi.end()) {
			AircraftRequestingTaxi.erase(std::remove(AircraftRequestingTaxi.begin(), AircraftRequestingTaxi.end(), flightPlan.GetCallsign()), AircraftRequestingTaxi.end());
		}
	}
}

void CVCHPlugin::resetRequest(CFlightPlan flightPlan) {
	setClearence(false, flightPlan);
	setPushback(false, flightPlan);
	setTaxi(false, flightPlan);
}

void CVCHPlugin::OnTimer(int Counter)
{
	FLASH = false;

	if (((clock() - timerOn) / CLOCKS_PER_SEC) > 5) {
		FLASH = true;
		timerOn = clock();
		if (sync == SYNC_NO && GetConnectionType() == CONNECTION_TYPE_DIRECT && GetConnectionType() != CONNECTION_TYPE_VIA_PROXY) {
			sync = SYNC_SERVER;
			/*syncServerThread = async(launch::async, [this] {
				setupSyncServer();
			});*/
		}
		else if (sync == SYNC_NO && GetConnectionType() == CONNECTION_TYPE_VIA_PROXY) {
			sync = SYNC_CLIENT;
			/*syncClientThread = async(launch::async, [this] {
				setupSyncClient();
			});*/
		}
		else if (GetConnectionType() == CONNECTION_TYPE_NO) {
			sync = SYNC_NO;
		}
		else if (sync == SYNC_SERVER) {
			//senden("Server!");
			
		}
		else if (sync == SYNC_CLIENT) {
			//senden("Client!");
		}
	}

	// throwing out callsigns that aren't there anymore
	for (string callsign: AircraftRequestingClearence) {
		CFlightPlan flightPlan = FlightPlanSelect(callsign.c_str());
		CRadarTarget radarTarget = RadarTargetSelect(callsign.c_str());
		if (!flightPlan.IsValid() || radarTarget.GetGS() > 80) {
			setClearence(false, flightPlan);
		}
	}
	for (string callsign : AircraftRequestingPushback) {
		CFlightPlan flightPlan = FlightPlanSelect(callsign.c_str());
		CRadarTarget radarTarget = RadarTargetSelect(callsign.c_str());
		if (!flightPlan.IsValid() || radarTarget.GetGS() > 80) {
			setPushback(false, flightPlan);
		}
	}
	for (string callsign : AircraftRequestingTaxi) {
		CFlightPlan flightPlan = FlightPlanSelect(callsign.c_str());
		CRadarTarget radarTarget = RadarTargetSelect(callsign.c_str());
		if (!flightPlan.IsValid() || radarTarget.GetGS() > 80) {
			setTaxi(false, flightPlan);
		}
	}

}
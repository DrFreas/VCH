#include "stdafx.h"
#include "requestMenu.hpp"

using namespace std;

clock_t timerOn;

bool FLASH = false;

int sync = SYNC_NO;

vector<string> AircraftRequestingClearence;
vector<string> AircraftRequestingPushback;
vector<string> AircraftRequestingTaxi;

void CVCHPlugin::displayMessage(string message) {
	DisplayUserMessage("Message", "VCH", message.c_str(), false, false, false, false, false);
}

void CVCHPlugin::setupSyncServer() {
	// Setting up the Server
	displayMessage("Server SyncPort created");
}

void CVCHPlugin::setupSyncClient() {
	// Setting up the Client
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

	DisplayUserMessage("Message", "VCH", std::string("Version " + MY_PLUGIN_VERSION + " loaded").c_str(), false, false, false, false, false);
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
					*pRGB = RGB(0, 255, 0);
				else
					*pRGB = RGB(130, 130, 130);

					strcpy_s(sItemString, 16, "R C");
			}
			else if (std::find(AircraftRequestingPushback.begin(), AircraftRequestingPushback.end(), FlightPlan.GetCallsign()) != AircraftRequestingPushback.end()) {
				*pColorCode = TAG_COLOR_RGB_DEFINED;
				if (FLASH)
					*pRGB = RGB(255, 255, 0);
				else
					*pRGB = RGB(130, 130, 130);

				strcpy_s(sItemString, 16, "R P");
			}
			else if (std::find(AircraftRequestingTaxi.begin(), AircraftRequestingTaxi.end(), FlightPlan.GetCallsign()) != AircraftRequestingTaxi.end()) {
				*pColorCode = TAG_COLOR_RGB_DEFINED;
				if (FLASH)
					*pRGB = RGB(255, 0, 0);
				else
					*pRGB = RGB(130, 130, 130);

				strcpy_s(sItemString, 16, "R T");
			}
			else {
				*pColorCode = TAG_COLOR_RGB_DEFINED;
				*pRGB = RGB(130, 130, 130);

				strcpy_s(sItemString, 16, "-");
			}
		}
	}
}

void CVCHPlugin::OnFunctionCall(int FunctionId, const char * sItemString, POINT Pt, RECT Area)
{
	if (FunctionId == TAG_FUNC_VCH_MENU) {
		CFlightPlan FlightPlan = FlightPlanSelectASEL();

		OpenPopupList(Area, "Request menu", 1);
		AddPopupListElement("Clearence", "", TAG_FUNC_VCH_CLEARENCE, false, 2, false);
		AddPopupListElement("Pushback", "", TAG_FUNC_VCH_PUSHBACK, false, 2, false);
		AddPopupListElement("Taxi", "", TAG_FUNC_VCH_TAXI, false, 2, false);
		AddPopupListElement("No request", "", TAG_FUNC_VCH_RESET, false, 2, false);
	}

	if (FunctionId == TAG_FUNC_VCH_CLEARENCE) {
		setClearence();
	}

	if (FunctionId == TAG_FUNC_VCH_PUSHBACK) {
		setPushback();
	}

	if (FunctionId == TAG_FUNC_VCH_TAXI) {
		setTaxi();
	}

	if (FunctionId == TAG_FUNC_VCH_RESET) {
		resetRequest();
	}
}

void CVCHPlugin::setClearence() {
	CFlightPlan FlightPlan = FlightPlanSelectASEL();

	if (FlightPlan.IsValid()) {

		AircraftRequestingClearence.push_back(FlightPlan.GetCallsign());

		if (std::find(AircraftRequestingPushback.begin(), AircraftRequestingPushback.end(), FlightPlan.GetCallsign()) != AircraftRequestingPushback.end()) {
			AircraftRequestingPushback.erase(std::remove(AircraftRequestingPushback.begin(), AircraftRequestingPushback.end(), FlightPlan.GetCallsign()), AircraftRequestingPushback.end());
		}
		if (std::find(AircraftRequestingTaxi.begin(), AircraftRequestingTaxi.end(), FlightPlan.GetCallsign()) != AircraftRequestingTaxi.end()) {
			AircraftRequestingTaxi.erase(std::remove(AircraftRequestingTaxi.begin(), AircraftRequestingTaxi.end(), FlightPlan.GetCallsign()), AircraftRequestingTaxi.end());
		}
	}

}

void CVCHPlugin::setPushback() {
	CFlightPlan FlightPlan = FlightPlanSelectASEL();

	if (FlightPlan.IsValid()) {
		time_t rawtime;
		time(&rawtime);
		std::stringstream newtimestamp;
		newtimestamp << rawtime;

		AircraftRequestingPushback.push_back(FlightPlan.GetCallsign());

		if (std::find(AircraftRequestingClearence.begin(), AircraftRequestingClearence.end(), FlightPlan.GetCallsign()) != AircraftRequestingClearence.end()) {
			AircraftRequestingClearence.erase(std::remove(AircraftRequestingClearence.begin(), AircraftRequestingClearence.end(), FlightPlan.GetCallsign()), AircraftRequestingClearence.end());
		}
		if (std::find(AircraftRequestingTaxi.begin(), AircraftRequestingTaxi.end(), FlightPlan.GetCallsign()) != AircraftRequestingTaxi.end()) {
			AircraftRequestingTaxi.erase(std::remove(AircraftRequestingTaxi.begin(), AircraftRequestingTaxi.end(), FlightPlan.GetCallsign()), AircraftRequestingTaxi.end());
		}
	}

}

void CVCHPlugin::setTaxi() {
	CFlightPlan FlightPlan = FlightPlanSelectASEL();

	if (FlightPlan.IsValid()) {
		time_t rawtime;
		time(&rawtime);
		std::stringstream newtimestamp;
		newtimestamp << rawtime;

		AircraftRequestingTaxi.push_back(FlightPlan.GetCallsign());

		if (std::find(AircraftRequestingClearence.begin(), AircraftRequestingClearence.end(), FlightPlan.GetCallsign()) != AircraftRequestingClearence.end()) {
			AircraftRequestingClearence.erase(std::remove(AircraftRequestingClearence.begin(), AircraftRequestingClearence.end(), FlightPlan.GetCallsign()), AircraftRequestingClearence.end());
		}
		if (std::find(AircraftRequestingPushback.begin(), AircraftRequestingPushback.end(), FlightPlan.GetCallsign()) != AircraftRequestingPushback.end()) {
			AircraftRequestingPushback.erase(std::remove(AircraftRequestingPushback.begin(), AircraftRequestingPushback.end(), FlightPlan.GetCallsign()), AircraftRequestingPushback.end());
		}
	}

}

void CVCHPlugin::resetRequest() {
	CFlightPlan FlightPlan = FlightPlanSelectASEL();

	if (FlightPlan.IsValid()) {
		if (std::find(AircraftRequestingClearence.begin(), AircraftRequestingClearence.end(), FlightPlan.GetCallsign()) != AircraftRequestingClearence.end()) {
			AircraftRequestingClearence.erase(std::remove(AircraftRequestingClearence.begin(), AircraftRequestingClearence.end(), FlightPlan.GetCallsign()), AircraftRequestingClearence.end());
		}
		if (std::find(AircraftRequestingPushback.begin(), AircraftRequestingPushback.end(), FlightPlan.GetCallsign()) != AircraftRequestingPushback.end()) {
			AircraftRequestingPushback.erase(std::remove(AircraftRequestingPushback.begin(), AircraftRequestingPushback.end(), FlightPlan.GetCallsign()), AircraftRequestingPushback.end());
		}
		if (std::find(AircraftRequestingTaxi.begin(), AircraftRequestingTaxi.end(), FlightPlan.GetCallsign()) != AircraftRequestingTaxi.end()) {
			AircraftRequestingTaxi.erase(std::remove(AircraftRequestingTaxi.begin(), AircraftRequestingTaxi.end(), FlightPlan.GetCallsign()), AircraftRequestingTaxi.end());
		}
	}

}

string CVCHPlugin::getTimestamp() {
	// returns the current raw time in string format

	time_t rawtime;
	time(&rawtime);
	std::stringstream newtimestamp;
	newtimestamp << rawtime;

	string buffer = newtimestamp.str();

	return buffer;
}

void CVCHPlugin::OnTimer(int Counter)
{
	FLASH = false;

	if (((clock() - timerOn) / CLOCKS_PER_SEC) > 5) {
		displayMessage(getTimestamp());
		FLASH = true;
		timerOn = clock();
		if (sync == SYNC_NO /*&& GetConnectionType() == CONNECTION_TYPE_DIRECT*/ && GetConnectionType() != CONNECTION_TYPE_VIA_PROXY) {
			sync = SYNC_SERVER;
			syncServerThread = async(launch::async, [this] {
				setupSyncServer();
			});
		}
		else if (sync == SYNC_NO && GetConnectionType() == CONNECTION_TYPE_VIA_PROXY) {
			sync = SYNC_CLIENT;
			syncClientThread = async(launch::async, [this] {
				setupSyncClient();
			});
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

	// automatically disabling requests depending on the ground state

	CFlightPlan FlightPlan = FlightPlanSelectASEL();

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
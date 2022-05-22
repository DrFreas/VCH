#pragma once

#include "stdafx.h"
#include <string>
#include "EuroScopePlugIn.h"
#include <vector>
#include <sstream>
#include <iomanip>

const int TAG_ITEM_VCH_REQ = 540;
const int TAG_ITEM_VCH_RQC = 541;
const int TAG_ITEM_VCH_RQP = 542;
const int TAG_ITEM_VCH_RQS = 543;
const int TAG_ITEM_VCH_RQT = 544;
const int TAG_ITEM_VCH_RQD = 545;
const int TAG_FUNC_VCH_RMEN = 548;

const int TAG_FUNC_VCH_REQRESET = 550;
const int TAG_FUNC_VCH_CLEARENCE = 551;
const int TAG_FUNC_VCH_PUSHBACK = 552;
const int TAG_FUNC_VCH_STARTUP = 553;
const int TAG_FUNC_VCH_TAXI = 554;
const int TAG_FUNC_VCH_DEPARTURE = 555;
const int TAG_FUNC_VCH_SORT = 556;

const int TAG_ITEM_VCH_HOS = 440;
const int TAG_FUNC_VCH_HMEN = 444;

const int TAG_FUNC_VCH_HOS = 445;
const int TAG_FUNC_VCH_HOSTEXT = 446;
const int TAG_FUNC_VCH_HOSRESET = 447;

const int TAG_ITEM_VCH_CTL = 640;
const int TAG_ITEM_VCH_SCL = 641;
const int TAG_ITEM_VCH_CFT = 642;
const int TAG_FUNC_VCH_CTL = 650;

const int TAG_ITEM_VCH_REM = 340;
const int TAG_ITEM_VCH_SRM = 340;
const int TAG_FUNC_VCH_REM = 346;

const int TAG_ITEM_SID_HINT = 700;

const int SYNC_REQUEST = 100;

const int TAG_STRIP_ANNO_REM = 8;

const COLORREF TAG_LIGHTGREEN = RGB(130, 190, 130);
const COLORREF TAG_LIGHTYELLOW = RGB(190, 190, 130);
const COLORREF TAG_LIGHTRED = RGB(255, 130, 130);
const COLORREF TAG_LIGHTORANGE = RGB(200, 160, 130);
const COLORREF TAG_GREEN = RGB(0, 220, 0);
const COLORREF TAG_RED = RGB(255, 0, 0);
const COLORREF TAG_YELLOW = RGB(220, 220, 0);
const COLORREF TAG_ORANGE = RGB(255, 190, 0);
const COLORREF TAG_CYAN = RGB(0, 220, 220);
const COLORREF TAG_BLUE = RGB(0, 0, 220);
const COLORREF TAG_GREY = RGB(130, 130, 130);
const COLORREF TAG_WHITE = RGB(220, 220, 220);


inline static bool startsWith(const char *pre, const char *str)
{
	size_t lenpre = strlen(pre), lenstr = strlen(str);
	return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
};

inline static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
};
inline static std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
};
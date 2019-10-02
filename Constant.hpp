#pragma once

#include "stdafx.h"
#include <string>
#include "EuroScopePlugIn.h"
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;
using namespace EuroScopePlugIn;

const int TAG_ITEM_VCH_STS = 444;
const int TAG_FUNC_VCH_MENU = 544;

const int TAG_FUNC_VCH_RESET = 545;
const int TAG_FUNC_VCH_CLEARENCE = 546;
const int TAG_FUNC_VCH_PUSHBACK = 547;
const int TAG_FUNC_VCH_TAXI = 548;

const int SYNC_REQUEST = 100;

const int SYNC_NO = 0;
const int SYNC_SERVER = 1;
const int SYNC_CLIENT = 2;

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
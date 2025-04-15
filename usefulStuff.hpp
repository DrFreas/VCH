#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

string capStr(string capMe);

char charThis(string yesThis);

time_t getTime();

COLORREF stringToColor(string color);

bool isNumber(const string& s);
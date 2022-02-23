#include "stdafx.h"
#include "usefulStuff.hpp"

string capStr(string capMe) {
	// Get everything in this string into capital letters
	for_each(capMe.begin(), capMe.end(), [&](char& c) {
		c = toupper(c);
	});
	return capMe;
}

char charThis(string yesThis) {
	if (yesThis == "") {
		yesThis = "X";
	}
	char* intoThis = new char[yesThis.size() + 1];
	strcpy_s(intoThis, yesThis.size() + 1, yesThis.c_str());
	return *intoThis;
}

time_t getTime() {
	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

COLORREF stringToColor(string color) {
	int rgb[3]{ 0, 0, 0 };
	if (color.length() == 9) {
		string buf = color;
		for (int i = 0; i < 3; i++) {
			buf.resize(3);
			rgb[i] = stoi(buf);
			color.erase(0, 3);
			buf = color;
		}
	}
	COLORREF colorCode = RGB(rgb[0], rgb[1], rgb[2]);
	return colorCode;
}
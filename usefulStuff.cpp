#include "stdafx.h"
#include "usefulStuff.hpp"

string capStr(string capMe) {
	// Get everything in this string into capital letters
	for_each(capMe.begin(), capMe.end(), [&](char& c) {
		c = toupper(c);
	});
	return capMe;
}

bool findInSVector(vector<string> *findMe, string findThis) {
	if (find(findMe->begin(), findMe->end(), findThis) != findMe->end()) {
		return true;
	} else {
		return false;
	}
}

bool findInIVector(vector<int> *findMe, int findThis) {
	if (find(findMe->begin(), findMe->end(), findThis) != findMe->end()) {
		return true;
	}
	else {
		return false;
	}
}

int findSequence(vector<string> *findMe, string findThis) {
	// Get the position of something in the vector
	if (find(findMe->begin(), findMe->end(), findThis) != findMe->end()) {
		return distance(findMe->begin(), find(findMe->begin(), findMe->end(), findThis));
	} else {
		return -1;
	}
}

string vectorSToString(vector<string> *stringMe) {
	// Get the content of a vector to a string
	string stringIntoThis;
	for (const auto& piece : *stringMe) {
		stringIntoThis += piece;
		stringIntoThis += ";";
	}
	return stringIntoThis;
}

string vectorIToString(vector<int> *stringMe) {
	// Get the content of a vector to a string
	string stringIntoThis;
	for (const auto& piece : *stringMe) {
		stringIntoThis += to_string(piece);
		stringIntoThis += ";";
	}
	return stringIntoThis;
}

void addToVector(vector<string>* modString, string thisString) {
	if (!findInSVector(modString, thisString)) {
		modString->push_back(thisString);
	}
}

void removeFromVector(vector<string>* modString, string thisString) {
	if (findInSVector(modString, thisString)) {
		modString->erase(remove(modString->begin(), modString->end(), thisString), modString->end());
	}
}

void addToVector(vector<string> *modString, string thisString, vector<int>* modInt, int thisInt) {
	if (!findInSVector(modString, thisString)) {
		modString->push_back(thisString);
		modInt->push_back(thisInt);
	}
}

void removeFromVector(vector<string>* modString, vector<int>* modInt, string thisString) {
	if (findInSVector(modString, thisString)) {
		modInt->erase(modInt->begin() + findSequence(modString, thisString));
		modString->erase(remove(modString->begin(), modString->end(), thisString), modString->end());
	}
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
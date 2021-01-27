#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

string capStr(string capMe);

bool findInSVector(vector<string> *findMe, string findThis);

bool findInIVector(vector<int> *findMe, int findThis);

int findSequence(vector<string> *findMe, string findThis);

string vectorSToString(vector<string> *stringMe);

string vectorIToString(vector<int> *stringMe);

void addToVector(vector<string>* modString, string thisString);

void removeFromVector(vector<string>* modString, string thisString);

void addToVector(vector<string>* modString, string thisString, vector<int>* modInt, int thisInt);

void removeFromVector(vector<string>* modString, vector<int>* modInt, string thisString);

char charThis(string yesThis);

time_t getTime();
#pragma once
#include <string>
#include <WinInet.h>

using namespace std;

class internetConnection
{
public:
	internetConnection(string version);
	~internetConnection();

	string getData();

	bool hasError;
	bool isRunning;
	string pluginVersion;

private:
	HINTERNET hOpenHandle, hConnectHandle, hResourceHandle;
	
};
#include "stdafx.h"
#include "internet.hpp"

internetConnection::internetConnection(string version) {
	hasError = false;
	isRunning = false;
	pluginVersion = version;
}

internetConnection::~internetConnection() {
	// Closing all handlers when closing the plugin
	if (hResourceHandle)
		InternetCloseHandle(hResourceHandle);
	if (hConnectHandle)
		InternetCloseHandle(hConnectHandle);
	if (hOpenHandle)
		InternetCloseHandle(hOpenHandle);
}

string internetConnection::getData() {
	string answer;

	DWORD dwFlags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE;

	string AGENT = "VCH/";
	AGENT += pluginVersion;

	hOpenHandle = InternetOpen(AGENT.c_str(), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, dwFlags);
	if (!hOpenHandle) {
		isRunning = false;
		hasError = true;
		return to_string(GetLastError());
	}
	hConnectHandle = InternetConnect(hOpenHandle, "raw.githubusercontent.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, dwFlags, 0);
	if (!hConnectHandle) {
		InternetCloseHandle(hOpenHandle);
		isRunning = false;
		hasError = true;
		return to_string(GetLastError());
	}
	string rtypestr = "GET";
	const char* rtype = rtypestr.c_str();

	hResourceHandle = HttpOpenRequest(hConnectHandle, rtype, "/DrFreas/VCH/master/version", NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);

	string ws_Headers = "Content-Type: application/x-www-form-urlencoded\r\n Cache-Control: no-cache";

	DWORD dwStatus;
	DWORD dwStatusSize = sizeof(dwStatus);
	bool res = HttpSendRequest(hResourceHandle, ws_Headers.c_str(), ws_Headers.size(), NULL, NULL);
	if (res) {
		DWORD dwBytes;
		char tmp;
		while (InternetReadFile(hResourceHandle, &tmp, 1, &dwBytes)) {
			if (dwBytes != 1) break;
			answer += tmp;
		}
		HttpQueryInfo(hResourceHandle, HTTP_QUERY_FLAG_NUMBER | HTTP_QUERY_STATUS_CODE, &dwStatus, &dwStatusSize, NULL);
		switch (dwStatus) {
		case HTTP_STATUS_OK:		
			break;
		case HTTP_STATUS_DENIED:
			answer = "ERROR: Access denied!";
			break;
		default:
			answer = "ERROR: ";
			answer += to_string(dwStatus);
			break;
		}		
	} else {
		answer += "ERROR: Request didn't work!";
	}

	InternetCloseHandle(hResourceHandle);
	InternetCloseHandle(hConnectHandle);
	InternetCloseHandle(hOpenHandle);

	answer.erase(answer.find_last_not_of("\n\r") + 1);

	return answer;

}
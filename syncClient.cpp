#include "stdafx.h"
#include "syncClient.hpp"
/*
SyncClient::SyncClient() {
	
	WSADATA WSAData;
	SOCKADDR_IN addr;

	WSAStartup(MAKEWORD(2, 0), &WSAData);
	server = socket(AF_INET, SOCK_STREAM, 0);

	inet_pton(AF_INET, "127.0.0.1", &(addr.sin_addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(51234);

	connect(server, (SOCKADDR *)&addr, sizeof(addr));

	sendClient("TesT");
}

void SyncClient::connectClient() {
	
}

void SyncClient::sendClient(std::string message) {
	char *buffer = new char[message.length() + 1];
	//strcpy(buffer, message.c_str());
	
	strcpy_s(buffer, sizeof buffer, message.c_str());

	send(server, buffer, sizeof(buffer), 0);

	delete[] buffer;
}

SyncClient::~SyncClient() {
	closesocket(server);
	WSACleanup();
}*/
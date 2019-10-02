#include "stdafx.h"
#include "syncServer.hpp"
/*
using namespace std;

SyncServer::SyncServer() {

	WSADATA WSAData;
	SOCKADDR_IN serverAddr, clientAddr;

	WSAStartup(MAKEWORD(2, 0), &WSAData);
	server = socket(AF_INET, SOCK_STREAM, 0);

	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(51234);

	bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	listen(server, 0);

	char buffer[1024];
	int clientAddrSize = sizeof(clientAddr);
	if ((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
	{
		fstream f;
		f.open("test.dat", ios::out);
		recv(client, buffer, sizeof(buffer), 0);
		f << buffer << endl;
		f.close();
		memset(buffer, 0, sizeof(buffer));

	}
}

SyncServer::~SyncServer() {

	closesocket(client);
}*/
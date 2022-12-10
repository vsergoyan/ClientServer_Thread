#include "UDPServer.h"

UDPServer::UDPServer(unsigned short serverport=8888) : UDPSocket() , mServerPort(serverport)
{
	mServer.sin_family = AF_INET;
	mServer.sin_addr.s_addr = INADDR_ANY;
	mServer.sin_port = htons(mServerPort);

	//Bind
	if (bind(s, (struct sockaddr*)&mServer, sizeof(mServer)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
}

UDPServer::~UDPServer()
{
}

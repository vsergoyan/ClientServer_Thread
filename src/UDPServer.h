#pragma once

#include "UDPSocket.h"

#define SERVER "127.0.0.1"	//ip address of udp server
#define BUFLEN 	1024		//Max length of buffer
#define PORT 8888			//The port on which to listen for incoming data

class UDPServer : public UDPSocket
{
private:
	unsigned short		mServerPort;
	struct sockaddr_in	mServer;
	UDPServer();
public:
	UDPServer(unsigned short serverport);
	virtual ~UDPServer();
};


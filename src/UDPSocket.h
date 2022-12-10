#pragma once
#include <stdio.h>
#ifndef _WIN32
using SOCKET = int;
#define WSAGetLastError() 1
#else
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#endif

#define SERVER "127.0.0.1"	//ip address of udp server
#define BUFLEN 	1024		//Max length of buffer
#define PORT 8888			//The port on which to listen for incoming data

class UDPSocket
{
protected:
	SOCKET s;

public:
	static int WSAInit();

	UDPSocket();
	virtual ~UDPSocket();

	int SendDatagram(char* msg, unsigned int msglen, struct sockaddr* si_dest, unsigned int slen);
	int RecvDatagram(char* buf, unsigned int buflen, struct sockaddr* si_dest, int *slen);
};

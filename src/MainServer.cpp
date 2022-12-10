/*
	UDP Server
*/

#include <stdio.h>
#include "UDPServer.h"

#ifndef _WIN32
using SOCKET = int
#define WSAGetLastError() 1
#else
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#endif

//#define SERVER "127.0.0.1"	//ip address of udp server
#define BUFLEN 	1024		    //Max length of buffer
#define PORT    8888			//The port on which to listen for incoming data

int main(int argc, char* argv[])
{
	struct sockaddr_in si_other;
	unsigned short srvport;
	int slen;
	char buf[BUFLEN];
	char msg[BUFLEN];

	srvport = (1 == argc) ? PORT : atoi(argv[1]);

	UDPServer server(srvport);
	slen = sizeof(si_other);

	//keep listening for data
	while (1)
	{
		printf("Waiting for data...   ");
		fflush(stdout);

		//clear the buffer by filling null, it might have previously received data
		memset(buf, '\0', BUFLEN);

		//try to receive some data, this is a blocking call
		server.RecvDatagram(buf, BUFLEN, (struct sockaddr*)&si_other, &slen);

		//print details of the client/peer and the data received
		printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		printf("Data: %s\n", buf);

		printf("\nAnswer : ");
		gets_s(msg, BUFLEN);
		//now reply the client with the same data

		server.SendDatagram(msg, (int)strlen(msg), (struct sockaddr*)&si_other, slen);

	}

	return 0;
}

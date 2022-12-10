#include "UDPSocket.h"

int UDPSocket::WSAInit()
{
	static unsigned int count = 0;
	if (count++)
		return 0;
#ifdef WIN32
	WSADATA wsa;

	//Initialize winsock
	printf("\nInitializing Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialized.\n");

#endif //WIN32
	return 0;
}

UDPSocket::UDPSocket()
{
	UDPSocket::WSAInit();

	//create socket
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
}

UDPSocket::~UDPSocket()
{
#ifdef _WIN32
	closesocket(s);
	WSACleanup();
#endif //_WIN32

}

int UDPSocket::SendDatagram(char* msg, unsigned int msglen, struct sockaddr* si_dest, unsigned int slen)
{
	//send the message
	if (sendto(s, msg, (int)msglen, 0, si_dest, slen) == SOCKET_ERROR)
	{
		printf("sendto() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	return 0;
}

int UDPSocket::RecvDatagram(char* buf, unsigned int buflen, struct sockaddr * si_dest, int* slen)
{
	if (recvfrom(s, buf, BUFLEN, 0, si_dest, slen) == SOCKET_ERROR)
	{
		printf("recvfrom() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	return 0;

}

#include <winsock2.h>
#include <cstdio>
#include <ws2tcpip.h>
#include <Windows.h>
#pragma comment(lib,"WS2_32")
int main()
{
	WSADATA wsaData;

	//Startup Windows socket library (Winsock)
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)	
	{
		fprintf(stderr, "WSAStartup failed. \n");
		exit(1);
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		//If version is not available, cleanup Winsock. Do this when you are finished using the Winsock library.
		fprintf(stderr, "Version of 2.2 Winsock is not available. \n");
		WSACleanup();
		exit(2);
	}

	//Winsock successfully started!
	printf("Winsock 2.2 available an WSAStartup successful! ");		






	WSACleanup();
}
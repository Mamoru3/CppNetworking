#include "stdfax.h"
#pragma comment(lib,"WS2_32")
int main(int argc, char* argv[])
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
	printf("Winsock 2.2 available and WSAStartup successful! \n");


	 struct addrinfo hints, * res, * p;
	 int status;
	 char ipstr[INET6_ADDRSTRLEN];
	
		 memset(&hints, 0, sizeof hints);
	 hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
	 hints.ai_socktype = SOCK_STREAM;
	
		 if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
		 fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		 return 2;
		
	}
	
		 printf("IP addresses for %s:\n\n", argv[1]);
	
		 for (p = res; p != NULL; p = p->ai_next)
		 {
			 void* addr;
			const char* ipver;

			 // get the pointer to the address itself,
			 // different fields in IPv4 and IPv6:
			 if (p->ai_family == AF_INET) { // IPv4
				 struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
				 addr = &(ipv4->sin_addr);
				 ipver = "IPv4";

			 }
			 else { // IPv6
				 struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)p->ai_addr;
				 addr = &(ipv6->sin6_addr);
				 ipver = "IPv6";

			 }

			 // convert the IP to a string and print it:
			 inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
			 printf(" %s: %s\n", ipver, ipstr);

		 }
	
		 freeaddrinfo(res); // free the linked list
	
		 return 0;
}

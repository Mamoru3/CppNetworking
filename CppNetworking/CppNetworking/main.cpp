#include "stdfax.h"
#pragma comment(lib,"WS2_32")

#define LISTENPORT "3490"		//Port users will connect to 
#define PENDINGCONNECTIONS 10	//How many pending connections can be in the queue

void* get_in_addr(struct sockaddr* sa)
{
	if (sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);

}
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

	int sockfd, new_fd; //listen for new connections on sock_fd, accept new connections on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; //connector's address information
	socklen_t sin_size;
	int yes = 1;
	char s[INET_ADDRSTRLEN];
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; //this uses local (my) ip
	
	if ((rv = getaddrinfo(NULL, LISTENPORT, &hints, &servinfo)) != 0)		//Get address info of local machine passing the previously initialized hints struct, save it in servinfo
	{
		fprintf(stderr, "getaddrinfo %s \n", gai_strerror(rv));
		return 1;
	}

	for (p = servinfo; p != NULL; p = p->ai_next)	//Loop through all servinfo and bind to the first one possible
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			perror("server: socket");
			continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(yes), sizeof(int)) == -1)
		{
			perror("setsockopt");
			exit(1);
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			closesocket(sockfd);
			perror("server: bind");
			continue;
		}
		break;		
	}

	freeaddrinfo(servinfo);

	if (p == NULL)
	{
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, PENDINGCONNECTIONS) == -1)
	{
		perror("listen");
		exit(1);
	}

	//CONTINUE
	return 0;
}

#ifndef BASICSOCK
#define BASICSOCK

#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

using namespace std;

class BasicSocket {
	private:
		// Variables
		int socket_infos;
		int socket_client;
		int c;
		int *socket_new;
		struct sockaddr_in server, client;
		string ReadFile(string fileName);

	public:
		BasicSocket();
		void sockListen();
		int sockAccept();
		int getSocketClient();
		void *socket_handler();
		int makeSocketUnblocked();
		void terminate();
};

#endif

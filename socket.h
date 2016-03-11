#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <unistd.h>

class socket {
	private:
		// Variables
		int socket_infos;
		int socket_client;
		int c;
		int *socket_new;
		struct sockaddr_in server, client;

	public:
		socket();
		void listen();
		int accept();
		int getSocketClient();
		void *socket::socket_handler(void *socket_infos);
};

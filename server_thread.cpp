#include<stdio.h>
#include<string.h> 
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include <iostream>
#include<unistd.h>
#include<pthread.h>
/*
For compilation used g++ server_thread.cpp -std=c++11 -g -pthread -o Server -Wno-write-strings and test nc 127.0.0.1 8081 ! Good Luck !
@author ImoucheG
@date 15/01/2016
@NB IMPORTANT The request is block in a infinity loop. I think that request waiting a connection close. 
*/

//PORT
#define SERVER_PORT 8081
using namespace std;
//Pointer of function handler for the thread socket
void *socket_handler(void *);
/*
@author ImoucheG
@date 15/01/2016
*/
int main()
{
	// Variables
	int socket_infos;
	int socket_client;
	int c;
	int *socket_new;
	struct sockaddr_in server, client;
	//Init description socket with addr, port, ...
	socket_infos = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_infos == -1)
	{
		cout << "Can't read infos socket" << endl;
		return -1;
	}
	cout << "Socket created" << endl;
	//Init the informations of socket in server
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(SERVER_PORT);
	// Mount the server parameters
	if (bind(socket_infos, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		cout << "Error : Bind parameters failed." << endl;
		return -1;
	}
	cout << "Bind success" << endl;
	// Marks the socket referred to by sockfd as a passive socket, 
	//that is, as a socket that will be used to accept incoming connection requests using accept
	//The backlog argument defines the maximum length to which the queue of pending 
	//connections for sockfd may grow. If a connection request arrives when the queue is full, 
	//the client may receive an error with an indication of ECONNREFUSED 
	listen(socket_infos, 10);
	cout << "Waiting connection from client ..." << endl;
	c = sizeof(struct sockaddr_in);
	//Wait the client connection
	while ((socket_client = accept(socket_infos, (struct sockaddr *)&client, (socklen_t*)&c)))
	{
		cout << "Connection accepted" << endl;
		//Create socket for accept multi -client ijn server
		pthread_t tsocket;
		//Alloc memory
		socket_new = (int*)malloc(1);
		*socket_new = socket_client;
		//Create socket with socket_handler function
		if (pthread_create(&tsocket, NULL, socket_handler, (void*)socket_new) < 0)
		{
			cout << "Can't create thread" << endl;
			return -1;
		}
		//Now join the thread , so that we dont terminate before the thread
		pthread_join( tsocket , NULL);
		cout << "Connection handler is assigned" << endl;
	}

	if (socket_client < 0)
	{
		cout << " Server don't accepted client" << endl;
		return -1;
	}
	return 1;
}
/*
@author ImoucheG
@date 15/01/2016
*/
void *socket_handler(void *socket_infos)
{
	//Get client socket
	int socket_client = *(int*)socket_infos;
	//HTTP Response
	char * response = "HTTP/1.1 200 OK\r\n"
						"Date: Fri, 15 Jun 2016 15:01:04 GMT\r\n"
						"Server: C++Socket/1.0\r\n"
						"Last-Modified: Fri, 15 Jun 2016 15:01:04 GMT\r\n"
						"Content-Type: text/html\r\n"
						"Content-Length: 9999\r\n"
						"Accept-Ranges: bytes\r\n"
						"Connection: close\r\n"
						"\r\n"
						"<html><body><h1>Bravo Charlie !! Tu gère !!</h1></body></html>";
	//Send response to client
	send(socket_client, response, strlen(response), 0);
	//Free the socket pointer
	free(socket_infos);
}

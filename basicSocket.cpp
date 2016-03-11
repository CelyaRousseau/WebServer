#include "basicSocket.h"
#include <string.h> 

//PORT
#define SERVER_PORT 8081
using namespace std;

basicSocket::basicSocket() {
	//Init description socket with addr, port, ...
	socket_infos = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_infos == -1)
	{
		cout << "Can't read infos socket" << endl;
		exit(1);
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
		exit(1);
	}
	cout << "Bind success" << endl;
	this->sockListen();
}

void basicSocket::sockListen() {
	// Marks the socket referred to by sockfd as a passive socket, 
	//that is, as a socket that will be used to accept incoming connection requests using accept
	//The backlog argument defines the maximum length to which the queue of pending 
	//connections for sockfd may grow. If a connection request arrives when the queue is full, 
	//the client may receive an error with an indication of ECONNREFUSED 
	listen(socket_infos, 10);
	cout << "Waiting connection from client ..." << endl;
	c = sizeof(struct sockaddr_in);
}

int basicSocket::sockAccept() {
	socket_client = accept(socket_infos, (struct sockaddr *)&client, (socklen_t*)&c);
	return true;
}

int basicSocket::getSocketClient() {
	return socket_client;
}

void *basicSocket::socket_handler(void *socket_infos)
{
	//Get client socket
	int socket_client = *(int*)socket_infos;
	//HTTP Response
	string content =  "HTTP/1.1 200 OK\r\n"
						"Date: Fri, 15 Jun 2016 15:01:04 GMT\r\n"
						"Server: C++Socket/1.0\r\n"
						"Last-Modified: Fri, 15 Jun 2016 15:01:04 GMT\r\n"
						"Content-Type: text/html\r\n"
						"Content-Length: 9999\r\n"
						"Accept-Ranges: bytes\r\n"
						"Connection: close\r\n"
						"\r\n" + *ReadFile("index.html");
	cout << content << endl;
	char * response = (char*) content.c_str();
	//Send response to client
	send(socket_client, response, strlen(response), 0);
	//Free the socket pointer
	free(socket_infos);
}

string *basicSocket::ReadFile(string fileName){
	fstream file;
	file.open(fileName,fstream::in);
	string content;
	if(file){
		cout << "File is open" << endl;
		string line;
		while(getline(file,line)){
			content = content + line;
		}
	}
	else
		cout << "Failed to open file" << endl;
	file.close();
	string * ret = &content;
	return ret;
}

int basicSocket::makeSocketUnblocked() {
  int flags, s;

  flags = fcntl (socket_infos, F_GETFL, 0);
  if (flags == -1)
    {
      perror ("fcntl");
      return -1;
    }

  s = fcntl (socket_infos, F_SETFL, O_NONBLOCK);
  if (s == -1)
    {
      perror ("fcntl");
      return -1;
    }

  return 0;}
#include <iostream>           // std::cout
#include <stdio.h>
#include "BasicSocket.cpp"
#include "servers/Server.cpp"
#include "servers/Thread.cpp"
#include "servers/ThreadP.cpp"
#include "servers/Epoll.cpp"
#include "servers/Fork.cpp"

using namespace std;

int main( int argc, const char* argv[] ) {
	// if server type is not missing
	if(argc > 1) {
		Server *serv;
		Epoll * epoll;
		bool isEpoll = false;
		string type = argv[1];

		if(type == "thread") {
		 	serv = new Thread(); 
		} else if( type == "threadp") {
			serv = new ThreadP();
		} else if (type == "fork") {
			serv = new Fork();
		} else if (type == "epoll") {
			epoll = new Epoll();
			isEpoll = true;
		} else {
			cout << "usage : <epoll:fork:thread:threadp>" << endl;
			return -1;
		}
		if(!isEpoll){
			BasicSocket* mySock = new BasicSocket();
			serv->run(mySock);
		}else
		 epoll->Run();
	} else
		cout << "usage : <epoll:fork:thread:threadp>" << endl;
	return 0;
}

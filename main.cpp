#include <iostream>           // std::cout
#include <stdio.h>
#include "basicSocket.cpp"
#include "servers/Thread.h"
#include "servers/ThreadP.h"
#include "servers/Epoll.h"
#include "servers/Fork.h"

using namespace std;

int main( int argc, const char* argv[] )
{
	// if server type is not missing
	if(argc > 0) {
		// create socket
		basicSocket* mySock = new basicSocket();

		// thread server
		if(argv[0] == "thread") {
			Thread serv;
		}
		// thread pool server
		else if(argv[0] == "threadp") {
			ThreadP serv;
		}
		// epoll server
		else if(argv[0] == "epoll") {
			Epoll serv;
			mySock.makeSocketUnblocked();
		}
		// fork server
		else if(argv[0] == "fork") {
			Fork serv;
		}
		else {
			cout << "usage : <thread:epoll:threadp:fork" << endl;
			return 0;
		}
		serv.run(mySock);
	}
	cout << "usage : <thread:epoll:threadp:fork" << endl;
	return 0;
}

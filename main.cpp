#include <iostream>           // std::cout
#include <stdio.h>
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

		switch(argv[0]) {
			case "thread":
				// thread server
				Thread serv;
				break;
			case "threadp":
				ThreadP serv;
				break;
			case "fork":
				Fork serv;
				break;
			case "epoll":
				Epoll serv;
				mySock->makeSocketUnblocked();
				break;
			default:
				cout << "usage : <thread:epoll:threadp:fork" << endl;
				return -1;
		}
		serv.run(mySock);
	}
	cout << "usage : <thread:epoll:threadp:fork" << endl;
	return 0;
}

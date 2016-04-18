#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
using namespace std;
class Epoll  {
	public :
		int CreateSocketNoBlock();
		int CreateBind();
		int Run();
		string ReadFile(string fileName);
		int MAXEVENTS = 64;
		char  *PORTS = "8081";
		Epoll();
		~Epoll();
		int sfd, s;
		int efd;
		struct epoll_event event;
		struct epoll_event *events;

	};
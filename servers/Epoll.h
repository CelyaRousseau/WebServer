#include "Server.cpp"

class Epoll : public Server {
	int run(BasicSocket * mySock);
	int message();
};
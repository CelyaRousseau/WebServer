#include "Server.cpp"

class ThreadP : public Server {
	int run(BasicSocket * mySock);
	int message();
};
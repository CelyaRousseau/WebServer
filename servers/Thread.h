#include "Server.cpp"

class Thread : public Server {
	int run(BasicSocket * mySock);
	int message();
	static void * runFunction(void * socket);
};

#include "Server.cpp"

class Fork : public Server {
	int run(BasicSocket * mySock);
	int message();
};
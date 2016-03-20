#ifndef BASICSERVER
#define BASICSERVER

#include "../BasicSocket.h"

class Server {
public:
	int run(BasicSocket * mySock);

private:
	BasicSocket * mySock;
protected:
	int message();
	
};

#endif
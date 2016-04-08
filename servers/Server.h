#ifndef BASICSERVER
#define BASICSERVER

#include "../BasicSocket.h"

class Server {
public:
	virtual int run(BasicSocket * mySock);
	virtual void run();

private:
	BasicSocket * mySock;
protected:
	virtual int message();
	
};

#endif

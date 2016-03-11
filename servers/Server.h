#include "../basicSocket.h"

class Server {
public:
	void run(basicSocket * mySock);

private:
	basicSocket * mySock;
	int message();
	
};
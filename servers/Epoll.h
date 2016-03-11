#include "../basicSocket.h"

class Epoll {
public:
	void run(basicSocket * mySock);

private:
	basicSocket * mySock;
	int message();

}
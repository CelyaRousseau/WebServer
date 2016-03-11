#include "../basicSocket.h"

class Fork {
public:
	void run(basicSocket * mySock);

private:
	basicSocket * mySock;
	int message();

}
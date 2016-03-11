#include "../basicSocket.h"

class ThreadP {
public:
	void run(basicSocket * mySock);

private:
	basicSocket * mySock;

}
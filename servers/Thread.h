#include "../basicSocket.h"

class Thread {
public:
	void run(basicSocket * mySock);

private:
	basicSocket * mySock;

}
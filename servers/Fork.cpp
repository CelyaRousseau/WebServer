#include "Fork.h"

void Fork::run(BasicSocket * mySock) {
	if(message()) return 0;

	while (mySock->sockAccept()) {

		if(mySock->getSocketClient() > 0) {
			cout << "Connection accepted" << endl;



		} else cout << "Connection refused";
	}
}

// set return state to 0 when code is implemented
int Epoll::message() {
	cout << "Start fork server" << endl;

	cout << "not yet implemented" << endl;
	return 1;
}
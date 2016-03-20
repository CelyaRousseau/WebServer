#include "Epoll.h"

int Epoll::run(BasicSocket * mySock) {
	if(message()) return 0;

	mySock->makeSocketUnblocked();
	while (mySock->sockAccept()) {

		if(mySock->getSocketClient() > 0) {
			cout << "Connection accepted" << endl;



		} else cout << "Connection refused";
	}
}

// set return state to 0 when code is implemented
int Epoll::message() {
	cout << "Start epoll server" << endl;

	cout << "not yet implemented" << endl;
	cout << flush ;
	return 1;
}
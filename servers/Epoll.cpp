#include "Epoll.h"

void Epoll::run(BasicSocket * mySock) {
	cout << "Start epoll server" << endl;
	while (mySock->sockAccept()) {

		if(mySock->getSocketClient() > 0) {
			cout << "Connection accepted" << endl;



		} else cout << "Connection refused";
	}
}
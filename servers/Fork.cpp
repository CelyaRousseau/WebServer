#include "Fork.h"

void Fork::run(BasicSocket * mySock) {
	cout << "Start fork server" << endl;
	while (mySock->sockAccept()) {

		if(mySock->getSocketClient() > 0) {
			cout << "Connection accepted" << endl;



		} else cout << "Connection refused";
	}
}
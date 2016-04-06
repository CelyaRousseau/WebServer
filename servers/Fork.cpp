#include "Fork.h"
#include<signal.h>
#include <sys/wait.h>

int Fork::run(BasicSocket * mySock) {
	if(message()) return 0;

	while (mySock->sockAccept()) {

		if(mySock->getSocketClient() > 0) {
			cout << "Connection accepted" << endl;

			/* Create child process */
			pid_t pid = fork();
			cout << "pid:" << pid << endl;
			if(pid < 0) {
				cout << "Error with fork" << endl;
		        }
			if (pid == 0) {
         			/* This is the client process */
         			mySock->socket_handler();
				mySock->terminate();
				exit(0);
		        } else {
				int * status;
				pid_t ret = waitpid(-1, status, 0);
				if(ret != 0) kill(ret, 9);
			}
		} else cout << "Connection refused";
	}
}

// set return state to 0 when code is implemented
int Fork::message() {
	cout << "Start fork server" << endl;
	return 0;
	cout << "not yet implemented" << endl;
	return 1;
}

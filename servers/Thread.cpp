#include "Thread.h"

void Thread::run(BasicSocket * mySock) {
	if(message()) return 0;

	while (mySock->sockAccept()) {

		if(mySock->getSocketClient() > 0) {
			cout << "Connection accepted" << endl;

		/* Exemple with pthread*/

		//Create socket for accept multi -client ijn server
		pthread_t tsocket;
		//Alloc memory
		socket_new = (int*)malloc(1);
		*socket_new = socket_client;
		cout << "try to create handler" << endl;
		//Create socket with socket_handler function
		if (pthread_create(&tsocket, NULL, socket_handler, (void*)socket_new) < 0)
		{
			cout << "Can't create thread" << endl;
			return -1;
		}
		else
			cout << "Handler is create";
		//Now join the thread , so that we dont terminate before the thread
		pthread_join( tsocket , NULL);
		cout << "Connection handler is assigned" << endl;
		} else cout << "Connection refused";
	}
}

// set return state to 0 when code is implemented
int Epoll::message() {
	cout << "Start thread server" << endl;

	//cout << "not yet implemented" << endl;
	return 0;
}
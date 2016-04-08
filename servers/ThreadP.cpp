#include "ThreadP.h"

int ThreadP::run(BasicSocket * mySock) {
	if(message()) return 0;
	this->threads = (pthread_t *) malloc(sizeof(pthread_t) * this->MAX_THREADS);
	int i = 0;
	cout << "MAX THREAD = " << this->MAX_THREADS << endl;
	for(i = 0; i <= this->MAX_THREADS; i++){
		pthread_t thread;
		this->threads[i] = thread;
		this->QUEUE[i] = true;
	}
	i = 0;
	while (mySock->sockAccept()) {
		i = 0;
		cout << "Iteration GENERAL = " << i << endl;
		bool freeThread = false;
		if(mySock->getSocketClient() > 0) {
			while(i <= this->MAX_THREADS || !freeThread){
                        	cout << "Iteration check free = " << i << endl;
				if(this->QUEUE[i] == true){
                               		freeThread = true;
                               		break;
                       		}
				i++;
                	}
			if(freeThread){
				cout << "ACCEPT for = " << i << endl;
				this->QUEUE[i] = false;
				cout << "Connection accepted" << endl;
				/* Exemple with pthread */
				//Create socket for accept multi -client ijn serve
				//Alloc memory
				//int * socket_new = (int*)malloc(1);
				//*socket_new = mySock->getSocketClient();
				cout << "try to create handler" << endl;
				//Create socket with socket_handler function
				if (pthread_create(&this->threads[i], NULL, &runFunction, mySock) < 0)
				{
					cout << "Can't create thread" << endl;
					return -1;
				}
				else
					cout << "Handler is create" << endl;
				//Now join the thread , so that we dont terminate before the thread
				pthread_join( this->threads[i] , NULL);
				cout << "Connection handler is assigned" << endl;
			}else
                                cout << "Full stack" << endl;
			} else
				cout << "Connection refused" << endl;
			mySock->terminate();
			this->QUEUE[i] = true;
		}
}

// set return state to 0 when code is implemented
int ThreadP::message() {
	cout << "Start thread server" << endl;
	//cout << "not yet implemented" << endl;
	return 0;
}

void * ThreadP::runFunction(void * socket) {
	((BasicSocket*)socket)->socket_handler();
}

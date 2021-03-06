#include "ThreadP.h"
typedef struct informations {
	int index;
	BasicSocket * mysocket;
	bool *QUEUE;
} informations;
int ThreadP::run(BasicSocket * mySock) {
	struct informations  * infos = (informations*)malloc(sizeof(informations));
	infos->index = 0;
	infos->mysocket = mySock;
	if(message()) return 0;
	int i = 0;
	this->threads = (pthread_t *) malloc(sizeof(pthread_t) * this->MAX_THREADS);
	cout << "MAX THREAD = " << this->MAX_THREADS << endl;
	for(i = 0; i <= this->MAX_THREADS; i++){
		pthread_t thread;
		this->threads[i] = thread;
		this->QUEUE[i] = true;
	}
	infos->QUEUE = this->QUEUE;
	infos->index = 0;
	while (infos->mysocket->sockAccept()) {
		infos->index = 0;
		cout << "Iteration GENERAL = " << infos->index << endl;
		bool freeThread = false;
		if(infos->mysocket->getSocketClient() > 0) {
			while(infos->index <= this->MAX_THREADS || !freeThread){
                        	cout << "Iteration check free = " << infos->index << endl;
				if(infos->QUEUE[infos->index] == true){
                               		freeThread = true;
                               		break;
                       		}
				infos->index++;
                	}
			if(freeThread){
				cout << "ACCEPT for = " << infos->index << endl;
				infos->QUEUE[infos->index] = false;
				cout << "Connection accepted" << endl;
				/* Exemple with pthread */
				//Create socket for accept multi -client ijn serve
				//Alloc memory
				//int * socket_new = (int*)malloc(1);
				//*socket_new = mySock->getSocketClient();
				cout << "try to create handler" << endl;
				//Create socket with socket_handler function
				void * tab;
				cout << infos->index << endl;
				if (pthread_create(&this->threads[infos->index], NULL, &runFunction, infos) < 0)
				{
					cout << "Can't create thread" << endl;
					return -1;
				}
				else
					cout << "Handler is create" << endl;
				//Now join the thread , so that we dont terminate before the thread
				//pthread_join( this->threads[i] , NULL);
				cout << "Connection handler is assigned" << endl;
			}else
                                cout << "Full stack" << endl;
			} else
				cout << "Connection refused" << endl;
			//infos->mysocket->terminate();
//			this->QUEUE[i] = true;
		}
}

// set return state to 0 when code is implemented
int ThreadP::message() {
	cout << "Start thread server" << endl;
	//cout << "not yet implemented" << endl;
	return 0;
}

void * ThreadP::runFunction(void * infos) {
	//struct informations prout = (struct informations)infos;
	struct informations * infosCast = ((struct informations *) infos);
	cout << "ID : "  << infosCast->index <<endl << "########################################" << endl;
	infosCast->mysocket->socket_handler();
	infosCast->mysocket->terminate();
	infosCast->QUEUE[infosCast->index] = true;
}

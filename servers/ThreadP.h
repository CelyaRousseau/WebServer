#include "Server.cpp"

class ThreadP : public Server {
	int MAX_THREADS = 10;
	bool QUEUE[10];
	pthread_t* threads;
	int run(BasicSocket * mySock);
	int message();
	static void * runFunction(struct informations * info);
};

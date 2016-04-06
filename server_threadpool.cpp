#include "server_threadpool.h"

void log(string message){
	cout << message << endl;
}

struct threadpool {
	pthread_mutex_t lock;
	pthread_cond_t signal;
	pthread_t *threads;
	int *queue;
	int head;
	int tail;
	int count;
};

static void *listen(void *pool);

void threadpool_deallocate(threadpool *pool);

threadpool *threadpool_init(){
	threadpool *pool;
	int i;
	int thread = 0;

	// Allocation de la mémoire
	pool = (threadpool *)malloc(sizeof(threadpool));
	pool->threads = (pthread_t *) malloc(sizeof(pthread_t) * MAX_THREADS);
	pool->queue = (int *) malloc(sizeof(int) * MAX_QUEUED);

	// Initialisation des variables
	pool->head = 0;
	pool->tail = 0;
	pool->count = 0;
	pthread_mutex_init(&(pool->lock), NULL);
	pthread_cond_init(&(pool->signal), NULL);

	// Boucle d'initialisation des threads
	for (i = 0; i < MAX_THREADS; i++)
	{
		// Création du thread
		thread = pthread_create(&(pool->threads[i]), NULL, listen, (void*) pool);
		if (thread != 0)
		{
			return NULL;
		}
	}

	return pool;
}

static void *listen(void *thread_pool)
{
	threadpool *pool = (threadpool *) thread_pool;
	int connection;
	char logbuff[200];

	while(true)
	{
		//pthread_mutex_lock(&(pool->lock));
		while (pool->count == 0)
		{
			log("En attente de connexion");
			pthread_cond_wait(&(pool->signal), &(pool->lock));
		}

		// Récupération de la première connexion dans la queue
		connection = pool->queue[pool->head];
		pool->head += 1;

		// Retour au début de la queue si l'index de fin est atteint
		if (pool->head == MAX_QUEUED)
		{
			pool->head = 0;
		}

		pool->count -= 1;
		//pthread_mutex_unlock(&(pool->thread_lock));

		// Send the connection to the router for processing
		(*(process_request))((void*)connection); 
		cout << "test";
	}

	pthread_exit(NULL);
	return NULL;
}

void *process_request(void *socket) {
	char logbuff[300];
	int sockfd = (int) socket;

	// Variables
	long buffer_bytes;	// Number of bytes in the buffer
	static char buffer[BUFFER_SIZE + 1];	// Buffer to hold request string

	// Receive the request information, place into buffer
	buffer_bytes = recv(sockfd, buffer, BUFFER_SIZE, 0);


	// Check that the request is not empty and that the BUFSIZE has not been exceeded
	if (buffer_bytes <= 0 || buffer_bytes > BUFFER_SIZE)
	{
		// Log error, send error
		if (buffer_bytes <= 0)
		{
			log("Buffer size is <= 0");
		}

		if (buffer_bytes > BUFFER_SIZE)
		{
			log("Buffer is larger than allowed buffer size");
		}
		pthread_exit(NULL);
		return 0;
	}

	// Check for a valid request method is being used
	if (!strncmp(buffer, "GET ", 4))
	{
		// Log GET request, check formatting of request, call process method
		log("Processing GET request");
		log(logbuff);
		write(socket, buffer, NULL);
		bzero(buffer, BUFFER_SIZE);
		return 0;
	}
}

int listen(int port){
	static struct sockaddr_in server;

	threadpool *pool;
	int socket_infos = socket(AF_INET, SOCK_STREAM, 0);

    // Création du socket d'écoute
    if(socket_infos < 0)
    {
        // Affichage d'un message d'erreur et fin du programme
        log("Error on socket call. Program ending.");
        return(SOCKET_ERROR);
    }
    else
    {
    	// Affichage d'un message à la création du socket
        log("Socket successfully created.");
    }

	// Initialise les informations relatives au serveur
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);

	// Association du socket au serveur
	if(bind(socket_infos, (struct sockaddr *) &server, sizeof(server)) < 0)
    {
        // Affichage d'un message d'erreur et fin du programme
        log("Error on bind call. Program ending.");
        return(SOCKET_ERROR);
    }
    else
    {
    	// Affichage d'un message à l'association du socket au serveur
        log("Socket bind successful.");
    }

    pool = threadpool_init();
}

int main (int argc, char *argv[]){
	listen(DEFAULT_PORT);
}


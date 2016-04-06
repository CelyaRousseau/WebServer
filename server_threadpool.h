#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_THREADS 10
#define MAX_QUEUED 25
#define DEFAULT_PORT 8484
#define SOCKET_ERROR 3
#define BUFFER_SIZE 8096

typedef struct threadpool threadpool;
using namespace std;

int listen(int);
void log(string);
threadpool *threadpool_init();
void *process_request(void*);
#include "Epoll.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>

int MAXEVENTS = 15;
int EPOLLCREATED = 1;

int Epoll::run(BasicSocket * mySock) {
	if(message()) return 0;

	int sfd, s, efd;
  	struct epoll_event event;
  	struct epoll_event *events;

	sfd = mySock->makeSocketUnblocked();

	while (mySock->sockAccept()) {
		if(mySock->getSocketClient() > 0) {
			cout << "Connection accepted" << endl;

			efd = epoll_create1(0);

			if(efd == -1){
				cout << "Epoll not created" << endl;
			}
 			cout << "Epoll created" << endl;


			if (epoll_ctl (efd, EPOLL_CTL_ADD, sfd, &event) == -1)
			{
				cout << "Epoll controller" << endl;
			}

 			event.data.fd = sfd;
			event.events = EPOLLIN | EPOLLET;


			/* Buffer where events are returned */
			events = calloc (MAXEVENTS, sizeof event);

			/* The event loop */
			while (1)
			{

			}
			



			
		    free (events);

		    close (sfd);

		}
	}
}

// set return state to 0 when code is implemented
int Epoll::message() {
	cout << "Start epoll server" << endl;
	return 0;
	}
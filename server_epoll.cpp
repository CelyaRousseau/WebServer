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

// MAIN PART
int main (int argc, char *argv[]){
    int sfd, epoll, s;
    struct epoll_event event;
    struct epoll_event *events;

    epoll = epoll_create(EPOLLCREATED);

    event.events = EPOLLIN;
    event.data.fd = listen_sock;

    /* Buffer where events are returned */
    events = calloc (MAXEVENTS, sizeof *event);

    /* The event loop */
    while (true) {
        int n, i;

        n = epoll_wait (efd, events, MAXEVENTS, -1);

        for (i = 0; i < n; i++) {
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN))) {
              /* An error has occured on this fd, or the socket is not
               * ready for reading (why were we notified then?) */
            fprintf (stderr, "epoll error. events=%u\n", events[i].events);
            close (events[i].data.fd);
            continue;
            } else if (sfd == events[i].data.fd) {
                /* We have a notification on the listening socket, which
                * means one or more incoming connections. */
                while (1) {
                    struct sockaddr in_addr;
                    socklen_t in_len;
                    int infd;

                    in_len = sizeof in_addr;
                    infd = accept (sfd, &in_addr, &in_len);
                    if (infd == -1) {
                        printf("errno=%d, EAGAIN=%d, EWOULDBLOCK=%d\n", errno, EAGAIN, EWOULDBLOCK);
                        if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                            /* We have processed all incoming connections. */
                            printf ("processed all incoming connections.\n");
                            break;
                        } else {
                            perror ("accept");
                            break;
                        }
                    }

                    /* Make the incoming socket non-blocking and add it to the
                    * list of fds to monitor. */
                    s = make_socket_non_blocking (infd);
                    if (s == -1)
                        abort ();

                    event.data.fd = infd;
                    event.events = EPOLLIN | EPOLLET;

                    printf("set events %u, infd=%d\n", event.events, infd);
                    s = epoll_ctl (efd, EPOLL_CTL_ADD, infd, &event);

                    if (s == -1) {
                        perror ("epoll_ctl");
                        abort();
                    }
                }
            } else {
                /* We have data on the fd waiting to be read. Read and
                * display it. We must read whatever data is available
                * completely, as we are running in edge-triggered mode
                * and won't get a notification again for the same
                * data. */
                int done = 0;

                while (1) {
                    ssize_t count;
                    char buf[512];

                    count = read (events[i].data.fd, buf, sizeof buf);
                    if (count == -1) {
                        /* If errno == EAGAIN, that means we have read all data. So go back to the main loop. */
                        if (errno != EAGAIN) {
                            perror ("read");
                            done = 1;
                        }
                        break;
                    } else if (count == 0) {
                        /* End of file. The remote has closed the connection. */
                        done = 1;
                        break;
                    }

                    /* Write the reply to connection */
                    s = write (events[i].data.fd, reply, sizeof(reply));

                    if (s == -1) {
                        perror ("write");
                        abort();
                    }
                }

                if (done) {
                    printf ("Closed connection on descriptor %d\n", events[i].data.fd);

                    /* Closing the descriptor will make epoll remove it from the set of descriptors which are monitored. */
                    close (events[i].data.fd);
                }
            }
        }
    }

    free (events);

    close (sfd);

    return 0;
}

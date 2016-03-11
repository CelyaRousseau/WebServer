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

int SOMAXCONN = 3;
int MAXEVENTS = 15;

struct addrinfo {
    int              ai_flags;
    int              ai_family;
    int              ai_socktype;
    int              ai_protocol;
    size_t           ai_addrlen;
    struct sockaddr *ai_addr;
    char            *ai_canonname;
    struct addrinfo *ai_next;
};

typedef union epoll_data {
    void        *ptr;
    int          fd;
    __uint32_t   u32;
    __uint64_t   u64;
} epoll_data_t;

struct epoll_event {
    __uint32_t   events; /* Epoll events */
    epoll_data_t data;   /* User data variable */
};

// MAIN PART
int main (int argc, char *argv[]){
    int sfd, epollfd, s;
    struct epoll_event event;
    struct epoll_event *events;

    if (argc != 2)
    {
        fprintf (stderr, "Usage: %s [port]\n", argv[0]);
        return -1;
    }

    /* Bind the socket */
    sfd = establish_bind(argv[0]);

    // Set Socket in non blocking mode 
    make_socket_non_blocking(sockfd);

    // Listen for connection
    if (listen(sockfd,SOMAXCONN) == -1) {
        die("failed to listen for connections (errno=%d)",errno);
    }

    epollfd = epoll_create(2);

    event.events = EPOLLIN;
    event.data.fd = listen_sock;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &event) == -1) {
        perror("epoll_ctl: listen_sock");
        return -1;
    }

    if (epollfd == -1)
    {
      perror ("epoll_create");
      abort ();
    }

    /* Buffer where events are returned */
    events = calloc (MAXEVENTS, sizeof event);

    /*  Event loop */
    while (1) {        
    
    }

  free (events);

  close (sfd);
}

static int establish_bind (char *port) {

    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int out, sfd;

    memset (&hints, 0, sizeof (struct addrinfo));
    hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
    hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
    hints.ai_flags = AI_PASSIVE;     /* All interfaces */

    /** 
    * getaddrinfo() returns a list of address structures.
    * Try each address until we successfully connect(2).
    * If socket(2) (or connect(2)) fails, we (close the socket
    * and) try the next address. 
    **/
    out = getaddrinfo(NULL, port, &hints, &result);

    if(out != 0) {
    fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (out));
    return -1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
   
        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
          break; /* Success */

        close(sfd);
    }

      if (rp == NULL)
    {
      fprintf (stderr, "Could not bind\n");
      return -1;
    }

    freeaddrinfo (result);

    return sfd;}}

static int make_socket_non_blocking (int sockfd) {
  int flags, s;

  flags = fcntl (sockfd, F_GETFL, 0);
  if (flags == -1)
    {
      perror ("fcntl");
      return -1;
    }

  s = fcntl (sockfd, F_SETFL, O_NONBLOCK);
  if (s == -1)
    {
      perror ("fcntl");
      return -1;
    }

  return 0;}

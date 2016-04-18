#include <stdio.h>
#include <fstream>
#include "EpollImoucheG.h"
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>

using namespace std;
// Create no-blocking socket
int Epoll::CreateSocketNoBlock(){
  int flags;
  flags = fcntl (this->sfd, F_GETFL, 0);
  if (flags == -1)
    {
      cout << "Error on no-blocking : fcntl " << endl;
      return -1;
    }
  flags |= O_NONBLOCK;
  this->s = fcntl (this->sfd, F_SETFL, flags);
  if (this->s == -1)
    {
     cout << "Error on no-blocking : fcntl " << endl;
      return -1;
    }
  return 0;
}

 int Epoll::CreateBind(){
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  memset (&hints, 0, sizeof (struct addrinfo));
  hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
  hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
  hints.ai_flags = AI_PASSIVE;     /* All interfaces */

  this->s = getaddrinfo (NULL, this->PORTS, &hints, &result);
  if (this->s != 0)
    {
      fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
      return -1;
    }

  for (rp = result; rp != NULL; rp = rp->ai_next)
    {
      this->sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
      if (this->sfd == -1)
        continue;

      this->s = bind (this->sfd, rp->ai_addr, rp->ai_addrlen);
      if (this->s == 0)
        {
          /* We managed to bind successfully! */
          break;
        }

      close (this->sfd);
    }

  if (rp == NULL)
    {
      fprintf (stderr, "Could not bind\n");
      return -1;
    }

  freeaddrinfo (result);

  return this->sfd;
}
Epoll::Epoll(){

}
Epoll::~Epoll(){
  delete(this);
}
string Epoll::ReadFile(string fileName){
              ifstream file;
              file.open(fileName,fstream::in);
              time_t now = time(0);
              bool ret = 0;
              string content = "HTTP/1.1 200 OK\r\n"
                                "Date: " + string(ctime(&now)) +
                                "Server: CAGJ/1.0\r\n"
                                "Last-Modified: "+ string(ctime(&now)) +
                                "Content-Type: text/html\r\n"
                                "Content-Length: $LENGTHXX\r\n"
                                "Accept-Ranges: bytes\r\n"
                                "Connection: close\r\n"
                                "\r\n";
              try {
                if(file.is_open()){
                  //cout << "File is open" << endl;
                  string line;
                  while(!file.eof()){
                    getline(file, line);
                    content = content + line;
                  }
                  if(file.eof())
                    ret =  1;
                    }
                else cout << "Failed to open file" << endl;
              } catch (const std::bad_alloc&) {
                cout << "bad alloc error" << endl;
              }
              file.close();
              return content;
}
int Epoll::Run ()
{
  this->sfd = this->CreateBind();
  if (this->sfd == -1)
    abort ();
  this->s = this->CreateSocketNoBlock();
  if (this->s == -1)
    abort ();

  this->s = listen (this->sfd, SOMAXCONN);
  if (this->s == -1)
    {
      perror ("listen");
      abort ();
    }

  this->efd = epoll_create1 (0);
  if (this->efd == -1)
    {
      perror ("epoll_create");
      abort ();
    }

  this->event.data.fd = sfd;
  this->event.events = EPOLLIN | EPOLLET;
  this->s = epoll_ctl (this->efd, EPOLL_CTL_ADD, sfd, &this->event);
  if (this->s == -1)
    {
      perror ("epoll_ctl");
      abort ();
    }
  size_t sizeEvent = sizeof(this->event);
  /* Buffer where events are returned */
  this->events = (epoll_event*)calloc (MAXEVENTS, sizeEvent);

  /* The event loop */
  while (1)
    {
      int n, i;

      n = epoll_wait (this->efd, this->events, MAXEVENTS, -1);
      for (i = 0; i < n; i++)
	{
	  if ((this->events[i].events & EPOLLERR) ||
              (this->events[i].events & EPOLLHUP) ||
              (!(this->events[i].events & EPOLLIN)))
	    {
              /* An error has occured on this fd, or the socket is not
                 ready for reading (why were we notified then?) */
	      fprintf (stderr, "epoll error\n");
	      close (this->events[i].data.fd);
	      continue;
	    }

	  else if (this->sfd == this->events[i].data.fd)
	    {
              /* We have a notification on the listening socket, which
                 means one or more incoming connections. */
              while (1)
                {
                  struct sockaddr in_addr;
                  socklen_t in_len;
                  int infd = this->sfd;
                  char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

                  in_len = sizeof in_addr;
                  infd = accept (this->sfd, &in_addr, &in_len);
                  if (infd == -1)
                    {
                      if ((errno == EAGAIN) ||
                          (errno == EWOULDBLOCK))
                        {
                          /* We have processed all incoming
                             connections. */
                          break;
                        }
                      else
                        {
                          perror ("accept");
                          break;
                        }
                    }

                  this->s = getnameinfo (&in_addr, in_len,
                                   hbuf, sizeof hbuf,
                                   sbuf, sizeof sbuf,
                                   NI_NUMERICHOST | NI_NUMERICSERV);
                  if (this->s == 0)
                    {
                      printf("Accepted connection on descriptor %d "
                             "(host=%s, port=%s)\n", infd, hbuf, sbuf);
                    }

                  /* Make the incoming socket non-blocking and add it to the
                     list of fds to monitor. */
                  this->s = CreateSocketNoBlock();
                  if (this->s == -1)
                    abort ();

                  this->event.data.fd = infd;
                  this->event.events = EPOLLIN | EPOLLET;
                  this->s = epoll_ctl (this->efd, EPOLL_CTL_ADD, infd, &this->event);
                  if (this->s == -1)
                    {
                      perror ("epoll_ctl");
                      abort ();
                    }
                }
              continue;
            }
          else
            {
              /* We have data on the fd waiting to be read. Read and
                 display it. We must read whatever data is available
                 completely, as we are running in edge-triggered mode
                 and won't get a notification again for the same
                 data. */
              int done = 0;
	            ifstream file;
            	file.open("index.html",fstream::in);
              time_t now = time(0);
            	string content = this->ReadFile("index.html");
              size_t index = 0;
              string sizeContent = std::to_string(content.size());
              content.replace(content.find("$LENGTHXX",0), index + 9, sizeContent);
            	char * response = (char*) content.c_str();
            	this->s = write(this->events[i].data.fd, response, strlen(response));
              done = 1;
            	if (this->s == -1) {
                     perror ("write");
                     abort ();
              }
              if (done)
                {
                  printf ("Closed connection on descriptor %d\n",
                          this->events[i].data.fd);
                  /* Closing the descriptor will make epoll remove it
                     from the set of descriptors which are monitored. */
                    shutdown(this->events[i].data.fd, SHUT_RDWR);
                    close (this->events[i].data.fd);
                }
            }
        }
    }
  free (this->events);
  close (this->sfd);

  return EXIT_SUCCESS;
}

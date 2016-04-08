#include "Server.cpp"

class Epoll : public Server {
	void run();
	int create_and_bind(char * port);
	int make_socket_non_blocking(int sfd);
	string ReadFile(string file);
};

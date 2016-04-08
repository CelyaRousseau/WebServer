#ifndef BASICSERVERCPP
#define BASICSERVERCPP

#include "Server.h"

int Server::run(BasicSocket * mySock){
	cout << "test" << endl;
	return 0;}
void Server::run() {
	cout << "test" << endl;
}
int Server::message(){
	cout << "test2" << endl;
	return 0;}

#endif

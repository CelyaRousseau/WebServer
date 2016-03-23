#!/usr/bin/env bash

if [ "$1" != "" ]; then
	case "$1" in
		"epoll") 	echo "Compiling EPOLL server"
					g++ server_epoll.cpp -std=c++11 -g -pthread -o server -Wno-write-strings 
					;;
		"fork") 	echo "Compiling FORK server"
					g++ server_fork.cpp -std=c++11 -g -pthread -o server -Wno-write-strings 
					;; 
		"thread") 	echo "Compiling THREAD server"
					g++ server_thread.cpp -std=c++11 -g -pthread -o server -Wno-write-strings
					;;
		"threadp") 	echo "Compiling THREAD POOL server"
					g++ server_threadpool.cpp -std=c++11 -g -pthread -o server -Wno-write-strings 
					;;
		*) echo "usage ${0} epoll:fork:thread:threadp"
					;;
	esac
else
    echo "usage ${0} epoll:fork:thread:threadp"
fi
#!/usr/bin/env bash

if [ "$1" != "" ]; then
	case "$1" in
		"epoll") 	echo "Compiling EPOLL server"
					g++ servers/Epoll.cpp -std=c++11 -g -pthread -o server -Wno-write-strings 
					;;
		"fork") 	echo "Compiling FORK server"
					g++ servers/Fork.cpp -std=c++11 -g -pthread -o server -Wno-write-strings 
					;; 
		"thread") 	echo "Compiling THREAD server"
					g++ servers/Thread.cpp -std=c++11 -g -pthread -o server -Wno-write-strings
					;;
		"threadp") 	echo "Compiling THREAD POOL server"
					g++ servers/ThreadP.cpp -std=c++11 -g -pthread -o server -Wno-write-strings 
					;;
		*) echo "usage ${0} epoll:fork:thread:threadp"
					;;
	esac
else
    echo "usage ${0} epoll:fork:thread:threadp"
fi
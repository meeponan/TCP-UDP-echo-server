CC = g++
CFLAGS= -O3 
LIBS = -lpthread\

all:  main
clean:
	rm -f *.o
	rm -f main

common.o: common.cpp common.hpp
	     $(CC) $(CFLAGS) -c common.cpp

TCPServer.o: TCPServer.cpp TCPServer.hpp common.hpp
	     $(CC) $(CFLAGS) -c TCPServer.cpp

UDPServer.o: UDPServer.cpp UDPServer.hpp common.hpp
	     $(CC) $(CFLAGS) -c UDPServer.cpp

main.o:	main.cpp TCPServer.hpp UDPServer.hpp
	     $(CC) $(CFLAGS) -c main.cpp 

main:		main.o UDPServer.o TCPServer.o common.o
	     $(CC) $(CFLAGS) -o main main.o UDPServer.o TCPServer.o common.o
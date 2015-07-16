#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

#include "TCPServer.hpp"
#include "UDPServer.hpp"

using namespace std;
//./server -m t -p 5000

int MINIMUM_ARGUMENT_COUNT = 4;
string UDP_SERVER = "u";
string TCP_SERVER = "t";

void parseCommandLineArgs( int numberOfArgs, char** arguments, vector<string>& out_CommandLineTokens ) {
	string token;
	for( int i = 1; i < numberOfArgs; ++i ){
		token = arguments[i];
		out_CommandLineTokens.push_back( token );
		//printf( token.c_str() );
		//printf( " " );
	}
}

int main( int argc, char** argv ) {

	vector<string> commandLineTokens;
	if(argc==5){
        parseCommandLineArgs( argc, argv, commandLineTokens );
        string ServerType = commandLineTokens[1];
        int ServerPort = atoi(commandLineTokens[3].c_str());


        if(ServerType == "t"){
            cout<<"TCPServer running on port:"<<ServerPort<<endl;
            TCPServer tcpServer(ServerPort);
			tcpServer.initializeAndRun();
        }
        else if(ServerType == "u"){
            cout<<"UDPServer running on port:"<<ServerPort<<endl;
            UDPServer udpServer(ServerPort);
			udpServer.initializeAndRun();
        }
        else{
            cout<<"Server type error!"<<endl;
        }
	}
	else{
        cout<<"Could not detect a proper configuration from the command line arguments."<<endl;
	}


	return 0;
}

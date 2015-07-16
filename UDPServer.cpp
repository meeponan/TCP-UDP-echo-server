#include "UDPServer.hpp"
#include "common.hpp"

fd_set master1;
fd_set read_fds1;

int highestsocket1;

void readUDP(int listener);

UDPServer::~UDPServer() {

}


UDPServer::UDPServer(int& portNumber ) {

	m_PortNumber = portNumber;
}


void UDPServer::initializeAndRun() {

    FD_ZERO(&master1);                // clear the master and temp sets
    FD_ZERO(&read_fds1);

	//printf("binding to %d\n",m_PortNumber);
    int yes=1;
    int listener;
	sockaddr_in myaddr;
    memset(&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(m_PortNumber);

    if ((listener = socket(PF_INET, SOCK_DGRAM, 0)) == -1){
       	printf("cannot create a UDP socket");
     	fflush(stdout);
        exit(1);
    }

    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1){
        printf("setsockopt");
	    fflush(stdout);
       	exit(1);
    }

    if (bind(listener, (sockaddr *) &myaddr, sizeof(myaddr)) < 0) {
        printf("could not bind to MYPORT");
	    fflush(stdout);
      	exit(1);
    }

    FD_SET(listener, &master1);

    if (listener > highestsocket1)
    {
        highestsocket1 = listener;
    }

    FD_SET(fileno(stdin), &master1);

    if (fileno(stdin) > highestsocket1)
    {
      	highestsocket1 = fileno(stdin);
    }

    struct timeval timeout;

    while(1){
        read_fds1 = master1;
        timeout.tv_sec = 1;
		timeout.tv_usec = 0;
        if (select(highestsocket1+1, &read_fds1, NULL, NULL, &timeout) == -1)
        {
            if (errno == EINTR)
            {
                cout << "got the EINTR error in select" << endl;
            }
            else
            {
                cout << "select problem, server got errno " << errno << endl;
                //printf("Select problem .. exiting server");
                fflush(stdout);
                exit(1);
            }
        }
        readUDP(listener);
    }
}

void readUDP(int listener){

    char recvbuf[RECVBUFLEN];
    int readResult = 1;
    int sendResult = 0;

    long *timeCount = new long[1000];
    long *interArrival = new long[1000];
    int i = 0;

    int msgRecv = 0;
    int totalBytesRecv = 0;
    int totalBytesSent = 0;
    struct timeval tv;

    if(FD_ISSET(listener,&read_fds1)){
        cout<<"Transmitting may take some time..Please wait.."<<endl;
        do{
            struct sockaddr_in remoteaddr;
            socklen_t sizeOfResultAddress = sizeof(remoteaddr);
            readResult = recvfrom(listener, recvbuf, RECVBUFLEN, 0, (struct sockaddr *)&remoteaddr, &sizeOfResultAddress);
            //readResult = recvfrom(listener, recvbuf, RECVBUFLEN, 0, NULL, NULL);

            gettimeofday(&tv,NULL);
            timeCount[i] = 1000000 * tv.tv_sec + tv.tv_usec;

            string receivedString(recvbuf);
            receivedString.resize(RECVBUFLEN);

                //Decide if received message is termination msg
            if(readResult == 3 && strcmp(receivedString.c_str(),"end") == 0){
                    //cout<<"Termination received. The connection with client is shutting down"<<endl;
                break;
            }
            else{
                if(readResult > 0){
                    i++;
                    msgRecv++;//number of message received
                    totalBytesRecv += readResult; //count total bytes received

                        //sendResult = send( clientSocket, receivedString.c_str(), receivedString.length(), 0 );
                    sendResult = sendto(listener, receivedString.c_str(), readResult, 0, (struct sockaddr *)&remoteaddr, sizeOfResultAddress);
                    if(sendResult == -1){
                        cout<<"send function call failed with error number:"<<endl;
                    }
                    totalBytesSent += readResult;
                }
                else{
                    cout<<"Recv failed"<<endl;
                    close(listener); // bye!
                    FD_CLR(listener, &master1);
                }
            }
            memset(recvbuf,'\0', 640);
        } while(readResult > 0);

        for(int j = 1; j < i-1; j++){
                interArrival[j-1] = timeCount[j] - timeCount[j-1];
        }

        cout<<"Messages rcvd: "<<msgRecv<<endl;
        cout<<"Bytes received: "<<totalBytesRecv<<endl;
        cout<<"Run time: "<<(timeCount[i-1]-timeCount[0])/1000<<endl;
        cout<<"Bytes sent: "<<totalBytesSent<<endl;
        cout<<"Min Lamda: "<<minInterArrival(interArrival,i-2)<<endl;
        cout<<"Max Lamda: "<<maxInterArrival(interArrival,i-2)<<endl;
        cout<<"Lamda:" <<meanInterArrival(interArrival,i-2)<<endl;
        cout<<"Termination received."<<endl;
        cout<<"Server running.You could start a new connection or enter 'q' to quit"<<endl;
        //closesocket(listener); // bye!
        //FD_CLR(listener, &master1);
    }
    else if (FD_ISSET(fileno(stdin),&read_fds1)){
         char c = getchar();
         if (c == 'q'){
			 cout<<"Server quitting."<<endl;
			 exit(0);
         }
    }
}

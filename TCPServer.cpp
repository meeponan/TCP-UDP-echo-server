#include "TCPServer.hpp"
#include "common.hpp"


// master file descriptor list
fd_set master;
fd_set read_fds; // temp file descriptor list for select()
// highest file descriptor seen so far
int highestsocket;

void readTCP(int listener);

TCPServer::~TCPServer() {

}


TCPServer::TCPServer( int& portNumber ) {
	m_PortNumber = portNumber;
}


void TCPServer::initializeAndRun() {

	//printf("%d\n",m_PortNumber);
	struct sockaddr_in myaddr;       // my address
    int yes=1;                       // for setsockopt() SO_REUSEADDR, below
    int listener;

    FD_ZERO(&master);                // clear the master and temp sets
    FD_ZERO(&read_fds);

    // get the listener
    if ((listener = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
       	printf("cannot create a socket");
     	fflush(stdout);
        exit(1);
    }

    // lose the pesky "address already in use" error message
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1)
    {
        printf("setsockopt");
	    fflush(stdout);
       	exit(1);
    }

    // bind to the port
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = INADDR_ANY;
    myaddr.sin_port = htons(m_PortNumber);
    memset(&(myaddr.sin_zero), '\0', 8);
    if (bind(listener, (struct sockaddr *)&myaddr, sizeof(myaddr)) == -1)
    {
        printf("could not bind to MYPORT");
	    fflush(stdout);
      	exit(1);
    }

    // listen
    if (listen(listener, 40) == -1)
    {
       printf("too many backlogged connections on listen");
  	   fflush(stdout);
       exit(1);
    }


     // add the listener to the master set
    FD_SET(listener, &master);

     // keep track of the biggest file descriptor
    if (listener > highestsocket)
    {
        highestsocket = listener;
    }

    FD_SET(fileno(stdin), &master);

    if (fileno(stdin) > highestsocket)
    {
      	highestsocket = fileno(stdin);
    }

    struct timeval timeout;

     // main loop

    while (1){
        read_fds = master;
	    timeout.tv_sec = 1;
		timeout.tv_usec = 0;
        if (select(highestsocket+1, &read_fds, NULL, NULL, &timeout) == -1)
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

        readTCP(listener);
    }
}

void readTCP(int listener){
    int clientSocket = 0;
    struct sockaddr_in remoteaddr;
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
    memset( recvbuf,'\0', 640 );
    if (FD_ISSET(listener,&read_fds)){

        clientSocket = accept(listener,NULL,NULL); //accept(listener,(struct sockaddr *)&remoteaddr,sizeof(remoteaddr));
        if(clientSocket == -1){
            cout<<"Trouble accepting a new connection"<<endl;
        }
        else{
            cout<<"Transmitting may take some time..Please wait.."<<endl;
            do{
                readResult = recv(clientSocket, recvbuf, RECVBUFLEN, 0);
                gettimeofday(&tv,NULL);
                timeCount[i] = 1000000 * tv.tv_sec + tv.tv_usec;
                //if(i > 0){
                //    interArrival[i-1] = timeCount[i] - timeCount[i-1];
                //}
                string receivedString(recvbuf);
                receivedString.resize(RECVBUFLEN);
                //Decide if received message is termination msg
                if(readResult == 3 && strcmp(receivedString.c_str(),"end") == 0){
                    cout<<"Termination received. The connection with client is shutting down"<<endl;
                    break;
                }
                else{
                    if(readResult > 0){
                        i++;
                        msgRecv++;//number of message received
                        totalBytesRecv += readResult; //count total bytes received

                        //sendResult = send( clientSocket, receivedString.c_str(), receivedString.length(), 0 );
                        sendResult = send( clientSocket, recvbuf, readResult, 0 );
                        if(sendResult == -1){
                            cout<<"send function call failed with error number:"<<endl;
                        }
                        totalBytesSent += readResult;
                    }
                    else{
                        cout<<"Recv failed"<<endl;
                        close(clientSocket); // bye!
                        FD_CLR(clientSocket, &master);
                    }
                }
                memset(recvbuf,'\0', 640);
            } while(readResult > 0);

            for(int j = 1; j < i-1; j++){
                interArrival[j-1] = timeCount[j] - timeCount[j-1];
            }
            //output information
            cout<<"Messages rcvd: "<<msgRecv<<endl;
            cout<<"Bytes received: "<<totalBytesRecv<<endl;
            cout<<"Run time: "<<(timeCount[i-1]-timeCount[0])/1000<<endl;
            cout<<"Bytes sent: "<<totalBytesSent<<endl;
            cout<<"Min Lamda: "<<minInterArrival(interArrival,i-2)<<endl;
            cout<<"Max Lamda: "<<maxInterArrival(interArrival,i-2)<<endl;
            cout<<"Lamda:" <<meanInterArrival(interArrival,i-1)<<endl;
            cout<<"Termination received."<<endl;
            cout<<"You could start a new connection or enter 'q' to quit"<<endl;
            close(clientSocket); // bye!
            FD_CLR(clientSocket, &master);
            //exit(0);
        }
    }
    else if (FD_ISSET(fileno(stdin),&read_fds)){
         char c = getchar();
         if (c == 'q'){
			 cout<<"Server quitting."<<endl;
			 exit(0);
         }
    }
}

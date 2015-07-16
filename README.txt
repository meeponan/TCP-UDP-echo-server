Using command 'make' to compile the program.

Run the server first.

To run the server, using command:
./main -m <t/u> -p <port> 

where (t = TCP, u = UDP) and <port> is the port number that server is using

To use the client use this command(I didn't write the client myself, just got it from my friend):
java -jar EchoClient.jar -m <t/u> -p <port>
where -m selects mode (t = TCP, u = UDP) and <port> is the port number
that your server is using.
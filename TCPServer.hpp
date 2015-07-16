#ifndef included_TCPServer
#define included_TCPServer

#include <string>

class TCPServer {
public:
	~TCPServer();
	explicit TCPServer(int& portNumber);
	void initializeAndRun();

protected:
	int	m_PortNumber;

private:

};

#endif

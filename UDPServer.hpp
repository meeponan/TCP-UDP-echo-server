#ifndef includedUDPServer
#define includedUDPServer

#include <string>

class UDPServer {
public:
	~UDPServer();
	explicit UDPServer(int& portNumber);
	void initializeAndRun();

protected:
	int			m_PortNumber;

private:


};


#endif

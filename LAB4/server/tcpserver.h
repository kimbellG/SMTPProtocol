#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "server.h"

class TCPSocket : public Server {

public:
	TCPSocket(const char *ip, const int port);
	TCPSocket(SOCKET s);
	~TCPSocket();

	int recv(char *buf, const int szbuf, sockaddr_in *from = nullptr) override;
	int send(const char *buf, const int szbuf, const char *toip = nullptr, const int toport = 0) override;
	SOCKET accept() override;
	void close() override {
		closesocket(s);
	}
};

#endif // TCPSERVER_H

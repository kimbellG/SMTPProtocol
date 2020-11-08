#ifndef CLIENT_H
#define CLIENT_H


#include <winsock2.h>

class Client {
	SOCKET s;
	sockaddr_in addr;
	int szaddr;
public:
	Client(const char *ip, const int port = 1280);
	void connection();
	int sendstr(const char *buf);
	int getstr(char *buf);
	void close();
};

#endif // CLIENT_H

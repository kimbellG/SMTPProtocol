#ifndef SERVER_H
#define SERVER_H


#include <winsock2.h>
#include <fstream>

class Server {

protected:
	SOCKET        s;
	sockaddr_in   local;
	int           szlocal;
	std::ofstream log;


public:
	Server(const char *ip, const int port, const int type, const char *logfile = "defaultserv.log");
	Server(SOCKET s);
	virtual ~Server();

	virtual int recv(char *buf, const int szbuf, sockaddr_in *from = nullptr) = 0;
	virtual int send(const char *buf, const int szbuf, const char *toip = nullptr, const int toport = 0) = 0;
	virtual SOCKET accept();
	virtual void close() = 0;
	void log_t(const char *message);
};

const int kbyte = 1024;

#endif // SERVER_H
